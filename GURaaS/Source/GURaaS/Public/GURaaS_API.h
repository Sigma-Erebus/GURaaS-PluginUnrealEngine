// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "EVerboseLevel.h"

#include "GURaaS_API.generated.h"

class UGURaaSGameInstance;
class UGLogChannel;

UCLASS()
class GURAAS_API UGLog_API : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "GURaaS")
		static void Initialize(FString GameID, UGURaaSGameInstance* GameInstance);

	UFUNCTION(BlueprintCallable, Category = "GURaas")
		static void StartSession(FString PlayerID = "", FString context = "");

	UFUNCTION(BlueprintCallable, Category = "GURaas")
		static void CloseSession();

	UFUNCTION(BlueprintCallable, Category = "GURaaS")
		static void Log(EGURaaSVerboseLevel VerboseLevel, FString Message, FString Tag1 = "", FString Tag2 = "", FString Tag3 = "");

	UFUNCTION(BlueprintCallable, Category = "GURaaS")
		static bool IsInitialized();

	UFUNCTION(BLueprintCallable, Category = "GURaaS")
		static void AddChannel(UGLogChannel* Channel, FString ChannelID);

	UFUNCTION(BlueprintCallable, Category = "GURaaS")
		static void RemoveChannel(FString ChannelID);

	UFUNCTION(BlueprintCallable, Category = "GURaaS")
		static void FlushChannels();

	UFUNCTION(BlueprintCallable, Category = "GURaaS")
		static FString ConvertToString(EGURaaSVerboseLevel VerboseLevel);


private:
	friend class UGLoggerAuthorizedReset;
	static void Reset();

	static UGURaaSGameInstance* m_GameInstance;
};
