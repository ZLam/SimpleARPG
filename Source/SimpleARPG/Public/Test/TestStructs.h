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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 num_int32;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float num_float;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bVal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString str;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UObject *obj;
};
