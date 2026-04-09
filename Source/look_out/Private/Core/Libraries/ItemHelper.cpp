// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.

#include "Core/Libraries/ItemHelper.h"

UItemDefinition* UItemHelper::BuildItemDefinition(FText Name, FText Description, float Weight,
	UTexture2D* Icon, AActor* ActorClass)
{
	UItemDefinition* Definition = NewObject<UItemDefinition>(nullptr, UItemDefinition::StaticClass());
	Definition->ActorClass = ActorClass;
	Definition->Weight = Weight;
	Definition->Icon = Icon;
	Definition->DisplayName = Name;
	Definition->Description = Description;
	
	return Definition;
}
