// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "View/WVViewBase.h"
#include "WVModule/Public/WVGameTypes.h"
#include "ChooseNumDialog.generated.h"

class USlider;

/**
 * 
 */
UCLASS()
class SIMPLEARPG_API UChooseNumDialog : public UWVViewBase
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FWVCallback_Float Callback_Confirm;

	UFUNCTION(BlueprintNativeEvent)
	void SetMin(float num);

	UFUNCTION(BlueprintNativeEvent)
	void SetMax(float num);

	UFUNCTION(BlueprintNativeEvent)
	void SetCur(float num);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Confirm();

	virtual void NativeOnInitialized() override;

protected:
	UPROPERTY()
	USlider *_Slider_Num;
};
