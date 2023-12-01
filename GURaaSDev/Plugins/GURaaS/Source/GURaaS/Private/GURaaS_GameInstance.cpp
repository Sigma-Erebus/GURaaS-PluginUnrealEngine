// Fill out your copyright notice in the Description page of Project Settings.
#include "GURaaS_GameInstance.h"

#include "GLogger.h"

void UGURaaSGameInstance::Init()
{
	Super::Init();
}

void UGURaaSGameInstance::Shutdown()
{
	Super::Shutdown();

	if ( m_Logger != nullptr && m_Logger->IsSessionStarted())
	{
		m_Logger->CloseSession();
	}
}


