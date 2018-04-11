// Copyright 2018 Ryan Gadz, Inc. All Rights Reserved.

#include "VoxelAddComponent.h"

// Sets default values for this component's properties
UVoxelAddComponent::UVoxelAddComponent(const FObjectInitializer &ObjectInitializer) : Super(ObjectInitializer)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.
	// You can turn these features off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// if (!InstancedMesh)
	// 	InstancedMesh = ObjectInitializer.CreateDefaultSubobject<UInstancedStaticMeshComponent>(this, TEXT("CubeInstaces"));

	// if (!StaticMesh)
	// {
	// 	StaticMesh = ObjectInitializer.CreateDefaultSubobject<UStaticMesh>(this, TEXT("MyMesh"));
	// 	static ConstructorHelpers::FObjectFinder<UStaticMesh> MyMesh(TEXT("/Game/CubeUV.CubeUV"));
	// 	StaticMesh = MyMesh.Object;
	// 	InstancedMesh->SetStaticMesh(StaticMesh);
	// }

	// if (!Material)
	// {
	// 	Material = ObjectInitializer.CreateDefaultSubobject<UMaterialInterface>(this, TEXT("MyMaterial"));
	// 	static ConstructorHelpers::FObjectFinder<UMaterialInterface> MyMaterial(TEXT("/Game/M_Color1.M_Color1"));
	// 	Material = MyMaterial.Object;
	// 	InstancedMesh->SetMaterial(0, Material);
	// }

	WorldContextObject = GetOwner();

	UOctreePluginBPLibrary::VoxelAdd(
		WorldContextObject,
		InstancedMesh,
		Location,
		ObjectTypes,
		Size,
		IterationsMax,
		0);
}

#if WITH_EDITOR
void UVoxelAddComponent::PostEditChangeProperty(FPropertyChangedEvent &PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FName MemberPropertyChanged = (PropertyChangedEvent.MemberProperty ? 
	 							   PropertyChangedEvent.MemberProperty->GetFName() : NAME_None);

	if ((MemberPropertyChanged == GET_MEMBER_NAME_CHECKED(UVoxelAddComponent, Material)))
	{
		InstancedMesh->SetMaterial(0,Material);
	}
	else if ((MemberPropertyChanged == GET_MEMBER_NAME_CHECKED(UVoxelAddComponent, StaticMesh)))
	{
		InstancedMesh->SetStaticMesh(StaticMesh);
	}

WorldContextObject = GetOwner();
		UOctreePluginBPLibrary::VoxelAdd(
			WorldContextObject,
			InstancedMesh,
			Location,
			ObjectTypes,
			Size,
			IterationsMax,
			0);
}
#endif

// Called when the game starts
void UVoxelAddComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UVoxelAddComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
WorldContextObject = GetOwner();
	// ...
		 UOctreePluginBPLibrary::VoxelAdd(
		WorldContextObject,
		InstancedMesh, 
		Location,
		ObjectTypes,
		Size,
		IterationsMax,
		0);

}

