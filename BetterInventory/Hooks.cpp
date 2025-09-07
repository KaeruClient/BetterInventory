#include "Hooks.h"
void* oItem_appendFormattedHovertext = nullptr;
static void Item_appendFormattedHovertext(Item* a1, ItemStack* itemStack, __int64* level, std::string& text, bool showCategory) {
	static auto oFunc = MemUtils::getFunction<void, Item*, ItemStack*, __int64*, std::string&, bool>(oItem_appendFormattedHovertext);
	oFunc(a1, itemStack, level, text, true);
	text.append(std::format("\n{}8{}", "\xc2\xa7", a1->getFullname()));
};

void HookManager::init()
{
	/*
	{
		uintptr_t address = SigScan("");
		hooks.push_back(std::make_unique<Hook>(address, Detour, original));
	}
	*/

	{
		uintptr_t address = SigScan("48 89 5C 24 ?? 55 56 57 41 54 41 55 41 56 41 57 48 8D 6C 24 D0 48 81 EC ?? ?? ?? ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 45 ?? 49 8B F1 4D 8B E0");
		hooks.push_back(std::make_unique<Hook>(address, Item_appendFormattedHovertext, oItem_appendFormattedHovertext));
	}
}

HookManager g_Hooks = HookManager();