// Scientific Ways

#pragma once

#include "ScWEquipmentSystem.h"

#include "ScWEquipmentFragment.generated.h"

#define MODULE_API SCWEQUIPMENTSYSTEM_API

class UScWEquipmentInstance;

/**
 *
 */
UCLASS(MinimalAPI, Const, Abstract, Blueprintable, BlueprintType, DefaultToInstanced, EditInlineNew, meta = (DisplayName = "[ScW] Handheld Fragment Base"))
class UScWEquipmentFragment : public UObject
{
	GENERATED_BODY()

//~ Begin Initialize
public:
	UScWEquipmentFragment();

	UFUNCTION(Category = "Initialize", BlueprintNativeEvent, meta = (DisplayName = "On Equipped"))
	MODULE_API void BP_OnEquipped(UScWEquipmentInstance* InInstance) const;
	MODULE_API virtual void BP_OnEquipped_Implementation(UScWEquipmentInstance* InInstance) const { ensure(false); }

	UFUNCTION(Category = "Initialize", BlueprintNativeEvent, meta = (DisplayName = "On Unequipped"))
	MODULE_API void BP_OnUnequipped(UScWEquipmentInstance* InInstance) const;
	MODULE_API virtual void BP_OnUnequipped_Implementation(UScWEquipmentInstance* InInstance) const { ensure(false); }
//~ End Initialize
};

#undef MODULE_API
