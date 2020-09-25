
#include "PhysicsPatches.h"

#include "Biped.h"
#include "Melee.h"
#include "Projectiles.h"

namespace PhysicsPatches
{
	// update values depending on tickrate
	void updateValues()
	{
		Melee::updateValues();
		Biped::updateValues();
	}

	// apply any hooks needed
	void ApplyHooks()
	{
		Melee::applyHooks();
		Biped::applyHooks();
		Projectiles::applyHooks();
	}

	// update tag values
	void OnMapLoad()
	{
		Projectiles::ApplyVelocity();
		Biped::applyFallDamagePatch();
	}
}

