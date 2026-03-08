// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.

#include "Libraries/ItemTagLibrary.h"

FItemTag UItemTagLibrary::MakeItemTagInt(const FString& TagName, int32 Value)
{
	FItemTag Tag;
	Tag.TagName = TagName;
	Tag.Type = EStatValueType::Int;
	Tag.IntValue = Value;
	return Tag;
}

FItemTag UItemTagLibrary::MakeItemTagFloat(const FString& TagName, float Value)
{
	FItemTag Tag;
	Tag.TagName = TagName;
	Tag.Type = EStatValueType::Float;
	Tag.FloatValue = Value;
	return Tag;
}

FItemTag UItemTagLibrary::MakeItemTagBool(const FString& TagName, bool Value)
{
	FItemTag Tag;
	Tag.TagName = TagName;
	Tag.Type = EStatValueType::Bool;
	Tag.BoolValue = Value;
	return Tag;
}

FItemTag UItemTagLibrary::MakeItemTagString(const FString& TagName, const FString& Value)
{
	FItemTag Tag;
	Tag.TagName = TagName;
	Tag.Type = EStatValueType::String;
	Tag.StringValue = Value;
	return Tag;
}

FItemTag UItemTagLibrary::MakeItemTagName(const FString& TagName, FName Value)
{
	FItemTag Tag;
	Tag.TagName = TagName;
	Tag.Type = EStatValueType::Name;
	Tag.NameValue = Value;
	return Tag;
}

FItemTag UItemTagLibrary::MakeItemTagStruct(const FString& TagName, const FInstancedStruct& Value)
{
	FItemTag Tag;
	Tag.TagName = TagName;
	Tag.Type = EStatValueType::Struct;
	Tag.StructValue = Value;
	return Tag;
}

int32 UItemTagLibrary::GetItemTagInt(const FItemTag& Tag)
{
	return Tag.IntValue;
}

float UItemTagLibrary::GetItemTagFloat(const FItemTag& Tag)
{
	return Tag.FloatValue;
}

bool UItemTagLibrary::GetItemTagBool(const FItemTag& Tag)
{
	return Tag.BoolValue;
}

FString UItemTagLibrary::GetItemTagString(const FItemTag& Tag)
{
	return Tag.StringValue;
}

FName UItemTagLibrary::GetItemTagName(const FItemTag& Tag)
{
	return Tag.NameValue;
}

FInstancedStruct UItemTagLibrary::GetItemTagStruct(const FItemTag& Tag)
{
	return Tag.StructValue;
}

FItemTag UItemTagLibrary::SetItemTagInt(FItemTag Tag, int32 Value)
{
	Tag.Type = EStatValueType::Int;
	Tag.IntValue = Value;
	return Tag;
}

FItemTag UItemTagLibrary::SetItemTagFloat(FItemTag Tag, float Value)
{
	Tag.Type = EStatValueType::Float;
	Tag.FloatValue = Value;
	return Tag;
}

FItemTag UItemTagLibrary::SetItemTagBool(FItemTag Tag, bool Value)
{
	Tag.Type = EStatValueType::Bool;
	Tag.BoolValue = Value;
	return Tag;
}

FItemTag UItemTagLibrary::SetItemTagString(FItemTag Tag, const FString& Value)
{
	Tag.Type = EStatValueType::String;
	Tag.StringValue = Value;
	return Tag;
}

FItemTag UItemTagLibrary::SetItemTagName(FItemTag Tag, FName Value)
{
	Tag.Type = EStatValueType::Name;
	Tag.NameValue = Value;
	return Tag;
}

FItemTag UItemTagLibrary::SetItemTagStruct(FItemTag Tag, const FInstancedStruct& Value)
{
	Tag.Type = EStatValueType::Struct;
	Tag.StructValue = Value;
	return Tag;
}

FString UItemTagLibrary::ItemTagToString(const FItemTag& Tag)
{
	switch (Tag.Type)
	{
	case EStatValueType::Int:    return FString::Printf(TEXT("%s = %d"),     *Tag.TagName, Tag.IntValue);
	case EStatValueType::Float:  return FString::Printf(TEXT("%s = %.2f"),   *Tag.TagName, Tag.FloatValue);
	case EStatValueType::Bool:   return FString::Printf(TEXT("%s = %s"),     *Tag.TagName, Tag.BoolValue ? TEXT("true") : TEXT("false"));
	case EStatValueType::String: return FString::Printf(TEXT("%s = \"%s\""), *Tag.TagName, *Tag.StringValue);
	case EStatValueType::Name:   return FString::Printf(TEXT("%s = %s"),     *Tag.TagName, *Tag.NameValue.ToString());
	case EStatValueType::Struct: return FString::Printf(TEXT("%s = [Struct: %s]"), *Tag.TagName,
		Tag.StructValue.IsValid() ? *Tag.StructValue.GetScriptStruct()->GetName() : TEXT("None"));
	default:                     return FString::Printf(TEXT("%s = None"),   *Tag.TagName);
	}
}

bool UItemTagLibrary::ItemTagHasName(const FItemTag& Tag, const FString& TagName)
{
	return Tag.TagName.Equals(TagName, ESearchCase::IgnoreCase);
}

bool UItemTagLibrary::FindItemTagByName(const TArray<FItemTag>& Tags, const FString& TagName, FItemTag& OutTag)
{
	for (const FItemTag& Tag : Tags)
	{
		if (Tag.TagName.Equals(TagName, ESearchCase::IgnoreCase))
		{
			OutTag = Tag;
			return true;
		}
	}
	OutTag = FItemTag{};
	return false;
}

bool UItemTagLibrary::IsItemTagType(const FItemTag& Tag, EStatValueType ExpectedType)
{
	return Tag.Type == ExpectedType;
}