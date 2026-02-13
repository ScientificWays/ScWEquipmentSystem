// Scientific Ways

#include "Equipment/ScWEquipmentActor_Melee.h"

#include "AbilitySystem/ScWCoreTags.h"
#include "AbilitySystem/ScWAbilitySystemGlobals.h"

#include "Animation/ScWAnimationsFunctionLibrary.h"

#include "Equipment/ScWEquipmentFragment.h"
#include "Equipment/ScWEquipmentInstance.h"
#include "Equipment/ScWEquipmentDefinition.h"
#include "Equipment/ScWEquipmentFragment_MeleeBase.h"

#include "Character/ScWCharacter.h"

#include "Damage/ScWDamageType.h"

AScWEquipmentActor_Melee::AScWEquipmentActor_Melee(const FObjectInitializer& InObjectInitializer)
	: Super(InObjectInitializer)
{
	SwingBaseDamage = 10.0f;
	SwingBaseDamageTypeClass = UScWDamageType::StaticClass();
	bSwingOwnerEffectOnlyDuringSwing = true;

	PostSwingComboTimeWindow = 0.4f;
	SwingAIMaxRange = 128.0f;
	SwingAIMaxRange_BlackboardKeyName = TEXT("MeleeRangeMax");

	SwingVariantBaseDuration = 1.0f;

	SwingParticlesAttachmentSocketName = NAME_None;
	SwingParticlesRelativeTransform = FTransform(FRotator::ZeroRotator, FVector(0.0f, 0.0f, 0.0f), FVector::OneVector);

	PreSwingMontageSectionIndex = 0;
	SwingMontageSectionIndex = 1;
	EndSwingMontageSectionIndex = 2;
}

//~ Begin Initialize
void AScWEquipmentActor_Melee::OnConstruction(const FTransform& InTransform) // AActor
{
	Super::OnConstruction(InTransform);

	
}

void AScWEquipmentActor_Melee::BeginPlay() // AActor
{
	const UScWEquipmentFragment_MeleeBase* MeleeFragment = GetMeleeFragment();
	ensureReturn(MeleeFragment);

	SwingCounter = 0;

	BP_ResetSwingComponents();

	SwingHitIgnoredActors = { GetInstigator(), this};
	TracePatternIgnoredActors = { GetInstigator(), this };

	if (UShapeComponent* CollisionComponent = BP_GetCollisionComponent())
	{
		CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AScWEquipmentActor_Melee::OnCollisionComponentBeginOverlap);
	}
	Super::BeginPlay();
}
//~ End Initialize

//~ Begin Components
void AScWEquipmentActor_Melee::OnCollisionComponentBeginOverlap(UPrimitiveComponent* InOverlappedComponent, AActor* InOtherActor, UPrimitiveComponent* InOtherComponent, int32 InOtherBodyIndex, bool bInFromSweep, const FHitResult& InSweepHitResult)
{
	if (InOtherActor == GetInstigator())
	{
		return;
	}
	if (bInFromSweep)
	{
		BP_HandleSwingHit(InOtherActor, InSweepHitResult);
	}
	else
	{
		FHitResult MinimalInfoHitResult;
		UScWDamageType::MakeMinimalInfoDamageImpactHitResult(this, InOverlappedComponent, GetInstigator(), InOtherActor, InOtherComponent, MinimalInfoHitResult);
		BP_HandleSwingHit(InOtherActor, MinimalInfoHitResult);
	}
}
//~ End Components

//~ Begin Equipment
void AScWEquipmentActor_Melee::BP_InitFromEquipmentInstance_Implementation(const UScWEquipmentInstance* InInstance) // IScWEquipmentActorInterface
{
	EquipmentInstance = InInstance;
}

const UScWEquipmentFragment_MeleeBase* AScWEquipmentActor_Melee::GetMeleeFragment() const
{
	ensureReturn(EquipmentInstance.IsValid(), nullptr);

	const UScWEquipmentDefinition* EquipmentDefinition = EquipmentInstance->GetEquipmentDefinition();
	ensureReturn(EquipmentDefinition, nullptr);
	return EquipmentDefinition->GetFragmentByClass<UScWEquipmentFragment_MeleeBase>();
}
//~ End Equipment

//~ Begin Swing
float AScWEquipmentActor_Melee::BP_GetSwingDamage_Implementation() const
{
	return SwingBaseDamage;
}

TSubclassOf<UScWDamageType> AScWEquipmentActor_Melee::BP_GetSwingDamageTypeClass_Implementation() const
{
	return SwingBaseDamageTypeClass;
}

float AScWEquipmentActor_Melee::BP_PreSwing_Implementation()
{
	CurrentSwingPhase = EScWSwingPhase::PreSwing;

	++SwingCounter;
	BP_UpdateCurrentSwingVariantData();

	const auto& CurrentSwingMontageData = CurrentSwingVariantData.MontageData;

	ensureReturn(CurrentSwingMontageData.GetRelevantTimingMontage(), -1.0f);
	ensureReturn(CurrentSwingMontageData.GetRelevantTimingMontage()->GetNumSections() >= 3, -1.0f);

	ACharacter* InstigatorCharacter = GetInstigator<ACharacter>();
	ensureReturn(InstigatorCharacter, -1.0f);
	UScWAnimationsFunctionLibrary::PlayCharacterMontagesFromData(InstigatorCharacter, CurrentSwingMontageData);

	float OutPreSwingDelay = UScWAnimationsFunctionLibrary::GetMontageSectionLengthByIndexFromData(CurrentSwingMontageData, PreSwingMontageSectionIndex) * SwingVariantBaseDuration;
	OnPreSwingDelegate.Broadcast(OutPreSwingDelay);
	return OutPreSwingDelay;
}

float AScWEquipmentActor_Melee::BP_BeginSwing_Implementation(float InSwingDamage, TSubclassOf<UDamageType> InSwingDamageTypeClass)
{
	CurrentSwingPhase = EScWSwingPhase::Swing;

	LastSwingDamage = InSwingDamage;
	LastSwingDamageTypeClass = InSwingDamageTypeClass;
	LastSwingAffectedActorArray.Empty();

	SwingPaticlesComponent = BP_InitializeSwingParticles();

	if (UShapeComponent* CollisionComponent = BP_GetCollisionComponent())
	{
		CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	else
	{
		BP_BeginTracePatterns();
	}
	float OutSwingDuration = UScWAnimationsFunctionLibrary::GetMontageSectionLengthByIndexFromData(CurrentSwingVariantData.MontageData, SwingMontageSectionIndex) * SwingVariantBaseDuration;
	OnBeginSwingDelegate.Broadcast(OutSwingDuration);
	return OutSwingDuration;
}

float AScWEquipmentActor_Melee::BP_EndSwing_Implementation(const bool bInWasCancelled)
{
	BP_ResetSwingComponents();

	float OutEndSwingDelay = UScWAnimationsFunctionLibrary::GetMontageSectionLengthByIndexFromData(CurrentSwingVariantData.MontageData, EndSwingMontageSectionIndex) * SwingVariantBaseDuration;
	OnEndSwingDelegate.Broadcast(OutEndSwingDelay, bInWasCancelled);
	return OutEndSwingDelay;
}

void AScWEquipmentActor_Melee::BP_ResetSwingComponents_Implementation()
{
	if (SwingPaticlesComponent)
	{
		SwingPaticlesComponent->DestroyComponent();
		SwingPaticlesComponent = nullptr;
	}
	if (UShapeComponent* CollisionComponent = BP_GetCollisionComponent())
	{
		CollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	CurrentSwingPhase = EScWSwingPhase::None;
}

void AScWEquipmentActor_Melee::BP_UpdateCurrentSwingVariantData_Implementation()
{
	const UScWEquipmentFragment_MeleeBase* MeleeFragment = GetMeleeFragment();
	ensureReturn(MeleeFragment);

	TArray<FScWMeleeSwingVariantData> VariantsArray = SwingVariants;
	MeleeFragment->BP_ModifySwingVariants(this, VariantsArray);

	ensureReturn(!VariantsArray.IsEmpty());
	CurrentSwingVariantData = VariantsArray[SwingCounter % VariantsArray.Num()];
}

bool AScWEquipmentActor_Melee::BP_HandleSwingHit_Implementation(AActor* InHitActor, const FHitResult& InHitResult)
{
	ensureReturn(CurrentSwingPhase == EScWSwingPhase::Swing, false);
	ensureReturn(InHitActor, false);

	if (SwingHitIgnoredActors.Contains(InHitActor) || LastSwingAffectedActorArray.Contains(InHitActor))
	{
		return false;
	}
	else
	{
		LastSwingAffectedActorArray.Add(InHitActor);

		FVector HitDirection = (InHitResult.TraceEnd - InHitResult.TraceStart).GetSafeNormal();

		ensureReturn(GetInstigator(), false);
		UScWDamageType::ApplyPointDamage(this, InHitActor, LastSwingDamage, HitDirection, InHitResult, GetInstigator()->GetController(), LastSwingDamageTypeClass);
		return true;
	}
}
//~ End Swing

//~ Begin Swing FX
USceneComponent* AScWEquipmentActor_Melee::BP_GetFXAttachComponent_Implementation() const
{
	ACharacter* InstigatorCharacter = Cast<ACharacter>(GetInstigator());
	return InstigatorCharacter ? InstigatorCharacter->GetMesh() : nullptr;
}

UNiagaraComponent* AScWEquipmentActor_Melee::BP_InitializeSwingParticles_Implementation() const
{
	if (DefaultSwingParticles)
	{
		USceneComponent* AttachToComponent = BP_GetFXAttachComponent();

		FFXSystemSpawnParameters SpawnParams;
		SpawnParams.WorldContextObject = this;
		SpawnParams.SystemTemplate = DefaultSwingParticles;
		SpawnParams.Location = SwingParticlesRelativeTransform.GetLocation();
		SpawnParams.Rotation = SwingParticlesRelativeTransform.GetRotation().Rotator();
		SpawnParams.Scale = SwingParticlesRelativeTransform.GetScale3D();
		SpawnParams.AttachToComponent = AttachToComponent;
		SpawnParams.AttachPointName = SwingParticlesAttachmentSocketName;
		SpawnParams.LocationType = EAttachLocation::KeepRelativeOffset;
		SpawnParams.bAutoDestroy = true;
		SpawnParams.bAutoActivate = true;
		SpawnParams.PoolingMethod = EPSCPoolMethod::None;
		SpawnParams.bPreCullCheck = true;
		SpawnParams.bIsPlayerEffect = UScWAbilitySystemGlobals::TargetHasMatchingGameplayTag(GetInstigator(), FScWCoreTags::Pawn_Type_Player);
		return UNiagaraFunctionLibrary::SpawnSystemAttachedWithParams(SpawnParams);
	}
	return nullptr;
}
//~ End Swing FX

//~ Begin Patterns
FVector AScWEquipmentActor_Melee::BP_GetPatternStartLocation_Implementation(const FScWMeleeSwingVariantData_TracePattern& InPatternData, int32 InPatternIndex) const
{
	ensureReturn(GetInstigator(), GetActorLocation());
	return GetInstigator()->GetPawnViewLocation() + GetInstigator()->GetViewRotation().Vector() * InPatternData.TraceOffsetLocation;
}

float AScWEquipmentActor_Melee::BP_GetNextPatternDelayTime_Implementation(int32 InNextPatternIndex) const
{
	const float SwingDuration = UScWAnimationsFunctionLibrary::GetMontageSectionLengthByIndexFromData(CurrentSwingVariantData.MontageData, SwingMontageSectionIndex) * SwingVariantBaseDuration;
	const auto& TracePatterns = CurrentSwingVariantData.TracePatterns;
	return (TracePatterns.Num() < 2) ? (0.0f) : (SwingDuration / (float)TracePatterns.Num());
}

void AScWEquipmentActor_Melee::BP_BeginTracePatterns_Implementation()
{
	ensureReturn(!CurrentSwingVariantData.TracePatterns.IsEmpty());
	BP_HandleTracePattern(CurrentSwingVariantData.TracePatterns[0], 0);
}

void AScWEquipmentActor_Melee::BP_HandleTracePattern_Implementation(const FScWMeleeSwingVariantData_TracePattern& InPatternData, int32 InPatternIndex)
{
	ensureReturn(GetInstigator());

	UWorld* World = GetWorld();
	ensureReturn(World);
	
	FVector TraceStart = BP_GetPatternStartLocation(InPatternData, InPatternIndex);
	FVector TraceEnd = TraceStart + (GetInstigator()->GetBaseAimRotation() + InPatternData.TraceOffsetRotation).Vector() * InPatternData.TraceLength;

	TArray<FHitResult> TraceHitResults;
	FCollisionQueryParams TraceParams = FCollisionQueryParams::DefaultQueryParam;
	UKismetSystemLibrary::SphereTraceMulti(this, TraceStart, TraceEnd, InPatternData.TraceShapeRadius, TraceTypeQuery_Melee, false, TracePatternIgnoredActors, TracePatternDebugType, TraceHitResults, true);
	
	/*if (TraceHitResults.IsEmpty())
	{
		return;
	}*/
	for (const FHitResult& SampleHitResult : TraceHitResults)
	{
		BP_HandleSwingHit(SampleHitResult.GetActor(), SampleHitResult);
	}
	int32 NextPatternIndex = InPatternIndex + 1;
	if (CurrentSwingVariantData.TracePatterns.IsValidIndex(NextPatternIndex))
	{
		float NextPatternDelayTime = BP_GetNextPatternDelayTime(NextPatternIndex);
		if (NextPatternDelayTime > 0.0f)
		{
			FTimerDelegate NextPatternMethodDelegate;
			NextPatternMethodDelegate.BindUFunction(this, GET_FUNCTION_NAME_CHECKED_TwoParams(ThisClass, BP_HandleTracePattern, const FScWMeleeSwingVariantData_TracePattern&, int32), CurrentSwingVariantData.TracePatterns[NextPatternIndex], NextPatternIndex);
			World->GetTimerManager().SetTimer(NextPatternDelayHandle, NextPatternMethodDelegate, NextPatternDelayTime, false);
		}
		else
		{
			BP_HandleTracePattern(InPatternData, NextPatternIndex);
		}
	}
}
//~ End Patterns
