// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameJam23/PlayerControllers/CustomPlayerController.h"
#include "CollectableActor.generated.h"


UENUM(BlueprintType)
enum class ECollectableType : uint8
{
	ECT_QuestSheet UMETA(DisplayName = "Quest Sheet"),
	ECT_Stone UMETA(DisplayName = "Stone"),
	ECT_Wood UMETA(DisplayName = "Wood"),
	ECT_Wool UMETA(DisplayName = "Wool"),

	ECT_MAX UMETA(DisplayName = "DefaultMax")
};

UCLASS()
class GAMEJAM23_API ACollectableActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ACollectableActor();
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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* SphereCollisionComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* WidgetComponent;

	UFUNCTION()
	void OnSphereBeginoverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	/*
	 * Miscellaneous
	 */
	UPROPERTY(EditAnywhere)
	ECollectableType CollectableType = ECollectableType::ECT_QuestSheet;
	UPROPERTY(EditAnywhere)
	int32 CollectAmount = 1;
	UPROPERTY()
	class APlayerCharacter* PlayerCharacter;

private:
	/*
	 * Quest functionality
	 */
	UPROPERTY(EditAnywhere, Category = Quest, meta = (AllowPrivateAccess = "true"))
	FString FurnitureToCreate = FString();
	UPROPERTY(EditAnywhere, Category = Quest, meta = (AllowPrivateAccess = "true"))
	int32 StonesToCollect = 0;
	UPROPERTY(EditAnywhere, Category = Quest, meta = (AllowPrivateAccess = "true"))
	int32 WoodToCollect = 0;
	UPROPERTY(EditAnywhere, Category = Quest, meta = (AllowPrivateAccess = "true"))
	int32 WoolToCollect = 0;

public:
	FQuestStruct QuestStruct;
	
};
