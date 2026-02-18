// Scientific Ways

#include "EquipmentSystem/ScWEquipmentFragment_SpawnActors.h"

#include "EquipmentSystem/ScWEquipmentInstance.h"
#include "EquipmentSystem/ScWEquipmentDefinition.h"
#include "EquipmentSystem/ScWEquipmentFunctionLibrary.h"
#include "EquipmentSystem/ScWEquipmentManagerComponent.h"

//~ Begin Statics
TArray<AActor*> UScWEquipmentFragment_SpawnActors::GetAllEquipmentSpawnedActors(UScWEquipmentManagerComponent* InEquipmentManagerComponent)
{
	TArray<AActor*> OutActors;
	ensureReturn(InEquipmentManagerComponent, OutActors);

	for (const FScWAppliedEquipmentEntry& Entry : InEquipmentManagerComponent->GetEquipmentList().GetEntries())
	{
		TSubclassOf<UScWEquipmentDefinition> DefinitionClass = Entry.GetDefinitionClass();
		ensureContinue(DefinitionClass);

		const UScWEquipmentDefinition* DefinitionCDO = GetDefault<UScWEquipmentDefinition>(DefinitionClass);
		ensureContinue(DefinitionCDO);

		const UScWEquipmentFragment_SpawnActors* Fragment = DefinitionCDO->GetFragmentByClass<UScWEquipmentFragment_SpawnActors>();
		ensureContinue(Fragment);

		OutActors.Append(Fragment->GetSpawnedActors(Entry.GetInstance()));
	}
	return OutActors;
}

AActor* UScWEquipmentFragment_SpawnActors::GetEquipmentSpawnedActorByClass(UScWEquipmentManagerComponent* InEquipmentManagerComponent, TSubclassOf<AActor> InClass)
{
	ensureReturn(InEquipmentManagerComponent, nullptr);

	for (const FScWAppliedEquipmentEntry& Entry : InEquipmentManagerComponent->GetEquipmentList().GetEntries())
	{
		TSubclassOf<UScWEquipmentDefinition> DefinitionClass = Entry.GetDefinitionClass();
		ensureContinue(DefinitionClass);

		const UScWEquipmentDefinition* DefinitionCDO = GetDefault<UScWEquipmentDefinition>(DefinitionClass);
		ensureContinue(DefinitionCDO);

		const UScWEquipmentFragment_SpawnActors* Fragment = DefinitionCDO->GetFragmentByClass<UScWEquipmentFragment_SpawnActors>();
		ensureContinue(Fragment);

		for (AActor* SampleActor : Fragment->GetSpawnedActors(Entry.GetInstance()))
		{
			ensureContinue(SampleActor);
			if (SampleActor->IsA(InClass))
			{
				return SampleActor;
			}
		}
	}
	return nullptr;
}
//~ End Statics

//~ Begin Initialize
UScWEquipmentFragment_SpawnActors::UScWEquipmentFragment_SpawnActors()
{
	
}

void UScWEquipmentFragment_SpawnActors::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const // UObject
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//DOREPLIFETIME(ThisClass, PerInstanceMemory);
}

void UScWEquipmentFragment_SpawnActors::BP_OnEquipped_Implementation(UScWEquipmentInstance* InInstance) const // UScWEquipmentFragment
{
	ensureReturn(InInstance);

	APawn* OwningPawn = InInstance->GetPawn();
	ensureReturn(OwningPawn);

	UWorld* World = OwningPawn->GetWorld();
	ensureReturn(World);

	FScWSpawnActorsMemory& InstanceMemory = PerInstanceMemory.FindOrAdd(InInstance);

	for (const FScWEquipmentActorToSpawn& SpawnInfo : ActorsToSpawn)
	{
		USceneComponent* AttachTarget = GetAttachmentForActor(InInstance, SpawnInfo);
		ensureContinue(AttachTarget);

		AActor* NewActor = World->SpawnActorDeferred<AActor>(SpawnInfo.ActorToSpawn, FTransform::Identity, OwningPawn, OwningPawn);
		NewActor->FinishSpawning(FTransform::Identity, /*bIsDefaultTransform=*/ true);
		NewActor->SetActorRelativeTransform(SpawnInfo.AttachTransform);

		NewActor->AttachToComponent(AttachTarget, FAttachmentTransformRules(EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, SpawnInfo.bIgnoreAttachTargetScale ? EAttachmentRule::KeepWorld : EAttachmentRule::SnapToTarget, true), SpawnInfo.AttachSocket);
		InstanceMemory.SpawnedActors.Add(NewActor);
	}
}

void UScWEquipmentFragment_SpawnActors::BP_OnUnequipped_Implementation(UScWEquipmentInstance* InInstance) const // UScWEquipmentFragment
{
	ensureReturn(InInstance);

	const FScWSpawnActorsMemory& InstanceMemory = PerInstanceMemory.FindOrAdd(InInstance);
	for (AActor* Actor : InstanceMemory.SpawnedActors)
	{
		if (Actor)
		{
			Actor->Destroy();
		}
	}
	PerInstanceMemory.Remove(InInstance);
}
//~ End Initialize

//~ Begin Actors
TArray<AActor*> UScWEquipmentFragment_SpawnActors::GetSpawnedActors(const UScWEquipmentInstance* InInstance) const
{
	if (PerInstanceMemory.Contains(InInstance))
	{
		return PerInstanceMemory[InInstance].SpawnedActors;
	}
	return {};
}

USceneComponent* UScWEquipmentFragment_SpawnActors::GetAttachmentForActor(UScWEquipmentInstance* InInstance, const FScWEquipmentActorToSpawn& InSpawnInfo) const
{
	switch (InSpawnInfo.AttachMesh)
	{
		case EScWEquipmentMesh::FirstPerson:
		{
			return UScWEquipmentFunctionLibrary::GetEquipmentFirstPersonMesh(InInstance);
		}
		case EScWEquipmentMesh::ThirdPerson:
		{
			return UScWEquipmentFunctionLibrary::GetEquipmentThirdPersonMesh(InInstance);
		}
		case EScWEquipmentMesh::Custom:
		{
			return BP_GetCustomAttachmentForActor(InInstance, InSpawnInfo);
		}
	}
	ensureReturn(false, nullptr);
}
//~ End Actors
