// Scientific Ways

#pragma once

#include "ScWEquipmentSystem.h"

#include "ScWEquipmentFragment.generated.h"

#define MODULE_API SCWEQUIPMENTSYSTEM_API

class UScWEquipmentInstance;

/**
 *
 */
UCLASS(MinimalAPI, Abstract, Blueprintable, BlueprintType, DefaultToInstanced, EditInlineNew, meta = (DisplayName = "[ScW] Handheld Fragment Base"))
class UScWEquipmentFragment : public UObject
{
	GENERATED_BODY()

//~ Begin Initialize
public:
	UScWEquipmentFragment();
protected:

	UFUNCTION(Category = "Initialize", BlueprintNativeEvent, meta = (DisplayName = "On Equipped"))
	MODULE_API void BP_OnEquipped(UScWEquipmentInstance* InInstance);
	MODULE_API virtual void BP_OnEquipped_Implementation(UScWEquipmentInstance* InInstance) { ensure(false); }

	UFUNCTION(Category = "Initialize", BlueprintNativeEvent, meta = (DisplayName = "On Unequipped"))
	MODULE_API void BP_OnUnequipped(UScWEquipmentInstance* InInstance);
	MODULE_API virtual void BP_OnUnequipped_Implementation(UScWEquipmentInstance* InInstance) { ensure(false); }
//~ End Initialize
};

#undef MODULE_API
