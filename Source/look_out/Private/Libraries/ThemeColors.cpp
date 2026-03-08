// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.


#include "Libraries/ThemeColors.h"

FLinearColor UThemeColors::SlotBackground()
{
	return FLinearColor::FromSRGBColor(FColor(62, 62, 62, 127));
}

FLinearColor UThemeColors::SlotBorder()
{
	return FLinearColor::FromSRGBColor(FColor(62, 62, 62, 127));
}

FLinearColor UThemeColors::MovingSlotBorder()
{
	return FLinearColor::FromSRGBColor(FColor(35, 138, 255, 127));
}

FLinearColor UThemeColors::MovingSlotBackground()
{
	return FLinearColor::FromSRGBColor(FColor(35, 138, 255, 63));
}

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

FLinearColor UThemeColors::GeneralSelectedBackground()
{
	return FLinearColor::FromSRGBColor(FColor(255, 137, 35, 63));
}

FLinearColor UThemeColors::GeneralSelectedBorder()
{
	return FLinearColor::FromSRGBColor(FColor(255, 137, 35, 127));
}
