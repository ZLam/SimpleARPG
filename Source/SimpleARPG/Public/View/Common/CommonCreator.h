// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "View/WVViewCreatorBase.h"
#include "WVModule/Public/WVGameTypes.h"
#include "CommonCreator.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLEARPG_API UCommonCreator : public UWVViewCreatorBase
{
	GENERATED_BODY()
	
public:
	virtual void OnEnter() override;

	UFUNCTION()
	void Rec_OnCreateMakeSureDialog(const FWVParams_CreateMakeSureDialog &Params);

	UFUNCTION()
	void Rec_OnCreateConfirmDialog(const FWVParams_CreateConfirmDialog &Params);

	UFUNCTION()
	void Rec_OnCreateChooseNumDialog(const FWVParams_CreateChooseNumDialog &Params);
};
