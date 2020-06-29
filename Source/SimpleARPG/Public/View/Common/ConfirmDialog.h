// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "View/WVViewBase.h"
#include "WVModule/Public/WVGameTypes.h"
#include "ConfirmDialog.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLEARPG_API UConfirmDialog : public UWVViewBase
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FWVCallback_Null Callback_Confirm;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Confirm();
	
	UFUNCTION(BlueprintNativeEvent)
	void SetDesc(const FText &txt);
};
