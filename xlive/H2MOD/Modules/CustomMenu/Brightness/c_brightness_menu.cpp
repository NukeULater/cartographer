
#include "c_brightness_menu.h"

#include "../Util/MemoryPatch.h"
#include "../Util/Hooks/Hook.h"

/*
	Re-implementation of default brightness menu
	Now with the abillity to override base class functions
*/

c_screen_with_menu_vtbl* c_brightness_menu::getVtbl()
{
	*(void**)this = Memory::GetAddressRelative<void**>(0x7D7ABC);
	return this;
}

c_brightness_menu::c_brightness_menu(int a3, int a4, int a5) 
	: c_screen_with_menu(272, a3, a4, a5, (BYTE*)this->data + 2652)
{
	// call the default constructor

	//auto c_brightness_menu_constructor_impl = Memory::GetAddressRelative<int(__thiscall*)(c_brightness_menu*, int, int, int)>(0x648B17);
	//c_brightness_menu_constructor_impl(this, a3, a4, a5);

	// initialize button list
	Memory::GetAddressRelative<void(__thiscall*)(void*, int a2)>(0x650E22)(this->data + 2652, a5); // c_brightness_level_edit_list
}

void* __cdecl c_brightness_menu::open_brightness_menu(menu_input_unk1* a1)
{
	c_brightness_menu* brightness_menu = nullptr;
	c_brightness_menu* ui = (c_brightness_menu*)ui_memory_pool_allocate(3388, 0);

	if (ui) {
		brightness_menu = new (ui) c_brightness_menu(a1->field_4, a1->field_8, *((WORD*)&a1->flags + 1)); // manually call the constructor
	}

	// override vtable of an already defined menu (c_brightness_menu)
	//*(void**)brightness_menu = getVtbl();

	*(BYTE*)((int)brightness_menu + 108) = 1;
	unk_used_after_constructor(brightness_menu, a1);
	return brightness_menu;
}

void replace_brightness_menu()
{
	WritePointer((DWORD)Memory::GetAddressRelative(0x6492DF) + 1, (void*)c_brightness_menu::open_brightness_menu);
	WritePointer((DWORD)Memory::GetAddressRelative(0x64966C) + 1, (void*)c_brightness_menu::open_brightness_menu);
}