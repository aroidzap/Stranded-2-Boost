#pragma once

void ddraw_hook_safe_call();
int DirectDrawCreateEx_CreateHook(void *original, void* VirtualProtect_func, void* hook, void **continue_address, void **oldcontent_a, void **oldcontent_b);
int DirectDrawCreateEx_RemoveHook(void *original, void* VirtualProtect_func, void *oldcontent_a, void *oldcontent_b);
void DirectDrawCreateEx_HookContinue(void *continue_address, int EBP);