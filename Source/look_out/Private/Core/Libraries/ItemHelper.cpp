// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.

#include "Core/Libraries/ItemHelper.h"

UItemDefinition* UItemHelper::BuildItemDefinition(FText Name, FText Description, float Weight,
	UTexture2D* Icon, TSubclassOf<AActor> ActorClass, UObject* Outer, float MaxHealth)
{
	UObject* DefinitionOuter = IsValid(Outer) ? Outer : GetTransientPackage();
	UItemDefinition* Definition = NewObject<UItemDefinition>(
		DefinitionOuter,
		UItemDefinition::StaticClass(),
		NAME_None,
		RF_Transient
	);
	Definition->ActorClass = ActorClass;
	Definition->Weight = Weight;
	Definition->Icon = Icon;
	Definition->DisplayName = Name;
	Definition->Description = Description;
	Definition->MaxHealth = MaxHealth;
	
	return Definition;
}
