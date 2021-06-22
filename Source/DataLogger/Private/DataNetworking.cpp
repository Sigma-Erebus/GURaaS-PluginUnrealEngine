//Fill out your copyright notice in the Description page of Project Settings.

#include "DataNetworking.h"
#include "Json.h"
#include "DataManager.h"
#include "CoreMisc.h"
#include "FileHelper.h"
#include "PlatformFile.h"

void UDataNetworking::Init()
{
	cacheFileName = "GuraasCache.sav";
	isConnected = false;
	Pushing = true;
}

void UDataNetworking::HttpRequest(const FString url)
{
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &UDataNetworking::OnResponseReceived);
	Request->SetURL(url);
	Request->SetVerb("GET");
	SetRequestHeaders(Request);
	Request->ProcessRequest();
}

void UDataNetworking::HttpPost(const FString url, const FString contentJsonString, bool repeatingPost)
{
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &UDataNetworking::OnResponseReceived);
	UE_LOG(LogTemp, Warning, TEXT("URL: %s"), *url);
	UE_LOG(LogTemp, Warning, TEXT("Content: %s"), *contentJsonString);
	UCustomSettings* Settings = GetMutableDefault<UCustomSettings>();
	if (Settings->_isEnabled) {
		Request->SetURL(url);
		Request->SetVerb("POST");
		SetRequestHeaders(Request);
		Request->SetContentAsString(contentJsonString);
		Request->ProcessRequest();
		//Try to handle cache
		/*if(isConnected && !repeatingPost)
		{
			ProcessCache(url, FPaths::ProjectContentDir(), cacheFileName);
		}*/
	}
}

void UDataNetworking::SetRequestHeaders(TSharedRef<IHttpRequest>& Request) {
	Request->SetHeader(TEXT("User-Agent"), TEXT("X-UnrealEngine-Agent"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetHeader(TEXT("Accepts"), TEXT("application/json"));
}

void UDataNetworking::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	isConnected = bWasSuccessful;
	FString JsonStr = "";
	if (!bWasSuccessful)
	{
		//FString r = Response->GetContentAsString();
		//SafeToFile(FPaths::ProjectContentDir(), cacheFileName, r);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("RESPONSE: %s"), *Response->GetContentAsString());
	}
}

bool UDataNetworking::SafeToFile(FString directory, FString fileName, FString textToSave)
{
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	// during the call.
	if (PlatformFile.CreateDirectoryTree(*directory))
	{
		// Get absolute file path
		const FString absolute_file_path = directory + fileName;
		return FFileHelper::SaveStringToFile(textToSave, *absolute_file_path, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), FILEWRITE_Append);
	}
	return false;
}

bool UDataNetworking::ProcessCache(FString url, FString directory, FString fileName)
{
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	const FString absolute_file_path = directory + fileName;
	TArray<FString> tResult;
	if(FFileHelper::LoadFileToStringArray(tResult, *absolute_file_path))
	{
		for(FString tStr : tResult)
		{
			HttpPost(url, tStr, true);
		}
		//Delete the file
		return FFileHelper::SaveStringToFile("", *absolute_file_path);
	}
	return false;
}
