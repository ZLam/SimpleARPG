// Fill out your copyright notice in the Description page of Project Settings.


#include "WVGameInstance.h"
#include "WVModule.h"
#include "Logger/WVLog.h"
#include "ConfigUtil/WVConfigUtil.h"
#include "EventSys/WVEventDispatcher.h"

void UWVGameInstance::Init()
{
	UE_LOG(LogWVModule, Display, TEXT("===UWVGameInstance::Init==="));
	
	FWVLog::GetInstance();
	UWVConfigUtil::GetInstance();
	UWVEventDispatcher::GetInstance();

	UWVEventDispatcher::GetInstance()->SetupGameInstance(this);
}

void UWVGameInstance::Shutdown()
{
	UWVEventDispatcher::Cleanup();
	UWVConfigUtil::Cleanup();
	FWVLog::Cleanup();

	UE_LOG(LogWVModule, Display, TEXT("===UWVGameInstance::Shutdown==="));
}
