// Copyright 2018 Ryan Gadz, Inc. All Rights Reserved.

#pragma once
#include "Runtime/Engine/Classes/Components/InstancedStaticMeshComponent.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "OctreeEnums.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "OctreePluginBPLibrary.generated.h"


/* 
*	Function library class.
*	Each function in it is expected to be static and represents blueprint node that can be called in any blueprint.
*
*	When declaring function you can define metadata for the node. Key function specifiers will be BlueprintPure and BlueprintCallable.
*	BlueprintPure - means the function does not affect the owning object in any way and thus creates a node without Exec pins.
*	BlueprintCallable - makes a function which can be executed in Blueprints - Thus it has Exec pins.
*	DisplayName - full name of the node, shown when you mouse over the node and in the blueprint drop down menu.
*				Its lets you name the node using characters not allowed in C++ function names.
*	CompactNodeTitle - the word(s) that appear on the node.
*	Keywords -	the list of keywords that helps you to find node when you search for it using Blueprint drop-down menu. 
*				Good example is "Print String" node which you can find also by using keyword "log".
*	Category -	the category your node will be under in the Blueprint drop-down menu.
*
*	For more info on custom blueprint nodes visit documentation:
*	https://wiki.unrealengine.com/Custom_Blueprint_Node_Creation
*/
UCLASS()
class UOctreePluginBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Execute Sample function", Keywords = "OctreePlugin sample test testing"), Category = "OctreePluginTesting")
	static float OctreePluginSampleFunction(float Param);



	// UFUNCTION(BlueprintCallable, meta = (DisplayName = "Build Octree", 
	// Keywords = "OctreePlugin sample test testing"), 
	// Category = "OctreePluginTesting")
	static void Octree(
		class UObject* WorldContextObject,
		class UInstancedStaticMeshComponent *InstancedMesh, 
		const FVector &Location,
		const TArray<TEnumAsByte<EObjectTypeQuery>> &ObjectTypes,
		const float &Size,
		int32& finalIndex,
		const int32 MaxIterations = 6,
		const int32 CurrentIterations = 0);

	static void OctreeSub(
		class UObject* WorldContextObject,
		class UInstancedStaticMeshComponent *InstancedMesh, 
		const FVector &Location,
		const TArray<TEnumAsByte<EObjectTypeQuery>> &ObjectTypes,
		const float &Size,
		int32& finalIndex,
		const int32 MaxIterations = 6,
		const int32 CurrentIterations = 0);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "VoxelAdd", 
	Keywords = "OctreePlugin sample test testing"), 
	Category = "OctreePluginTesting")
	static void VoxelAdd(
		class UObject* WorldContextObject,
		class UInstancedStaticMeshComponent *InstancedMesh, 
		const FVector &Location,
		const TArray<TEnumAsByte<EObjectTypeQuery>> &ObjectTypes,
		const float &Size,
		const int32 MaxIterations = 6,
		const int32 CurrentIterations = 0,
		const EVoxelType1 VoxelType = EVoxelType1::E_Add);
	


};
