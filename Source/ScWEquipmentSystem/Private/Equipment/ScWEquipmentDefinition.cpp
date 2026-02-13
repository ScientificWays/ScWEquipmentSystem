// Scientific Ways

#include "Equipment/ScWEquipmentDefinition.h"

#include "Equipment/ScWEquipmentTags.h"
#include "Equipment/ScWEquipmentFragment.h"
#include "Equipment/ScWEquipmentInstance.h"

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

