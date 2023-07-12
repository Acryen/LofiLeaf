// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CustomPlayerController.generated.h"


USTRUCT(BlueprintType)
struct FQuestStruct
{
	GENERATED_BODY()

	FString FurnitureToCreate;
	int32 StonesToCollect;
	int32 WoodToCollect;
	int32 WoolToCollect;
};

UCLASS()
class GAMEJAM23_API ACustomPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

private:
	virtual void SetupInputComponent() override;
	void Interact();

	/*
	 * Place furniture functionality
	 */
	UPROPERTY()
	class AFurnitureActor* FurnitureActor;
	void StartPreviewFurniture();
	bool bPlacingFurnitureActive = false;
	void RotateFurniture(float Value);
	void PlaceFurniture();
	void SelectNextFurniture();
	void SelectPreviousFurniture();
	int32 CurrentSelectionIndex = 0;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = PlaceFurniture)
	bool bIsInTreeHouse = false;
	
private:
	//
	// Place furniture funcitonality - Editables
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Furniture, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AFurnitureActor> FurnitureActorBlueprint;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Furniture, meta = (AllowPrivateAccess = "true"))
	float TraceDistance = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Furniture, meta = (AllowPrivateAccess = "true"))
	float ProjectileSimulationDuration = 15.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Furniture, meta = (AllowPrivateAccess = "true"))
	float ProjectileZCorrection = 0.4f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Furniture, meta = (AllowPrivateAccess = "true"))
	double RotateAmount = 10;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Furniture, meta = (AllowPrivateAccess = "true"))
	float ZPositionCorrection = 5;

	/*
	 * Inventory
	 */
	UPROPERTY()
	TMap<UStaticMesh*, int32> FurnitureMeshMap;
	UPROPERTY()
	TArray<UTexture2D*> FurnitureImagesArray;
	UPROPERTY()
	TArray<UStaticMesh*> FurnitureMeshArray;

	//
	// Inventory - Einsammeln Mechanics
	UFUNCTION(BlueprintCallable)
	void UnlockNewFurniture(FString FurnitureName);

	//
	// Inventory - Editables
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Inventory, meta = (AllowPrivateAccess))
	FString FurnitureMeshFolderPath = FString(TEXT("/Game/Assets/Geometry/Props"));

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Inventory, meta = (AllowPrivateAccess))
	FString FurnitureImagesFolderPath = FString(TEXT("/Game/Assets/Textures"));

public:
	/*
	 * Collectables
	 */
	bool SetupWidget();
	void TogglePressFText();
	bool bCanInteractWithQuestSheet = false;

	int32 StonesCollected = 0;
	int32 WoodCollected = 0;
	int32 WoolCollected = 0;

	UPROPERTY()
	class ACollectableActor* CollectableActor;
	
private:
	UPROPERTY()
	class UCollectableWidget* CollectableWidget;
	
	//
	// Collectables - Editables
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Questsheets, meta = (AllowPrivateAccess))
	TSubclassOf<class UUserWidget> CollectableWidgetBlueprint;

	/*
	 * Quest System
	 */
	TArray<FQuestStruct> QuestStructsArray;
	FQuestStruct ActiveQuestStruct;
	void ToggleJournal();
	void UpdateActiveQuest();
	void UpdateQuestStatus();
	void UpdateActiveQuestText();
	void UpdateRessourcesText();

	UPROPERTY(EditAnywhere)
	class USoundBase* OpenJournal;

	UPROPERTY(EditAnywhere)
	USoundBase* CloseJournal;

public:
	/*
	 * Journal
	 */
	void SetActiveQuest(int32 Index);

private:
	UPROPERTY(EditAnywhere, Category = Journal)
	TSubclassOf<UUserWidget> QuestWidgetBlueprint;
};
