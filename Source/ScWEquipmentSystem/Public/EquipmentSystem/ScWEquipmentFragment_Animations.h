// Scientific Ways

#pragma once

#include "ScWEquipmentSystem.h"

#include "Animation/ScWTypes_Animation.h"
#include "EquipmentSystem/ScWEquipmentFragment.h"

#include "ScWEquipmentFragment_Animations.generated.h"

#define MODULE_API SCWEQUIPMENTSYSTEM_API

/**
 *
 */
UCLASS(MinimalAPI, Const, Blueprintable, BlueprintType, meta = (DisplayName = "[ScW] Equipment Fragment (Animations)"))
class UScWEquipmentFragment_Animations : public UScWEquipmentFragment
{
	GENERATED_BODY()

//~ Begin Initialize
public:
	MODULE_API UScWEquipmentFragment_Animations();
protected:
	MODULE_API virtual void BP_OnEquipped_Implementation(class UScWEquipmentInstance* InInstance) const override; // UScWEquipmentFragment
	MODULE_API virtual void BP_OnUnequipped_Implementation(class UScWEquipmentInstance* InInstance) const override; // UScWEquipmentFragment
//~ End Initialize

//~ Begin Animations
public:

	UPROPERTY(Category = "Animations", EditDefaultsOnly, BlueprintReadOnly, meta = (InlineEditConditionToggle))
	bool bApplyFirstPersonStaticAnimationData;

	UPROPERTY(Category = "Animations", EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition = "bApplyFirstPersonStaticAnimationData"))
	FScWFirstPersonStaticAnimationData FirstPersonStaticAnimationData;

	UPROPERTY(Category = "Animations", EditDefaultsOnly, BlueprintReadOnly, meta = (InlineEditConditionToggle))
	bool bApplyThirdPersonStaticAnimationData;

	UPROPERTY(Category = "Animations", EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition = "bApplyThirdPersonStaticAnimationData"))
	FScWThirdPersonStaticAnimationData ThirdPersonStaticAnimationData;
//~ End Animations
};

#undef MODULE_API
