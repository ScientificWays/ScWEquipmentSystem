// Scientific Ways

#pragma once

#include "AbilitySystem/ScWGameplayAbility.h"

#include "ScWGameplayAbility_FromEquipment.generated.h"

class UScWEquipmentInstance;
class UScWInventoryItemInstance;

/**
 * UScWGameplayAbility_FromEquipment
 *
 * An ability granted by and associated with an equipment instance
 */
UCLASS()
class UScWGameplayAbility_FromEquipment : public UScWGameplayAbility
{
	GENERATED_BODY()

public:

	UScWGameplayAbility_FromEquipment(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category="ScW|Ability")
	UScWEquipmentInstance* GetAssociatedEquipment() const;

	//UFUNCTION(BlueprintCallable, Category = "ScW|Ability")
	//UScWInventoryItemInstance* GetAssociatedItem() const;

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
#endif

};
