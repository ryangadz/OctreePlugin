// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

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
	class AActor *Actor,
	class UInstancedStaticMeshComponent *InstancedMesh,
	const FVector &Location,
	const TArray<TEnumAsByte<EObjectTypeQuery>> &ObjectTypes,
	const float &Size,
	int32& finalIndex,
	const int32 MaxIterations,
	const int32 CurrentIterations)
{
	FVector BoxPos;
	FVector BoxExtent;

	float currentSize = Size / 2;
	for (int32 i = 0; i <= 7; i++)
	{
		BoxPos = Subdivided[i] * (currentSize) + Location;
		BoxExtent = FVector(currentSize / 2);
		FTransform locTrans(
			FQuat(0.f, 0.f, 0.f, 1.f),
			BoxPos,
			FVector(currentSize / 8));
		if (UKismetSystemLibrary::BoxOverlapComponents(Actor, BoxPos, BoxExtent, ObjectTypes,
													   ClassFilter, ActorsToIgnore, OutComponents))
		{
			if (MaxIterations > (CurrentIterations + 1))
				Octree(Actor, InstancedMesh, BoxPos, ObjectTypes,
					   currentSize, finalIndex, MaxIterations, CurrentIterations + 1);
			else
			{
				InstancedMesh->UpdateInstanceTransform(finalIndex, locTrans, true, false, true);
				if (!InstancedMesh->UpdateInstanceTransform(finalIndex, locTrans, true, false, true))
				{
					InstancedMesh->AddInstanceWorldSpace(locTrans);
				}
				finalIndex++;
			}
		}
	}
}

void UOctreePluginBPLibrary::VoxelAdd(
	class AActor *Actor,
	class UInstancedStaticMeshComponent *InstancedMesh,
	const FVector &Location,
	const TArray<TEnumAsByte<EObjectTypeQuery>> &ObjectTypes,
	const float &Size,
	const int32 MaxIterations,
	const int32 CurrentIterations)
{
	int32 finalIndex = 0;
			FTransform OutInstanceTransform(
			FQuat(0.f, 0.f, 0.f, 1.f),
			FVector(0.f),
			FVector(0.f));
	Octree(Actor, InstancedMesh, Location, ObjectTypes, Size, finalIndex, MaxIterations, CurrentIterations);
	InstancedMesh->GetInstanceTransform(0, OutInstanceTransform, true);
	InstancedMesh->UpdateInstanceTransform(0,OutInstanceTransform, true, true, true);
	int32 instanceCount = InstancedMesh->GetInstanceCount()-1;
	if (instanceCount>(finalIndex))
	{
		 for (int32 i = finalIndex; i <= instanceCount; i++)
		 	InstancedMesh->RemoveInstance(i);
	}

}
