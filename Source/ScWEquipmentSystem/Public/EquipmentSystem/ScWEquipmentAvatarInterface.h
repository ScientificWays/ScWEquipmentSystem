// Scientific Ways

#pragma once

#include "ScWEquipmentSystem.h"

#include "ScWEquipmentAvatarInterface.generated.h"

#define MODULE_API SCWEQUIPMENTSYSTEM_API

class USkeletalMeshComponent;
class UScWEquipmentManagerComponent;

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UScWEquipmentAvatarInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class IScWEquipmentAvatarInterface
{
	GENERATED_BODY()
	
//~ Begin Equipment
public:
	
	UFUNCTION(Category = "Equipment", BlueprintNativeEvent, BlueprintCallable, meta = (DisplayName = "Get Equipment Manager Component"))
	UScWEquipmentManagerComponent* BP_GetEquipmentManagerComponent() const;
	UScWEquipmentManagerComponent* BP_GetEquipmentManagerComponent_Implementation() const { ensureReturn(false, nullptr); };
//~ End Equipment

//~ Begin Meshes
public:
	
	UFUNCTION(Category = "Meshes", BlueprintImplementableEvent, BlueprintCallable, meta = (DisplayName = "Get Equipment First Mesh", KeyWords = "GetMesh, GetOwnerMesh"))
	USkeletalMeshComponent* BP_GetEquipmentFirstPersonMesh() const;

	UFUNCTION(Category = "Meshes", BlueprintImplementableEvent, BlueprintCallable, meta = (DisplayName = "Get Equipment Third Person Mesh", KeyWords = "GetMesh, GetOwnerMesh"))
	USkeletalMeshComponent* BP_GetEquipmentThirdPersonMesh() const;
//~ End Meshes
};

#undef MODULE_API
