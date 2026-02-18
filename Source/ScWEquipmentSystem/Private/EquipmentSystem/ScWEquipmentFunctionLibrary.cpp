// Scientific Ways

#include "EquipmentSystem/ScWEquipmentFunctionLibrary.h"

#include "Character/ScWCharacterMesh_FirstPerson.h"
#include "Character/ScWCharacterMesh_ThirdPerson.h"

#include "EquipmentSystem/ScWEquipmentInstance.h"
#include "EquipmentSystem/ScWEquipmentAvatarInterface.h"
#include "EquipmentSystem/ScWEquipmentManagerComponent.h"

//~ Begin Equipment Manager
UScWEquipmentManagerComponent* UScWEquipmentFunctionLibrary::GetEquipmentManagerComponentFromActor(const AActor* InActor)
{
	if (!InActor)
	{
		return nullptr;
	}
	//ensureReturn(InActor, nullptr);

	if (InActor->Implements<UScWEquipmentAvatarInterface>())
	{
		return IScWEquipmentAvatarInterface::Execute_BP_GetEquipmentManagerComponent(InActor);
	}
	return InActor->FindComponentByClass<UScWEquipmentManagerComponent>();
}
//~ End Equipment Manager

//~ Begin Meshes
USkeletalMeshComponent* UScWEquipmentFunctionLibrary::GetEquipmentFirstPersonMesh(const UScWEquipmentInstance* InInstance)
{
	ensureReturn(InInstance, nullptr);

	APawn* OwnerPawn = InInstance->GetPawn();
	ensureReturn(OwnerPawn, nullptr);

	if (OwnerPawn->Implements<UScWEquipmentAvatarInterface>())
	{
		return IScWEquipmentAvatarInterface::Execute_BP_GetEquipmentFirstPersonMesh(OwnerPawn);
	}
	else if (UScWCharacterMesh_FirstPerson* FirstPersonMesh = OwnerPawn->FindComponentByClass<UScWCharacterMesh_FirstPerson>())
	{
		return FirstPersonMesh;
	}
	return nullptr;
}

USkeletalMeshComponent* UScWEquipmentFunctionLibrary::GetEquipmentThirdPersonMesh(const UScWEquipmentInstance* InInstance)
{
	ensureReturn(InInstance, nullptr);

	APawn* OwnerPawn = InInstance->GetPawn();
	ensureReturn(OwnerPawn, nullptr);
	
	if (OwnerPawn->Implements<UScWEquipmentAvatarInterface>())
	{
		return IScWEquipmentAvatarInterface::Execute_BP_GetEquipmentThirdPersonMesh(OwnerPawn);
	}
	else if (UScWCharacterMesh_ThirdPerson* ThirdPersonMesh = OwnerPawn->FindComponentByClass<UScWCharacterMesh_ThirdPerson>())
	{
		return ThirdPersonMesh;
	}
	return nullptr;
}
//~ End Meshes

//~ Begin Actors
/*AActor* UScWEquipmentFunctionLibrary::GetOwnerEquipmentActorWithDefinitionTag(const AActor* InOwnerActor, const FGameplayTag& InEquipmentTag)
{
	UScWEquipmentManagerComponent* OwnerManager = GetEquipmentManagerComponentFromActor(InOwnerActor);
	ensureReturn(OwnerManager, nullptr);
	
	UScWEquipmentInstance* TagInstance = OwnerManager->GetFirstInstanceWithDefinitionTag(InEquipmentTag);
	return TagInstance->GetSpawnedActors();
}*/
//~ End Actors

//~ Begin Abilities
UScWEquipmentInstance* UScWEquipmentFunctionLibrary::GetAbilityAssociatedEquipment(const UGameplayAbility* InAbility) const
{
	ensureReturn(InAbility, nullptr);
	if (FGameplayAbilitySpec* Spec = InAbility->GetCurrentAbilitySpec())
	{
		return Cast<UScWEquipmentInstance>(Spec->SourceObject.Get());
	}
	return nullptr;
}

/*UScWInventoryItemInstance* UScWEquipmentFunctionLibrary::GetAbilityAssociatedItem(const UGameplayAbility* InAbility) const
{
	ensureReturn(InAbility, nullptr);
	if (UScWEquipmentInstance* Equipment = GetAssociatedEquipment(InAbility))
	{
		return Cast<UScWInventoryItemInstance>(Equipment->GetInstigator());
	}
	return nullptr;
}*/
//~ End Abilities
