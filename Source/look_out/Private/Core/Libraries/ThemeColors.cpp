// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.


#include "Core/Libraries/ThemeColors.h"

FLinearColor UThemeColors::GeneralBackground()
{
	return FLinearColor::FromSRGBColor(FColor(31, 31, 31, 216));
}

FLinearColor UThemeColors::GeneralBorder()
{
	return FLinearColor::FromSRGBColor(FColor(77, 77, 77, 216));
}

FLinearColor UThemeColors::GeneralClose()
{
	return FLinearColor::FromSRGBColor(FColor(255, 97, 97, 255));
}

FLinearColor UThemeColors::GeneralSelected()
{
	return FLinearColor::FromSRGBColor(FColor(240, 144, 64, 255));
}

FButtonStyle UThemeColors::DefaultButtonStyle()
{
	FButtonStyle Style;
	Style.Normal  = MakeBrush(GeneralBackground());
	Style.Hovered = MakeBrush(GeneralSelected());
	Style.Pressed = MakeBrush(GeneralSelected() * 0.8f);
	return Style;
}

FSlateBrush UThemeColors::MakeBrush(FLinearColor Color)
{
	FSlateBrush Brush;
	Brush.TintColor = FSlateColor(Color);
	Brush.DrawAs = ESlateBrushDrawType::Box;
	return Brush;
}
