// Scientific Ways

#include "EquipmentSystem/ScWEquipmentDefinition.h"

#include "Tags/ScWEquipmentTags.h"
#include "EquipmentSystem/ScWEquipmentFragment.h"
#include "EquipmentSystem/ScWEquipmentInstance.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ScWEquipmentDefinition)

UScWEquipmentDefinition::UScWEquipmentDefinition(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	TypeTag = FScWEquipmentTags::Equipment_Type_Weapon;
	InstanceType = UScWEquipmentInstance::StaticClass();
}

const UScWEquipmentFragment* UScWEquipmentDefinition::GetFragmentByClass(TSubclassOf<UScWEquipmentFragment> InTypeClass) const
{
	for (const UScWEquipmentFragment* SampleFragment : Fragments)
	{
		if (SampleFragment->IsA(InTypeClass))
		{
			return SampleFragment;
		}
	}
	return nullptr;
}

