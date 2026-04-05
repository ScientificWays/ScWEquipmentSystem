// Scientific Ways

#pragma once

#include "ScWEquipmentSystem.h"

#include "EquipmentSystem/ScWEquipmentFragment.h"

#include "Animation/ScWTypes_Animation.h"

#include "ScWEquipmentFragment_SpawnActors.generated.h"

#define MODULE_API SCWEQUIPMENTSYSTEM_API

/**
 *	Selects which avatar mesh or custom attachment point spawned equipment actors should use.
 */
UENUM(BlueprintType)
enum class EScWEquipmentMesh : uint8
{
	FirstPerson = 0,
	ThirdPerson = 1,
	Custom = 2,
};

/**
 *	Describes a single actor that should be spawned and attached while equipment is active.
 */
USTRUCT(BlueprintType)
struct FScWEquipmentActorToSpawn
{
	GENERATED_BODY()

	UPROPERTY(Category = "Spawn", EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> ActorToSpawn;

	UPROPERTY(Category = "Spawn", EditAnywhere, BlueprintReadWrite)
	EScWEquipmentMesh AttachMesh = EScWEquipmentMesh::ThirdPerson;

	UPROPERTY(Category = "Spawn", EditAnywhere, BlueprintReadWrite)
	FName AttachSocket = TEXT("EquipmentSocket");

	UPROPERTY(Category = "Spawn", EditAnywhere, BlueprintReadWrite)
	FTransform AttachTransform = FTransform::Identity;

	UPROPERTY(Category = "Spawn", EditAnywhere, BlueprintReadWrite)
	bool bIgnoreAttachTargetScale = true;
};

/**
 *	Stores transient spawned-actor state for a single equipment instance.
 */
USTRUCT()
struct FScWSpawnActorsMemory
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<TObjectPtr<AActor>> SpawnedActors;
};

/**
 *	Spawns and cleans up attached actors while an equipment instance is equipped.
 */
UCLASS(MinimalAPI, Blueprintable, BlueprintType, meta = (DisplayName = "[ScW] Equipment Fragment (Spawn Actors)"))
class UScWEquipmentFragment_SpawnActors : public UScWEquipmentFragment
{
	GENERATED_BODY()

//~ Begin Statics
public:

	UFUNCTION(Category = "Statics", BlueprintCallable)
	static MODULE_API TArray<AActor*> GetAllEquipmentSpawnedActors(UScWEquipmentManagerComponent* InEquipmentManagerComponent);

	UFUNCTION(Category = "Statics", BlueprintCallable, meta = (DeterminesOutputType = "InClass"))
	static MODULE_API AActor* GetEquipmentSpawnedActorByClass(UScWEquipmentManagerComponent* InEquipmentManagerComponent, TSubclassOf<AActor> InClass);
//~ End Statics

//~ Begin Initialize
public:
	UScWEquipmentFragment_SpawnActors();
protected:
	virtual bool IsSupportedForNetworking() const override { return true; } // UObject
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override; // UObject
	virtual void BP_OnEquipped_Implementation(class UScWEquipmentInstance* InInstance) const override; // UScWEquipmentFragment
	virtual void BP_OnUnequipped_Implementation(class UScWEquipmentInstance* InInstance) const override; // UScWEquipmentFragment
//~ End Initialize
	
//~ Begin Actors
public:

	UFUNCTION(Category = "Actors", BlueprintCallable)
	MODULE_API TArray<AActor*> GetSpawnedActors(const UScWEquipmentInstance* InInstance) const;

	UPROPERTY(Category = "Actors", EditDefaultsOnly)
	TArray<FScWEquipmentActorToSpawn> ActorsToSpawn;

protected:
	USceneComponent* GetAttachmentForActor(UScWEquipmentInstance* InInstance, const FScWEquipmentActorToSpawn& InSpawnInfo) const;

	UFUNCTION(Category = "Actors", BlueprintImplementableEvent, meta = (DisplayName = "Get Custom Attachment for Actor"))
	USceneComponent* BP_GetCustomAttachmentForActor(UScWEquipmentInstance* InInstance, const FScWEquipmentActorToSpawn& InSpawnInfo) const;

	UPROPERTY(/*Replicated*/)
	mutable TMap<TObjectPtr<UScWEquipmentInstance>, FScWSpawnActorsMemory> PerInstanceMemory;
//~ End Actors
};

#undef MODULE_API
