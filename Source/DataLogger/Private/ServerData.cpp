// Fill out your copyright notice in the Description page of Project Settings.

#include "ServerData.h"

FServerData::FServerData(FString _data, TArray<FString> _tags)
{
	data = _data;
	tags = _tags;
}

FString FServerData::GetFullData()
{
	FString tagsString = data;
	for (auto str : tags)
	{
		tagsString += " ," + str;
	}
	return tagsString;
}

void FServerData::SetData(FString _data, TArray<FString> _tags)
{
	data = _data;
	tags = _tags;
}