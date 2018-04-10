// Fill out your copyright notice in the Description page of Project Settings.

#include "ActorVoxel.h"


// Sets default values
AActorVoxel::AActorVoxel(const FObjectInitializer &ObjectInitializer) : Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 1/30.f;

		VolumeOutline = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("Outline"));
		static ConstructorHelpers::FObjectFinder<UStaticMesh> Outline(TEXT("/Game/CubeUV.CubeUV"));
		VolumeOutline->SetStaticMesh(Outline.Object);
		VolumeOutline->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		VolumeOutline->SetWorldScale3D(FVector((Size)/8.f)+.1);
		static ConstructorHelpers::FObjectFinder<UMaterial> MaterialOutline(TEXT("Material'/OctreePlugin/M_GridRuler.M_GridRuler'"));
		if (MaterialOutline.Succeeded()) BaseMat = MaterialOutline.Object;
		//VolumeOutline->bHiddenInGame = true;

		InstancedMesh = ObjectInitializer.CreateDefaultSubobject<UInstancedStaticMeshComponent>(this, TEXT("CubeInstaces"));


		StaticMesh = ObjectInitializer.CreateDefaultSubobject<UStaticMesh>(this, TEXT("MyMesh"));
		static ConstructorHelpers::FObjectFinder<UStaticMesh> MyMesh(TEXT("/Game/CubeUV.CubeUV"));
		StaticMesh = MyMesh.Object;
		InstancedMesh->SetStaticMesh(StaticMesh);



		Material = ObjectInitializer.CreateDefaultSubobject<UMaterial>(this, TEXT("MyMaterial"));
		static ConstructorHelpers::FObjectFinder<UMaterial> MyMaterial(TEXT("/Game/M_Cube.M_Cube"));
		Material = MyMaterial.Object;
		InstancedMesh->SetMaterial(0, Material);

	InstancedMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	InstancedMesh->KeepInstanceBufferCPUAccess = true; 
	InstancedMesh->UseDynamicInstanceBuffer = false;



}

void AActorVoxel::PostActorCreated()
{
	Super::PostActorCreated();

	if (BaseMat)
	{
		MaterialOutlineInst = UMaterialInstanceDynamic::Create(BaseMat, this);
		VolumeOutline->SetMaterial(0, MaterialOutlineInst);
		MaterialOutlineInst->SetScalarParameterValue("Iterations", IterationsMax);
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

#if WITH_EDITOR
void AActorVoxel::PostEditChangeProperty(FPropertyChangedEvent &PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	FName MemberPropertyChanged = (PropertyChangedEvent.MemberProperty ? PropertyChangedEvent.MemberProperty->GetFName() : NAME_None);

	if ((MemberPropertyChanged == GET_MEMBER_NAME_CHECKED(AActorVoxel, Material)))
	{
		InstancedMesh->SetMaterial(0, Material);
	}
	if ((MemberPropertyChanged == GET_MEMBER_NAME_CHECKED(AActorVoxel, StaticMesh)))
	{
		InstancedMesh->SetStaticMesh(StaticMesh);
	}
	if ((MemberPropertyChanged == GET_MEMBER_NAME_CHECKED(AActorVoxel, Size)))
	{
		VolumeOutline->SetWorldScale3D(FVector((Size) / 8.f)+.1);
	}
	if ((MemberPropertyChanged == GET_MEMBER_NAME_CHECKED(AActorVoxel, IterationsMax)))
	{
		if(!MaterialOutlineInst){
			MaterialOutlineInst = UMaterialInstanceDynamic::Create(BaseMat, this);
			VolumeOutline->SetMaterial(0, MaterialOutlineInst);
			MaterialOutlineInst->SetScalarParameterValue("Iterations", IterationsMax);
		}
		else MaterialOutlineInst->SetScalarParameterValue("Iterations", IterationsMax);
	}

	InstancedMesh->ClearInstances();
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

