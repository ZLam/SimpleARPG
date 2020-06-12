// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "TestDataAsset.generated.h"

class UTexture2D;

/**
 * 
 */
UCLASS(BlueprintType)
class SIMPLEARPG_API UTestDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Num_Float;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Data_Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSoftObjectPtr<UTexture2D>> Data_AssetArr;
};
