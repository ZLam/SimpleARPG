// Fill out your copyright notice in the Description page of Project Settings.


#include "ConfigUtil/WVConfigUtil.h"
#include "Logger/WVLog.h"

UWVConfigUtil* UWVConfigUtil::_instance = nullptr;

UWVConfigUtil::UWVConfigUtil()
{
	UE_LOG(WVLogger, Display, TEXT("===UWVConfigUtil::UWVConfigUtil==="))
}

UWVConfigUtil::~UWVConfigUtil()
{
	UE_LOG(WVLogger, Display, TEXT("===UWVConfigUtil::~UWVConfigUtil==="))

	_instance = nullptr;
}

UWVConfigUtil* UWVConfigUtil::GetInstance()
{
	if (!IsValid(_instance))
	{
		_instance = NewObject<UWVConfigUtil>();
		_instance->AddToRoot();
	}
	return _instance;
}

void UWVConfigUtil::Cleanup()
{
	if (IsValid(_instance))
	{
		_instance->RemoveFromRoot();
	}
}


