﻿#pragma once

#include "CoreMinimal.h"
#include "InputTypes.generated.h"

UENUM(BlueprintType)
enum class EInputKeyState : uint8
{
	None		UMETA(DisplayName = "None"),
	Pressed		UMETA(DisplayName = "Pressed"),
	Held		UMETA(DisplayName = "Held"),
	Released	UMETA(DisplayName = "Released"),
};

USTRUCT()
struct FInputInfo
{
	GENERATED_USTRUCT_BODY()
public:
	FInputInfo():
	IsValid(false),
	KeyState(EInputKeyState::None),
	StartTime(0.0f),
	LastTime(0.0f),
	AxisValue(0.0f)
	{}
	bool IsValid;
	EInputKeyState KeyState;
	float StartTime;
	float LastTime;
	FVector AxisValue;
};