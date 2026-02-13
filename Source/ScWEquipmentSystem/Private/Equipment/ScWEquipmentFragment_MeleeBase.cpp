// Scientific Ways

#include "Equipment/ScWEquipmentFragment_MeleeBase.h"

#include "Equipment/ScWEquipmentInstance.h"

//~ Begin Initialize
UScWEquipmentFragment_MeleeBase::UScWEquipmentFragment_MeleeBase()
{
	
}

void UScWEquipmentFragment_MeleeBase::BP_OnEquipped_Implementation(UScWEquipmentInstance* InInstance) // UScWEquipmentFragment
{

}

void UScWEquipmentFragment_MeleeBase::BP_OnUnequipped_Implementation(UScWEquipmentInstance* InInstance) // UScWEquipmentFragment
{

}
//~ End Initialize

//~ Begin Swing
float UScWEquipmentFragment_MeleeBase::BP_ModifySwingBaseDamage_Implementation(const AActor* InMeleeEquipmentActor, float InDamage) const
{
	ensureReturn(InMeleeEquipmentActor, InDamage);
	return InDamage;
}

TSubclassOf<class UScWDamageType> UScWEquipmentFragment_MeleeBase::BP_ModifySwingBaseDamageTypeClass_Implementation(const AActor* InMeleeEquipmentActor, TSubclassOf<class UScWDamageType> InDamageTypeClass) const
{
	ensureReturn(InMeleeEquipmentActor, InDamageTypeClass);
	return InDamageTypeClass;
}
//~ End Swing

//~ Begin Swing Variants
void UScWEquipmentFragment_MeleeBase::BP_ModifySwingVariants_Implementation(const AActor* InMeleeEquipmentActor, TArray<FScWMeleeSwingVariantData>& InOutVariants) const
{
	ensureReturn(InMeleeEquipmentActor);
}
//~ End Swing Variants
