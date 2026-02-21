// Scientific Ways

#pragma once

#include "ScWGameCore.h"

#include "Character/ScWPawnDataFragment.h"

#include "ScWPawnDataFragment_InitialEquipment.generated.h"

#define MODULE_API SCWEQUIPMENTSYSTEM_API

class UScWEquipmentDefinition;

/**
 *
 */
UCLASS(MinimalAPI, Blueprintable, BlueprintType, meta = (DisplayName = "[ScW] Pawn Data Fragment (Initial Equipment)"))
class UScWPawnDataFragment_InitialEquipment : public UScWPawnDataFragment
{
	GENERATED_BODY()

//~ Begin Initialize
public:
	MODULE_API UScWPawnDataFragment_InitialEquipment();
protected:
	MODULE_API virtual void BP_InitializePawn_Implementation(UScWPawnExtensionComponent* InPawnExtComponent) const override; // UScWPawnDataFragment
	MODULE_API virtual void BP_UninitializePawn_Implementation(UScWPawnExtensionComponent* InPawnExtComponent) const override; // UScWPawnDataFragment
//~ End Initialize
	
//~ Begin Equipment
public:

	// Give these equipment to Pawn on init. Is not removed on uninit.
	UPROPERTY(Category = "Equipment", EditDefaultsOnly, BlueprintReadOnly)
	TArray<TSubclassOf<UScWEquipmentDefinition>> InitialEquipment;
//~ End Equipment
};

#undef MODULE_API
