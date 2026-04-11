// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.


#include "Widgets/Screen/BaseMainMenuScreenWidget.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Misc/LogCategories.h"

void UBaseMainMenuScreenWidget::NativeConstruct()
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
		ExitButton->Button->OnClicked.AddDynamic(this, &UBaseMainMenuScreenWidget::OnExitClicked);
	}
}

void UBaseMainMenuScreenWidget::OnExitClicked()
{
	UE_LOG(LogUI, Display, TEXT("Exiting game"));
	UKismetSystemLibrary::QuitGame(
		this,
		GetOwningPlayer(),
		EQuitPreference::Quit,
		false
	);
}
