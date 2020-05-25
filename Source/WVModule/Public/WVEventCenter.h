﻿#pragma once

#include "CoreMinimal.h"
#include "WVEventCenter.generated.h"

#define WVEventSignature(category, eventName) (FString::Printf(TEXT("%s_%s"), category, eventName))

/**
 * 事件分类（实际上使用时可以自定义分类，可以不用在这里声明，这是些通用的。一般情况也够用，不用怎么分类）
 */
UENUM(BlueprintType)
enum class EWVEventCategory : uint8
{
	Begin		UMETA(DisplayName = "None"),		//
	
	Inner		UMETA(DisplayName = "Inner"),		//内部事件
	Network		UMETA(DisplayName = "Network"),		//网络事件	@TODO
	
	End			UMETA(DisplayName = "None"),
};

/**
 * 事件名（实际上使用时可以自定义事件，可以不用在这里声明，这是些通用的。不同的是，自定义的话，可能到时要改，在BP里或cpp里要全部查出来可能比较麻烦）
 */
UENUM(BlueprintType)
enum class EWVEventName : uint8
{
	Begin			UMETA(DisplayName = "None"),
	
	//for test begin
	TestActor		UMETA(DisplayName = "TestActor"),
	TestUI			UMETA(DisplayName = "TestUI"),
	//for test end

	End				UMETA(DisplayName = "None"),
};