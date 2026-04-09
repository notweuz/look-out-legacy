// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.


#include "Widgets/Screen/BaseMainScreenWidget.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Misc/LogCategories.h"

void UBaseMainScreenWidget::NativeConstruct()
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
		ExitButton->OnClicked.AddDynamic(this, &UBaseMainScreenWidget::OnExitClicked);
	}
}

void UBaseMainScreenWidget::OnExitClicked()
{
	UE_LOG(LogUI, Display, TEXT("Exiting game"));
	UKismetSystemLibrary::QuitGame(
		this,
		GetOwningPlayer(),
		EQuitPreference::Quit,
		false
	);
}
