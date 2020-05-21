// Fill out your copyright notice in the Description page of Project Settings.


#include "EventSys/WVEventListener.h"

FWVEventListener::FWVEventListener(UObject* inCaller, const FString& inFuncName):
caller(inCaller),
funcName(inFuncName),
validType(EWVEventListenerValid::None)
{
	UE_LOG(LogTemp, Display, TEXT("===FWVEventListener::FWVEventListener==="))
}

FWVEventListener::~FWVEventListener()
{
	UE_LOG(LogTemp, Display, TEXT("===FWVEventListener::~FWVEventListener==="))
}

EWVEventListenerValid FWVEventListener::IsValid()
{
	if (validType != EWVEventListenerValid::None)
	{
		return validType;
	}
	validType = EWVEventListenerValid::Fatal;
	if (caller && caller->IsValidLowLevel())
	{
		func = caller->FindFunction(FName(*funcName));
		if (func)
		{
			validType = EWVEventListenerValid::FuncName;
		}
	}
	return  validType;
}
