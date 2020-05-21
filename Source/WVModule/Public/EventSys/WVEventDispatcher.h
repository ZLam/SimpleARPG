// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "WVEventCenter.h"
#include "WVEventDispatcher.generated.h"

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
	
public:
	static UWVEventDispatcher* GetInstance();
	static void Cleanup();
	
	FString GetEventSignature(EWVEventCategory category, EWVEventName eventName)
	{
		return WVEventSignature(*_categoryNames[category], *_eventNames[eventName]);
	}

	void AddListener(const FString &inEventSignature, UObject* inCaller, const FString &inFuncName);
	
	void RemoveListener(const FString &inEventSignature, UObject* inCaller);

	void FireEvent(const FString &inEventSignature);

protected:
	void Init();

protected:
	UWVEventDispatcher();
	~UWVEventDispatcher();
};
