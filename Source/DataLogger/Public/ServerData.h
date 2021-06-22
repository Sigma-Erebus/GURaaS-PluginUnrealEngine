// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ServerData.generated.h"

USTRUCT(BlueprintType)
struct FServerData
{
	GENERATED_BODY()

public:
	FServerData() = default;
	FServerData(FString _data, TArray<FString> _tags);
	FString GetFullData();
	void SetData(FString _data, TArray<FString> _tags);
public:
	FString data;
	TArray<FString> tags;
};
