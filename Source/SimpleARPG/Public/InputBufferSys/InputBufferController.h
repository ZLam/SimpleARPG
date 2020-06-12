﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputBufferController.generated.h"

class UInputBufferComp;

/**
 * 
 */
UCLASS()
class SIMPLEARPG_API AInputBufferController : public APlayerController
{
	GENERATED_BODY()

public:
	AInputBufferController();

	virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;

	UFUNCTION(BlueprintNativeEvent)
	void HandlePostInput();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputBufferComp *Comp_InputBuffer;
};