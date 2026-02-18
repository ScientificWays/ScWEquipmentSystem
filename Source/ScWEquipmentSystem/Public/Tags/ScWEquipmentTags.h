// Scientific Ways

#pragma once

#include "ScWEquipmentSystem.h"

#include "NativeGameplayTags.h"

#define MODULE_API SCWEQUIPMENTSYSTEM_API

namespace FScWEquipmentTags
{
	// Equipment Types
	MODULE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Equipment_Type_Weapon);

	// Messages
	MODULE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayMessage_Equipment);
	MODULE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayMessage_Equipment_InstanceEquipped);
	MODULE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayMessage_Equipment_InstanceUnequipped);
}

#undef MODULE_API
