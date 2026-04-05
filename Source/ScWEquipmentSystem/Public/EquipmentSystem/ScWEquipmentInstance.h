// Scientific Ways

#pragma once

#include "ScWEquipmentSystem.h"

#include "ScWEquipmentInstance.generated.h"

class AActor;
class APawn;
class UScWEquipmentDefinition;
struct FFrame;
struct FScWEquipmentActorToSpawn;
class UScWEquipmentManagerComponent;

/**
 *	Published when an equipment instance is equipped or unequipped.
 */
USTRUCT(BlueprintType)
struct FGameplayMessage_EquipmentInstance
{
	GENERATED_BODY()

	UPROPERTY(Category = "Equipment", EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UScWEquipmentInstance> EquipmentInstance;
};

/**
 *	Represents a runtime equipment instance applied to a pawn.
 *	It owns equip and unequip state and forwards lifecycle events to equipment fragments.
 */
UCLASS(BlueprintType, Blueprintable, meta = (DisplayName = "[ScW] Equipment Instance"))
class UScWEquipmentInstance : public UObject
{
	GENERATED_BODY()

public:
	UScWEquipmentInstance(const FObjectInitializer& InObjectInitializer = FObjectInitializer::Get());

	virtual UWorld* GetWorld() const override final; // UObject
	virtual bool IsSupportedForNetworking() const override { return true; } // UObject
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override; // UObject

	UFUNCTION(Category = "Equipment", BlueprintPure)
	UScWEquipmentManagerComponent* GetOwningEquipmentManagerComponent() const;

	UFUNCTION(Category = "Equipment", BlueprintPure)
	UObject* GetInstigator() const { return Instigator; }
	void SetInstigator(UObject* InInstigator) { Instigator = InInstigator; }

	UFUNCTION(Category = "Equipment", BlueprintPure)
	const UScWEquipmentDefinition* GetEquipmentDefinition() const { return EquipmentDefinition; }
	void SetEquipmentDefinition(const UScWEquipmentDefinition* InEquipmentDefinition) { EquipmentDefinition = InEquipmentDefinition; }

	UFUNCTION(Category = "Equipment", BlueprintPure)
	APawn* GetPawn() const;

	UFUNCTION(Category = "Equipment", BlueprintPure, meta = (DeterminesOutputType = PawnType))
	APawn* GetTypedPawn(TSubclassOf<APawn> PawnType) const;

	virtual void OnEquipped();
	virtual void OnUnequipped();

protected:
#if UE_WITH_IRIS
	/** Register all replication fragments */
	virtual void RegisterReplicationFragments(UE::Net::FFragmentRegistrationContext& Context, UE::Net::EFragmentRegistrationFlags RegistrationFlags) override;
#endif // UE_WITH_IRIS

	UFUNCTION(BlueprintImplementableEvent, Category = "Equipment", meta = (DisplayName="On Equipped"))
	void BP_OnEquipped();

	UFUNCTION(BlueprintImplementableEvent, Category = "Equipment", meta = (DisplayName="On Unequipped"))
	void BP_OnUnequipped();

private:
	void TryApplyEquipmentState();
	void TryRemoveEquipmentState();

	UPROPERTY(ReplicatedUsing = "OnRep_Instigator")
	TObjectPtr<UObject> Instigator;

	UFUNCTION()
	void OnRep_Instigator();

	UPROPERTY(ReplicatedUsing = "OnRep_EquipmentDefinition")
	TObjectPtr<const UScWEquipmentDefinition> EquipmentDefinition;

	UFUNCTION()
	void OnRep_EquipmentDefinition();

	bool bIsEquipped = false;
	bool bHasAppliedEquipmentState = false;
};
