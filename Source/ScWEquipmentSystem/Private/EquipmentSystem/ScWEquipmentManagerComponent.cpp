// Scientific Ways

#include "EquipmentSystem/ScWEquipmentManagerComponent.h"

#include "AbilitySystem/ScWAbilitySystemComponent.h"

#include "EquipmentSystem/ScWEquipmentInstance.h"
#include "EquipmentSystem/ScWEquipmentDefinition.h"

#include "Engine/ActorChannel.h"
#include "AbilitySystemGlobals.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ScWEquipmentManagerComponent)

class FLifetimeProperty;
struct FReplicationFlags;

//////////////////////////////////////////////////////////////////////
// FScWAppliedEquipmentEntry

FString FScWAppliedEquipmentEntry::GetDebugString() const
{
	return FString::Printf(TEXT("%s of %s"), *GetNameSafe(Instance), *GetNameSafe(DefinitionClass.Get()));
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

UScWEquipmentInstance* FScWEquipmentList::AddEntry(TSubclassOf<UScWEquipmentDefinition> InDefinitionClass)
{
	UScWEquipmentInstance* Result = nullptr;

	check(InDefinitionClass != nullptr);
 	check(OwnerComponent);
	check(OwnerComponent->GetOwner()->HasAuthority());
	
	const UScWEquipmentDefinition* EquipmentCDO = GetDefault<UScWEquipmentDefinition>(InDefinitionClass);

	TSubclassOf<UScWEquipmentInstance> InstanceType = EquipmentCDO->InstanceType;
	if (InstanceType == nullptr)
	{
		InstanceType = UScWEquipmentInstance::StaticClass();
	}
	FScWAppliedEquipmentEntry& NewEntry = Entries.AddDefaulted_GetRef();
	NewEntry.DefinitionClass = InDefinitionClass;
	NewEntry.Instance = NewObject<UScWEquipmentInstance>(OwnerComponent->GetOwner(), InstanceType);  //@TODO: Using the actor instead of component as the outer due to UE-127172
	NewEntry.Instance->SetEquipmentDefinition(EquipmentCDO);
	Result = NewEntry.Instance;

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
			EntryIt.RemoveCurrent();
			MarkArrayDirty();
		}
	}
}

//////////////////////////////////////////////////////////////////////
// UScWEquipmentManagerComponent

//~ Begin Initialize
UScWEquipmentManagerComponent::UScWEquipmentManagerComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	//, EquipmentList(this)
{
	SetIsReplicatedByDefault(true);
	bWantsInitializeComponent = true;
}

void UScWEquipmentManagerComponent::InitializeComponent() // UActorComponent
{
	Super::InitializeComponent();

	EquipmentList = FScWEquipmentList(this);
}

void UScWEquipmentManagerComponent::UninitializeComponent() // UActorComponent
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

void UScWEquipmentManagerComponent::BeginPlay() // UActorComponent
{
	Super::BeginPlay();

	ensureReturn(GetOwner());
	if (GetOwner()->HasAuthority())
	{
		for (const auto& SampleDefinition : InitialEquipment)
		{
			EquipItem(SampleDefinition);
		}
	}
}

void UScWEquipmentManagerComponent::EndPlay(const EEndPlayReason::Type InEndPlayReason) // UActorComponent
{
	Super::EndPlay(InEndPlayReason);


}
//~ End Initialize

//~ Begin Replication
void UScWEquipmentManagerComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const // UObject
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, EquipmentList);
}

void UScWEquipmentManagerComponent::ReadyForReplication() // UActorComponent
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

bool UScWEquipmentManagerComponent::ReplicateSubobjects(UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) // UObject
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
//~ End Replication

//~ Begin Equip
UScWEquipmentInstance* UScWEquipmentManagerComponent::EquipItem(TSubclassOf<UScWEquipmentDefinition> InDefinitionClass)
{
	ensureReturn(InDefinitionClass, nullptr);

	UScWEquipmentInstance* NewInstance = EquipmentList.AddEntry(InDefinitionClass);
	ensureReturn(NewInstance, nullptr);

	NewInstance->OnEquipped();

	if (IsUsingRegisteredSubObjectList() && IsReadyForReplication())
	{
		AddReplicatedSubObject(NewInstance);
	}
	//OnEquippedItem.Broadcast(NewInstance);
	return NewInstance;
}

void UScWEquipmentManagerComponent::UnequipItem(UScWEquipmentInstance* InInstance)
{
	ensureReturn(InInstance);

	if (IsUsingRegisteredSubObjectList())
	{
		RemoveReplicatedSubObject(InInstance);
	}
	InInstance->OnUnequipped();
	EquipmentList.RemoveEntry(InInstance);

	//OnUnequippedItem.Broadcast(InInstance);
}
//~ End Equip

//~ Begin Instances
UScWEquipmentInstance* UScWEquipmentManagerComponent::GetFirstInstanceOfType(TSubclassOf<UScWEquipmentInstance> InstanceType) const
{
	for (const FScWAppliedEquipmentEntry& Entry : EquipmentList.Entries)
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

UScWEquipmentInstance* UScWEquipmentManagerComponent::GetFirstInstanceWithDefinitionTag(const FGameplayTag& InTag) const
{
	for (const FScWAppliedEquipmentEntry& Entry : EquipmentList.Entries)
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

TArray<UScWEquipmentInstance*> UScWEquipmentManagerComponent::GetAllInstancesWithDefinitionTag(const FGameplayTag& InTag) const
{
	TArray<UScWEquipmentInstance*> OutInstances;

	for (const FScWAppliedEquipmentEntry& Entry : EquipmentList.Entries)
	{
		ensureContinue(Entry.Instance);
		ensureContinue(Entry.Instance->GetEquipmentDefinition());

		if (Entry.Instance->GetEquipmentDefinition()->TypeTag == InTag)
		{
			OutInstances.Add(Entry.Instance);
		}
	}
	return OutInstances;
}
//~ End Instances
