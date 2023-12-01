// Fill out your copyright notice in the Description page of Project Settings.


#include "GURaaS_API.h"
#include "GURaaS_GameInstance.h"
#include "GURaaS_Channel.h"
#include "GLogger.h"


#include "Kismet/GameplayStatics.h"

UGURaaSGameInstance* UGLog_API::m_GameInstance = nullptr;

void UGLog_API::Initialize(FString GameID, UGURaaSGameInstance* GameInstance)
{
	if (GameInstance == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("GURaaS.Initialize: GameInstance is null"));
		return;
	}
	if (m_GameInstance != nullptr && m_GameInstance != GameInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("GURaaS.Initialize: GameInstance is already initialized with another instance be aware custom channels will not be shared"));
		return;
	}
	if (GameID.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("GURaaS.Initialize: GameID is empty"));
		return;
	}

	m_GameInstance = GameInstance;
	m_GameInstance->m_Logger = NewObject<UGLogger>(m_GameInstance);
	m_GameInstance->m_Logger->SetGameID(GameID);

	m_GameInstance->m_Logger->AddChannel(NewObject<UGLogChannelConsole>(m_GameInstance), "Console");
	m_GameInstance->m_Logger->AddChannel(NewObject<UGLogChannelGURaaS>(m_GameInstance), "GURaaS");
	
}

void UGLog_API::StartSession(FString PlayerID, FString context)
{
	if( !m_GameInstance)
	{ 
		UE_LOG(LogTemp, Error, TEXT("GURaaS: Not initialized"));
		return;
	}
	m_GameInstance->m_Logger->StartSession(PlayerID, context);
	
}

void UGLog_API::CloseSession()
{
	if (!m_GameInstance)
	{
		UE_LOG(LogTemp, Error, TEXT("GURaaS: Not initialized"));
		return;
	}
	m_GameInstance->m_Logger->CloseSession();
}

void UGLog_API::Log(EGURaaSVerboseLevel VerboseLevel, FString Message, FString Tag1, FString Tag2, FString Tag3)
{
	if (!m_GameInstance)
	{
		UE_LOG(LogTemp, Error, TEXT("GURaaS: Not initialized"));
		return;
	}
	if (!m_GameInstance->m_Logger->IsSessionStarted())
	{
		UE_LOG(LogTemp, Error, TEXT("GURaaS.Log: Session not started"));
		return;
	}
	
	m_GameInstance->m_Logger->Log(VerboseLevel, Message, Tag1, Tag2, Tag3);
}

bool UGLog_API::IsInitialized()
{
	return m_GameInstance != nullptr;
}

void UGLog_API::AddChannel(UGLogChannel* Channel, FString ChannelID)
{
	if (!m_GameInstance)
	{
		UE_LOG(LogTemp, Error, TEXT("GURaaS: Not initialized"));
		return;
	}
	if (!Channel)
	{
		UE_LOG(LogTemp, Error, TEXT("GURaaS.AddChannel: Channel is null"));
		return;
	}

	m_GameInstance->m_Logger->AddChannel(Channel, ChannelID);
}

void UGLog_API::RemoveChannel(FString ChannelID)
{
	if (!m_GameInstance)
	{
		UE_LOG(LogTemp, Error, TEXT("GURaaS: Not initialized"));
		return;
	}
	m_GameInstance->m_Logger->RemoveChannel(ChannelID);
}

void UGLog_API::FlushChannels()
{
	if (!m_GameInstance)
	{
		UE_LOG(LogTemp, Error, TEXT("GURaaS: Not initialized"));
		return;
	}
	m_GameInstance->m_Logger->FlushChannels();
}

FString UGLog_API::ConvertToString(EGURaaSVerboseLevel VerboseLevel)
{
	static UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EGURaaSVerboseLevel"));
	
	if (!EnumPtr)
	{
		return FString("Invalid");
	}

	return EnumPtr->GetDisplayNameTextByIndex((int32)VerboseLevel).ToString();
}

void UGLog_API::Reset()
{
	m_GameInstance = nullptr;
}


