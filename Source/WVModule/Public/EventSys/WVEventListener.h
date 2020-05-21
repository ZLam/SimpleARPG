// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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

	FWVEventListener() = default;
	FWVEventListener(UObject* inCaller, const FString &inFuncName);
	~FWVEventListener();

	EWVEventListenerValid IsValid();
};
