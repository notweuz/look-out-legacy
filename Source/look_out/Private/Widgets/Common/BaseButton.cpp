// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.


#include "Widgets/Common/BaseButton.h"

#include "Core/Libraries/ThemeColors.h"

void UBaseButton::NativePreConstruct()
{
	Super::NativePreConstruct();
	
	SetupStyle();
}

void UBaseButton::SetupStyle()
{
	FSlateBrush Normal = Button->GetStyle().Normal;
	FSlateBrush Hovered = Button->GetStyle().Hovered;
	FSlateBrush Pressed = Button->GetStyle().Pressed;
	
	Normal.TintColor = FSlateColor(FLinearColor::Transparent);
	Hovered.TintColor = FSlateColor(UThemeColors::GeneralSelectedBackground());
	Pressed.TintColor = FSlateColor(UThemeColors::GeneralSelectedBackground() * 0.75f);
	
	FButtonStyle NewStyle;
	NewStyle.SetNormal(Normal);
	NewStyle.SetHovered(Hovered);
	NewStyle.SetPressed(Pressed);
	Button->SetStyle(NewStyle);
	
	Button->SetBackgroundColor(UThemeColors::GeneralBackground());
}
