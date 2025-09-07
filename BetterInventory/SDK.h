#pragma once
#include <concepts>
#include <string>

namespace hat {
    template<typename To, typename From>
    struct constness_as : std::type_identity<std::remove_const_t<To>> {};
    template<typename To, typename From>
    struct constness_as<To, const From> : std::type_identity<const To> {};
    template<typename To, typename From>
    using constness_as_t = typename constness_as<To, From>::type;
    template<typename MemberType, typename Base>
    auto& fieldAt(Base* ptr, std::integral auto offset) {
        using Ret = constness_as_t<MemberType, Base>;
        return *reinterpret_cast<Ret*>(reinterpret_cast<uintptr_t>(ptr) + offset);
    }
}
class Item;
class ItemStack {
private:
	uintptr_t** vTable;
	Item** item;
public:
	Item* getItem() {
		if (!item) return nullptr;
		return *item;
	}
};

class Item {
public:
    std::string& getFullname() {
        return hat::fieldAt<std::string>(this, 0x128);
    }
};