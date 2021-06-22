// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "DataNetworking.generated.h"

/**
 *
 */
UCLASS()
class UDataNetworking : public UObject
{
	GENERATED_BODY()
	FString cacheFileName;
	bool isConnected;

public:
	void Init();
	void HttpRequest(const FString  url);
	void HttpPost(const FString url, const FString contentJsonString, bool repeatingPost = false);
	bool Pushing;
private:
	void SetRequestHeaders(TSharedRef<IHttpRequest>& Request);
	void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	bool SafeToFile(FString directory, FString fileName, FString textToSave);
	bool ProcessCache(FString url, FString directory, FString fileName);
};