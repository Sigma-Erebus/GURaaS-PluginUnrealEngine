// Copyright Epic Games, Inc. All Rights Reserved.

#include "GURaaS.h"

#include "GURaaS_Channel.h"

#include "engine/StreamableManager.h"
#include "AssetRegistry/AssetRegistryHelpers.h"
#include "AssetRegistry/IAssetRegistry.h"

#define LOCTEXT_NAMESPACE "FGURaaSModule"

DEFINE_LOG_CATEGORY(LogGLog);

void FGURaaSModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FGURaaSModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FGURaaSModule, GURaaS)