// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Model/WVModelBase.h"
#include "Model_Player.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLEARPG_API UModel_Player : public UWVModelBase
{
	GENERATED_BODY()

public:
	virtual void OnEnter() override;

	UFUNCTION()
	void Rec_Test();
};
