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
	UFUNCTION(BlueprintCallable)
	int32 GetMoney();

	UFUNCTION(BlueprintCallable)
	void SetMoney(int32 num, bool bFireEvent = true);
	
protected:
	int32 _MoneyNum;

public:
	UModel_Player();

	virtual void OnEnter() override;
};
