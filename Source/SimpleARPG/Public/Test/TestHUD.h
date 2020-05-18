// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TestHUD.generated.h"

class UTestUserWidget;

/**
 * 
 */
UCLASS()
class SIMPLEARPG_API ATestHUD : public AHUD
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UTestUserWidget> _mainViewClass;
	UPROPERTY()
	UTestUserWidget* _mainView;

protected:
	virtual void BeginPlay() override;
};
