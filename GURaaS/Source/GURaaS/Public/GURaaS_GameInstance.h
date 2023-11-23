// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#include "GURaaS_GameInstance.generated.h"

class UGLogger;

/**
 * 
 */
UCLASS()
class GURAAS_API UGURaaSGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	virtual void Init() override;
	virtual void Shutdown() override;

	friend class UGLog_API; // Allow UGURaaS_API to access private members
private:
	
	UPROPERTY() UGLogger* m_Logger;
	

};
