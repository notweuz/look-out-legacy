// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.


#include "Constants/ThemeColors.h"

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

FLinearColor UThemeColors::GeneralBackground()
{
	return GetLinearColorFromHex("1f1f1fd9");
}

FLinearColor UThemeColors::GeneralBorder()
{
	return GetLinearColorFromHex("3b3b3bd9");
}

FLinearColor UThemeColors::GeneralClose()
{
	return GetLinearColorFromHex("ff6161");
}

FLinearColor UThemeColors::GeneralSelectedBackground()
{
	return GetLinearColorFromHex("ff8a2340");
}

FLinearColor UThemeColors::GeneralSelectedBorder()
{
	return GetLinearColorFromHex("ff8a2380");
}

FLinearColor UThemeColors::GetLinearColorFromHex(const FString& InHex)
{
	return FLinearColor::FromSRGBColor(FColor::FromHex(InHex));
}
