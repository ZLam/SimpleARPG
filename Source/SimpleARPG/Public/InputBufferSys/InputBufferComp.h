// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InputCoreTypes.h"
#include "InputBufferSys/InputTypes.h"
#include "WVModule/Public/WVGameTypes.h"
#include "InputBufferComp.generated.h"

class UPlayerInput;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SIMPLEARPG_API UInputBufferComp : public UActorComponent
{
	GENERATED_BODY()

	friend class AInputBufferController;

public:
	UPROPERTY(EditDefaultsOnly)
	TArray<FKey> RegistKey;

	UFUNCTION(BlueprintCallable)
	bool Match(FKey InKey, EWVInputMatchStyle InMatchStyle, const FWVInputMatchData &InMatchParams, bool bNeedInvalid = false);

	UFUNCTION(BlueprintCallable)
	bool MatchAll(const TArray<FKey> &InKeys, const TArray<EWVInputMatchStyle> &InMatchStyles, const TArray<FWVInputMatchData> &InMatchParamsArr, bool bNeedInvalid = false);

	UFUNCTION(BlueprintCallable)
	void Invalid();

	UFUNCTION(BlueprintCallable)
	void PrintLog();

protected:
	UPROPERTY()
	TMap<FKey, FInputInfo> _CurInputInfoMap;

	void HandlePostProcessInput(const float DeltaTime, const bool bGamePaused, UPlayerInput *InPlayerInput);

public:	
	// Sets default values for this component's properties
	UInputBufferComp();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
