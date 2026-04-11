// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.


#include "Core/Libraries/ThemeColors.h"
#include "Widgets/Screen/BaseMainMenuScreenWidget.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Misc/LogCategories.h"

void UBaseMainMenuScreenWidget::OnStartButtonPressed()
{
	UBaseMenuWidget* MenuWidget = CreateWidget<UBaseMenuWidget>(GetWorld(), MenuWidgetClass);
	if (!MenuWidget) return;
	
	UBaseSaveMenuContentWidget* SaveMenuContentWidget = CreateWidget<UBaseSaveMenuContentWidget>(GetWorld(), SaveMenuContentWidgetClass);
	if (!SaveMenuContentWidget) return;
	
	MenuWidget->SetBodyWidget(SaveMenuContentWidget);
	
	MenuWidget->TitleTextBlock->SetText(FText::FromString("Save Manager"));
	MenuWidget->CloseButton->SetVisibility(ESlateVisibility::Visible);
	
	MenuSlot->ClearChildren();
	MenuSlot->AddChild(MenuWidget);
}

void UBaseMainMenuScreenWidget::NativeConstruct()
{
	Super::NativeConstruct();

	StartButton->ActionButton->OnPressed.AddDynamic(this, &UBaseMainMenuScreenWidget::OnStartButtonPressed);
	
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
