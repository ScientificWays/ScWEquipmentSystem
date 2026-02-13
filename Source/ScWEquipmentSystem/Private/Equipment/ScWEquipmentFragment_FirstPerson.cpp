// Scientific Ways

#include "Equipment/ScWEquipmentFragment_FirstPerson.h"

#include "Animation/ScWAnimInstance_FirstPerson.h"

#include "Equipment/ScWEquipmentInstance.h"
#include "Equipment/ScWEquipmentFunctionLibrary.h"

//~ Begin Initialize
UScWEquipmentFragment_FirstPerson::UScWEquipmentFragment_FirstPerson()
{
	MeshAttachmentSocketName = TEXT("EquipmentSocket");
}

void UScWEquipmentFragment_FirstPerson::BP_OnEquipped_Implementation(UScWEquipmentInstance* InInstance) // UScWEquipmentFragment
{
	ensureReturn(InInstance);

	USkeletalMeshComponent* AttachMesh = UScWEquipmentFunctionLibrary::GetEquipmentFirstPersonMesh(InInstance);
	AActor* PrimaryActor = InInstance->GetPrimarySpawnedActor();
	ensureReturn(AttachMesh && PrimaryActor);
	
	PrimaryActor->AttachToComponent(AttachMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, MeshAttachmentSocketName);

	if (UScWAnimInstance_FirstPerson* AnimInstance = Cast<UScWAnimInstance_FirstPerson>(AttachMesh->GetAnimInstance()))
	{
		AnimInstance->SetCurrentStaticAnimationData(StaticAnimationData);
	}
}

void UScWEquipmentFragment_FirstPerson::BP_OnUnequipped_Implementation(UScWEquipmentInstance* InInstance) // UScWEquipmentFragment
{
	ensureReturn(InInstance);

	USkeletalMeshComponent* AttachMesh = UScWEquipmentFunctionLibrary::GetEquipmentFirstPersonMesh(InInstance);
	AActor* PrimaryActor = InInstance->GetPrimarySpawnedActor();
	ensureReturn(AttachMesh && PrimaryActor);

	if (PrimaryActor->GetRootComponent()->GetAttachParent() == AttachMesh)
	{
		PrimaryActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	}
	if (UScWAnimInstance_FirstPerson* AnimInstance = Cast<UScWAnimInstance_FirstPerson>(AttachMesh->GetAnimInstance()))
	{
		AnimInstance->ResetToDefaultStaticAnimationData();
	}
}
//~ End Initialize
