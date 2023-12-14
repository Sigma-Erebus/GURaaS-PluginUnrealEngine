#pragma once

#include "EVerboseLevel.generated.h"

UENUM(BlueprintType)
enum class EGURaaSVerboseLevel : uint8
{
	EGURaaS_VerboseLevel_None = 0 UMETA(DisplayName = "None"),
	EGURaaS_VerboseLevel_Info = 1 UMETA(DisplayName = "Info"),
	EGURaaS_VerboseLevel_Debug = 2 UMETA(DisplayName = "Debug"),
	EGURaaS_VerboseLevel_Player = 3 UMETA(DisplayName = "Player"),
	EGURaaS_VerboseLevel_Event = 4 UMETA(DisplayName = "Event"),
	EGURaaS_VerboseLevel_Performance = 5 UMETA(DisplayName = "Performance"),
	EGURaaS_VerboseLevel_Subsystem = 6 UMETA(DisplayName = "Subsystem"),
	EGURaaS_VerboseLevel_Warning = 7 UMETA(DisplayName = "Warning"),
	EGURaaS_VerboseLevel_Error = 8 UMETA(DisplayName = "Error")
};

