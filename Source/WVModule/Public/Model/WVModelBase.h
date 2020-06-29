// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "WVModelBase.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class WVMODULE_API UWVModelBase : public UObject
{
	GENERATED_BODY()

public:
	UWVModelBase();
	~UWVModelBase();
	
	virtual void OnEnter();
	virtual void OnExit();
};
