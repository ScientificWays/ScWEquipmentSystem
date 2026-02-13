// Scientific Ways

#include "Equipment/ScWEquipmentFragment_ThirdPerson.h"

#include "Animation/ScWAnimInstance_ThirdPerson.h"

#include "Equipment/ScWEquipmentInstance.h"
#include "Equipment/ScWEquipmentFunctionLibrary.h"

#include "Character/ScWCharacterMesh_ThirdPerson.h"

//~ Begin Initialize
UScWEquipmentFragment_ThirdPerson::UScWEquipmentFragment_ThirdPerson()
{
	MeshAttachmentSocketName = TEXT("EquipmentSocket");
}

void UScWEquipmentFragment_ThirdPerson::BP_OnEquipped_Implementation(UScWEquipmentInstance* InInstance) // UScWEquipmentFragment
{
	ensureReturn(InInstance);

	AActor* PrimaryActor = InInstance->GetPrimarySpawnedActor();
	USkeletalMeshComponent* AttachMesh = UScWEquipmentFunctionLibrary::GetEquipmentThirdPersonMesh(InInstance);
	ensureReturn(AttachMesh && PrimaryActor);
	
	PrimaryActor->AttachToComponent(AttachMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, MeshAttachmentSocketName);

	if (UScWAnimInstance_ThirdPerson* AnimInstance = Cast<UScWAnimInstance_ThirdPerson>(AttachMesh->GetAnimInstance()))
	{
		AnimInstance->SetCurrentStaticAnimationData(StaticAnimationData);
	}
}

void UScWEquipmentFragment_ThirdPerson::BP_OnUnequipped_Implementation(UScWEquipmentInstance* InInstance) // UScWEquipmentFragment
{
	ensureReturn(InInstance);

	AActor* PrimaryActor = InInstance->GetPrimarySpawnedActor();
	USkeletalMeshComponent* AttachMesh = UScWEquipmentFunctionLibrary::GetEquipmentThirdPersonMesh(InInstance);
	ensureReturn(AttachMesh && PrimaryActor);

	if (PrimaryActor->GetRootComponent()->GetAttachParent() == AttachMesh)
	{
		PrimaryActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	}
	if (UScWAnimInstance_ThirdPerson* AnimInstance = Cast<UScWAnimInstance_ThirdPerson>(AttachMesh->GetAnimInstance()))
	{
		AnimInstance->ResetToDefaultStaticAnimationData();
	}
}
//~ End Initialize
