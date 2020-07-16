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
_registedEvents(),
_gameIns(nullptr)
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
	for (int32 nCategory = (int32)EWVEventCategory::Begin + 1; nCategory != (int32)EWVEventCategory::End; ++nCategory)
	{
		FText txt = UEnum::GetDisplayValueAsText<EWVEventCategory>((EWVEventCategory)nCategory);
		_categoryNames.Emplace((EWVEventCategory)nCategory, txt.ToString());
	}

	//添加事件名
	for (int32 nEvent = (int32)EWVEventName::Begin + 1; nEvent != (int32)EWVEventName::End; ++nEvent)
	{
		FText txt = UEnum::GetDisplayValueAsText<EWVEventName>((EWVEventName)nEvent);
		_eventNames.Emplace((EWVEventName)nEvent, txt.ToString());
	}
}

void UWVEventDispatcher::AddListener(const FString& inEventSignature, UObject* inCaller, const FString& inFuncName)
{
	auto eventSignatures = _registedEvents.Find(inCaller);

	if (eventSignatures && eventSignatures->Contains(inEventSignature))
	{
		WVLogW(TEXT("%s with %s has added"), *(inCaller->GetName()), *inEventSignature)
		return;
	}
	
	UWVEventHandler* pHandle = _handlers.FindRef(inEventSignature);
	if (!pHandle)
	{
		pHandle = NewObject<UWVEventHandler>();
		_handlers.Add(inEventSignature, pHandle);
	}
	TSharedPtr<FWVEventListener> pListener = MakeShareable(new FWVEventListener(inCaller, inFuncName));
	pHandle->Add(pListener);

	RegistEvent(inEventSignature, inCaller);
}

void UWVEventDispatcher::AddListener(const FString& inEventSignature, UObject* inCaller,
	FWVEventDelegate_One inDelegateOne)
{
	auto eventSignatures = _registedEvents.Find(inCaller);

	if (eventSignatures && eventSignatures->Contains(inEventSignature))
	{
		WVLogW(TEXT("%s with %s has added"), *(inCaller->GetName()), *inEventSignature)
			return;
	}
	
	UWVEventHandler* pHandle = _handlers.FindRef(inEventSignature);
	if (!pHandle)
	{
		pHandle = NewObject<UWVEventHandler>();
		_handlers.Add(inEventSignature, pHandle);
	}
	TSharedPtr<FWVEventListener> pListener = MakeShareable(new FWVEventListener(inCaller, inDelegateOne));
	pHandle->Add(pListener);

	RegistEvent(inEventSignature, inCaller);
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

	UnRegistEvent(inEventSignature, inCaller);
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

void UWVEventDispatcher::RegistEvent(const FString& inEventSignature, UObject* inCaller)
{
	auto &events = _registedEvents.FindOrAdd(inCaller);
	events.Add(inEventSignature);
}

void UWVEventDispatcher::UnRegistEvent(const FString& inEventSignature, UObject* inCaller)
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

void UWVEventDispatcher::FireEvent_SP(EWVEventCategory inCategory, EWVEventName inEventName)
{
	FireEvent(GetEventSignature(inCategory, inEventName));
}

void UWVEventDispatcher::FireEvent_SP(EWVEventCategory inCategory, EWVEventName inEventName, bool bVal)
{
	FireEvent(GetEventSignature(inCategory, inEventName), bVal);
}

void UWVEventDispatcher::FireEvent_SP(EWVEventCategory inCategory, EWVEventName inEventName, int32 num)
{
	FireEvent(GetEventSignature(inCategory, inEventName), num);
}

void UWVEventDispatcher::FireEvent_SP(EWVEventCategory inCategory, EWVEventName inEventName, float num)
{
	FireEvent(GetEventSignature(inCategory, inEventName), num);
}

void UWVEventDispatcher::FireEvent_SP(EWVEventCategory inCategory, EWVEventName inEventName, FString str)
{
	FireEvent(GetEventSignature(inCategory, inEventName), str);
}

void UWVEventDispatcher::FireEvent_SP(EWVEventCategory inCategory, EWVEventName inEventName, void* obj)
{
	FireEvent(GetEventSignature(inCategory, inEventName), obj);
}

void UWVEventDispatcher::FireEvent_BP(const FString &inEventSignature, FWVEventParams_BP &params_bp)
{
	UWVEventHandler* pHandle = _handlers.FindRef(inEventSignature);
	if (IsValid(pHandle))
	{
		auto ret = pHandle->FireEvent_BP(params_bp);
		if (ret == EWVEventHandlerRet::Fail)
		{
			WVLogW(TEXT("fire event'%s' fail"), *inEventSignature)
		}
	}
}
