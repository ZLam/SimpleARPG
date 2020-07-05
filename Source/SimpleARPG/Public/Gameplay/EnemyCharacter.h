// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/ActionCharacter.h"
#include "EnemyCharacter.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLEARPG_API AEnemyCharacter : public AActionCharacter
{
	GENERATED_BODY()

public:
	AEnemyCharacter();
	
	UFUNCTION(BlueprintPure)
	AActor* GetTarget() { return _CurTarget; }

	UFUNCTION(BlueprintCallable)
	void SetTarget(AActor *InTarget) { _CurTarget = InTarget; }

protected:
	UPROPERTY(VisibleAnywhere)
	AActor *_CurTarget;
};
