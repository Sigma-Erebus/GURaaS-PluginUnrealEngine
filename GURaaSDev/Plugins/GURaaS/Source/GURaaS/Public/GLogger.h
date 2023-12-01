// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EVerboseLevel.h"

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GLogger.generated.h"

class UGLogChannel;

/**
 * 
 */

class UGLoggerAuthorizedReset {
	friend class UGLogger;
	static void Reset();
};

UCLASS()
class GURAAS_API UGLogger : public UObject
{
	GENERATED_BODY()
	
public:
	
	virtual void FinishDestroy() override;

	UFUNCTION(BlueprintCallable, Category = "GURaaS")
		void StartSession(FString PlayerId, FString Context);

	UFUNCTION(BlueprintCallable, Category = "GURaaS")
		void CloseSession();

	UFUNCTION(BlueprintCallable, Category = "GURaaS")
		void AddChannel(UGLogChannel* Channel, FString ChannelID);

	UFUNCTION(BlueprintCallable, Category = "GURaaS")
		void RemoveChannel(FString ChannelID);

	UFUNCTION(BlueprintCallable, Category = "GURaaS")
		bool HasChannel(FString ChannelID);

	UFUNCTION(BlueprintCallable, Category = "GURaaS")
		void SetGameID(FString GameID);

	UFUNCTION(BlueprintCallable, Category = "GURaaS")
		FString GetGameID();

	UFUNCTION(BlueprintCallable, Category = "GURaaS")
		void Log(EGURaaSVerboseLevel VerboseLevel, FString Message, FString Tag1, FString Tag2, FString Tag3);
	
	UFUNCTION(BlueprintCallable, Category = "GURaaS")
		bool IsSessionStarted();

	UFUNCTION(BlueprintCallable, Category = "GURaaS")
		void FlushChannels();

private:

	UPROPERTY() TMap<FString, UGLogChannel*> m_Channels;
	UPROPERTY() FString m_GameID;
	UPROPERTY() bool m_SessionStarted = false;

};
