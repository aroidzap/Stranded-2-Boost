#pragma once

typedef unsigned int m32;

void ddraw_hook_safe_call();
int DirectDrawCreateEx_CreateHook(void *original, void* VirtualProtect_func, void* hook, void **continue_address, m32 *oldcontent_a, m32 *oldcontent_b);
int DirectDrawCreateEx_RemoveHook(void *original, void* VirtualProtect_func, m32 oldcontent_a, m32 oldcontent_b);
