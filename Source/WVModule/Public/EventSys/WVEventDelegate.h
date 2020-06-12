// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WVEventDelegate.generated.h"

/**
 * 
 */

//Delegate要传参要用的结构
USTRUCT(BlueprintType)
struct WVMODULE_API FWVEventDelegateParams_One
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	UProperty* dataInfoPtr;

	void* dataPtr;
};

//从BP接受参数用
struct WVMODULE_API FWVEventParams_BP
{
	UProperty *propertyInfoPtr;
	void *propertyPtr;
};

DECLARE_DYNAMIC_DELEGATE_OneParam(FWVEventDelegate_One, const FWVEventDelegateParams_One&, obj);
