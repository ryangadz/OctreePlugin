// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "OctreePluginBPLibrary.h"
#include "VoxelAddComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class OCTREEPLUGIN_API UVoxelAddComponent : public USceneComponent
{
	GENERATED_BODY()

  public:
	// Sets default values for this component's properties



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
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif 

	UPROPERTY()
	class UInstancedStaticMeshComponent *InstancedMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName = "WorldContextObject", Category = "Setup Properties")
	class AActor* WorldContextObject;

	FVector Location;

	UVoxelAddComponent(const FObjectInitializer &ObjectInitializer);

  protected:
	// Called when the game starts
	virtual void BeginPlay() override;



  public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
};
