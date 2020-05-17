// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "WVBlueprintFunctionLibrary.generated.h"

UENUM(BlueprintType)
enum class EWVLogVerbosity : uint8
{
	Info		UMETA(DisplayName = "Info"),
	Warning		UMETA(DisplayName = "Warning"),
	Fatal		UMETA(DisplayName = "Fatal"),
	Error		UMETA(DisplayName = "Error"),
};

/**
 * 
 */
UCLASS()
class WVMODULE_API UWVBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
};
