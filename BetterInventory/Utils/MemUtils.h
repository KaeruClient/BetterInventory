#pragma once

#include <windows.h>
#include <memory>
#include <Psapi.h>
#include <libhat.hpp>
struct MemRange {
private:
	std::byte* begin = nullptr;
	std::byte* end = nullptr;
public:
	MemRange(std::byte* begin, std::byte* end) {
		this->begin = begin;
		this->end = end;
	};
	inline const std::byte* getBegin() const { return begin; }
	inline const std::byte* getEnd() const { return end; }
};
namespace MemUtils {
	MemRange getRange();
	template <typename TRet, typename... TArgs>
	inline auto* getFunction(void* original) {
		using Fn = TRet(__fastcall*)(TArgs...);
		return reinterpret_cast<Fn>(original);
	};

	struct SignatureHandle {
	private:
		hat::scan_result rawResult;
		__forceinline uintptr_t get() const {
			return std::bit_cast<uintptr_t>(rawResult.get());
		}
		__forceinline uintptr_t at(unsigned int index) const {
			return std::bit_cast<uintptr_t*>(rawResult.get())[index];
		}
	public:
		SignatureHandle(uintptr_t address)
			: rawResult(reinterpret_cast<std::byte*>(address)) {
		}

		SignatureHandle(void* address)
			: rawResult(reinterpret_cast<std::byte*>(address)) {
		}
		SignatureHandle(hat::scan_result result) : rawResult(result) {};
		SignatureHandle() : rawResult(nullptr) {};
		__forceinline operator uintptr_t() const { return get(); }

		inline SignatureHandle reference() const {
			uintptr_t address = get();
			int rel = *reinterpret_cast<int*>(address + 1);
			return SignatureHandle(std::bit_cast<std::byte*>(address + 0x5 + rel));
		}
		__forceinline uintptr_t operator[](unsigned int index) const {
			return at(index);
		}
		template<typename T = void*>
		void protect(SIZE_T size, DWORD type) {
			DWORD oldProtect;
			void* address = reinterpret_cast<void*>(get());
			VirtualProtect(address, size, PAGE_READWRITE, &oldProtect);
		}
		SignatureHandle resolve(size_t size) const {
			uintptr_t instr = get();
			int32_t disp = *reinterpret_cast<int32_t*>(instr + size);
			uintptr_t target = instr + size + sizeof(int32_t) + disp;
			return SignatureHandle((std::byte*)target);
		}

		__forceinline SignatureHandle vtable(int offset = 3) const {
			uintptr_t sigAddr = get();
			int finalOffset = *reinterpret_cast<int*>(sigAddr + offset);
			return SignatureHandle(std::bit_cast<std::byte*>(sigAddr + finalOffset + 7));
			/*
			Since the instruction size of Lea is 7 bytes, adding 7 bytes to the current instruction address allows you to reach the start of the VTable pointer.
			More precisely, you first read the relative offset stored at sigOffset + offset, then add 7 (the size of the instruction) to calculate the absolute address of the VTable.
			*/
		}
		template<typename Func>
		__forceinline operator Func* () const {
			return reinterpret_cast<Func*>(static_cast<uintptr_t>(*this));
		}
	};
	inline SignatureHandle findSignature(hat::signature_view signature) {
		const static MemRange range = MemUtils::getRange();
		hat::scan_result result = hat::find_pattern(signature);
		return SignatureHandle(result);
	}
}

#define SigScan(sign)                                  \
	([]() {                                                         \
		constexpr auto compiled_sign = hat::compile_signature<sign>(); \
		return MemUtils::findSignature(compiled_sign);                                 \
	})()