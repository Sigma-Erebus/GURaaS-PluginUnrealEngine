// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class jsonHelper
{
public:
	static FString FieldToJson(FString value, FString data, bool isLastField = false);
	static FString StringArrayToJson(FString value, TArray<FString> data, bool isLastField = false);
};
