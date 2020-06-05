﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "View/WVViewCreatorBase.h"
#include "MainCreator.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLEARPG_API UMainCreator : public UWVViewCreatorBase
{
	GENERATED_BODY()

public:
	virtual void OnEnter() override;

	UFUNCTION()
	void Rec_OnCreateMainView();
};
