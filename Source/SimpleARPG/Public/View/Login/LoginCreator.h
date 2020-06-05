// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "View/WVViewCreatorBase.h"
#include "LoginCreator.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLEARPG_API ULoginCreator : public UWVViewCreatorBase
{
	GENERATED_BODY()

public:
	virtual void OnEnter() override;

	UFUNCTION()
	void Rec_OnCreateLoginView();
};
