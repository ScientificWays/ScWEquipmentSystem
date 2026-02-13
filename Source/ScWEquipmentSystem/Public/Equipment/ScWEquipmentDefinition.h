// Scientific Ways

#pragma once

#include "ScWEquipmentSystem.h"

#include "ScWEquipmentDefinition.generated.h"

class AActor;
class UScWAbilitySet;
class UScWEquipmentFragment;
class UScWEquipmentInstance;

USTRUCT()
struct FScWEquipmentActorToSpawn
{
	GENERATED_BODY()

	FScWEquipmentActorToSpawn() {}

	UPROPERTY(EditAnywhere, Category=Equipment)
	TSubclassOf<AActor> ActorToSpawn;

	UPROPERTY(EditAnywhere, Category=Equipment)
	FName AttachSocket;

	UPROPERTY(EditAnywhere, Category=Equipment)
	FTransform AttachTransform;
};


/**
 * UScWEquipmentDefinition
 *
 * Definition of a piece of equipment that can be applied to a pawn
 */
UCLASS(Const, Abstract, Blueprintable, BlueprintType)
class UScWEquipmentDefinition : public UObject
{
	GENERATED_BODY()

public:
	UScWEquipmentDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintPure, Category = "Equipment", meta = (DeterminesOutputType = "InTypeClass"))
	const UScWEquipmentFragment* GetFragmentByClass(TSubclassOf<UScWEquipmentFragment> InTypeClass) const;

	template<class T>
	const T* GetFragmentByClass() const
	{
		return Cast<T>(GetFragmentByClass(T::StaticClass()));
	}

	// Tag used to define equipment general type (e.g. Weapon)
	UPROPERTY(Category = "Equipment", EditDefaultsOnly)
	FGameplayTag TypeTag;

	// Actors to spawn on the pawn when this is equipped
	UPROPERTY(Category = "Equipment", EditDefaultsOnly, Instanced)
	TArray<TObjectPtr<const UScWEquipmentFragment>> Fragments;

	// Class to spawn
	UPROPERTY(Category = "Equipment", EditDefaultsOnly)
	TSubclassOf<UScWEquipmentInstance> InstanceType;

	// Gameplay ability sets to grant when this is equipped
	UPROPERTY(Category = "Equipment", EditDefaultsOnly)
	TArray<TObjectPtr<const UScWAbilitySet>> AbilitySetsToGrant;

	// Actors to spawn on the pawn when this is equipped
	UPROPERTY(Category = "Equipment", EditDefaultsOnly)
	TArray<FScWEquipmentActorToSpawn> ActorsToSpawn;
};
