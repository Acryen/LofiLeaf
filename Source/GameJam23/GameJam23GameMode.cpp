// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameJam23GameMode.h"
#include "GameJam23Character.h"
#include "UObject/ConstructorHelpers.h"

AGameJam23GameMode::AGameJam23GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/StarterAssets/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
