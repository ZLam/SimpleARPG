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
	// virtual void NativeOnInitialized() override;
	// virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};
