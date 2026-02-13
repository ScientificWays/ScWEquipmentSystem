// Scientific Ways

#include "Equipment/ScWEquipmentManagerComponent.h"

#include "AbilitySystem/ScWAbilitySystemComponent.h"

#include "Equipment/ScWEquipmentInstance.h"
#include "Equipment/ScWEquipmentDefinition.h"

#include "Engine/ActorChannel.h"
#include "AbilitySystemGlobals.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ScWEquipmentManagerComponent)

class FLifetimeProperty;
struct FReplicationFlags;

//////////////////////////////////////////////////////////////////////
// FScWAppliedEquipmentEntry

FString FScWAppliedEquipmentEntry::GetDebugString() const
{
	return FString::Printf(TEXT("%s of %s"), *GetNameSafe(Instance), *GetNameSafe(EquipmentDefinition.Get()));
}

//////////////////////////////////////////////////////////////////////
// FScWEquipmentList

void FScWEquipmentList::PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize)
{
 	for (int32 Index : RemovedIndices)
 	{
 		const FScWAppliedEquipmentEntry& Entry = Entries[Index];
		if (Entry.Instance != nullptr)
		{
			Entry.Instance->OnUnequipped();
		}
 	}
}

void FScWEquipmentList::PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize)
{
	for (int32 Index : AddedIndices)
	{
		const FScWAppliedEquipmentEntry& Entry = Entries[Index];
		if (Entry.Instance != nullptr)
		{
			Entry.Instance->OnEquipped();
		}
	}
}

void FScWEquipmentList::PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize)
{
// 	for (int32 Index : ChangedIndices)
// 	{
// 		const FGameplayTagStack& Stack = Stacks[Index];
// 		TagToCountMap[Stack.Tag] = Stack.StackCount;
// 	}
}

UScWAbilitySystemComponent* FScWEquipmentList::GetAbilitySystemComponent() const
{
	check(OwnerComponent);
	AActor* OwningActor = OwnerComponent->GetOwner();
	return Cast<UScWAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwningActor));
}

UScWEquipmentInstance* FScWEquipmentList::AddEntry(TSubclassOf<UScWEquipmentDefinition> EquipmentDefinition)
{
	UScWEquipmentInstance* Result = nullptr;

	check(EquipmentDefinition != nullptr);
 	check(OwnerComponent);
	check(OwnerComponent->GetOwner()->HasAuthority());
	
	const UScWEquipmentDefinition* EquipmentCDO = GetDefault<UScWEquipmentDefinition>(EquipmentDefinition);

	TSubclassOf<UScWEquipmentInstance> InstanceType = EquipmentCDO->InstanceType;
	if (InstanceType == nullptr)
	{
		InstanceType = UScWEquipmentInstance::StaticClass();
	}
	
	FScWAppliedEquipmentEntry& NewEntry = Entries.AddDefaulted_GetRef();
	NewEntry.EquipmentDefinition = EquipmentDefinition;
	NewEntry.Instance = NewObject<UScWEquipmentInstance>(OwnerComponent->GetOwner(), InstanceType);  //@TODO: Using the actor instead of component as the outer due to UE-127172
	NewEntry.Instance->SetEquipmentDefinition(EquipmentCDO);
	Result = NewEntry.Instance;

	if (UScWAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		for (const TObjectPtr<const UScWAbilitySet>& AbilitySet : EquipmentCDO->AbilitySetsToGrant)
		{
			AbilitySet->GiveToAbilitySystem(ASC, /*inout*/ &NewEntry.GrantedHandles, Result);
		}
	}
	else
	{
		//@TODO: Warning logging?
	}

	Result->SpawnEquipmentActors(EquipmentCDO->ActorsToSpawn);


	MarkItemDirty(NewEntry);

	return Result;
}

void FScWEquipmentList::RemoveEntry(UScWEquipmentInstance* Instance)
{
	for (auto EntryIt = Entries.CreateIterator(); EntryIt; ++EntryIt)
	{
		FScWAppliedEquipmentEntry& Entry = *EntryIt;
		if (Entry.Instance == Instance)
		{
			if (UScWAbilitySystemComponent* ASC = GetAbilitySystemComponent())
			{
				Entry.GrantedHandles.TakeFromAbilitySystem(ASC);
			}

			Instance->DestroyEquipmentActors();
			

			EntryIt.RemoveCurrent();
			MarkArrayDirty();
		}
	}
}

//////////////////////////////////////////////////////////////////////
// UScWEquipmentManagerComponent

UScWEquipmentManagerComponent::UScWEquipmentManagerComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, EquipmentList(this)
{
	SetIsReplicatedByDefault(true);
	bWantsInitializeComponent = true;
}

void UScWEquipmentManagerComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, EquipmentList);
}

UScWEquipmentInstance* UScWEquipmentManagerComponent::EquipItem(TSubclassOf<UScWEquipmentDefinition> EquipmentClass)
{
	UScWEquipmentInstance* Result = nullptr;
	if (EquipmentClass != nullptr)
	{
		Result = EquipmentList.AddEntry(EquipmentClass);
		if (Result != nullptr)
		{
			Result->OnEquipped();

			if (IsUsingRegisteredSubObjectList() && IsReadyForReplication())
			{
				AddReplicatedSubObject(Result);
			}
		}
	}
	return Result;
}

void UScWEquipmentManagerComponent::UnequipItem(UScWEquipmentInstance* ItemInstance)
{
	if (ItemInstance != nullptr)
	{
		if (IsUsingRegisteredSubObjectList())
		{
			RemoveReplicatedSubObject(ItemInstance);
		}

		ItemInstance->OnUnequipped();
		EquipmentList.RemoveEntry(ItemInstance);
	}
}

bool UScWEquipmentManagerComponent::ReplicateSubobjects(UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	for (FScWAppliedEquipmentEntry& Entry : EquipmentList.Entries)
	{
		UScWEquipmentInstance* Instance = Entry.Instance;

		if (IsValid(Instance))
		{
			WroteSomething |= Channel->ReplicateSubobject(Instance, *Bunch, *RepFlags);
		}
	}

	return WroteSomething;
}

void UScWEquipmentManagerComponent::InitializeComponent()
{
	Super::InitializeComponent();
}

void UScWEquipmentManagerComponent::UninitializeComponent()
{
	TArray<UScWEquipmentInstance*> AllEquipmentInstances;

	// gathering all instances before removal to avoid side effects affecting the equipment list iterator	
	for (const FScWAppliedEquipmentEntry& Entry : EquipmentList.Entries)
	{
		AllEquipmentInstances.Add(Entry.Instance);
	}

	for (UScWEquipmentInstance* EquipInstance : AllEquipmentInstances)
	{
		UnequipItem(EquipInstance);
	}

	Super::UninitializeComponent();
}

void UScWEquipmentManagerComponent::ReadyForReplication()
{
	Super::ReadyForReplication();

	// Register existing ScWEquipmentInstances
	if (IsUsingRegisteredSubObjectList())
	{
		for (const FScWAppliedEquipmentEntry& Entry : EquipmentList.Entries)
		{
			UScWEquipmentInstance* Instance = Entry.Instance;

			if (IsValid(Instance))
			{
				AddReplicatedSubObject(Instance);
			}
		}
	}
}

UScWEquipmentInstance* UScWEquipmentManagerComponent::GetFirstInstanceOfType(TSubclassOf<UScWEquipmentInstance> InstanceType)
{
	for (FScWAppliedEquipmentEntry& Entry : EquipmentList.Entries)
	{
		if (UScWEquipmentInstance* Instance = Entry.Instance)
		{
			if (Instance->IsA(InstanceType))
			{
				return Instance;
			}
		}
	}
	return nullptr;
}

TArray<UScWEquipmentInstance*> UScWEquipmentManagerComponent::GetEquipmentInstancesOfType(TSubclassOf<UScWEquipmentInstance> InstanceType) const
{
	TArray<UScWEquipmentInstance*> Results;
	for (const FScWAppliedEquipmentEntry& Entry : EquipmentList.Entries)
	{
		if (UScWEquipmentInstance* Instance = Entry.Instance)
		{
			if (Instance->IsA(InstanceType))
			{
				Results.Add(Instance);
			}
		}
	}
	return Results;
}

UScWEquipmentInstance* UScWEquipmentManagerComponent::GetFirstInstanceWithDefinitionTag(const FGameplayTag& InTag)
{
	for (FScWAppliedEquipmentEntry& Entry : EquipmentList.Entries)
	{
		ensureContinue(Entry.Instance);
		ensureContinue(Entry.Instance->GetEquipmentDefinition());
		if (Entry.Instance->GetEquipmentDefinition()->TypeTag == InTag)
		{
			return Entry.Instance;
		}
	}
	return nullptr;
}
