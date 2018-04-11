// Copyright 2018 Ryan Gadz, Inc. All Rights Reserved.

#include "OctreePluginBPLibrary.h"
#include "OctreePlugin.h"

UOctreePluginBPLibrary::UOctreePluginBPLibrary(const FObjectInitializer &ObjectInitializer)
	: Super(ObjectInitializer)
{
}

float UOctreePluginBPLibrary::OctreePluginSampleFunction(float Param)
{
	return -1;
}

TArray<FVector> Subdivided = {
	FVector(-.5f, -.5f, -.5f),
	FVector(-.5f, .5f, -.5f),
	FVector(.5f, -.5f, -.5f),
	FVector(.5f, .5f, -.5f),
	FVector(-.5f, -.5f, .5f),
	FVector(-.5f, .5f, .5f),
	FVector(.5f, -.5f, .5f),
	FVector(.5f, .5f, .5f),

};

UClass *ClassFilter = nullptr;
TArray<AActor *> ActorsToIgnore;
TArray<UPrimitiveComponent *> OutComponents;
int32 index = 0;

void UOctreePluginBPLibrary::Octree(
	class UObject* WorldContextObject,
	class UInstancedStaticMeshComponent *InstancedMesh,
	const FVector &Location,
	const TArray<TEnumAsByte<EObjectTypeQuery>> &ObjectTypes,
	const float &Size,
	int32 &finalIndex,
	const int32 MaxIterations,
	const int32 CurrentIterations)
{
	FVector BoxPos;
	FVector BoxExtent;
	if (!InstancedMesh)
		return;
	float currentSize = Size / 2;
	for (int32 i = 0; i <= 7; i++)
	{
		BoxPos = Subdivided[i] * (currentSize) + Location;
		BoxExtent = FVector(currentSize / 2);
		FTransform locTrans(
			FQuat(0.f, 0.f, 0.f, 1.f),
			BoxPos,
			FVector(currentSize / 8));
		if (UKismetSystemLibrary::BoxOverlapComponents(WorldContextObject, BoxPos, BoxExtent, ObjectTypes,
													   ClassFilter, ActorsToIgnore, OutComponents))
		{
			if (MaxIterations > (CurrentIterations + 1))
				Octree(WorldContextObject, InstancedMesh, BoxPos, ObjectTypes,
					   currentSize, finalIndex, MaxIterations, CurrentIterations + 1);
			else
			{
				if(InstancedMesh->GetInstanceCount() <= finalIndex)
				//if (!InstancedMesh->UpdateInstanceTransform(finalIndex, locTrans, true, false, true))
				{
					InstancedMesh->AddInstanceWorldSpace(locTrans);
				}
				else
					InstancedMesh->UpdateInstanceTransform(finalIndex, locTrans, true, false, true);
				finalIndex++;
			}
		}
	}
}

void UOctreePluginBPLibrary::VoxelAdd(
	class UObject *WorldContextObject,
	class UInstancedStaticMeshComponent *InstancedMesh,
	const FVector &Location,
	const TArray<TEnumAsByte<EObjectTypeQuery>> &ObjectTypes,
	const float &Size,
	const int32 MaxIterations,
	const int32 CurrentIterations,
	const EVoxelType1 VoxelType)
{
	if (!InstancedMesh)
		return;
	int32 finalIndex = 0;
	FTransform OutInstanceTransform(
		FQuat(0.f, 0.f, 0.f, 1.f),
		FVector(0.f),
		FVector(0.f));
	//	InstancedMesh->ClearInstances();
	if(VoxelType == EVoxelType1::E_Add)
	Octree(WorldContextObject, InstancedMesh, Location, ObjectTypes, Size, finalIndex, MaxIterations, CurrentIterations);
	else
	OctreeSub(WorldContextObject, InstancedMesh, Location, ObjectTypes, Size, finalIndex, MaxIterations, CurrentIterations);
	int32 instanceCount = InstancedMesh->GetInstanceCount();
	if (instanceCount > (finalIndex))
	{
		for (int32 i = finalIndex; i <= instanceCount; ++i)
			// removing the instance doesnt actually remove them until the next frame it seems
			// so, adding a transform of size 0 actually is the fastest and cleanest approach
			//	InstancedMesh->RemoveInstance(i)
			InstancedMesh->UpdateInstanceTransform(i, OutInstanceTransform, true, true, true);
	}
	if (InstancedMesh->GetInstanceTransform(0, OutInstanceTransform, true))
	{
		InstancedMesh->GetInstanceTransform(0, OutInstanceTransform, true);
		InstancedMesh->UpdateInstanceTransform(0, OutInstanceTransform, true, true, true);
	}

}

void UOctreePluginBPLibrary::OctreeSub(
	class UObject *WorldContextObject,
	class UInstancedStaticMeshComponent *InstancedMesh,
	const FVector &Location,
	const TArray<TEnumAsByte<EObjectTypeQuery>> &ObjectTypes,
	const float &Size,
	int32 &finalIndex,
	const int32 MaxIterations,
	const int32 CurrentIterations)
{
	FVector BoxPos;
	FVector BoxExtent;
	if (!InstancedMesh)
		return;
	float currentSize = Size / 2;
	for (int32 i = 0; i <= 7; i++)
	{
		BoxPos = Subdivided[i] * (currentSize) + Location;
		BoxExtent = FVector(currentSize / 2);
		FTransform locTrans(
			FQuat(0.f, 0.f, 0.f, 1.f),
			BoxPos,
			FVector(currentSize / 8));
		if (UKismetSystemLibrary::BoxOverlapComponents(WorldContextObject, BoxPos, BoxExtent, ObjectTypes,
													   ClassFilter, ActorsToIgnore, OutComponents))
		{
			if (MaxIterations > (CurrentIterations + 1))
				OctreeSub(WorldContextObject, InstancedMesh, BoxPos, ObjectTypes,
					   currentSize, finalIndex, MaxIterations, CurrentIterations + 1);
		}
		else
		{
			if (InstancedMesh->GetInstanceCount() <= finalIndex)
			{
				InstancedMesh->AddInstanceWorldSpace(locTrans);
			}
			else
				InstancedMesh->UpdateInstanceTransform(finalIndex, locTrans, true, false, true);
			finalIndex++;
		}
	}
}