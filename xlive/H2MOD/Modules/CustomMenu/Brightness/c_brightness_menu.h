#pragma once

#include "..\MenusShared.h"
#include "..\c_screen_with_menu.h"

class c_brightness_menu : public c_screen_with_menu
{
public:
	c_brightness_menu::c_brightness_menu(int a3, int a4, int a5);

	static void* open_brightness_menu(menu_input_unk1* a1);

	c_screen_with_menu_vtbl* c_brightness_menu::getVtbl() override;
};

void replace_brightness_menu();