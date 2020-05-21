// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TestUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLEARPG_API UTestUserWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;

private:
	UFUNCTION()
	void Btn_Test1_Callback();

	UFUNCTION()
	void Btn_Test2_Callback();

	UFUNCTION()
	void Btn_TestEvent1_Callback();
};
