// Scientific Ways

#pragma once

#include "ScWEquipmentSystem.h"

#include "ScWEquipmentInstance.generated.h"

class AActor;
class APawn;
class UScWEquipmentDefinition;
struct FFrame;
struct FScWEquipmentActorToSpawn;

/**
 *	A piece of equipment spawned and applied to a pawn
 */
UCLASS(BlueprintType, Blueprintable, meta = (DisplayName = "[ScW] Equipment Instance"))
class UScWEquipmentInstance : public UObject
{
	GENERATED_BODY()

public:
	UScWEquipmentInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~UObject interface
	virtual bool IsSupportedForNetworking() const override { return true; }
	virtual UWorld* GetWorld() const override final;
	//~End of UObject interface

	UFUNCTION(BlueprintPure, Category = "Equipment")
	UObject* GetInstigator() const { return Instigator; }
	void SetInstigator(UObject* InInstigator) { Instigator = InInstigator; }

	UFUNCTION(BlueprintPure, Category = "Equipment")
	const UScWEquipmentDefinition* GetEquipmentDefinition() const { return EquipmentDefinition; }
	void SetEquipmentDefinition(const UScWEquipmentDefinition* InEquipmentDefinition) { EquipmentDefinition = InEquipmentDefinition; }

	UFUNCTION(BlueprintPure, Category = "Equipment")
	APawn* GetPawn() const;

	UFUNCTION(BlueprintPure, Category = "Equipment", meta = (DeterminesOutputType = PawnType))
	APawn* GetTypedPawn(TSubclassOf<APawn> PawnType) const;

	UFUNCTION(BlueprintPure, Category = "Equipment")
	TArray<AActor*> GetSpawnedActors() const { return SpawnedActors; }

	UFUNCTION(BlueprintPure, Category = "Equipment")
	AActor* GetPrimarySpawnedActor() const { return SpawnedActors.IsEmpty() ? nullptr : SpawnedActors[0]; }

	virtual void SpawnEquipmentActors(const TArray<FScWEquipmentActorToSpawn>& ActorsToSpawn);
	virtual void DestroyEquipmentActors();

	virtual void OnEquipped();
	virtual void OnUnequipped();

protected:
#if UE_WITH_IRIS
	/** Register all replication fragments */
	virtual void RegisterReplicationFragments(UE::Net::FFragmentRegistrationContext& Context, UE::Net::EFragmentRegistrationFlags RegistrationFlags) override;
#endif // UE_WITH_IRIS

	UFUNCTION(BlueprintImplementableEvent, Category = "Equipment", meta=(DisplayName="OnEquipped"))
	void K2_OnEquipped();

	UFUNCTION(BlueprintImplementableEvent, Category = "Equipment", meta=(DisplayName="OnUnequipped"))
	void K2_OnUnequipped();

private:

	UPROPERTY(Replicated)
	TArray<TObjectPtr<AActor>> SpawnedActors;

	UPROPERTY(ReplicatedUsing = "OnRep_Instigator")
	TObjectPtr<UObject> Instigator;

	UFUNCTION()
	void OnRep_Instigator();

	UPROPERTY(ReplicatedUsing = "OnRep_EquipmentDefinition")
	TObjectPtr<const UScWEquipmentDefinition> EquipmentDefinition;

	UFUNCTION()
	void OnRep_EquipmentDefinition();
};
