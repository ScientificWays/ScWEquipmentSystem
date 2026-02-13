// Scientific Ways

#include "Equipment/ScWEquipmentFragment_UI.h"

#include "Equipment/ScWEquipmentInstance.h"

//~ Begin Initialize
UScWEquipmentFragment_UI::UScWEquipmentFragment_UI()
{
	DisplayName = FText::FromString(TEXT("Common Handheld"));
}

void UScWEquipmentFragment_UI::BP_OnEquipped_Implementation(UScWEquipmentInstance* InInstance) // UScWEquipmentFragment
{
	
}

void UScWEquipmentFragment_UI::BP_OnUnequipped_Implementation(UScWEquipmentInstance* InInstance) // UScWEquipmentFragment
{
	
}
//~ End Initialize
