// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WVViewBase.generated.h"

/**
 * 
 */
UCLASS()
class WVMODULE_API UWVViewBase : public UUserWidget
{
	GENERATED_BODY()

protected:
	
	/**
	 * 构造的时候好似不是马上调NativeConstruct，所以用回NativeOnInitialized，该方法在构造时马上调
	 * 如果在NativeConstruct里初始化值，那么最好不要在构造后马上使用，因为可能未调NativeConstruct，值都是空的
	 * 可以用IsConstructed()判断是否调了NativeConstruct
	 */
	
	virtual void NativeOnInitialized() override;
	
	virtual void NativeDestruct() override;
	
	// virtual void NativePreConstruct() override;
	// virtual void NativeConstruct() override;
	// virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};
