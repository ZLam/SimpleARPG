// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WVEventDelegate.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct WVMODULE_API FWVEventDelegateParams_One
{
	GENERATED_USTRUCT_BODY()

		UPROPERTY()
		UProperty* data;

	void* dataPtr;
};

DECLARE_DYNAMIC_DELEGATE_OneParam(FWVEventDelegate_One, const FWVEventDelegateParams_One&, obj);
