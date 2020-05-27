// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/Engine.h"
#include "WVViewCreatorBase.generated.h"

/**
 * 
 */
UCLASS()
class WVMODULE_API UWVViewCreatorBase : public UObject
{
	GENERATED_BODY()

protected:
	UPROPERTY()
		UGameInstance *_gameIns;

public:
	UWVViewCreatorBase();
	~UWVViewCreatorBase();

	void SetupGameInstance(UGameInstance *ins)
	{
		_gameIns = ins;
	}
	
	virtual void OnEnter();
	virtual void OnExit();
};
