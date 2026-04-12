// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.


#include "Core/BaseGameplayGameMode.h"

#include "Core/Save/SaveManager.h"

void ABaseGameplayGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	
	USaveManager* SaveManager = GetGameInstance()->GetSubsystem<USaveManager>();
	if (!SaveManager) return;

	SaveManager->LoadSave(SaveManager->ActiveSlotName);
	NewPlayer->bShowMouseCursor = false;
	NewPlayer->SetInputMode(FInputModeGameOnly());
}
