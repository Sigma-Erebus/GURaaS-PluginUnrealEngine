// Fill out your copyright notice in the Description page of Project Settings.

#include "jsonHelper.h"

FString jsonHelper::FieldToJson(FString value, FString data, bool isLastField)
{
	FString tStr = "";
	tStr += "\"" + value + "\": \"" + data + "\"";
	if (!isLastField) tStr += ", ";
	return tStr;
}

FString jsonHelper::StringArrayToJson(FString value, TArray<FString> data, bool isLastField)
{
	FString tStr = "";
	tStr += "\"" + value + "\": [";
	int i = 0;
	for (FString itStr : data)
	{
		if (i != 0) tStr += ", ";
		i++;
		tStr += "\"" + itStr + "\"";
	}
	tStr += "]";
	if (!isLastField) tStr += ", ";
	return tStr;
}