// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Model/WVModelBase.h"
#include "Model_Item.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLEARPG_API UModel_Item : public UWVModelBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void AddItem(int32 ItemId, int32 Num);

	UFUNCTION(BlueprintCallable)
	int32 GetItemNum(int32 ItemId);

	UFUNCTION(BlueprintCallable)
	TArray<int32> GetItemsForShow();

protected:
	UPROPERTY()
	TMap<int32, int32> _ItemMap;

public:
	UModel_Item();

	virtual void OnEnter() override;
};
