// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EventSys/WVEventDelegate.h"
#include "WVEventListener.generated.h"

UENUM()
enum class EWVEventListenerValid : uint8
{
	None,
	FuncName,
	Delegate,
	Fatal,
};

/**
 * 
 */
USTRUCT()
struct WVMODULE_API FWVEventListener
{
	GENERATED_USTRUCT_BODY()

public:
	UObject* caller;
	FString funcName;
	EWVEventListenerValid validType;
	UFunction* func;
	FWVEventDelegate_One delegateOne;

	FWVEventListener() = default;
	FWVEventListener(UObject* inCaller, const FString &inFuncName);
	FWVEventListener(UObject* inCaller, FWVEventDelegate_One inDelegateOne);
	~FWVEventListener();

	EWVEventListenerValid IsReady();
};
