// Scientific Ways

#include "Equipment/ScWEquipmentFragment_Drop.h"

#include "Equipment/ScWEquipmentInstance.h"

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
