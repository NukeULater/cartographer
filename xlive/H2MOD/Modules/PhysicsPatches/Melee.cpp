
#include "Melee.h"

#include "H2MOD.h"
#include "Util\Hooks\Hook.h"

#include "Blam/Engine/Game/GameTime.h"

namespace Melee
{
	static float melee_lunge_distance_factor = 0.33333334f;
	static float melee_lunge_distance_factor2 = 4.f;
	static float melee_lunge_distance_factor3 = 3.0f;
	static float unk3 = 5.f;
	static float unk_time_4 = 2.5f;
	static float unk_time_5 = 0.087155744f;
	static float melee_lunge_min_speed_per_tick = 0.375f;

	// update the values based on tickrate
	void updateValues()
	{
		if (!time_globals::get()->initialized)
			return;

		melee_lunge_min_speed_per_tick = (0.375f * 30.f) * time_globals::get()->seconds_per_tick;
		unk_time_4 = (0.25f * 30.f) * time_globals::get()->seconds_per_tick; // convert 7.5 ticks to seconds, then back to ticks
		unk_time_5 = (0.087155744f * 30.f) * time_globals::get()->seconds_per_tick; // convert 2.61 ticks to seconds, then back to ticks

		melee_lunge_distance_factor = 0.33333334f * ((float)time_globals::get()->ticks_per_second / 30.f);
		melee_lunge_distance_factor2 = 4.f * ((float)time_globals::get()->ticks_per_second / 30.f);
		melee_lunge_distance_factor3 = 3.f * ((float)time_globals::get()->ticks_per_second / 30.f);
	}

	void applyHooks()
	{
		WritePointer(h2mod->GetAddress(0x10BAE0, 0xFDB5B) + 4, &unk_time_4);
		WritePointer(h2mod->GetAddress(0x10BAAC, 0xFE0DC) + 4, &unk_time_5);

		if (!h2mod->Server)
		{
			WritePointer(h2mod->GetAddress(0x10B094, 0x0) + 4, &melee_lunge_min_speed_per_tick);
			WritePointer(h2mod->GetAddress(0x10B655, 0x0) + 4, &melee_lunge_min_speed_per_tick);
		}

		//WriteValue<BYTE>(h2mod->GetAddress(0x10B408, 0xFDA38) + 2, toggle ? 5 : 6); // sword
		//WriteValue<BYTE>(h2mod->GetAddress(0x10B40B, 0xFDA3B) + 2, toggle ? 2 : 1); // generic weapon

		//unk3 = 5.f * ((float)tickrate / 30.f);
		//WritePointer(h2mod->GetAddress(0x10B7ED, 0xFDE1D) + 4, &unk3);

		WritePointer(h2mod->GetAddress(0x10B160, 0xFD790) + 4, &melee_lunge_distance_factor);
		WritePointer(h2mod->GetAddress(0x10B354, 0xFD984) + 4, &melee_lunge_distance_factor); // distance when mid air
		WritePointer(h2mod->GetAddress(0x10B6A3, 0xFDCD3) + 4, &melee_lunge_distance_factor); // acceleration
		WritePointer(h2mod->GetAddress(0x10BB78, 0xFE1A8) + 4, &melee_lunge_distance_factor);

		WritePointer(h2mod->GetAddress(0x10B17D, 0xFD7AD) + 4, &melee_lunge_distance_factor2);
		WritePointer(h2mod->GetAddress(0x10B37A, 0xFD9AA) + 4, &melee_lunge_distance_factor2);

		if (!h2mod->Server)
		{
			WritePointer(h2mod->GetAddress(0x10B16F, 0x0) + 4, &melee_lunge_distance_factor3);
			WritePointer(h2mod->GetAddress(0x10B363, 0x0) + 4, &melee_lunge_distance_factor3);
			WritePointer(h2mod->GetAddress(0x10B6EE, 0x0) + 4, &melee_lunge_distance_factor3);
			WritePointer(h2mod->GetAddress(0x10B8BD, 0x0) + 4, &melee_lunge_distance_factor3);
			WritePointer(h2mod->GetAddress(0x10B963, 0x0) + 4, &melee_lunge_distance_factor3);
		}
	}
}