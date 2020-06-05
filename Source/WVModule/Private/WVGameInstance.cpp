// Fill out your copyright notice in the Description page of Project Settings.


#include "WVGameInstance.h"
#include "WVModule.h"
#include "Logger/WVLog.h"
#include "ConfigUtil/WVConfigUtil.h"
#include "EventSys/WVEventDispatcher.h"
#include "Model/WVModelMgr.h"
#include "View/WVViewCreatorMgr.h"
#include "View/WVViewMgr.h"

void UWVGameInstance::Init()
{
	UE_LOG(LogWVModule, Display, TEXT("===UWVGameInstance::Init==="));
	
	FWVLog::GetInstance();
	UWVConfigUtil::GetInstance();
	UWVEventDispatcher::GetInstance();
	UWVModelMgr::GetInstance();
	UWVViewCreatorMgr::GetInstance();
	UWVViewMgr::GetInstance();

	UWVEventDispatcher::GetInstance()->SetupGameInstance(this);
	UWVViewCreatorMgr::GetInstance()->SetupGameInstance(this);
	UWVViewMgr::GetInstance()->SetupGameInstance(this);
}

void UWVGameInstance::Shutdown()
{
	UWVViewMgr::GetInstance()->Cleanup();
	UWVViewCreatorMgr::GetInstance()->Cleanup();
	UWVModelMgr::GetInstance()->Cleanup();
	UWVEventDispatcher::GetInstance()->Cleanup();
	UWVConfigUtil::GetInstance()->Cleanup();
	FWVLog::GetInstance()->Cleanup();

	UE_LOG(LogWVModule, Display, TEXT("===UWVGameInstance::Shutdown==="));
}
