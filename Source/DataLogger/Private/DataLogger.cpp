// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "IDataLogger.h"
#include "CustomSettings.h"
#include "Developer/Settings/Public/ISettingsModule.h"
#include "Developer/Settings/Public/ISettingsSection.h"
#include "Developer/Settings/Public/ISettingsContainer.h"

#define LOCTEXT_NAMESPACE "CustomSettings"

class FDataLogger : public IDataLogger
{
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	virtual bool SupportsDynamicReloading() override {
		return true;
	}

private:

	// Callback for when the settings were saved.
	bool HandleSettingsSaved() {
		UCustomSettings* Settings = GetMutableDefault<UCustomSettings>();
		bool ResaveSettings = false;
		// You can put any validation code in here and resave the settings in case an invalid
		// value has been entered
		if (ResaveSettings) {
			Settings->SaveConfig();
		}
		return true;
	}

	void RegisterSettings() {
		// Registering some settings is just a matter of exposing the default UObject of
		// your desired class, feel free to add here all those settings you want to expose
		// to your LDs or artists.
		if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings")) {
			// Create the new category
			ISettingsContainerPtr SettingsContainer = SettingsModule->GetContainer("Project");
			SettingsContainer->DescribeCategory("Plugins",
												LOCTEXT("RuntimeWDCategoryName", "Plugins"),
												LOCTEXT("RuntimeWDCategoryDescription", "Game configuration for the CustomSettings game module"));

			// Register the settings
			ISettingsSectionPtr SettingsSection = SettingsModule->RegisterSettings("Project", "Plugins", "Data Logger",
																				   LOCTEXT("RuntimeGeneralSettingsName", "Data Logger"),
																				   LOCTEXT("RuntimeGeneralSettingsDescription", "Base configuration for our game module"),
																				   GetMutableDefault<UCustomSettings>()
			);

			// Register the save handler to your settings, you might want to use it to
			// validate those or just act to settings changes.
			if (SettingsSection.IsValid()) {
				//SettingsSection->OnModified().BindRaw(this, &FCustomSettingsModule::HandleSettingsSaved);
			}
		}
	}

	void UnregisterSettings() {
		// Ensure to unregister all of your registered settings here, hot-reload would
		// otherwise yield unexpected results.

		if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings")) {
			SettingsModule->UnregisterSettings("Project", "CustomSettings", "General");
		}
	}
};

IMPLEMENT_MODULE( FDataLogger, DataLogger )
void FDataLogger::StartupModule()
{
	RegisterSettings();
}


void FDataLogger::ShutdownModule()
{
	if (UObjectInitialized()) {
		UnregisterSettings();
	}
}

