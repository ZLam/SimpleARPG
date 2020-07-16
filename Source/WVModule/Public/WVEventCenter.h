#pragma once

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
	Begin							UMETA(DisplayName = "None"),

	//Normal Inner Event Begin
	PlayerPowerChange				UMETA(DisplayName = "PlayerPowerChange"),
	PlayerHPChange					UMETA(DisplayName = "PlayerHPChange"),
	MoneyChange						UMETA(DisplayName = "MoneyChange"),
	ComboStepExecute				UMETA(DisplayName = "ComboStepExecute"),		//暂时拿来看看按键
	ComboResumeExecute				UMETA(DisplayName = "ComboResumeExecute"),		//暂时拿来看看按键
	//Normal Inner Event End

	//Create View Event Begin
	CreateLoginView					UMETA(DisplayName = "CreateLoginView"),
	CreateMainView					UMETA(DisplayName = "CreateMainView"),
	CreateShopView					UMETA(DisplayName = "CreateShopView"),
	CreateMakeSureDialog			UMETA(DisplayName = "CreateMakeSureDialog"),
	CreateConfirmDialog				UMETA(DisplayName = "CreateConfirmDialog"),
	CreateChooseNumDialog			UMETA(DisplayName = "CreateChooseNumDialog"),
	CreateItemView					UMETA(DisplayName = "CreateItemView"),
	CreateChooseLevelView			UMETA(DisplayName = "CreateChooseLevelView"),
	//Create View Event End





	
	//for test begin
	TestActor						UMETA(DisplayName = "TestActor"),
	TestUI							UMETA(DisplayName = "TestUI"),
	CreateTestView					UMETA(DisplayName = "CreateTestView"),
	//for test end

	End								UMETA(DisplayName = "None"),
};
