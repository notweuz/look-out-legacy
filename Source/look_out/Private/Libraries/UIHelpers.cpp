// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.


#include "Libraries/UIHelpers.h"


ABaseCharacter* UUIHelpers::GetBasePlayerFromWidget(const UUserWidget* Widget)
{
	if (!Widget) return nullptr;
	const APlayerController* PC = Widget->GetOwningPlayer();
	if (!PC) return nullptr;
	return Cast<ABaseCharacter>(PC->GetPawn());
}