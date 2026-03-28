// Scientific Ways

#include "EquipmentSystem/ScWEquipmentFragment_UI.h"

#include "EquipmentSystem/ScWEquipmentInstance.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ScWEquipmentFragment_UI)

//~ Begin Initialize
UScWEquipmentFragment_UI::UScWEquipmentFragment_UI()
{
	DisplayName = FText::FromString(TEXT("Common Handheld"));
}

void UScWEquipmentFragment_UI::BP_OnEquipped_Implementation(UScWEquipmentInstance* InInstance) const // UScWEquipmentFragment
{
}

void UScWEquipmentFragment_UI::BP_OnUnequipped_Implementation(UScWEquipmentInstance* InInstance) const // UScWEquipmentFragment
{
}
//~ End Initialize
