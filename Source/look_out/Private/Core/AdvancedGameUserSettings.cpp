// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.


#include "Core/AdvancedGameUserSettings.h"

UAdvancedGameUserSettings* UAdvancedGameUserSettings::GetAdvancedGameUserSettings()
{
	return Cast<UAdvancedGameUserSettings>(GetGameUserSettings());
}

void UAdvancedGameUserSettings::ApplySettings(bool bCheckForCommandLineOverrides)
{
	Super::ApplySettings(bCheckForCommandLineOverrides);
}
