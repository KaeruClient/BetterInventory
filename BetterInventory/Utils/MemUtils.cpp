#include "MemUtils.h"

MemRange MemUtils::getRange() {
    static MemRange range = []() -> MemRange {
        HMODULE base = GetModuleHandleA("Minecraft.Windows.exe");
        if (!base) return { nullptr, nullptr };

        MODULEINFO info{};
        if (!GetModuleInformation(GetCurrentProcess(), base, &info, sizeof(info)))
            return { nullptr, nullptr };

        auto b = std::bit_cast<std::byte*>(base);
        return { b, b + info.SizeOfImage };
        }();
    return range;
}