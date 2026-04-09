// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.


#include "Widgets/Menu/BaseMainMenuWidget.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Misc/LogCategories.h"

void UBaseMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	FString VersionString;

	GConfig->GetString(
		TEXT("/Script/EngineSettings.GeneralProjectSettings"),
		TEXT("ProjectVersion"),
		VersionString,
		GGameIni
	);

	Version->SetText(FText::FromString(VersionString));

	if (ExitButton)
	{
		ExitButton->OnClicked.AddDynamic(this, &UBaseMainMenuWidget::OnExitClicked);
	}
}

void UBaseMainMenuWidget::OnExitClicked()
{
	UE_LOG(LogUI, Display, TEXT("Exiting game"));
	UKismetSystemLibrary::QuitGame(
		this,
		GetOwningPlayer(),
		EQuitPreference::Quit,
		false
	);
}
