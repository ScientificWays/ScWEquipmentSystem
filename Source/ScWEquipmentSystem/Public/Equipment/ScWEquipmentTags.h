// Scientific Ways

#pragma once

#include "ScWEquipmentSystem.h"

#include "NativeGameplayTags.h"

#define MODULE_API SCWEQUIPMENTSYSTEM_API

namespace FScWEquipmentTags
{
	MODULE_API FGameplayTag FindTagByString(const FString& TagString, bool bMatchPartialString = false);

	// Equipment Types
	MODULE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Equipment_Type_Weapon);
}

#undef MODULE_API
