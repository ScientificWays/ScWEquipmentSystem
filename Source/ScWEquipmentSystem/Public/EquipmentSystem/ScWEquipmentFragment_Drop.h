// Scientific Ways

#pragma once

#include "ScWEquipmentSystem.h"

#include "EquipmentSystem/ScWEquipmentFragment.h"

#include "ScWEquipmentFragment_Drop.generated.h"

#define MODULE_API SCWEQUIPMENTSYSTEM_API

/**
 *
 */
UCLASS(MinimalAPI, Blueprintable, BlueprintType, meta = (DisplayName = "[ScW] Equipment Fragment (Drop)"))
class UScWEquipmentFragment_Drop : public UScWEquipmentFragment
{
	GENERATED_BODY()
	
//~ Begin Initialize
public:
	MODULE_API UScWEquipmentFragment_Drop();
protected:
	MODULE_API virtual void BP_OnEquipped_Implementation(class UScWEquipmentInstance* InInstance) override; // UScWEquipmentFragment
	MODULE_API virtual void BP_OnUnequipped_Implementation(class UScWEquipmentInstance* InInstance) override; // UScWEquipmentFragment
//~ End Initialize

//~ Begin Drop
public:

	UPROPERTY(Category = "Drop", EditDefaultsOnly, BlueprintReadOnly)
	bool bCanDrop;

	UPROPERTY(Category = "Drop", EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition = "bCanDrop"))
	bool bDropOnDeath;
//~ End Drop
};

#undef MODULE_API
