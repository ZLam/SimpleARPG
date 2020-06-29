// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "WVModule/Public/WVGameTypes.h"
#include "MyGameHelper.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLEARPG_API UMyGameHelper : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * 获取玩法里资源数量
	 */
	UFUNCTION(BlueprintCallable)
	static FWVGameResData GetGameResNum(const FWVGameResData& InGameResData);

	/**
	 * 获取品质的颜色
	 */
	UFUNCTION(BlueprintPure)
	static FLinearColor GetQualityColor(EWVQualityType InQuality);
};
