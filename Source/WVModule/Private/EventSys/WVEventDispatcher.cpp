// Fill out your copyright notice in the Description page of Project Settings.


#include "EventSys/WVEventDispatcher.h"
#include "WVModule.h"
#include "EventSys/WVEventListener.h"
#include "EventSys/WVEventHandler.h"
#include "Logger/WVLog.h"

UWVEventDispatcher* UWVEventDispatcher::_instance = nullptr;

UWVEventDispatcher::UWVEventDispatcher():
_categoryNames(),
_eventNames(),
_handlers(),
_registedEvents()
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

	RegistEvent(inCaller, inEventSignature);
}

void UWVEventDispatcher::AddListener(const FString& inEventSignature, UObject* inCaller,
	FWVEventDelegate_One inDelegateOne)
{
	UWVEventHandler* pHandle = _handlers.FindRef(inEventSignature);
	if (!pHandle)
	{
		pHandle = NewObject<UWVEventHandler>();
		_handlers.Add(inEventSignature, pHandle);
	}
	TSharedPtr<FWVEventListener> pListener = MakeShareable(new FWVEventListener(inCaller, inDelegateOne));
	pHandle->Add(pListener);

	RegistEvent(inCaller, inEventSignature);
}

void UWVEventDispatcher::AddListener(EWVEventCategory inCategory, EWVEventName inEventName, UObject* inCaller,
	const FString& inFuncName)
{
	FString eventSignature = GetEventSignature(inCategory, inEventName);
	AddListener(eventSignature, inCaller, inFuncName);
}

void UWVEventDispatcher::AddListener(EWVEventCategory inCategory, EWVEventName inEventName, UObject* inCaller,
	FWVEventDelegate_One inDelegateOne)
{
	FString eventSignature = GetEventSignature(inCategory, inEventName);
	AddListener(eventSignature, inCaller, inDelegateOne);
}

void UWVEventDispatcher::_RemoveListener(const FString& inEventSignature, UObject* inCaller)
{
	UWVEventHandler* pHandle = _handlers.FindRef(inEventSignature);
	if (IsValid(pHandle))
	{
		pHandle->Delete(inCaller);
	}
}

void UWVEventDispatcher::RemoveListener(const FString& inEventSignature, UObject* inCaller)
{
	_RemoveListener(inEventSignature, inCaller);

	UnRegistEvent(inCaller, inEventSignature);
}

void UWVEventDispatcher::RemoveAllListener(UObject *inCaller)
{
	auto events = _registedEvents.Find(inCaller);

	if (events && events->Num() > 0)
	{
		for (auto &eventSignature : *events)
		{
			_RemoveListener(eventSignature, inCaller);
		}
	}
	
	UnRegistAllEvent(inCaller);
}

void UWVEventDispatcher::RegistEvent(UObject* inCaller, const FString& inEventSignature)
{
	auto &events = _registedEvents.FindOrAdd(inCaller);
	events.Add(inEventSignature);
}

void UWVEventDispatcher::UnRegistEvent(UObject* inCaller, const FString& inEventSignature)
{
	auto events = _registedEvents.Find(inCaller);
	if (events)
	{
		events->Remove(inEventSignature);
	}
}

void UWVEventDispatcher::UnRegistAllEvent(UObject* inCaller)
{
	_registedEvents.Remove(inCaller);
}

void UWVEventDispatcher::FireEvent(const FString &inEventSignature)
{
	UWVEventHandler* pHandle = _handlers.FindRef(inEventSignature);
	if (IsValid(pHandle))
	{
		pHandle->FireEvent();
	}
}

void UWVEventDispatcher::FireEvent(const FString& inEventSignature, bool bVal)
{
	UWVEventHandler* pHandle = _handlers.FindRef(inEventSignature);
	if (IsValid(pHandle))
	{
		pHandle->FireEvent(&bVal);
	}
}

void UWVEventDispatcher::FireEvent(const FString& inEventSignature, int32 num)
{
	UWVEventHandler* pHandle = _handlers.FindRef(inEventSignature);
	if (IsValid(pHandle))
	{
		pHandle->FireEvent(&num);
	}
}

void UWVEventDispatcher::FireEvent(const FString& inEventSignature, float num)
{
	UWVEventHandler* pHandle = _handlers.FindRef(inEventSignature);
	if (IsValid(pHandle))
	{
		pHandle->FireEvent(&num);
	}
}

void UWVEventDispatcher::FireEvent(const FString& inEventSignature, FString str)
{
	UWVEventHandler* pHandle = _handlers.FindRef(inEventSignature);
	if (IsValid(pHandle))
	{
		pHandle->FireEvent(&str);
	}
}

void UWVEventDispatcher::FireEvent(const FString& inEventSignature, void* obj)
{
	UWVEventHandler* pHandle = _handlers.FindRef(inEventSignature);
	if (IsValid(pHandle))
	{
		pHandle->FireEvent(obj);
	}
}

void UWVEventDispatcher::FireEvent_BP(FWVEventParams_BP &params_bp)
{
	WVLogI(TEXT("%s_%p_%p"), *params_bp.eventSignature, params_bp.propertyInfoPtr, params_bp.propertyPtr)
}
