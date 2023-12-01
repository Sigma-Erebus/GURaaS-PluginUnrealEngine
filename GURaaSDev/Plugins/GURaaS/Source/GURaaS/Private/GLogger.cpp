// Fill out your copyright notice in the Description page of Project Settings.

#include "GLogger.h"

#include "GURaaS_Channel.h"
#include "GURaaS_API.h"

#include "misc/Guid.h"

bool UGLogger::IsSessionStarted()
{
	return m_SessionStarted;
}

void UGLogger::FlushChannels()
{
	for (auto& channel : m_Channels)
	{
		if (channel.Value == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("GURaaS: Channel %s is null"), *channel.Key);
			continue;
		}
		channel.Value->Flush();
	}
}

void UGLogger::FinishDestroy()
{
	if (m_SessionStarted)
	{
		CloseSession();
		FlushChannels();
	}
	UGLoggerAuthorizedReset::Reset();
	Super::FinishDestroy();
}

void UGLogger::StartSession(FString PlayerId, FString Context)
{
	if (m_SessionStarted) 
	{
		UE_LOG(LogTemp, Warning, TEXT("GURaaS.StartSession: Session already started"));
		CloseSession();
	}

	FSessionInfo SessionInfo = FSessionInfo( m_GameID, FGuid::NewGuid().ToString(EGuidFormats::DigitsWithHyphensLower), PlayerId, Context, FDateTime::UtcNow());

	for (auto& channel : m_Channels)
	{
		channel.Value->StartSession(SessionInfo);
	}

	this->Log(EGURaaSVerboseLevel::EGURaaS_VerboseLevel_Event, FString::Printf(TEXT("UE%d.%d"), ENGINE_MAJOR_VERSION, ENGINE_MINOR_VERSION), "GLog", "Session", "Start");
	m_SessionStarted = true;
}

void UGLogger::CloseSession()
{
	if (m_SessionStarted == false)
	{
		return;
	}

	this->Log(EGURaaSVerboseLevel::EGURaaS_VerboseLevel_Event, "", "GLog", "Session", "Close");
	m_SessionStarted = false;
}

void UGLogger::AddChannel(UGLogChannel* Channel, FString ChannelID)
{
	if (Channel == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("GURaaS.AddChannel: Channel is null"));
		return;
	}

	auto* channel = m_Channels.FindRef(ChannelID);
	if (channel != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("GURaaS.AddChannel: Channel with ID %s already exists, and old channel is replaced"), *ChannelID);

		channel->MarkAsGarbage();

		m_Channels.Remove(ChannelID);
	}

	m_Channels.Add(ChannelID, Channel);
}

void UGLogger::RemoveChannel(FString ChannelID)
{
	auto* channel = m_Channels.FindRef(ChannelID);
	if (channel != nullptr)
	{
		channel->MarkAsGarbage();
		m_Channels.Remove(ChannelID);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("GURaaS.RemoveChannel: Channel with ID %s does not exist"), *ChannelID);
	}
}

bool UGLogger::HasChannel(FString ChannelID)
{
	return m_Channels.Contains(ChannelID) && m_Channels[ChannelID] != nullptr;
}

void UGLogger::SetGameID(FString GameID)
{
	m_GameID = GameID;
}

FString UGLogger::GetGameID()
{
	return m_GameID;
}

void UGLogger::Log(EGURaaSVerboseLevel VerboseLevel, FString Message, FString Tag1, FString Tag2, FString Tag3)
{
	for (auto& channel : m_Channels)
	{
		if (channel.Value == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("GURaaS: Channel %s is null"), *channel.Key);
			continue;
		}
		channel.Value->Log(VerboseLevel, Message, Tag1, Tag2, Tag3);
	}
}

void UGLoggerAuthorizedReset::Reset()
{
	UGLog_API::Reset();
}
