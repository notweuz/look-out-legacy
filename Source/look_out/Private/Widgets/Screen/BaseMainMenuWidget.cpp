// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.


#include "Core/Libraries/ThemeColors.h"
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
		ExitButton->ActionButton->OnClicked.AddDynamic(this, &UBaseMainMenuScreenWidget::OnExitClicked);
	}
	ExitButton->ButtonText->SetColorAndOpacity(UThemeColors::GeneralClose());
	
	FSlateFontInfo NewFont = StartButton->ButtonText->GetFont();
	NewFont.Size = 32;
	
	StartButton->ButtonText->SetFont(NewFont);
	SettingsButton->ButtonText->SetFont(NewFont);
	CreditsButton->ButtonText->SetFont(NewFont);
	ExitButton->ButtonText->SetFont(NewFont);
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
