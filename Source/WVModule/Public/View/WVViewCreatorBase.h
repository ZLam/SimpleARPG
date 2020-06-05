// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/Engine.h"
#include "WVViewCreatorBase.generated.h"

class UWVGameInstance;

/**
 * 
 */
UCLASS()
class WVMODULE_API UWVViewCreatorBase : public UObject
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	UWVGameInstance *_gameIns;

public:
	UWVViewCreatorBase();
	~UWVViewCreatorBase();

	void SetupGameInstance(UWVGameInstance *ins)
	{
		_gameIns = ins;
	}
	
	virtual void OnEnter();
	virtual void OnExit();
};
