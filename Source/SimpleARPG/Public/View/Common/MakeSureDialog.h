// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "View/WVViewBase.h"
#include "WVModule/Public/WVGameTypes.h"
#include "MakeSureDialog.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLEARPG_API UMakeSureDialog : public UWVViewBase
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FWVCallback_Bool Callback_IsOk;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Confirm();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Cancel();

	UFUNCTION(BlueprintNativeEvent)
	void SetDesc(const FText &txt);

	UFUNCTION(BlueprintNativeEvent)
	void SetOk(const FText &txt);

	UFUNCTION(BlueprintNativeEvent)
	void SetCancel(const FText &txt);
};
