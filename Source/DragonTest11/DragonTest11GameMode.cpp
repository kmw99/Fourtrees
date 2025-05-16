// Copyright Epic Games, Inc. All Rights Reserved.

#include "DragonTest11GameMode.h"
#include "DragonTest11Character.h"
#include "UObject/ConstructorHelpers.h"

ADragonTest11GameMode::ADragonTest11GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
