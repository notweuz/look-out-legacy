// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.

#include "Libraries/SaveSystemUtils.h"

#include "Engine/Level.h"
#include "GameFramework/Actor.h"

namespace SaveSystemUtilsPrivate
{
	FString StripPiePrefixFromSegment(const FString& Segment)
	{
		if (!Segment.StartsWith(TEXT("UEDPIE_")))
		{
			return Segment;
		}

		int32 FirstUnderscoreIndex = INDEX_NONE;
		int32 SecondUnderscoreIndex = INDEX_NONE;
		if (!Segment.FindChar(TEXT('_'), FirstUnderscoreIndex))
		{
			return Segment;
		}

		SecondUnderscoreIndex = Segment.Find(TEXT("_"), ESearchCase::CaseSensitive, ESearchDir::FromStart,
		                                     FirstUnderscoreIndex + 1);
		if (SecondUnderscoreIndex == INDEX_NONE || SecondUnderscoreIndex + 1 >= Segment.Len())
		{
			return Segment;
		}

		return Segment.Mid(SecondUnderscoreIndex + 1);
	}
}

bool FSaveSystemUtils::IsLevelPlacedActor(const AActor* Actor)
{
	return IsValid(Actor) && Actor->HasAnyFlags(RF_WasLoaded);
}

FString FSaveSystemUtils::BuildStableLevelActorId(const AActor* Actor)
{
	if (!IsValid(Actor))
	{
		return FString();
	}

	const ULevel* Level = Actor->GetLevel();
	const FString LevelPath = NormalizeObjectPath(Level ? Level->GetPathName() : FString());
	return FString::Printf(TEXT("%s:%s"), *LevelPath, *Actor->GetName());
}

FString FSaveSystemUtils::NormalizeObjectPath(const FString& ObjectPath)
{
	if (ObjectPath.IsEmpty())
	{
		return ObjectPath;
	}

	TArray<FString> Segments;
	ObjectPath.ParseIntoArray(Segments, TEXT("/"), true);

	for (FString& Segment : Segments)
	{
		Segment = SaveSystemUtilsPrivate::StripPiePrefixFromSegment(Segment);
	}

	const FString Joined = FString::Join(Segments, TEXT("/"));
	return ObjectPath.StartsWith(TEXT("/")) ? FString(TEXT("/")) + Joined : Joined;
}
