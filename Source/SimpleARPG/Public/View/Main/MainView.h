// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "View/WVViewBase.h"
#include "MainView.generated.h"

class UTextBlock;

/**
 * 
 */
UCLASS()
class SIMPLEARPG_API UMainView : public UWVViewBase
{
	GENERATED_BODY()
	
protected:
	UTextBlock *_Txt_ComboKeys;
	
	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void Rec_ComboStepExecute(TArray<FKey> &InComboMatchKeys);

	UFUNCTION()
	void Rec_ComboResumeExecute();
};
