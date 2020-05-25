// Fill out your copyright notice in the Description page of Project Settings.


#include "EventSys/WVEventListener.h"
#include "WVBlueprintFunctionLibrary.h"
#include "Logger/WVLog.h"

FWVEventListener::FWVEventListener(UObject* inCaller, const FString& inFuncName):
caller(inCaller),
funcName(inFuncName),
validType(EWVEventListenerValid::None),
func(nullptr),
delegateOne()
{
	UE_LOG(LogTemp, Display, TEXT("===FWVEventListener::FWVEventListener==="))
}

FWVEventListener::FWVEventListener(UObject* inCaller, FWVEventDelegate_One inDelegateOne):
caller(inCaller),
funcName(),
validType(EWVEventListenerValid::None),
func(nullptr),
delegateOne(inDelegateOne)
{
	UE_LOG(LogTemp, Display, TEXT("===FWVEventListener::FWVEventListener==="))
}

FWVEventListener::~FWVEventListener()
{
	UE_LOG(LogTemp, Display, TEXT("===FWVEventListener::~FWVEventListener==="))
}

EWVEventListenerValid FWVEventListener::IsReady()
{
	if (!IsValid(caller))
	{
		validType = EWVEventListenerValid::Fatal;
		return validType;
	}
	
	if (validType == EWVEventListenerValid::None)
	{
		if (delegateOne.IsBound())
		{
			validType = EWVEventListenerValid::Delegate;
		}
		else if (!funcName.IsEmpty())
		{
			func = caller->FindFunction(FName(*funcName));
			if (func)
			{
				validType = EWVEventListenerValid::FuncName;
			}
		}
	}
	
	return  validType;
}

bool FWVEventListener::IsTypeFuncParamsFirst(const FString& strType)
{
	bool ret = false;
	if (IsReady() == EWVEventListenerValid::FuncName)
	{
		TArray<UProperty*> arr;
		UWVBlueprintFunctionLibrary::FuncParamsOfProperties(func, arr);
		if (arr.Num() > 0)
		{
			ret = arr[0]->GetCPPType() == strType;
		}
	}
	if (!ret)
	{
		WVLogW(TEXT("listener func'%s' params dont match '%s'"), *(func->GetName()), *(strType))
	}
	return ret;
}

bool FWVEventListener::IsClassFuncParamsFirst(UClass* cls)
{
	bool ret = false;
	if (IsReady() == EWVEventListenerValid::FuncName)
	{
		TArray<UProperty*> arr;
		UWVBlueprintFunctionLibrary::FuncParamsOfProperties(func, arr);
		if (arr.Num() > 0)
		{
			ret = arr[0]->GetClass()->IsChildOf(cls);
			// ret = arr[0]->StaticClass()->IsChildOf(cls);
		}
	}
	if (!ret)
	{
		WVLogW(TEXT("listener func'%s' params dont match '%s'"), *(func->GetName()), *(cls->GetName()))
	}
	return ret;
}
