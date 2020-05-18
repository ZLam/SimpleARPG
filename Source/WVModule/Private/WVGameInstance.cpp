// Fill out your copyright notice in the Description page of Project Settings.


#include "WVGameInstance.h"
#include "WVModule.h"
#include "Logger/WVLog.h"
#include "ConfigUtil/WVConfigUtil.h"

void UWVGameInstance::Init()
{
	UE_LOG(LogWVModule, Display, TEXT("===UWVGameInstance::Init==="));
	
	FWVLog::GetInstance();
	UWVConfigUtil::GetInstance();
}

void UWVGameInstance::Shutdown()
{
	UWVConfigUtil::Cleanup();
	FWVLog::Cleanup();

	UE_LOG(LogWVModule, Display, TEXT("===UWVGameInstance::Shutdown==="));
}
