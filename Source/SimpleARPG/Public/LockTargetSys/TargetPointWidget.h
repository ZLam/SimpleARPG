// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TargetPointWidget.generated.h"

UENUM()
enum class ETargetPointType : uint8
{
	None		UMETA(DisplayName = "None"),
	Green		UMETA(DisplayName = "Green"),
	Red			UMETA(DisplayName = "Red"),
	Yellow		UMETA(DisplayName = "Yellow"),
};

/**
 * 
 */
UCLASS()
class SIMPLEARPG_API UTargetPointWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void Show(ETargetPointType type);

protected:
	UPROPERTY()
	ETargetPointType _curType;
	
	UPROPERTY()
	UWidget *_red;

	UPROPERTY()
	UWidget *_green;

	UPROPERTY()
	UWidget *_yellow;
	
	virtual void NativeOnInitialized() override;
};
