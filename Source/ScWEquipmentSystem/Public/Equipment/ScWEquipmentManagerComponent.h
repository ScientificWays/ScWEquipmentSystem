// Scientific Ways

#pragma once

#include "ScWEquipmentSystem.h"

#include "AbilitySystem/ScWAbilitySet.h"

#include "ScWEquipmentManagerComponent.generated.h"

#define MODULE_API SCWEQUIPMENTSYSTEM_API

class UActorComponent;
class UScWAbilitySystemComponent;
class UScWEquipmentDefinition;
class UScWEquipmentInstance;
class UScWEquipmentManagerComponent;
class UObject;
struct FFrame;
struct FScWEquipmentList;
struct FNetDeltaSerializeInfo;
struct FReplicationFlags;

/** A single piece of applied equipment */
USTRUCT(BlueprintType)
struct FScWAppliedEquipmentEntry : public FFastArraySerializerItem
{
	GENERATED_BODY()

	FScWAppliedEquipmentEntry()
	{}

	FString GetDebugString() const;

private:
	friend FScWEquipmentList;
	friend UScWEquipmentManagerComponent;

	// The equipment class that got equipped
	UPROPERTY()
	TSubclassOf<UScWEquipmentDefinition> EquipmentDefinition;

	UPROPERTY()
	TObjectPtr<UScWEquipmentInstance> Instance = nullptr;

	// Authority-only list of granted handles
	UPROPERTY(NotReplicated)
	FScWAbilitySet_GrantedHandles GrantedHandles;
};

/** List of applied equipment */
USTRUCT(BlueprintType)
struct FScWEquipmentList : public FFastArraySerializer
{
	GENERATED_BODY()

	FScWEquipmentList()
		: OwnerComponent(nullptr)
	{
	}

	FScWEquipmentList(UActorComponent* InOwnerComponent)
		: OwnerComponent(InOwnerComponent)
	{
	}

public:
	//~FFastArraySerializer contract
	void PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize);
	void PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize);
	void PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize);
	//~End of FFastArraySerializer contract

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FScWAppliedEquipmentEntry, FScWEquipmentList>(Entries, DeltaParms, *this);
	}

	UScWEquipmentInstance* AddEntry(TSubclassOf<UScWEquipmentDefinition> EquipmentDefinition);
	void RemoveEntry(UScWEquipmentInstance* Instance);

private:
	UScWAbilitySystemComponent* GetAbilitySystemComponent() const;

	friend UScWEquipmentManagerComponent;

private:
	// Replicated list of equipment entries
	UPROPERTY()
	TArray<FScWAppliedEquipmentEntry> Entries;

	UPROPERTY(NotReplicated)
	TObjectPtr<UActorComponent> OwnerComponent;
};

template<>
struct TStructOpsTypeTraits<FScWEquipmentList> : public TStructOpsTypeTraitsBase2<FScWEquipmentList>
{
	enum { WithNetDeltaSerializer = true };
};

/**
 * Manages equipment applied to a pawn
 */
UCLASS(MinimalAPI, BlueprintType)
class UScWEquipmentManagerComponent : public UPawnComponent
{
	GENERATED_BODY()

public:
	MODULE_API UScWEquipmentManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	MODULE_API UScWEquipmentInstance* EquipItem(TSubclassOf<UScWEquipmentDefinition> EquipmentDefinition);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	MODULE_API void UnequipItem(UScWEquipmentInstance* ItemInstance);

	//~UObject interface
	MODULE_API virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
	//~End of UObject interface

	//~UActorComponent interface
	//virtual void EndPlay() override;
	MODULE_API virtual void InitializeComponent() override;
	MODULE_API virtual void UninitializeComponent() override;
	MODULE_API virtual void ReadyForReplication() override;
	//~End of UActorComponent interface

	/** Returns the first equipped instance of a given type, or nullptr if none are found */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	MODULE_API UScWEquipmentInstance* GetFirstInstanceOfType(TSubclassOf<UScWEquipmentInstance> InstanceType);

 	/** Returns all equipped instances of a given type, or an empty array if none are found */
 	UFUNCTION(BlueprintCallable, BlueprintPure)
	MODULE_API TArray<UScWEquipmentInstance*> GetEquipmentInstancesOfType(TSubclassOf<UScWEquipmentInstance> InstanceType) const;

	template <typename T>
	T* GetFirstInstanceOfType()
	{
		return (T*)GetFirstInstanceOfType(T::StaticClass());
	}

	UFUNCTION(BlueprintCallable, BlueprintPure)
	MODULE_API UScWEquipmentInstance* GetFirstInstanceWithDefinitionTag(const FGameplayTag& InTag);

private:
	UPROPERTY(Replicated)
	FScWEquipmentList EquipmentList;
};

#undef MODULE_API
