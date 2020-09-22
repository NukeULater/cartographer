#pragma once

struct __declspec(align(4)) menu_input_unk1
{
	DWORD flags;
	DWORD field_4;
	int field_8;
	DWORD field_C;
	DWORD field_10;
	DWORD field_14;
	DWORD field_18;
	DWORD field_1C;
};

BYTE* __cdecl ui_memory_pool_allocate(int size, int unk);
int __cdecl unk_used_after_constructor(void *ui_memory, menu_input_unk1 *a2);

void override_vtbl(void* vTable, void** newVtable);