// Scientific Ways

#include "Equipment/Abilities/ScWGameplayAbility_CommonSwing.h"

#include "AbilitySystem/ScWCoreTags.h"
#include "AbilitySystem/Tasks/ScWAT_WaitDelay.h"
#include "AbilitySystem/ScWAbilitySystemGlobals.h"

#include "Damage/ScWDamageType.h"

#include "Character/ScWCharacter.h"

#include "Equipment/ScWEquipmentTags.h"
#include "Equipment/ScWEquipmentActor_Melee.h"
#include "Equipment/ScWEquipmentFunctionLibrary.h"

UScWGameplayAbility_CommonSwing::UScWGameplayAbility_CommonSwing()
{
	{
		auto Tags = GetAssetTags();
		Tags.AddTag(FScWCoreTags::Ability_Handheld_Swing);
		SetAssetTags(Tags);
	}
	CancelAbilitiesWithTag.AddTag(FScWCoreTags::Ability_CancelBy_SwingActivate);
	BlockAbilitiesWithTag.AddTag(FScWCoreTags::Ability_CancelBy_SwingActivate);

	ActivationOwnedTags.AddTag(FScWCoreTags::State_Swinging);

	bCaptureDamageDataOnBeginSwingSequence = true;

	bLoopIfInputIsPressed = true;
}

//~ Begin Ability
void UScWGameplayAbility_CommonSwing::ActivateAbility(const FGameplayAbilitySpecHandle InHandle, const FGameplayAbilityActorInfo* InActorInfo, const FGameplayAbilityActivationInfo InActivationInfo, const FGameplayEventData* InTriggerEventData) // UScWGameplayAbility
{
	ensureCancelAbilityReturn(InActorInfo);

	ACharacter* OwnerCharacter = Cast<ACharacter>(CurrentActorInfo->AvatarActor.Get());
	ensureCancelAbilityReturn(OwnerCharacter);

	if (CommitAbility(InHandle, InActorInfo, InActivationInfo))
	{
		OwnerMeleeEquipmentActor = Cast<AScWEquipmentActor_Melee>(UScWEquipmentFunctionLibrary::GetOwnerEquipmentActorWithDefinitionTag(OwnerCharacter, FScWEquipmentTags::Equipment_Type_Weapon));
		ensureCancelAbilityReturn(OwnerMeleeEquipmentActor);

		BeginSwingSequence();
	}
	else
	{
		K2_CancelAbility();
	}
}

void UScWGameplayAbility_CommonSwing::BeginSwingSequence()
{
	if (bCaptureDamageDataOnBeginSwingSequence)
	{
		CapturedSwingDamage = BP_GetSwingDamage();
		CapturedSwingDamageTypeClass = BP_GetSwingDamageTypeClass();
	}
	float PreSwingDelay = BP_HandlePreSwing();

	COMMON_WAIT_DELAY_OR_FINISH_NEXT_TICK_TASK(PreSwingWaitDelayTask, PreSwingDelay, OnPreSwingDelayFinished);
}

void UScWGameplayAbility_CommonSwing::OnPreSwingDelayFinished()
{
	float SwingDuration = BP_HandleBeginSwing();

	COMMON_WAIT_DELAY_OR_FINISH_NEXT_TICK_TASK(PatternsWaitDelayTask, SwingDuration, OnSwingFinished);
}

void UScWGameplayAbility_CommonSwing::OnSwingFinished()
{
	float PostSwingDelay = BP_HandleEndSwing();

	if (bLoopIfInputIsPressed && IsAbilityInputPressed())
	{
		BeginSwingSequence();
	}
	else
	{
		COMMON_WAIT_DELAY_OR_FINISH_NEXT_TICK_TASK(PostSwingWaitDelayTask, PostSwingDelay, OnPostSwingDelayFinished);
	}
}

void UScWGameplayAbility_CommonSwing::OnPostSwingDelayFinished()
{
	BP_HandlePostSwing();
}

void UScWGameplayAbility_CommonSwing::EndAbility(const FGameplayAbilitySpecHandle InHandle, const FGameplayAbilityActorInfo* InActorInfo, const FGameplayAbilityActivationInfo InActivationInfo, bool bInReplicateEndAbility, bool bInWasCancelled) // UGameplayAbility
{
	Super::EndAbility(InHandle, InActorInfo, InActivationInfo, bInReplicateEndAbility, bInWasCancelled);

	if (CurrentSwingEffectHandle.IsValid())
	{
		BP_RemoveSwingOwnerEffect();
	}
	ensureReturn(OwnerMeleeEquipmentActor);
	if (OwnerMeleeEquipmentActor->GetCurrentSwingPhase() != EScWSwingPhase::None)
	{
		OwnerMeleeEquipmentActor->BP_EndSwing(bInWasCancelled);
	}
}
//~ End Ability

//~ Begin Swing
float UScWGameplayAbility_CommonSwing::BP_HandlePreSwing_Implementation()
{
	ensureCancelAbilityReturn(OwnerMeleeEquipmentActor, 0.0f);
	float OutPreSwingDelay = OwnerMeleeEquipmentActor->BP_PreSwing();
	ensureCancelAbilityReturn(OutPreSwingDelay >= 0.0f, 0.0f);

	if (OwnerMeleeEquipmentActor->SwingOwnerEffect && !OwnerMeleeEquipmentActor->bSwingOwnerEffectOnlyDuringSwing)
	{
		BP_ApplySwingOwnerEffect();
	}
	return OutPreSwingDelay;
}

float UScWGameplayAbility_CommonSwing::BP_HandleBeginSwing_Implementation()
{
	ensureCancelAbilityReturn(OwnerMeleeEquipmentActor, 0.0f);

	float OutSwingDuration = 0.0f;

	if (bCaptureDamageDataOnBeginSwingSequence)
	{
		OutSwingDuration = OwnerMeleeEquipmentActor->BP_BeginSwing(CapturedSwingDamage, CapturedSwingDamageTypeClass);
	}
	else
	{
		OutSwingDuration = OwnerMeleeEquipmentActor->BP_BeginSwing(BP_GetSwingDamage(), BP_GetSwingDamageTypeClass());
	}
	ensureCancelAbilityReturn(OutSwingDuration >= 0.0f, 0.0f);

	if (OwnerMeleeEquipmentActor->SwingOwnerEffect && OwnerMeleeEquipmentActor->bSwingOwnerEffectOnlyDuringSwing)
	{
		BP_ApplySwingOwnerEffect();
	}
	return OutSwingDuration;
}

float UScWGameplayAbility_CommonSwing::BP_HandleEndSwing_Implementation()
{
	ensureCancelAbilityReturn(OwnerMeleeEquipmentActor, 0.0f);
	float OutPostSwingDelay = OwnerMeleeEquipmentActor->BP_EndSwing(false);
	ensureCancelAbilityReturn(OutPostSwingDelay >= 0.0f, 0.0f);

	if (CurrentSwingEffectHandle.IsValid() && OwnerMeleeEquipmentActor->bSwingOwnerEffectOnlyDuringSwing)
	{
		BP_RemoveSwingOwnerEffect();
	}
	return OutPostSwingDelay;
}

void UScWGameplayAbility_CommonSwing::BP_HandlePostSwing_Implementation()
{
	if (CurrentSwingEffectHandle.IsValid() && !OwnerMeleeEquipmentActor->bSwingOwnerEffectOnlyDuringSwing)
	{
		BP_RemoveSwingOwnerEffect();
	}
	K2_EndAbility();
}

float UScWGameplayAbility_CommonSwing::BP_GetSwingDamage_Implementation() const
{
	ensureReturn(OwnerMeleeEquipmentActor, 0.0f);
	return OwnerMeleeEquipmentActor->BP_GetSwingDamage();
}

TSubclassOf<UScWDamageType> UScWGameplayAbility_CommonSwing::BP_GetSwingDamageTypeClass_Implementation() const
{
	ensureReturn(OwnerMeleeEquipmentActor, UScWDamageType::StaticClass());
	return OwnerMeleeEquipmentActor->BP_GetSwingDamageTypeClass();
}

void UScWGameplayAbility_CommonSwing::BP_ApplySwingOwnerEffect_Implementation()
{
	ensureCancelAbilityReturn(OwnerMeleeEquipmentActor);
	ensureCancelAbilityReturn(OwnerMeleeEquipmentActor->SwingOwnerEffect);

	if (CurrentSwingEffectHandle.IsValid())
	{
		BP_RemoveSwingOwnerEffect();
	}
	UAbilitySystemComponent* OwnerASC = GetAbilitySystemComponentFromActorInfo();
	ensureCancelAbilityReturn(OwnerASC);
	CurrentSwingEffectHandle = UScWAbilitySystemGlobals::TryApplyGameplayEffectByClass(OwnerASC, OwnerMeleeEquipmentActor->SwingOwnerEffect);
}

void UScWGameplayAbility_CommonSwing::BP_RemoveSwingOwnerEffect_Implementation()
{
	UAbilitySystemComponent* OwnerASC = GetAbilitySystemComponentFromActorInfo();
	ensureCancelAbilityReturn(OwnerASC);

	ensureCancelAbilityReturn(CurrentSwingEffectHandle.IsValid());
	OwnerASC->RemoveActiveGameplayEffect(CurrentSwingEffectHandle);

	CurrentSwingEffectHandle.Invalidate();
}
//~ End Swing
