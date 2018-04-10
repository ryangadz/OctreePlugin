// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "OctreePluginBPLibrary.h"
#include "ActorVoxel.generated.h"

UCLASS()
class OCTREEPLUGIN_API AActorVoxel : public AActor
{
	GENERATED_BODY()


public:	
	// Sets default values for this actor's properties
	AActorVoxel(const FObjectInitializer &ObjectInitializer);

	class UStaticMeshComponent *VolumeOutline;
	class UMaterialInstanceDynamic * MaterialOutlineInst;
	class UMaterial* BaseMat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "Static Mesh", Category = "Setup Properties")
	class UStaticMesh *StaticMesh;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "Material", Category = "Setup Properties")
	class UMaterialInterface *Material;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "Object Types", Category = "Setup Properties")
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes = {(EObjectTypeQuery::ObjectTypeQuery1)};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "Size", Category = "Setup Properties")
	float Size = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "Iterations Max", Category = "Setup Properties",
			  meta = (ClampMin = "0", ClampMax = "6", UIMin = "0", UIMax = "6"))
	int32 IterationsMax = 4;

//	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "Instanced Mesh", Category = "Setup Properties")
	class UInstancedStaticMeshComponent *InstancedMesh;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif 

virtual void PostActorCreated();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
