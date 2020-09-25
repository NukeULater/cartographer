
#include "H2MOD.h"

#include "GameTime.h"

time_globals* time_globals::get()
{
	return *h2mod->GetAddress<time_globals**>(0x4C06E4, 0x4CF0EC);
}