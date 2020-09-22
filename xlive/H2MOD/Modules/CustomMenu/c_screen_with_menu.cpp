#include "stdafx.h"
#include "c_screen_with_menu.h"
#include "CustomMenuGlobals.h"

#include "MenusShared.h"

#include "../Util/MemoryPatch.h"

c_screen_with_menu::c_screen_with_menu(int menu_wgit_type, int a3, int a4, __int16 a5, void* a6)
{
	typedef void*(__thiscall* c_screen_with_menu_ctor_game_impl)(c_screen_with_menu*, int a2, int a3, int a4, __int16 a5, void* a6);
	auto p_c_screen_with_menu_ctor_game_impl = Memory::GetAddressRelative<c_screen_with_menu_ctor_game_impl>(0x611159);
	// call the constructor built-in game, which will set-up the vtable and everything
	p_c_screen_with_menu_ctor_game_impl(this, menu_wgit_type, a3, a4, a5, a6); sizeof(c_screen_with_menu);
}

c_screen_with_menu_vtbl* c_screen_with_menu_vtbl::getVtbl()
{
	// set the vtable to default vtbl
	*(void**)this = Memory::GetAddressRelative<void*>(0x7CF3A4);
	return this;
}

void call_screen_with_menu(menu_input_unk1* a1)
{
	c_screen_with_menu* ui = (c_screen_with_menu*)ui_memory_pool_allocate(sizeof(c_screen_with_menu), false);
	if (ui)
		new (ui) c_screen_with_menu(247, a1->field_4, a1->field_8, (WORD)a1->flags, &ui->data[2652]); // manually call the constructor, and use the functions on the memory allocated by ui_memory_pool_allocate
}
