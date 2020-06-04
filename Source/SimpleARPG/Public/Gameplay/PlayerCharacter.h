// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/ActionCharacter.h"
#include "PlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class ULockTargetComp;

/**
 * 
 */
UCLASS()
class SIMPLEARPG_API APlayerCharacter : public AActionCharacter
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TArray<AActor*> Targets;

	int32 Index_Target = -1;
	
	UFUNCTION(BlueprintCallable)
	void TestLockTarget();

	UFUNCTION()
	void TestLockTarget_Callback_Distance(float dist);

protected:
	UPROPERTY(VisibleAnywhere)
	bool _bReadyAtk;

	UPROPERTY(EditAnywhere)
	USpringArmComponent *_Comp_SpringArm;

	UPROPERTY(EditAnywhere)
	UCameraComponent *_Comp_Cam;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ULockTargetComp *_Comp_LockTarget;

public:
	// Sets default values for this character's properties
	APlayerCharacter();
};
