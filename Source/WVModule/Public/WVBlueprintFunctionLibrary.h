// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "EventSys/WVEventDelegate.h"
#include "WVBlueprintFunctionLibrary.generated.h"

enum class EWVConfigName : uint8;
enum class EWVEventCategory : uint8;
enum class EWVEventName : uint8;
enum class EWVModelName : uint8;
class UDataTable;
class UWVViewBase;
class UWVModelBase;

/**
 * 
 */
UCLASS()
class WVMODULE_API UWVBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	//
	//Log Begin
	//
	
	UFUNCTION(BlueprintCallable, meta = (AdvancedDisplay = "1"))
	static void LogI(const FString &strLog, bool bPrintToLog = true, bool bPrintToScreen = true, float duration = 10);

	UFUNCTION(BlueprintCallable, meta = (AdvancedDisplay = "1"))
	static void LogW(const FString &strLog, bool bPrintToLog = true, bool bPrintToScreen = true, float duration = 10);

	UFUNCTION(BlueprintCallable, meta = (AdvancedDisplay = "1"))
	static void LogF(const FString &strLog, bool bPrintToLog = true, bool bPrintToScreen = true, float duration = 10);

	UFUNCTION(BlueprintCallable, meta = (AdvancedDisplay = "1"))
	static void LogE(const FString &strLog, bool bPrintToLog = true, bool bPrintToScreen = true, float duration = 10);

	//
	//Log End
	//

	//
	//Config Begin
	//
	
	UFUNCTION(BlueprintCallable)
	static UDataTable* GetConfig(EWVConfigName configName);

	//
	//Config End
	//

	//
	//Event Begin
	//
	
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
	static bool ConvEventOneParamsToBool(const FWVEventDelegateParams_One &delegateParams, bool &out);
	
	UFUNCTION(BlueprintPure)
	static bool ConvEventOneParamsToInt32(const FWVEventDelegateParams_One &delegateParams, int32 &out);

	UFUNCTION(BlueprintPure)
	static bool ConvEventOneParamsToFloat(const FWVEventDelegateParams_One &delegateParams, float &out);

	UFUNCTION(BlueprintPure)
	static bool ConvEventOneParamsToString(const FWVEventDelegateParams_One &delegateParams, FString &out);

	UFUNCTION(BlueprintPure)
	static bool ConvEventOneParamsToObj(const FWVEventDelegateParams_One &delegateParams, UObject * &out);

	UFUNCTION(BlueprintPure, CustomThunk, meta = (CustomStructureParam = "out"))
	static bool ConvEventOneParamsToStruct(const FWVEventDelegateParams_One &delegateParams, UProperty * &out);

	static bool _ConvEventOneParamsToStruct(FWVEventDelegateParams_One &inParams, FWVEventDelegateParams_One &outParams);

	//
	//Event End
	//

	//
	//View Begin
	//

	UFUNCTION(BlueprintCallable)
	static void PopFuncView(UWVViewBase *view = nullptr);

	UFUNCTION(BlueprintCallable)
	static void PopDialog(UWVViewBase *view = nullptr);
	
	//
	//View End
	//

	//
	//Model Begin
	//

	UFUNCTION(BlueprintCallable)
	static UWVModelBase* GetModel(EWVModelName InModelName);
	
	//
	//Model End
	//
	
	//
	//Common Begin
	//

	/**
	 * 把参数的UProperty取出来
	 */
	static void FuncParamsOfProperties(UFunction *inFunc, TArray<UProperty*> &outArr);

	UFUNCTION(BlueprintCallable)
	static float GetAngleBetween2Vector(FVector v1, FVector v2);

	UFUNCTION(BlueprintCallable)
	static int32 GetRelativeDirection(AActor *srcActor, AActor *dstActor);

	//
	//Common End
	//












	

	/**
	 * The CustomStructureParam metadata is what tells the Blueprint code generator to treat the pin as a wildcard.
	 * The CustomThunk UFUNCTION flag prevents UHT from generating the code that glues the Blueprint to the C++ function and requires you to provider your own.
	 * the UProperty only contains reflection data upon the property (its type). It does not contain the value itself!
	 *
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

	DECLARE_FUNCTION(execConvEventOneParamsToStruct)
	{
		Stack.MostRecentProperty = nullptr;

		FWVEventDelegateParams_One inParams;
		FWVEventDelegateParams_One outParams;
		bool r = false;
		
		Stack.StepCompiledIn<UStructProperty>(&inParams);
		
		Stack.Step(Stack.Object, nullptr);

		outParams.dataInfoPtr = Stack.MostRecentProperty;
		outParams.dataPtr = Stack.MostRecentPropertyAddress;

		P_FINISH;
		P_NATIVE_BEGIN;
		r = _ConvEventOneParamsToStruct(inParams, outParams);
		P_NATIVE_END;

		*(bool*)RESULT_PARAM = r;
	}
};



