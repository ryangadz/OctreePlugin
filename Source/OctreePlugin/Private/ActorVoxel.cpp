// Fill out your copyright notice in the Description page of Project Settings.

#include "ActorVoxel.h"


// Sets default values
AActorVoxel::AActorVoxel(const FObjectInitializer &ObjectInitializer) : Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (!InstancedMesh)
		InstancedMesh = ObjectInitializer.CreateDefaultSubobject<UInstancedStaticMeshComponent>(this, TEXT("CubeInstaces"));

	if (!StaticMesh)
	{
		StaticMesh = ObjectInitializer.CreateDefaultSubobject<UStaticMesh>(this, TEXT("MyMesh"));
		static ConstructorHelpers::FObjectFinder<UStaticMesh> MyMesh(TEXT("/Game/CubeUV.CubeUV"));
		StaticMesh = MyMesh.Object;
		InstancedMesh->SetStaticMesh(StaticMesh);
	}

	if (!Material)
	{
		Material = ObjectInitializer.CreateDefaultSubobject<UMaterial>(this, TEXT("MyMaterial"));
		static ConstructorHelpers::FObjectFinder<UMaterial> MyMaterial(TEXT("/Game/M_Color1.M_Color1"));
		Material = MyMaterial.Object;
		InstancedMesh->SetMaterial(0, Material);
	}

	InstancedMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	InstancedMesh->KeepInstanceBufferCPUAccess = true; 

	UOctreePluginBPLibrary::VoxelAdd(
		this,
		InstancedMesh,
		this->GetActorLocation(),
		ObjectTypes,
		Size,
		IterationsMax,
		0);
}

#if WITH_EDITOR
void AActorVoxel::PostEditChangeProperty(FPropertyChangedEvent &PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
		FName MemberPropertyChanged = (PropertyChangedEvent.MemberProperty ? 
	 							   PropertyChangedEvent.MemberProperty->GetFName() : NAME_None);

	if ((MemberPropertyChanged == GET_MEMBER_NAME_CHECKED(AActorVoxel, Material)))
	{
		InstancedMesh->SetMaterial(0,Material);
	}
	else if ((MemberPropertyChanged == GET_MEMBER_NAME_CHECKED(AActorVoxel, StaticMesh)))
	{
		InstancedMesh->SetStaticMesh(StaticMesh);
	}


		UOctreePluginBPLibrary::VoxelAdd(
			this,
			InstancedMesh,
			this->GetActorLocation(),
			ObjectTypes,
			Size,
			IterationsMax,
			0);
}
#endif

// Called when the game starts or when spawned
void AActorVoxel::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AActorVoxel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UOctreePluginBPLibrary::VoxelAdd(
		this,
		InstancedMesh,
		this->GetActorLocation(),
		ObjectTypes,
		Size,
		IterationsMax,
		0);
}

