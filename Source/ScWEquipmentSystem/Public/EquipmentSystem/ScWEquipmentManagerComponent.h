// Scientific Ways

#pragma once

#include "ScWEquipmentSystem.h"

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

	TSubclassOf<UScWEquipmentDefinition> GetDefinitionClass() const { return DefinitionClass; }
	const UScWEquipmentInstance* GetInstance() const { return Instance; }

private:
	friend FScWEquipmentList;
	friend UScWEquipmentManagerComponent;

	// The equipment class that got equipped
	UPROPERTY()
	TSubclassOf<UScWEquipmentDefinition> DefinitionClass;

	UPROPERTY()
	TObjectPtr<UScWEquipmentInstance> Instance = nullptr;
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

	const TArray<FScWAppliedEquipmentEntry>& GetEntries() const { return Entries; }

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

//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FScWEquipmentInstanceEventSignature, UScWEquipmentInstance*, InInstance);

/**
 *	Manages equipment applied to a pawn
 */
UCLASS(MinimalAPI, ClassGroup = "EquipmentSystem", BlueprintType, HideCategories = (Object, LOD, Lighting, Transform, Sockets, TextureStreaming), EditInlineNew, meta = (DisplayName = "[ScW] Equipment Manager Component", BlueprintSpawnableComponent))
class UScWEquipmentManagerComponent : public UPawnComponent
{
	GENERATED_BODY()
	
//~ Begin Initialize
public:
	MODULE_API UScWEquipmentManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
protected:
	virtual void InitializeComponent() override; // UActorComponent
	virtual void UninitializeComponent() override; // UActorComponent
	virtual void BeginPlay() override; // UActorComponent
	virtual void EndPlay(const EEndPlayReason::Type InEndPlayReason) override; // UActorComponent
//~ End Initialize
	
//~ Begin Replication
protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override; // UObject
	virtual void ReadyForReplication() override; // UActorComponent
	virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override; // UObject
//~ End Replication

//~ Begin Equip
public:

	UFUNCTION(Category = "Equip", BlueprintCallable)
	MODULE_API const FScWEquipmentList& GetEquipmentList() const { return EquipmentList; }

	UFUNCTION(Category = "Equip", BlueprintCallable, BlueprintAuthorityOnly)
	MODULE_API UScWEquipmentInstance* EquipItem(TSubclassOf<UScWEquipmentDefinition> InDefinition);

	UFUNCTION(Category = "Equip", BlueprintCallable, BlueprintAuthorityOnly)
	MODULE_API void UnequipItem(UScWEquipmentInstance* InInstance);

	//UPROPERTY(Category = "Equip", BlueprintAssignable)
	//FScWEquipmentInstanceEventSignature OnEquippedItem;

	//UPROPERTY(Category = "Equip", BlueprintAssignable)
	//FScWEquipmentInstanceEventSignature OnUnequippedItem;

protected:

	//UPROPERTY(Category = "Equip", EditAnywhere, BlueprintReadOnly)
	//TArray<TSubclassOf<UScWEquipmentDefinition>> InitialEquipment;

private:

	UPROPERTY(Replicated)
	FScWEquipmentList EquipmentList;
//~ End Equip

//~ Begin Instances
public:

	/** Returns the first equipped instance of a given type, or nullptr if none are found */
	UFUNCTION(Category = "Instances", BlueprintCallable)
	MODULE_API UScWEquipmentInstance* GetFirstInstanceOfType(TSubclassOf<UScWEquipmentInstance> InstanceType) const;

 	/** Returns all equipped instances of a given type, or an empty array if none are found */
 	UFUNCTION(Category = "Instances", BlueprintCallable)
	MODULE_API TArray<UScWEquipmentInstance*> GetEquipmentInstancesOfType(TSubclassOf<UScWEquipmentInstance> InstanceType) const;

	template <typename T>
	T* GetFirstInstanceOfType()
	{
		return (T*)GetFirstInstanceOfType(T::StaticClass());
	}

	UFUNCTION(Category = "Instances", BlueprintCallable)
	MODULE_API UScWEquipmentInstance* GetFirstInstanceWithDefinitionTag(const FGameplayTag& InTag) const;

	UFUNCTION(Category = "Instances", BlueprintCallable)
	MODULE_API TArray<UScWEquipmentInstance*> GetAllInstancesWithDefinitionTag(const FGameplayTag& InTag) const;
//~ End Instances
};

#undef MODULE_API
