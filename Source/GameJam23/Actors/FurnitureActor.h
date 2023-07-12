// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FurnitureActor.generated.h"

UCLASS()
class GAMEJAM23_API AFurnitureActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AFurnitureActor();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

private:
	/*
	 * Components
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USceneComponent* SceneRoot;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* MeshComponent;

	/*
	 * Miscellaneous
	 */
	UFUNCTION()
	void OnMeshBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
	void OnMeshEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	bool bIsOverlapping = false;
	
	
	

	// UPROPERTY()
	// UMaterialInstanceDynamic* PreviewDynamicMaterial;
	//
	// //
	// // Editables
	// UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = KeineAhnung, meta = (AllowPrivateAccess = "true"))
	// UMaterialInterface* PreviewMaterial;

public:
	FORCEINLINE UStaticMeshComponent* GetMesh() { return MeshComponent; }
	FORCEINLINE bool GetIsOverlapping () { return bIsOverlapping; }
};
