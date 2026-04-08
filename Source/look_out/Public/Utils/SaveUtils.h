#pragma once

#include "CoreMinimal.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

namespace SaveUtils
{
	inline void Save(UObject* Object, TArray<uint8>& OutBytes)
	{
		FMemoryWriter Writer(OutBytes);
		FObjectAndNameAsStringProxyArchive Ar(Writer, true);
		Ar.ArIsSaveGame = true;
		Object->Serialize(Ar);
	}

	inline void Load(UObject* Object, const TArray<uint8>& InBytes)
	{
		FMemoryReader Reader(InBytes);
		FObjectAndNameAsStringProxyArchive Ar(Reader, true);
		Ar.ArIsSaveGame = true;
		Object->Serialize(Ar);
	}
}