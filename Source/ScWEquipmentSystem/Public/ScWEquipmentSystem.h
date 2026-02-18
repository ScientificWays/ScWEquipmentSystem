// Scientific Ways

#pragma once

#include "CoreMinimal.h"

#include "Engine/World.h"
#include "Templates/SubclassOf.h"

#include "Kismet/KismetSystemLibrary.h"

#include "GameFramework/Actor.h"
#include "Components/PawnComponent.h"

#include "GameplayEffect.h"

#include "Net/UnrealNetwork.h"
#include "Net/Serialization/FastArraySerializer.h"

#include "Utils/ScWUtils.h"

class FScWEquipmentSystemModule : public IModuleInterface
{
public:
	virtual void StartupModule() override; // IModuleInterface
	virtual void ShutdownModule() override; // IModuleInterface
};
