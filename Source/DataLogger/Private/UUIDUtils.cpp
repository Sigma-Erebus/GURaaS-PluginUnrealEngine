#include "UUIDUtils.h"
#include "Engine.h"


// guraas needs a specific player ID format for each player.
// Format is aaaa0000-aaaa-0000-aaaa-0000aaaa0000.
// Use this utility to generate a new one
FString UUUIDUtils::GenerateUUID() {
	FString tGuid = FGuid::NewGuid().ToString().ToLower();
	tGuid.InsertAt(20, '-');
	tGuid.InsertAt(16, '-');
	tGuid.InsertAt(12, '-');
	tGuid.InsertAt(8, '-');
	return tGuid;
}
