// Scientific Ways

#pragma once

#include "ScWEquipmentSystem.h"

#include "ScWEquipmentDefinition.generated.h"

class AActor;
class UScWAbilitySet;
class UScWEquipmentFragment;
class UScWEquipmentInstance;

/**
 *	Definition of a piece of equipment that can be applied to a pawn
 */
UCLASS(Const, Abstract, Blueprintable, BlueprintType)
class UScWEquipmentDefinition : public UObject
{
	GENERATED_BODY()

public:
	UScWEquipmentDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// Tag used to define equipment general type (e.g. Weapon)
	UPROPERTY(Category = "Equipment", EditDefaultsOnly)
	FGameplayTag TypeTag;

	UFUNCTION(Category = "Equipment", BlueprintCallable, meta = (DeterminesOutputType = "InTypeClass"))
	const UScWEquipmentFragment* GetFragmentByClass(TSubclassOf<UScWEquipmentFragment> InTypeClass) const;

	template<class T>
	const T* GetFragmentByClass() const
	{
		return Cast<T>(GetFragmentByClass(T::StaticClass()));
	}

	// Actors to spawn on the pawn when this is equipped
	UPROPERTY(Category = "Equipment", EditDefaultsOnly, Instanced)
	TArray<TObjectPtr<const UScWEquipmentFragment>> Fragments;

	// Class to spawn
	UPROPERTY(Category = "Equipment", EditDefaultsOnly)
	TSubclassOf<UScWEquipmentInstance> InstanceType;
};
