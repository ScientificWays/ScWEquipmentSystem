// Scientific Ways

#pragma once

#include "ScWEquipmentSystem.h"

#include "Equipment/ScWEquipmentFragment.h"

#include "ScWEquipmentFragment_MeleeBase.generated.h"

#define MODULE_API SCWEQUIPMENTSYSTEM_API

/**
 *
 */
UCLASS(MinimalAPI, Abstract, Blueprintable, BlueprintType, DefaultToInstanced, EditInlineNew, meta = (DisplayName = "[ScW] Equipment Fragment (Melee Base)"))
class UScWEquipmentFragment_MeleeBase : public UScWEquipmentFragment
{
	GENERATED_BODY()
	
//~ Begin Initialize
public:
	MODULE_API UScWEquipmentFragment_MeleeBase();
protected:
	MODULE_API virtual void BP_OnEquipped_Implementation(class UScWEquipmentInstance* InInstance) override; // UScWEquipmentFragment
	MODULE_API virtual void BP_OnUnequipped_Implementation(class UScWEquipmentInstance* InInstance) override; // UScWEquipmentFragment
//~ End Initialize
	
//~ Begin Swing
public:

	UFUNCTION(Category = "Swing", BlueprintCallable, BlueprintNativeEvent)
	MODULE_API float BP_ModifySwingBaseDamage(const class AActor* InMeleeEquipmentActor, float InDamage) const;

	UFUNCTION(Category = "Swing", BlueprintCallable, BlueprintNativeEvent)
	MODULE_API TSubclassOf<class UScWDamageType> BP_ModifySwingBaseDamageTypeClass(const class AActor* InMeleeEquipmentActor, TSubclassOf<class UScWDamageType> InDamageTypeClass) const;
//~ End Swing

//~ Begin Swing Variants
public:

	UFUNCTION(Category = "Swing Variants", BlueprintCallable, BlueprintNativeEvent, meta = (DisplayName = "Modify Swing Variants"))
	MODULE_API void BP_ModifySwingVariants(const class AActor* InMeleeEquipmentActor, TArray<struct FScWMeleeSwingVariantData>& InOutVariants) const;
//~ End Swing Variants
};

#undef MODULE_API
