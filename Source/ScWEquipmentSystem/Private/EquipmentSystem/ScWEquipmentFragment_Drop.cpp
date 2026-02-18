// Scientific Ways

#include "EquipmentSystem/ScWEquipmentFragment_Drop.h"

#include "EquipmentSystem/ScWEquipmentInstance.h"

//~ Begin Initialize
UScWEquipmentFragment_Drop::UScWEquipmentFragment_Drop()
{
	bCanDrop = false;
	bDropOnDeath = false;
}

void UScWEquipmentFragment_Drop::BP_OnEquipped_Implementation(UScWEquipmentInstance* InInstance) // UScWEquipmentFragment
{

}

void UScWEquipmentFragment_Drop::BP_OnUnequipped_Implementation(UScWEquipmentInstance* InInstance) // UScWEquipmentFragment
{

}
//~ End Initialize
