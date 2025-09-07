#pragma once
#include "SDK.h"

#include <format>
#include "Utils/MemUtils.h"
#include <MinHook.h>
#include <vector>
#include <memory>
#include <string>

class Hook {
protected:
    void* target;
public:
    Hook(uintptr_t address, void* detour, void*& orignal) {
        target = (void*)address;
        MH_CreateHook(target, detour, &orignal);
        MH_EnableHook(target);
    }
    Hook() {};
    ~Hook() {
        MH_DisableHook(target);
        MH_RemoveHook(target);
    }
};

class HookManager {
    std::vector<std::unique_ptr<Hook>> hooks;
public:
	void init();
};
extern HookManager g_Hooks;