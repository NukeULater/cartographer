
#include "Biped.h"

#include "H2MOD.h"
#include "H2MOD/Tags/TagInterface.h"

#include "Util\Hooks\Hook.h"

#include "Blam/Engine/Game/GameTime.h"

namespace Biped
{
	static float edgeDropFactor = 0.117f;

	void updateValues()
	{
		if (!time_globals::get()->initialized)
			return;

		edgeDropFactor = (0.117f * 30.f) * time_globals::get()->seconds_per_tick; // convert seconds to ticks, then back to seconds but adjusted to the tickrate
	}

	void applyHooks()
	{
		// fixes edge drop fast fall when using higher tickrates than 30
		WritePointer(h2mod->GetAddress(0x106E23, 0xF9143) + 4, &edgeDropFactor);
	}

	void applyFallDamagePatch()
	{
		//DWORD map_checksum = *(DWORD*)(h2mod->GetAddress(0x47CD68) + 0x2F0);

		//*(float*)(FloatOffsets + 0xE610A0) = 5.0f; /*masterchief_mp hlmt min game acc default value doubled*/
		//*(float*)(FloatOffsets + 0xE610A4) = 9.0f; /*masterchief_mp hlmt max game acc default value doubled*/

		*(float*)(&tags::get_tag_data()[0xE610B0]) = 14.0f; /*masterchief_mp hlmt max abs acc default value doubled*/
		*(float*)(&tags::get_tag_data()[0xE610B4]) = 20.0f; /*masterchief_mp hlmt max abs acc default value doubled*/

		//*(float*)(FloatOffsets + 0xE65D88) = 5.0f; /*elite_mp hlmt min game acc default value doubled*/
		//*(float*)(FloatOffsets + 0xE65D8C) = 9.0f; /*elite_mp hlmt max game acc default value doubled*/

		*(float*)(&tags::get_tag_data()[0xE65D98]) = 14.0f; /*elite_mp hlmt max abs acc default value doubled*/
		*(float*)(&tags::get_tag_data()[0xE65D9C]) = 20.0f; /*elite_mp hlmt max abs acc default value doubled*/
	}
}