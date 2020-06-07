// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ActionCharacter.generated.h"

class UAnimMontage;

UCLASS()
class SIMPLEARPG_API AActionCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	bool bShowDebug_Direction;

	UPROPERTY(EditAnywhere)
	bool bShowDebug_Velocity;

	UPROPERTY(EditAnywhere)
	bool bShowDebug_LastMovementInputVector;
	
	UFUNCTION(BlueprintPure)
	FORCEINLINE bool IsReadyAtk() { return _bReadyAtk; }
	
	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetRunSpeed() { return _RunSpeed; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetSprintSpeed() { return _SprintSpeed; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetSprintCostPerSecond() { return _SprintCostPerSecond; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE bool IsSprinting() { return _bSprinting; }

	UFUNCTION(BlueprintCallable)
	void SetSprint(bool bVal);

	UFUNCTION(BlueprintCallable)
	void Dodge();

	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetCurPower() { return _CurPower; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetMaxPower() { return _MaxPower; }

protected:
	UPROPERTY(VisibleAnywhere)
	bool _bReadyAtk;

	UPROPERTY(VisibleAnywhere)
	float _AtkRange;

	UPROPERTY(EditAnywhere)
	FName _CharactorName;

	UPROPERTY(VisibleAnywhere)
	float _RunSpeed;

	UPROPERTY(VisibleAnywhere)
	float _SprintSpeed;

	UPROPERTY(VisibleAnywhere)
	float _SprintCostPerSecond;

	UPROPERTY(VisibleAnywhere)
	bool _bSprinting;

	UPROPERTY(VisibleAnywhere)
	float _MaxPower;

	UPROPERTY(VisibleAnywhere)
	float _CurPower;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* _AnimMontage_Dodge;

	void ShowDebug_Direction();

	void ShowDebug_Velocity();

	void ShowDebug_LastMovementInputVector();

public:
	// Sets default values for this character's properties
	AActionCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
