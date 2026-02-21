// Scientific Ways

#include "Character/ScWPawnDataFragment_InitialEquipment.h"

#include "Character/ScWPawnExtensionComponent.h"

#include "EquipmentSystem/ScWEquipmentDefinition.h"
#include "EquipmentSystem/ScWEquipmentFunctionLibrary.h"
#include "EquipmentSystem/ScWEquipmentManagerComponent.h"

//~ Begin Initialize
UScWPawnDataFragment_InitialEquipment::UScWPawnDataFragment_InitialEquipment()
{
	
}

void UScWPawnDataFragment_InitialEquipment::BP_InitializePawn_Implementation(UScWPawnExtensionComponent* InPawnExtComponent) const // UScWPawnDataFragment
{
	ensureReturn(InPawnExtComponent);

	UScWEquipmentManagerComponent* EquipmentManager = UScWEquipmentFunctionLibrary::GetEquipmentManagerComponentFromActor(InPawnExtComponent->GetOwner());
	ensureReturn(EquipmentManager);

	for (const auto& SampleDefinition : InitialEquipment)
	{
		ensureContinue(SampleDefinition);
		EquipmentManager->EquipItem(SampleDefinition);
	}
}

void UScWPawnDataFragment_InitialEquipment::BP_UninitializePawn_Implementation(UScWPawnExtensionComponent* InPawnExtComponent) const // UScWPawnDataFragment
{
	ensureReturn(InPawnExtComponent);

	// Does not make sense to remember instances and remove them on uninit because pawn is being destroyed anyway?
}
//~ End Initialize
