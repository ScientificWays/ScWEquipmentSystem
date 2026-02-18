// Scientific Ways

#pragma once

#include "ScWEquipmentSystem.h"

#include "AbilitySystem/ScWAbilitySet.h"

#include "EquipmentSystem/ScWEquipmentFragment.h"

#include "ScWEquipmentFragment_AbilitySets.generated.h"

#define MODULE_API SCWEQUIPMENTSYSTEM_API

/**
 *
 */
UCLASS(MinimalAPI, Blueprintable, BlueprintType, meta = (DisplayName = "[ScW] Equipment Fragment (Ability Sets)"))
class UScWEquipmentFragment_AbilitySets : public UScWEquipmentFragment
{
	GENERATED_BODY()

//~ Begin Initialize
public:
	MODULE_API UScWEquipmentFragment_AbilitySets();
protected:
	MODULE_API virtual void BP_OnEquipped_Implementation(class UScWEquipmentInstance* InInstance) const override; // UScWEquipmentFragment
	MODULE_API virtual void BP_OnUnequipped_Implementation(class UScWEquipmentInstance* InInstance) const override; // UScWEquipmentFragment
//~ End Initialize
	
//~ Begin Ability Sets
public:

	UPROPERTY(Category = "Ability Sets", EditDefaultsOnly)
	TArray<TObjectPtr<const UScWAbilitySet>> AbilitySetsToGrant;

protected:
	void GiveAbilitySet(UScWEquipmentInstance* InInstance) const;
	void TakeAbilitySet(UScWEquipmentInstance* InInstance) const;

	// Authority-only list of granted handles
	UPROPERTY()
	mutable TMap<TObjectPtr<UScWEquipmentInstance>, FScWAbilitySet_GrantedHandles> PerInstanceGrantedHandles;
//~ End Ability Sets
};

#undef MODULE_API
