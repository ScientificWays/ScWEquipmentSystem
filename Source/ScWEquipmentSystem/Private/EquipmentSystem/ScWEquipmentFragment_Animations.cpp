// Scientific Ways

#include "EquipmentSystem/ScWEquipmentFragment_Animations.h"

#include "Animation/ScWAnimInstance_FirstPerson.h"
#include "Animation/ScWAnimInstance_ThirdPerson.h"

#include "EquipmentSystem/ScWEquipmentInstance.h"
#include "EquipmentSystem/ScWEquipmentFunctionLibrary.h"

//~ Begin Initialize
UScWEquipmentFragment_Animations::UScWEquipmentFragment_Animations()
{
	
}

void UScWEquipmentFragment_Animations::BP_OnEquipped_Implementation(UScWEquipmentInstance* InInstance) const // UScWEquipmentFragment
{
	ensureReturn(InInstance);

	if (bApplyFirstPersonStaticAnimationData)
	{
		USkeletalMeshComponent* FirstPesonMesh = UScWEquipmentFunctionLibrary::GetEquipmentFirstPersonMesh(InInstance);
		ensureReturn(FirstPesonMesh);

		UScWAnimInstance_FirstPerson* AnimInstance = Cast<UScWAnimInstance_FirstPerson>(FirstPesonMesh->GetAnimInstance());
		ensureReturn(AnimInstance);
		AnimInstance->SetCurrentStaticAnimationData(FirstPersonStaticAnimationData);
	}
	if (bApplyThirdPersonStaticAnimationData)
	{
		USkeletalMeshComponent* ThirdPesonMesh = UScWEquipmentFunctionLibrary::GetEquipmentThirdPersonMesh(InInstance);
		ensureReturn(ThirdPesonMesh);

		UScWAnimInstance_ThirdPerson* AnimInstance = Cast<UScWAnimInstance_ThirdPerson>(ThirdPesonMesh->GetAnimInstance());
		ensureReturn(AnimInstance);
		AnimInstance->SetCurrentStaticAnimationData(ThirdPersonStaticAnimationData);
	}
}

void UScWEquipmentFragment_Animations::BP_OnUnequipped_Implementation(UScWEquipmentInstance* InInstance) const // UScWEquipmentFragment
{
	ensureReturn(InInstance);

	if (bApplyFirstPersonStaticAnimationData)
	{
		USkeletalMeshComponent* FirstPesonMesh = UScWEquipmentFunctionLibrary::GetEquipmentFirstPersonMesh(InInstance);
		ensureReturn(FirstPesonMesh);

		UScWAnimInstance_FirstPerson* AnimInstance = Cast<UScWAnimInstance_FirstPerson>(FirstPesonMesh->GetAnimInstance());
		ensureReturn(AnimInstance);
		AnimInstance->ResetToDefaultStaticAnimationData();
	}
	if (bApplyThirdPersonStaticAnimationData)
	{
		USkeletalMeshComponent* ThirdPesonMesh = UScWEquipmentFunctionLibrary::GetEquipmentThirdPersonMesh(InInstance);
		ensureReturn(ThirdPesonMesh);

		UScWAnimInstance_ThirdPerson* AnimInstance = Cast<UScWAnimInstance_ThirdPerson>(ThirdPesonMesh->GetAnimInstance());
		ensureReturn(AnimInstance);
		AnimInstance->ResetToDefaultStaticAnimationData();
	}
}
//~ End Initialize
