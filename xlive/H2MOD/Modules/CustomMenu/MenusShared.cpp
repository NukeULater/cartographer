
#include "MenusShared.h"

#include "../Util/MemoryPatch.h"

BYTE* ui_memory_pool_allocate(int size, int unk)
{
	/* unk is not used, but def pushed on the stack */
	auto p_ui_memory_pool_allocate = Memory::GetAddressRelative<BYTE*(__cdecl*)(int, int)>(0x60D2D8);
	return p_ui_memory_pool_allocate(size, unk);
}

// 60B8C3
int __cdecl unk_used_after_constructor(void *ui_memory, menu_input_unk1 *a2)
{
	auto p_unk_used_after_constructor = Memory::GetAddressRelative<int(__cdecl*)(void*, menu_input_unk1*)>(0x60B8C3);
	return p_unk_used_after_constructor(ui_memory, a2);
}

void override_vtbl(void* classPtr, void* newVtable)
{
	**(void***)classPtr = newVtable;
}

