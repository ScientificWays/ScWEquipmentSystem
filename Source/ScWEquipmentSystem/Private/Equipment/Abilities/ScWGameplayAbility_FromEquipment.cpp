// Scientific Ways

#include "Equipment/Abilities/ScWGameplayAbility_FromEquipment.h"

#include "Equipment/ScWEquipmentInstance.h"
//#include "Inventory/ScWInventoryItemInstance.h"

#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

#include UE_INLINE_GENERATED_CPP_BY_NAME(ScWGameplayAbility_FromEquipment)

UScWGameplayAbility_FromEquipment::UScWGameplayAbility_FromEquipment(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

UScWEquipmentInstance* UScWGameplayAbility_FromEquipment::GetAssociatedEquipment() const
{
	if (FGameplayAbilitySpec* Spec = UGameplayAbility::GetCurrentAbilitySpec())
	{
		return Cast<UScWEquipmentInstance>(Spec->SourceObject.Get());
	}
	return nullptr;
}

/*UScWInventoryItemInstance* UScWGameplayAbility_FromEquipment::GetAssociatedItem() const
{
	if (UScWEquipmentInstance* Equipment = GetAssociatedEquipment())
	{
		return Cast<UScWInventoryItemInstance>(Equipment->GetInstigator());
	}
	return nullptr;
}*/


#if WITH_EDITOR
EDataValidationResult UScWGameplayAbility_FromEquipment::IsDataValid(FDataValidationContext& Context) const
{
	EDataValidationResult Result = Super::IsDataValid(Context);

PRAGMA_DISABLE_DEPRECATION_WARNINGS
	if (InstancingPolicy == EGameplayAbilityInstancingPolicy::NonInstanced)
PRAGMA_ENABLE_DEPRECATION_WARNINGS
	{
		Context.AddError(NSLOCTEXT("ScW", "EquipmentAbilityMustBeInstanced", "Equipment ability must be instanced"));
		Result = EDataValidationResult::Invalid;
	}

	return Result;
}

#endif
