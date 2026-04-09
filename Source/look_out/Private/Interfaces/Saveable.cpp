// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.


#include "Interfaces/Saveable.h"

#include "Utils/SaveUtils.h"

void ISaveable::DefaultSaveObject(UObject* Object, TArray<uint8>& OutBytes)
{
	SaveUtils::Save(Object, OutBytes);
}

void ISaveable::DefaultLoadObject(UObject* Object, const TArray<uint8>& InBytes)
{
	SaveUtils::Load(Object, InBytes);
	Execute_OnPostLoadFromSave(Object);
}
