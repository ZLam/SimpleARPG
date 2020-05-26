// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Test/TestStructs.h"
#include "WVModule/Public/View/WVViewBase.h"
#include "TestUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLEARPG_API UTestUserWidget : public UWVViewBase
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

private:
	FString str_testEvent;
	
	UFUNCTION()
	void TestEvent_MultiParams(int32 &n32, float &fVal, bool &bVal, FString &str, UObject* obj);

	UFUNCTION()
	void TestEvent_MultiParams2(FTestMultiParamsEvent &data);

	UFUNCTION()
	void Rec_Test();
	
	UFUNCTION()
	void Btn_Test1_Callback();

	UFUNCTION()
	void Btn_Test2_Callback();

	UFUNCTION()
	void Btn_Test3_Callback();

	UFUNCTION()
	void Btn_TestEvent1_Callback();

	UFUNCTION()
	void Btn_TestEvent2_Callback();
};
