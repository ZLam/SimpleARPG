// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/Engine.h"
#include "WVEventCenter.h"
#include "EventSys/WVEventDelegate.h"
#include "WVEventDispatcher.generated.h"

class UWVGameInstance;
class UWVEventHandler;

/**
 * 
 */
UCLASS()
class WVMODULE_API UWVEventDispatcher : public UObject
{
	GENERATED_BODY()

protected:
	static UWVEventDispatcher* _instance;

	TMap<EWVEventCategory, FString> _categoryNames;
	TMap<EWVEventName, FString> _eventNames;

	UPROPERTY()
	TMap<FString, UWVEventHandler*> _handlers;

	TMap<UObject*, TArray<FString>> _registedEvents;

	UPROPERTY()
	UWVGameInstance *_gameIns;
	
public:
	static UWVEventDispatcher* GetInstance();
	void Cleanup();

	void SetupGameInstance(UWVGameInstance *ins)
	{
		_gameIns = ins;
	}
	
	FString GetEventSignature(EWVEventCategory category, EWVEventName eventName)
	{
		return WVEventSignature(*_categoryNames[category], *_eventNames[eventName]);
	}

	void AddListener(const FString &inEventSignature, UObject *inCaller, const FString &inFuncName);

	void AddListener(const FString &inEventSignature, UObject *inCaller, FWVEventDelegate_One inDelegateOne);

	void AddListener(EWVEventCategory inCategory, EWVEventName inEventName, UObject *inCaller, const FString &inFuncName);

	void AddListener(EWVEventCategory inCategory, EWVEventName inEventName, UObject *inCaller, FWVEventDelegate_One inDelegateOne);
	
	void RemoveListener(const FString &inEventSignature, UObject *inCaller);

	void _RemoveListener(const FString &inEventSignature, UObject *inCaller);

	void RemoveAllListener(UObject *inCaller);

	void RegistEvent(const FString &inEventSignature, UObject *inCaller);

	void UnRegistEvent(const FString &inEventSignature, UObject *inCaller);

	void UnRegistAllEvent(UObject *inCaller);

	void FireEvent(const FString &inEventSignature);

	void FireEvent(const FString &inEventSignature, bool bVal);

	void FireEvent(const FString &inEventSignature, int32 num);

	void FireEvent(const FString &inEventSignature, float num);

	void FireEvent(const FString &inEventSignature, FString str);

	void FireEvent(const FString &inEventSignature, void *obj);

	void FireEvent_BP(const FString &inEventSignature, FWVEventParams_BP &params_bp);

protected:
	void Init();

protected:
	UWVEventDispatcher();
	~UWVEventDispatcher();
};
