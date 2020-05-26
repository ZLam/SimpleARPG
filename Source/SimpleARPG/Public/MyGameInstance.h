// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WVGameInstance.h"
#include "MyGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLEARPG_API UMyGameInstance : public UWVGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;
};
