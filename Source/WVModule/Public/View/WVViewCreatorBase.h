// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "WVViewCreatorBase.generated.h"

/**
 * 
 */
UCLASS()
class WVMODULE_API UWVViewCreatorBase : public UObject
{
	GENERATED_BODY()

public:
	UWVViewCreatorBase();
	~UWVViewCreatorBase();
	
	virtual void OnEnter();
	virtual void OnExit();
};
