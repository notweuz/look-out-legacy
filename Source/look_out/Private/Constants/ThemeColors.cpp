// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.


#include "Constants/ThemeColors.h"

FLinearColor UThemeColors::SelectedSlotBackground()
{
	return GetLinearColorFromHex("ff8a2340");
}

FLinearColor UThemeColors::SelectedSlotBorder()
{
	return GetLinearColorFromHex("ff8a2380");
}

FLinearColor UThemeColors::SlotBackground()
{
	return GetLinearColorFromHex("3e3e3e80");
}

FLinearColor UThemeColors::SlotBorder()
{
	return GetLinearColorFromHex("3e3e3e80");
}

FLinearColor UThemeColors::MovingSlotBorder()
{
	return GetLinearColorFromHex("3e3eff80");
}

FLinearColor UThemeColors::MovingSlotBackground()
{
	return GetLinearColorFromHex("#3e3eff40");
}

FLinearColor UThemeColors::GetLinearColorFromHex(const FString& InHex)
{
	return FLinearColor::FromSRGBColor(FColor::FromHex(InHex));
}
