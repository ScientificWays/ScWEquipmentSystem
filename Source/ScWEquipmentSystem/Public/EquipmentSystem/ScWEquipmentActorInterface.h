// Scientific Ways

#pragma once

#include "CoreMinimal.h"

#include "ScWEquipmentActorInterface.generated.h"

#define MODULE_API SCWEQUIPMENTSYSTEM_API

class UScWEquipmentInstance;

/**
 *	Implemented by spawned equipment actors that need initialization from an equipment instance.
 */
UINTERFACE(MinimalAPI, BlueprintType)
class UScWEquipmentActorInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *	Provides the equipment-instance initialization contract for spawned equipment actors.
 */
class IScWEquipmentActorInterface
{
	GENERATED_BODY()
	
//~ Begin Equipment Instance
public:
	
	UFUNCTION(Category = "Equipment Instance", BlueprintNativeEvent, meta = (DisplayName = "Init From Equipment Instance"))
	void BP_InitFromEquipmentInstance(const UScWEquipmentInstance* InInstance);
	virtual void BP_InitFromEquipmentInstance_Implementation(const UScWEquipmentInstance* InInstance) {}
//~ End Equipment Instance
};

#undef MODULE_API
