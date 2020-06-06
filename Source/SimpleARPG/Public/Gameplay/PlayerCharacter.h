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
	UFUNCTION(BlueprintCallable)
	void ScanTargets(const TArray<FName> &tagNames);

	UFUNCTION(BlueprintCallable)
	void CleanupTargets();

	UFUNCTION(BlueprintCallable)
	void LockTarget();

	UFUNCTION(BlueprintCallable)
	void UnlockTarget();

protected:
	//---property---

	UPROPERTY(VisibleAnywhere)
	TArray<AActor*> _Targets;

	UPROPERTY(VisibleAnywhere)
	int32 _Index_Target;

	UPROPERTY(EditAnywhere)
	USpringArmComponent *_Comp_SpringArm;

	UPROPERTY(EditAnywhere)
	UCameraComponent *_Comp_Cam;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ULockTargetComp *_Comp_LockTarget;

	//---func---
	
	UFUNCTION()
	void LockTarget_Callback_Distance(float dist);

public:
	// Sets default values for this character's properties
	APlayerCharacter();
};
