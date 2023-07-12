// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QuestJournalWidget.generated.h"

/**
 * 
 */
UCLASS()
class GAMEJAM23_API UQuestJournalWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual bool Initialize() override;

public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* QuestText;

	UPROPERTY(meta = (BindWidget))
	class UButton* SetActiveQuestButton;

	int32 ArrayIndex = 0;

	UPROPERTY()
	class ACustomPlayerController* PlayerController;

	UFUNCTION()
	void OnButtonClicked();
	
};
