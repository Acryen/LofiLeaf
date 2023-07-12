// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CollectableWidget.generated.h"

/**
 * 
 */
UCLASS()
class GAMEJAM23_API UCollectableWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* PressFText;

	UPROPERTY(meta = (BindWidget))
	class UImage* ClipboardImage;

	UPROPERTY(meta = (BindWidget))
	class UBorder* ActiveQuestBorder;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ActiveQuestText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* RessourcesText;

	UPROPERTY(meta = (BindWidget))
	class UScrollBox* JournalScrollBox;

	UPROPERTY(meta = (BindWidget))
	UImage* CollectableBackground;
};
