// Fill out your copyright notice in the Description page of Project Settings.


#include "EventSys/WVEventDispatcher.h"
#include "WVModule.h"
#include "EventSys/WVEventListener.h"
#include "EventSys/WVEventHandler.h"

UWVEventDispatcher* UWVEventDispatcher::_instance = nullptr;

UWVEventDispatcher::UWVEventDispatcher():
_categoryNames(),
_eventNames(),
_handlers()
{
	UE_LOG(LogWVModule, Display, TEXT("===UWVEventDispatcher::UWVEventDispatcher==="))
}

UWVEventDispatcher::~UWVEventDispatcher()
{
	UE_LOG(LogWVModule, Display, TEXT("===UWVEventDispatcher::~UWVEventDispatcher==="))

	_instance = nullptr;
}

UWVEventDispatcher* UWVEventDispatcher::GetInstance()
{
	if (!IsValid(_instance))
	{
		_instance = NewObject<UWVEventDispatcher>();
		_instance->AddToRoot();
		_instance->Init();
	}
	return _instance;
}

void UWVEventDispatcher::Cleanup()
{
	if (IsValid(_instance))
	{
		_instance->RemoveFromRoot();
	}
}

void UWVEventDispatcher::Init()
{
	//添加事件分类
	_categoryNames.Emplace(EWVEventCategory::Inner, FString(TEXT("Inner")));
	_categoryNames.Emplace(EWVEventCategory::Network, FString(TEXT("Network")));

	//添加事件名
	_eventNames.Emplace(EWVEventName::TestActor, FString(TEXT("TestActor")));
	_eventNames.Emplace(EWVEventName::TestUI, FString(TEXT("TestUI")));
}

void UWVEventDispatcher::AddListener(const FString& inEventSignature, UObject* inCaller, const FString& inFuncName)
{
	UWVEventHandler* pHandle = _handlers.FindRef(inEventSignature);
	if (!pHandle)
	{
		pHandle = NewObject<UWVEventHandler>();
		_handlers.Add(inEventSignature, pHandle);
	}
	TSharedPtr<FWVEventListener> pListener = MakeShareable(new FWVEventListener(inCaller, inFuncName));
	pHandle->Add(pListener);
}

void UWVEventDispatcher::RemoveListener(const FString& inEventSignature, UObject* inCaller)
{
	UWVEventHandler* pHandle = _handlers.FindRef(inEventSignature);
	if (pHandle->IsValidLowLevel())
	{
		pHandle->Delete(inCaller);
	}
}

void UWVEventDispatcher::FireEvent(const FString &inEventSignature)
{
	UWVEventHandler* pHandle = _handlers.FindRef(inEventSignature);
	if (pHandle->IsValidLowLevel())
	{
		pHandle->FireEvent();
	}
}
