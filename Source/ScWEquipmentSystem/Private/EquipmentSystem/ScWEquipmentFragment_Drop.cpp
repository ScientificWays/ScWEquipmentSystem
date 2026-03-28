// Scientific Ways

#include "EquipmentSystem/ScWEquipmentFragment_Drop.h"

#include "EquipmentSystem/ScWEquipmentInstance.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ScWEquipmentFragment_Drop)

//~ Begin Initialize
UScWEquipmentFragment_Drop::UScWEquipmentFragment_Drop()
{
	bCanDrop = false;
	bDropOnDeath = false;
}

void UScWEquipmentFragment_Drop::BP_OnEquipped_Implementation(UScWEquipmentInstance* InInstance) const // UScWEquipmentFragment
{
}

void UScWEquipmentFragment_Drop::BP_OnUnequipped_Implementation(UScWEquipmentInstance* InInstance) const // UScWEquipmentFragment
{
}
//~ End Initialize
