// Scientific Ways

#pragma once

#include "ScWEquipmentSystem.h"

#include "EquipmentSystem/ScWEquipmentFragment.h"

#include "ScWEquipmentFragment_UI.generated.h"

#define MODULE_API SCWEQUIPMENTSYSTEM_API

/**
 *
 */
UCLASS(MinimalAPI, Blueprintable, BlueprintType, meta = (DisplayName = "[ScW] Equipment Fragment (UI)"))
class UScWEquipmentFragment_UI : public UScWEquipmentFragment
{
	GENERATED_BODY()
	
//~ Begin Initialize
public:
	MODULE_API UScWEquipmentFragment_UI();
protected:
	MODULE_API virtual void BP_OnEquipped_Implementation(class UScWEquipmentInstance* InInstance) override; // UScWEquipmentFragment
	MODULE_API virtual void BP_OnUnequipped_Implementation(class UScWEquipmentInstance* InInstance) override; // UScWEquipmentFragment
//~ End Initialize

//~ Begin UI
public:

	UPROPERTY(Category = "UI", EditDefaultsOnly, BlueprintReadOnly)
	FText DisplayName;
//~ End UI
};

#undef MODULE_API
