// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/ActionCharacter.h"
#include "PlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;

/**
 * 
 */
UCLASS()
class SIMPLEARPG_API APlayerCharacter : public AActionCharacter
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere)
	USpringArmComponent *_comp_springArm;
	UPROPERTY(EditAnywhere)
	UCameraComponent *_comp_cam;

public:
	// Sets default values for this character's properties
	APlayerCharacter();
};
