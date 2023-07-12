// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomPlayerController.h"

#include "NavigationSystem.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Blueprint/UserWidget.h"
#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "GameJam23/Actors/CollectableActor.h"
#include "GameJam23/Actors/FurnitureActor.h"
#include "GameJam23/UI/CollectableWidget.h"
#include "GameJam23/UI/QuestJournalWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/GameplayStaticsTypes.h"


void ACustomPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("PreviewFurniture", IE_Pressed, this, &ThisClass::StartPreviewFurniture);
	InputComponent->BindAxis("RotateFurniture", this, &ThisClass::RotateFurniture);

	InputComponent->BindAction("PlaceFurniture", IE_Pressed, this, &ThisClass::PlaceFurniture);
	InputComponent->BindAction("SelectNextFurniture", IE_Pressed, this, &ThisClass::SelectNextFurniture);
	InputComponent->BindAction("SelectPreviousFurniture", IE_Pressed, this, &ThisClass::SelectPreviousFurniture);

	InputComponent->BindAction("Interact", IE_Pressed, this, &ThisClass::Interact);
	InputComponent->BindAction("ToggleJournal", IE_Pressed, this, &ThisClass::ToggleJournal);
}

void ACustomPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// Hol die ganzen StaticMeshes
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	TArray<FAssetData> AssetDataArray;
	AssetRegistryModule.Get().GetAssetsByPath(FName(FurnitureMeshFolderPath), AssetDataArray);

	for (auto& AssetData : AssetDataArray)
	{
		if (AssetData.GetClass() == UStaticMesh::StaticClass())
		{
			FString AssetPath = AssetData.GetObjectPathString();
			UStaticMesh* StaticMesh = LoadObject<UStaticMesh>(nullptr, *AssetPath);

			if (StaticMesh)
			{
				FurnitureMeshMap.Add(StaticMesh, 0);
			}
		}
	}

	// Hol die Images
	AssetDataArray.Reset();
	AssetRegistryModule.Get().GetAssetsByPath(FName(FurnitureImagesFolderPath), AssetDataArray);

	for (auto& AssetData : AssetDataArray)
	{
		if (AssetData.GetClass() == UTexture2D::StaticClass())
		{
			FString AssetPath = AssetData.GetObjectPathString();
			UTexture2D* Texture2D = LoadObject<UTexture2D>(nullptr, *AssetPath);

			if (Texture2D)
			{
				FurnitureImagesArray.Add(Texture2D);
			}
		}
	}

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Map:%i, Array:%i"), FurnitureMeshMap.Num(), FurnitureMeshArray.Num()));

	UnlockNewFurniture(FString(TEXT("Laterne")));
	UnlockNewFurniture(FString(TEXT("Bowl")));
	UnlockNewFurniture(FString(TEXT("Plant2")));
}

void ACustomPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bPlacingFurnitureActive)
	{
		int32 ViewportSizeX, ViewportSizeY;
		GetViewportSize(ViewportSizeX, ViewportSizeY);

		FVector2D ScreenPosition(ViewportSizeX / 2.f, ViewportSizeY / 2.f);

		FVector WorldLocation, WorldDirection;
		DeprojectScreenPositionToWorld(ScreenPosition.X, ScreenPosition.Y, WorldLocation, WorldDirection);

		WorldDirection = FVector(WorldDirection.X, WorldDirection.Y, WorldDirection.Z + ProjectileZCorrection);
		
		FPredictProjectilePathParams ProjectilePathParams(
			10,
			WorldLocation,
			WorldDirection * TraceDistance,
			ProjectileSimulationDuration,
			ECC_Visibility,
			GetPawn());

		FPredictProjectilePathResult ProjectilePathResult;
		bool bProjectHitSuccess = UGameplayStatics::PredictProjectilePath(GetWorld(), ProjectilePathParams, ProjectilePathResult);

		if (bProjectHitSuccess)
		{
			if (!FurnitureActor)
			{
				AActor* NewFurnitureActor = GetWorld()->SpawnActor<AFurnitureActor>(FurnitureActorBlueprint, ProjectilePathResult.HitResult.Location, GetPawn()->GetActorRotation().GetInverse());
				FurnitureActor = Cast<AFurnitureActor>(NewFurnitureActor);
			}
			else
			{
				FVector Origin;
				FVector BoxExtent;
				float SphereRadius;
				UKismetSystemLibrary::GetComponentBounds(FurnitureActor->GetMesh(), Origin, BoxExtent, SphereRadius);
				
				FurnitureActor->SetActorLocation(ProjectilePathResult.HitResult.Location/* + SphereRadius / 2*/ - FVector(0, 0, ZPositionCorrection));

				if (!FurnitureActor->GetMesh()->IsVisible())
				{
					FurnitureActor->GetMesh()->SetVisibility(true);
				}
			}
		}
	}
	else
	{
		if (FurnitureActor)
		{
			FurnitureActor->GetMesh()->SetVisibility(false);
		}
	}
}

void ACustomPlayerController::StartPreviewFurniture()
{
	// Das wird in Blueprint geändert
	if (!bIsInTreeHouse) return;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Map:%i, Array:%i"), FurnitureMeshMap.Num(), FurnitureMeshArray.Num()));
	
	if (bPlacingFurnitureActive)
	{
		bPlacingFurnitureActive = false;
	}
	else
	{
		bPlacingFurnitureActive = true;
		
		FurnitureMeshArray.Empty();
		for (auto& Elem : FurnitureMeshMap)
		{
			if (Elem.Value == 0) // TODO das hier muss am Ende zu einer 1 gemacht werden
			{
				FurnitureMeshArray.Add(Elem.Key);
			}
		}
	}
}

void ACustomPlayerController::RotateFurniture(float Value)
{
	if (!FurnitureActor) return;
	double NewRotateAmount = RotateAmount * Value;
	FurnitureActor->AddActorLocalRotation(FRotator(0, NewRotateAmount, 0));
}

void ACustomPlayerController::PlaceFurniture()
{
	if (!FurnitureActor) return;
	if (bPlacingFurnitureActive && !FurnitureActor->GetIsOverlapping())
	{		
		AFurnitureActor* NewFurnitureActor = GetWorld()->SpawnActor<AFurnitureActor>(
			FurnitureActorBlueprint,
			FurnitureActor->GetActorLocation(),
			FurnitureActor->GetActorRotation());

		NewFurnitureActor->GetMesh()->SetStaticMesh(FurnitureMeshArray[CurrentSelectionIndex]);

		NewFurnitureActor->GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
		NewFurnitureActor->GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
}

void ACustomPlayerController::SelectNextFurniture()
{
	if (!FurnitureActor || FurnitureMeshArray.IsEmpty()) return;

	CurrentSelectionIndex++;

	if (CurrentSelectionIndex > FurnitureMeshArray.Num() - 1)
	{
		CurrentSelectionIndex = 0;
	}

	FurnitureActor->GetMesh()->SetStaticMesh(FurnitureMeshArray[CurrentSelectionIndex]);
}

void ACustomPlayerController::SelectPreviousFurniture()
{
	if (!FurnitureActor || FurnitureMeshArray.IsEmpty()) return;

	CurrentSelectionIndex--;

	if (CurrentSelectionIndex < 0)
	{
		CurrentSelectionIndex = FurnitureMeshArray.Num() - 1;
	}

	FurnitureActor->GetMesh()->SetStaticMesh(FurnitureMeshArray[CurrentSelectionIndex]);
}

void ACustomPlayerController::UnlockNewFurniture(FString FurnitureName)
{	
	for (auto& Elem : FurnitureMeshMap)
	{
		if (Elem.Key->GetName() == FurnitureName)
		{
			Elem.Value = 1;
		}
	}
}

void ACustomPlayerController::Interact()
{
	if (!CollectableActor) return;
	switch (CollectableActor->CollectableType)
	{
	case ECollectableType::ECT_QuestSheet:
		QuestStructsArray.Add(CollectableActor->QuestStruct);
		if (QuestStructsArray.Num() == 1)
		{
			ActiveQuestStruct = QuestStructsArray[0];
		}
		UpdateActiveQuest();
		break;
	case ECollectableType::ECT_Stone:
		StonesCollected += CollectableActor->CollectAmount;
		UpdateRessourcesText();
		UpdateQuestStatus();
		break;
	case ECollectableType::ECT_Wood:
		WoodCollected += CollectableActor->CollectAmount;
		UpdateRessourcesText();
		UpdateQuestStatus();
		break;
	case ECollectableType::ECT_Wool:
		WoolCollected += CollectableActor->CollectAmount;
		UpdateRessourcesText();
		UpdateQuestStatus();
		break;

	default:
		break;
	}

	CollectableActor->Destroy();
	CollectableActor = nullptr;	
}

bool ACustomPlayerController::SetupWidget()
{
	if (!CollectableWidgetBlueprint) return false;
	
	if (!CollectableWidget)
	{
		CollectableWidget = CreateWidget<UCollectableWidget>(this, CollectableWidgetBlueprint);
		CollectableWidget->AddToViewport();
	}

	return true;
}

void ACustomPlayerController::TogglePressFText()
{
	if (!SetupWidget()) return;

	if (CollectableWidget->PressFText->IsVisible())
	{
		CollectableWidget->PressFText->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		CollectableWidget->PressFText->SetVisibility(ESlateVisibility::Visible);
	}
}

void ACustomPlayerController::ToggleJournal()
{
	if (!SetupWidget()) return;

	if (CollectableWidget->ClipboardImage->IsVisible())
	{
		CollectableWidget->ClipboardImage->SetVisibility(ESlateVisibility::Hidden);
		CollectableWidget->JournalScrollBox->SetVisibility(ESlateVisibility::Hidden);

		FInputModeGameOnly InputMode;
		InputMode.SetConsumeCaptureMouseDown(false);
		SetInputMode(InputMode);
		bShowMouseCursor = false;

		if (CloseJournal)
		{
			UGameplayStatics::PlaySound2D(GetWorld(), CloseJournal);
		}
	}
	else
	{
		CollectableWidget->ClipboardImage->SetVisibility(ESlateVisibility::Visible);
		CollectableWidget->JournalScrollBox->SetVisibility(ESlateVisibility::Visible);
		CollectableWidget->JournalScrollBox->ClearChildren();

		if (!QuestWidgetBlueprint) return;
		for (int32 i = 0; i < QuestStructsArray.Num(); i++)
		{
			auto QuestJournalRowWidget = CreateWidget<UQuestJournalWidget>(this, QuestWidgetBlueprint);
			QuestJournalRowWidget->QuestText->SetText(FText::FromString(FString::Printf(TEXT("Furniture: %s\nStone: %i - Wood: %i - Wool: %i"),
				*QuestStructsArray[i].FurnitureToCreate,
				QuestStructsArray[i].StonesToCollect,
				QuestStructsArray[i].WoodToCollect,
				QuestStructsArray[i].WoolToCollect)));

			QuestJournalRowWidget->ArrayIndex = i;
			QuestJournalRowWidget->PlayerController = this;

			CollectableWidget->JournalScrollBox->AddChild(QuestJournalRowWidget);
		}

		FInputModeGameAndUI InputMode;
		InputMode.SetWidgetToFocus(CollectableWidget->JournalScrollBox->TakeWidget());
		SetInputMode(InputMode);
		bShowMouseCursor = true;

		if (OpenJournal)
		{
			UGameplayStatics::PlaySound2D(GetWorld(), OpenJournal);
		}
	}
}

void ACustomPlayerController::UpdateActiveQuest()
{
	if (!SetupWidget()) return;

	if (!CollectableWidget->ActiveQuestText->IsVisible() || !CollectableWidget->ActiveQuestBorder->IsVisible())
	{
		CollectableWidget->ActiveQuestText->SetVisibility(ESlateVisibility::Visible);
		CollectableWidget->ActiveQuestBorder->SetVisibility(ESlateVisibility::Visible);
	}
	
	UpdateActiveQuestText();
}

void ACustomPlayerController::UpdateActiveQuestText()
{
	CollectableWidget->ActiveQuestText->SetText(FText::FromString(FString::Printf(TEXT("Active Quest for\nFurniture: %s\nTo collect:\nStone: %i\nWood: %i\nWool: %i"),
		*ActiveQuestStruct.FurnitureToCreate,
		ActiveQuestStruct.StonesToCollect,
		ActiveQuestStruct.WoodToCollect,
		ActiveQuestStruct.WoolToCollect)));
}

void ACustomPlayerController::UpdateQuestStatus()
{	
	if (
		StonesCollected >= ActiveQuestStruct.StonesToCollect &&
		WoodCollected >= ActiveQuestStruct.WoodToCollect &&
		WoolCollected >= ActiveQuestStruct.WoolToCollect)
	{
		StonesCollected -= ActiveQuestStruct.StonesToCollect;
		WoodCollected -= ActiveQuestStruct.WoodToCollect;
		WoolCollected -= ActiveQuestStruct.WoolToCollect;
		UpdateRessourcesText();

		UnlockNewFurniture(ActiveQuestStruct.FurnitureToCreate);

		if (!CollectableWidget) return;
		CollectableWidget->ActiveQuestText->SetText(FText::FromString(FString::Printf(TEXT("%s unlocked!"), *ActiveQuestStruct.FurnitureToCreate)));

		for (int32 i = 0; i < QuestStructsArray.Num(); i++)
		{
			if (QuestStructsArray[i].FurnitureToCreate == ActiveQuestStruct.FurnitureToCreate)
			{
				QuestStructsArray.RemoveAt(i);
				break;
			}
		}
	}
}

void ACustomPlayerController::UpdateRessourcesText()
{
	if (!SetupWidget()) return;

	if (!CollectableWidget->RessourcesText->IsVisible())
	{
		CollectableWidget->RessourcesText->SetVisibility(ESlateVisibility::Visible);
		CollectableWidget->CollectableBackground->SetVisibility(ESlateVisibility::Visible);
	}

	CollectableWidget->RessourcesText->SetText(FText::FromString(FString::Printf(TEXT("Stones: %i\nWood: %i\nWool: %i"), StonesCollected, WoodCollected, WoolCollected)));
}

void ACustomPlayerController::SetActiveQuest(int32 Index)
{
	ActiveQuestStruct = QuestStructsArray[Index];
	UpdateActiveQuest();
	UpdateQuestStatus();
}
