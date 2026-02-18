// Scientific Ways

#include "EquipmentSystem/ScWEquipmentInstance.h"

#include "Tags/ScWEquipmentTags.h"
#include "EquipmentSystem/ScWEquipmentDefinition.h"
#include "EquipmentSystem/ScWEquipmentFunctionLibrary.h"
#include "EquipmentSystem/ScWEquipmentManagerComponent.h"

#include "Net/UnrealNetwork.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"

#include "GameFramework/GameplayMessageSubsystem.h"

#if UE_WITH_IRIS
#include "Iris/ReplicationSystem/ReplicationFragmentUtil.h"
#endif // UE_WITH_IRIS

#include UE_INLINE_GENERATED_CPP_BY_NAME(ScWEquipmentInstance)

class FLifetimeProperty;
class UClass;
class USceneComponent;

UScWEquipmentInstance::UScWEquipmentInstance(const FObjectInitializer& InObjectInitializer)
	: Super(InObjectInitializer)
{

}

UWorld* UScWEquipmentInstance::GetWorld() const
{
	if (APawn* OwningPawn = GetPawn())
	{
		return OwningPawn->GetWorld();
	}
	else
	{
		return nullptr;
	}
}

void UScWEquipmentInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, Instigator);
}

#if UE_WITH_IRIS
void UScWEquipmentInstance::RegisterReplicationFragments(UE::Net::FFragmentRegistrationContext& Context, UE::Net::EFragmentRegistrationFlags RegistrationFlags)
{
	using namespace UE::Net;

	// Build descriptors and allocate PropertyReplicationFragments for this object
	FReplicationFragmentUtil::CreateAndRegisterFragmentsForObject(this, Context, RegistrationFlags);
}
#endif // UE_WITH_IRIS

UScWEquipmentManagerComponent* UScWEquipmentInstance::GetOwningEquipmentManagerComponent() const
{
	AActor* OuterActor = Cast<AActor>(GetOuter());
	ensureReturn(OuterActor, nullptr);
	return UScWEquipmentFunctionLibrary::GetEquipmentManagerComponentFromActor(OuterActor);
}

APawn* UScWEquipmentInstance::GetPawn() const
{
	return Cast<APawn>(GetOuter());
}

APawn* UScWEquipmentInstance::GetTypedPawn(TSubclassOf<APawn> PawnType) const
{
	APawn* Result = nullptr;
	if (UClass* ActualPawnType = PawnType)
	{
		if (GetOuter()->IsA(ActualPawnType))
		{
			Result = Cast<APawn>(GetOuter());
		}
	}
	return Result;
}

void UScWEquipmentInstance::OnEquipped()
{
	ensureReturn(EquipmentDefinition);
	for (const UScWEquipmentFragment* SampleFragment : EquipmentDefinition->Fragments)
	{
		ensureContinue(SampleFragment);
		SampleFragment->BP_OnEquipped(this);
	}
	BP_OnEquipped();

	UGameplayMessageSubsystem& GameplayMessageSubsystem = UGameplayMessageSubsystem::Get(this);
	FGameplayMessage_EquipmentInstance EquippedMessage = { this };
	GameplayMessageSubsystem.BroadcastMessage(FScWEquipmentTags::GameplayMessage_Equipment_InstanceEquipped, EquippedMessage);
}

void UScWEquipmentInstance::OnUnequipped()
{
	ensureReturn(EquipmentDefinition);
	for (const UScWEquipmentFragment* SampleFragment : EquipmentDefinition->Fragments)
	{
		ensureContinue(SampleFragment);
		SampleFragment->BP_OnUnequipped(this);
	}
	BP_OnUnequipped();

	UGameplayMessageSubsystem& GameplayMessageSubsystem = UGameplayMessageSubsystem::Get(this);
	FGameplayMessage_EquipmentInstance UnequippedMessage = { this };
	GameplayMessageSubsystem.BroadcastMessage(FScWEquipmentTags::GameplayMessage_Equipment_InstanceUnequipped, UnequippedMessage);
}

void UScWEquipmentInstance::OnRep_Instigator()
{

}

void UScWEquipmentInstance::OnRep_EquipmentDefinition()
{

}
