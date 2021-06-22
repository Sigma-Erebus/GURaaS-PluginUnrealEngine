// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DataNetworking.h"
#include "CustomSettings.h"
#include "jsonHelper.h"
#include "DataManager.generated.h"

USTRUCT(BlueprintType)
struct FData
{
	GENERATED_USTRUCT_BODY()
public:
	// Constructors
	FData(FString d, FString t1, FString t2, FString t3, FString t4, TArray<FString> addTag, FString t) {
		data = d;
		tag1 = t1;
		tag2 = t2;
		tag3 = t3;
		tag4 = t4;
		additional_tags = addTag;
		time = t;
	}
	FData() {

	}

	FString ToJsonString()
	{
		FString tStr = "{";
		tStr += jsonHelper::FieldToJson("time", time);
		tStr += jsonHelper::FieldToJson("data", data);
		tStr += jsonHelper::FieldToJson("tag1", tag1);
		tStr += jsonHelper::FieldToJson("tag2", tag2);
		tStr += jsonHelper::FieldToJson("tag3", tag3);
		tStr += jsonHelper::FieldToJson("tag4", tag4);
		tStr += jsonHelper::StringArrayToJson("additional_tags", additional_tags, true);
		tStr += "}";
		return tStr;
	}

	void SetTime(FString nTime) {
		time = nTime;
	}

public:
	FString data;
	FString tag1;
	FString tag2;
	FString tag3;
	FString tag4;
	FString time;
	TArray<FString> additional_tags;
};


UCLASS()
class ADataManager : public AActor
{
	GENERATED_BODY()
private:
	FString gameID, sessionID, postURL, statusURL;
	TArray<FData> Logs;

	float uploadIntervalSeconds;

	UPROPERTY()
	UDataNetworking* networker;
	// Builds the URL to send the POST request to
	void BuildURL();
	
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void OnBeginPlayFinished();

public:
	// Sets default values for this actor's properties
	ADataManager();
	~ADataManager() = default;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category=DataLogger)
	void QueueData(FString data, FString tag1, FString tag2, FString tag3, FString tag4, TArray<FString> addTag);

	// Push the current data to the server
	UFUNCTION(BlueprintCallable, Category = DataLogger)
	void Push(FString _playerID, FString _sessionID, FString context);

	void Init(FString _gameID, int32 _version);
	static FString GetCurrentTime();

};

USTRUCT()
struct FGURaaSDataStruct {
	GENERATED_USTRUCT_BODY()
public:
	FString ToJsonString() {

		UCustomSettings* Settings = GetMutableDefault<UCustomSettings>();

		FString tStr = "{";
		tStr += jsonHelper::FieldToJson("id_session", session_id);
		tStr += jsonHelper::FieldToJson("id_player", player_id);
		tStr += jsonHelper::FieldToJson("version", FString::FromInt(Settings->_gameVersion));
		tStr += jsonHelper::FieldToJson("start", ADataManager::GetCurrentTime());
		tStr += jsonHelper::FieldToJson("end", ADataManager::GetCurrentTime());
		tStr += jsonHelper::FieldToJson("context", context);
		int i = 0;
		tStr += "\"data\": [";
		for (FData tData : data) {
			if (i != 0) tStr += ", ";
			i++;
			tStr += tData.ToJsonString();
		}
		tStr += ']';
		tStr += "}";
		return tStr;
	}
public:
	FString player_id;
	FString session_id;
	FString context;
	TArray<FData> data;
};