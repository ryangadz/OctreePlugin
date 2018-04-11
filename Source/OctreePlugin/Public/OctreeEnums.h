// // Copyright 2018 Ryan Gadz, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedEnum.h"
#include "OctreeEnums.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EVoxelType1 : uint8
{
	E_Add UMETA(DisplayName = "Additive"),
	E_Sub UMETA(DisplayName = "Subtractive")
};

UCLASS()
class OCTREEPLUGIN_API UOctreeEnums : public UUserDefinedEnum
{
	GENERATED_BODY()
};
