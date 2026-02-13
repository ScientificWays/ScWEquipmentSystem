// Scientific Ways

#pragma once

#include "ScWEquipmentSystem.h"

#include "Equipment/ScWEquipmentFragment.h"

#include "Animation/ScWTypes_Animation.h"

#include "ScWEquipmentFragment_ThirdPerson.generated.h"

#define MODULE_API SCWEQUIPMENTSYSTEM_API

/**
 *
 */
UCLASS(MinimalAPI, Blueprintable, BlueprintType, meta = (DisplayName = "[ScW] Equipment Fragment (Owner Mesh)"))
class UScWEquipmentFragment_ThirdPerson : public UScWEquipmentFragment
{
	GENERATED_BODY()

//~ Begin Initialize
public:
	MODULE_API UScWEquipmentFragment_ThirdPerson();
protected:
	MODULE_API  void BP_OnEquipped_Implementation(class UScWEquipmentInstance* InInstance) override; // UScWEquipmentFragment
	MODULE_API virtual void BP_OnUnequipped_Implementation(class UScWEquipmentInstance* InInstance) override; // UScWEquipmentFragment
//~ End Initialize
	
//~ Begin Attachment
public:

	UPROPERTY(Category = "Attachment", EditDefaultsOnly, BlueprintReadOnly)
	FName MeshAttachmentSocketName;
//~ End Attachment

//~ Begin Animations
public:

	UPROPERTY(Category = "Animations", EditDefaultsOnly, BlueprintReadOnly)
	FScWThirdPersonStaticAnimationData StaticAnimationData;
//~ End Animations
};

#undef MODULE_API
