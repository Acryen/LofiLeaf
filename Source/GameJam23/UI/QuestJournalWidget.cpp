// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestJournalWidget.h"

#include "Components/Button.h"
#include "GameJam23/PlayerControllers/CustomPlayerController.h"

bool UQuestJournalWidget::Initialize()
{
	Super::Initialize();

	if (!SetActiveQuestButton) return false;
	SetActiveQuestButton->OnClicked.AddDynamic(this, &ThisClass::OnButtonClicked);

	return true;
}

void UQuestJournalWidget::OnButtonClicked()
{
	if (!PlayerController) return;
	PlayerController->SetActiveQuest(ArrayIndex);

	// if (!SetActiveQuestButton) return;
	// SetActiveQuestButton->SetIsEnabled(false);
}
