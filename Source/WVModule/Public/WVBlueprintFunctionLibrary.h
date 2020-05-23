// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "WVEventCenter.h"
#include "EventSys/WVEventDelegate.h"
#include "EventSys/WVEventDispatcher.h"
#include "WVBlueprintFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class WVMODULE_API UWVBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, meta = (AdvancedDisplay = "1"))
	static void LogI(const FString &strLog, bool bPrintToLog = true, bool bPrintToScreen = true, float duration = 10);

	UFUNCTION(BlueprintCallable, meta = (AdvancedDisplay = "1"))
	static void LogW(const FString &strLog, bool bPrintToLog = true, bool bPrintToScreen = true, float duration = 10);

	UFUNCTION(BlueprintCallable, meta = (AdvancedDisplay = "1"))
	static void LogF(const FString &strLog, bool bPrintToLog = true, bool bPrintToScreen = true, float duration = 10);

	UFUNCTION(BlueprintCallable, meta = (AdvancedDisplay = "1"))
	static void LogE(const FString &strLog, bool bPrintToLog = true, bool bPrintToScreen = true, float duration = 10);

	UFUNCTION(BlueprintPure)
	static FString ConvToEventSignature(EWVEventCategory inCategory, EWVEventName inEventName);

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "inCaller"))
	static void AddListener_FN(const FString &inEventSignature, UObject *inCaller, const FString &inFuncName);

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "inCaller"))
	static void AddListener_FN_SP(EWVEventCategory inCategory, EWVEventName inEventName, UObject *inCaller, const FString &inFuncName);

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "inCaller"))
	static void AddListener_DL(const FString &inEventSignature, UObject *inCaller, FWVEventDelegate_One inDelegateOne);

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "inCaller"))
	static void AddListener_DL_SP(EWVEventCategory inCategory, EWVEventName inEventName, UObject *inCaller, FWVEventDelegate_One inDelegateOne);

	UFUNCTION(BlueprintCallable)
	static void FireEvent(const FString &inEventSignature);

	UFUNCTION(BlueprintCallable)
	static void FireEvent_SP(EWVEventCategory inCategory, EWVEventName inEventName);

	UFUNCTION(BlueprintCallable, CustomThunk, meta = (CustomStructureParam = "params"))
	static void FireEvent_OneParams(const FString &inEventSignature, UProperty *params = nullptr);

	/**
	 * 呢个要搞清BP VM的大概工作原理才能理解
	 * some info:
	 * link -> https://forums.unrealengine.com/community/community-content-tools-and-tutorials/27351-tutorial-how-to-accept-wildcard-structs-in-your-ufunctions
	 * link -> https://zhuanlan.zhihu.com/p/69552168
	 */
	DECLARE_FUNCTION(execFireEvent_OneParams)
	{
		/**
		 * 目前唔知点解呢度要nullptr，系唔系一定要nullptr？？
		 * 呢个属性应该系，函数执行的堆栈环境中最近使用到的那个UProperty的地址
		 */
		Stack.MostRecentProperty = nullptr;

		FWVEventParams_BP params_bp;

		/**
		 * 把数据取出来
		 * 好似可以通过2种方式
		 *		1：执行字节码（堆栈环境里执行一次，然后有数据就返回出嚟？字节码里有操作也有数据？）
		 *		2：PropertyChainForCompiledIn属性里有数据
		 */
		Stack.StepCompiledIn<UStrProperty>(&params_bp.eventSignature);

		/**
		 * 在堆栈环境里执行一次？
		 */
		Stack.Step(Stack.Object, nullptr);

		UProperty *tPropertyInfoPtr = Cast<UProperty>(Stack.MostRecentProperty);
		void *tPropertyPtr = Stack.MostRecentPropertyAddress;
		params_bp.propertyInfoPtr = tPropertyInfoPtr;
		params_bp.propertyPtr = tPropertyPtr;

		P_FINISH;
		P_NATIVE_BEGIN;
		UWVEventDispatcher::GetInstance()->FireEvent_BP(params_bp);
		P_NATIVE_END;
	}
};



