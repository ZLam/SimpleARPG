// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TestStructs.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FTestMultiParamsEvent
{
	GENERATED_USTRUCT_BODY()

	int32 num_int32;
	float num_float;
	bool bVal;
	FString str;
	UObject* obj;
};
