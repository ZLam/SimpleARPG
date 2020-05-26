// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
// #include "ConfigUtil/WVConfigUtil.h"
// #include "WVEventCenter.h"
#include "EventSys/WVEventDelegate.h"
// #include "EventSys/WVEventDispatcher.h"
#include "WVBlueprintFunctionLibrary.generated.h"

enum class EWVConfigName : uint8;
enum class EWVEventCategory : uint8;
enum class EWVEventName : uint8;
class UDataTable;

/**
 * 
 */
UCLASS()
class WVMODULE_API UWVBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/**
	 * 把参数的UProperty取出来
	 */
	static void FuncParamsOfProperties(UFunction *inFunc, TArray<UProperty*> &outArr);

	UFUNCTION(BlueprintCallable, meta = (AdvancedDisplay = "1"))
	static void LogI(const FString &strLog, bool bPrintToLog = true, bool bPrintToScreen = true, float duration = 10);

	UFUNCTION(BlueprintCallable, meta = (AdvancedDisplay = "1"))
	static void LogW(const FString &strLog, bool bPrintToLog = true, bool bPrintToScreen = true, float duration = 10);

	UFUNCTION(BlueprintCallable, meta = (AdvancedDisplay = "1"))
	static void LogF(const FString &strLog, bool bPrintToLog = true, bool bPrintToScreen = true, float duration = 10);

	UFUNCTION(BlueprintCallable, meta = (AdvancedDisplay = "1"))
	static void LogE(const FString &strLog, bool bPrintToLog = true, bool bPrintToScreen = true, float duration = 10);

	UFUNCTION(BlueprintCallable)
	static UDataTable* GetConfig(EWVConfigName configName);

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

	UFUNCTION(BlueprintCallable, CustomThunk, meta = (CustomStructureParam = "params"))
	static void FireEvent_OneParams_SP(EWVEventCategory inCategory, EWVEventName inEventName, UProperty *params = nullptr);

	static void _FireEvent_OneParams(const FString& inEventSignature, FWVEventParams_BP &params);

	static void _FireEvent_OneParams_SP(EWVEventCategory inCategory, EWVEventName inName, FWVEventParams_BP &params);

	UFUNCTION(BlueprintPure)
	static void ConvEventOneParamsToInt32(const FWVEventDelegateParams_One &delegateParams, int32 &out);

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
		FString eventSignature;

		/**
		 * 把数据取出来
		 * 好似可以通过2种方式
		 *		1：执行字节码（堆栈环境里执行一次，然后有数据就返回出嚟？字节码里有操作也有数据？）
		 *		2：PropertyChainForCompiledIn属性里有数据
		 */
		Stack.StepCompiledIn<UStrProperty>(&eventSignature);

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
		_FireEvent_OneParams(eventSignature, params_bp);
		P_NATIVE_END;
	}

	DECLARE_FUNCTION(execFireEvent_OneParams_SP)
	{
		Stack.MostRecentProperty = nullptr;

		FWVEventParams_BP params_bp;
		EWVEventCategory tCategory;
		EWVEventName tName;

		Stack.StepCompiledIn<UEnumProperty>(&tCategory);
		Stack.StepCompiledIn<UEnumProperty>(&tName);

		Stack.Step(Stack.Object, nullptr);

		UProperty *tPropertyInfoPtr = Cast<UProperty>(Stack.MostRecentProperty);
		void *tPropertyPtr = Stack.MostRecentPropertyAddress;
		params_bp.propertyInfoPtr = tPropertyInfoPtr;
		params_bp.propertyPtr = tPropertyPtr;

		P_FINISH;
		P_NATIVE_BEGIN;
		_FireEvent_OneParams_SP(tCategory, tName, params_bp);
		P_NATIVE_END;
	}
};



