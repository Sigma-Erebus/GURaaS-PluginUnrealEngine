// Copyright 2015 Moritz Wundke. All Rights Reserved.
// Released under MIT.

#pragma once

#include "CustomSettings.generated.h"

/**
 * Setting object used to hold both config settings and editable ones in one place
 * To ensure the settings are saved to the specified config file make sure to add
 * props using the globalconfig or config meta.
 */
UCLASS(config = Game, defaultconfig)
class UCustomSettings : public UObject
{
	GENERATED_BODY()

public:
	UCustomSettings(const FObjectInitializer& ObjectInitializer);
	UPROPERTY(EditAnywhere, config, Category = PluginSettings)
	bool _isEnabled = true;

	UPROPERTY(EditAnywhere, config, Category = GameConfig)
	FString _gameID;

	UPROPERTY(EditAnywhere, config, Category = GameConfig)
	bool _useLocalhost = false;

	UPROPERTY(EditAnywhere, config, Category = GameConfig)
	int32 _gameVersion;
};