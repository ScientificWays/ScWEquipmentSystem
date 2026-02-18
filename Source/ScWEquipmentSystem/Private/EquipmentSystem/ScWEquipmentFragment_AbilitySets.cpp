// Scientific Ways

#include "EquipmentSystem/ScWEquipmentFragment_AbilitySets.h"

#include "Character/ScWPawnExtensionComponent.h"

#include "EquipmentSystem/ScWEquipmentInstance.h"

//~ Begin Initialize
UScWEquipmentFragment_AbilitySets::UScWEquipmentFragment_AbilitySets()
{
	
}

void UScWEquipmentFragment_AbilitySets::BP_OnEquipped_Implementation(UScWEquipmentInstance* InInstance) const // UScWEquipmentFragment
{
	ensureReturn(InInstance);

	UScWPawnExtensionComponent* PawnExtComponent = UScWPawnExtensionComponent::GetPawnExtensionComponent(InInstance->GetPawn());
	ensureReturn(PawnExtComponent);
	PawnExtComponent->OnAbilitySystemInitialized_RegisterAndCall(FSimpleMulticastDelegate::FDelegate::CreateUObject(this, &ThisClass::GiveAbilitySet, InInstance));
	PawnExtComponent->OnAbilitySystemUninitialized_Register(FSimpleMulticastDelegate::FDelegate::CreateUObject(this, &ThisClass::TakeAbilitySet, InInstance));
}

void UScWEquipmentFragment_AbilitySets::BP_OnUnequipped_Implementation(UScWEquipmentInstance* InInstance) const // UScWEquipmentFragment
{
	ensureReturn(InInstance);

	UScWPawnExtensionComponent* PawnExtComponent = UScWPawnExtensionComponent::GetPawnExtensionComponent(InInstance->GetPawn());
	ensureReturn(PawnExtComponent);
	PawnExtComponent->OnAbilitySystemInitialized_UnregisterObject(this);
	PawnExtComponent->OnAbilitySystemUninitialized_UnregisterObject(this);

	if (UScWAbilitySystemComponent* PawnASC = PawnExtComponent->GetScWAbilitySystemComponent())
	{
		TakeAbilitySet(InInstance);
	}
}
//~ End Initialize

//~ Begin Ability System
void UScWEquipmentFragment_AbilitySets::GiveAbilitySet(UScWEquipmentInstance* InInstance) const
{
	ensureReturn(InInstance);

	TakeAbilitySet(InInstance); // Prevent from adding more than once
	ensureReturn(!PerInstanceGrantedHandles.Contains(InInstance));

	UScWPawnExtensionComponent* PawnExtComponent = UScWPawnExtensionComponent::GetPawnExtensionComponent(InInstance->GetPawn());
	ensureReturn(PawnExtComponent);

	UScWAbilitySystemComponent* PawnASC = PawnExtComponent->GetScWAbilitySystemComponent();
	ensureReturn(PawnASC);

	FScWAbilitySet_GrantedHandles GrantedHandles;
	for (const auto& AbilitySet : AbilitySetsToGrant)
	{
		AbilitySet->GiveToAbilitySystem(PawnASC, &GrantedHandles, const_cast<UScWEquipmentFragment_AbilitySets*>(this));
	}
	PerInstanceGrantedHandles.Add(InInstance, GrantedHandles);
}

void UScWEquipmentFragment_AbilitySets::TakeAbilitySet(UScWEquipmentInstance* InInstance) const
{
	ensureReturn(InInstance);
	if (PerInstanceGrantedHandles.Contains(InInstance))
	{
		UScWPawnExtensionComponent* PawnExtComponent = UScWPawnExtensionComponent::GetPawnExtensionComponent(InInstance->GetPawn());
		ensureReturn(PawnExtComponent);

		UScWAbilitySystemComponent* PawnASC = PawnExtComponent->GetScWAbilitySystemComponent();
		ensureReturn(PawnASC);

		FScWAbilitySet_GrantedHandles GrantedHandles = PerInstanceGrantedHandles[InInstance];
		GrantedHandles.TakeFromAbilitySystem(PawnASC);

		PerInstanceGrantedHandles.Remove(InInstance);
	}
}
//~ End Ability System
