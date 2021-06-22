// Fill out your copyright notice in the Description page of Project Settings.

#include "DataManager.h"
#include "Json.h"
#include "CustomSettings.h"
#include "Engine.h"
#include "Math/UnrealMathUtility.h"

// Sets default values
ADataManager::ADataManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ADataManager::BeginPlay()
{
	Super::BeginPlay();
	if (networker == nullptr)
	{
		networker = NewObject<UDataNetworking>(this, UDataNetworking::StaticClass());
		networker->Init();
	}
	
	UCustomSettings* Settings = GetMutableDefault<UCustomSettings>();
	FString GameId = Settings->_gameID;
	
	this->Init(GameId, 1);
	OnBeginPlayFinished();
}

void ADataManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADataManager::QueueData(FString data, FString tag1, FString tag2, FString tag3, FString tag4, TArray<FString> addTag)
{
	FData d(FData(data, tag1, tag2, tag3, tag4, addTag, GetCurrentTime()));
	Logs.Add(d);
}

void ADataManager::Push(FString _playerID, FString _sessionID, FString context) {
	// Must have at least one log to push
	if(Logs.Num() > 0){
		FGURaaSDataStruct tObj = FGURaaSDataStruct();
		tObj.player_id = _playerID;
		tObj.session_id = _sessionID;
		tObj.context = context;
		tObj.data = Logs;
		BuildURL(); // Settings may have been changed, rebuild before sending
		networker->HttpPost(postURL, tObj.ToJsonString());
		// clear Logs
		Logs.Empty();
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Pushing an empty array"));
	}
}

FString ADataManager::GetCurrentTime() {
	FDateTime CurrentDateAndTime = FDateTime::UtcNow();
	FString time = "";
	time.AppendInt(CurrentDateAndTime.GetYear()); time += '-';
	time.AppendInt(CurrentDateAndTime.GetMonth()); time += '-';
	time.AppendInt(CurrentDateAndTime.GetDay());
	time += ' ';
	time.AppendInt(CurrentDateAndTime.GetHour()); time += ':';
	time.AppendInt(CurrentDateAndTime.GetMinute()); time += ':';
	time.AppendInt(CurrentDateAndTime.GetSecond());

	return time;
}

void ADataManager::Init(FString _gameID, int32 _version)
{
	gameID = _gameID;
	BuildURL();
}

void ADataManager::BuildURL() {
	UCustomSettings* Settings = GetMutableDefault<UCustomSettings>();

	FString tail = "/v" + FString::FromInt(Settings->_gameVersion) + "/games/" + Settings->_gameID + "/data";
	if (Settings->_useLocalhost) {
		postURL = "http://localhost:9001" + tail;
	}
	else {
		postURL = "https://grg.service.guraas.com" + tail;
	}
}