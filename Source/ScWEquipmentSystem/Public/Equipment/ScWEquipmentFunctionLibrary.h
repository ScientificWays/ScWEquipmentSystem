// Scientific Ways

#pragma once

#include "ScWEquipmentSystem.h"

#include "ScWEquipmentFunctionLibrary.generated.h"

#define MODULE_API SCWEQUIPMENTSYSTEM_API

/**
 * 
 */
UCLASS(MinimalAPI, meta = (DisplayName = "[ScW] Gameplay Function Library"))
class UScWEquipmentFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
//~ Begin Equipment Manager
public:

	UFUNCTION(Category = "Equipment Manager", BlueprintCallable, BlueprintPure)
	static MODULE_API class UScWEquipmentManagerComponent* GetEquipmentManagerComponentFromActor(const AActor* InActor);
//~ End Equipment Manager

//~ Begin Meshes
public:

	UFUNCTION(Category = "Meshes", BlueprintCallable, BlueprintPure)
	static MODULE_API class USkeletalMeshComponent* GetEquipmentFirstPersonMesh(const class UScWEquipmentInstance* InInstance);

	UFUNCTION(Category = "Meshes", BlueprintCallable, BlueprintPure)
	static MODULE_API class USkeletalMeshComponent* GetEquipmentThirdPersonMesh(const class UScWEquipmentInstance* InInstance);
//~ End Meshes

//~ Begin Actors
public:

	UFUNCTION(Category = "Actors", BlueprintCallable, BlueprintPure)
	static MODULE_API class AActor* GetOwnerEquipmentActorWithDefinitionTag(const class AActor* InOwnerActor, const FGameplayTag& InEquipmentTag);
//~ End Actors
};

#undef MODULE_API
