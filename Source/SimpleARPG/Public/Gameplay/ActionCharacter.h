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

	// UFUNCTION(BlueprintPure)
	// FORCEINLINE bool IsRunning() { return FMath::Abs(GetVelocity().Size() - _RunSpeed) < 5; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE bool IsSprinting() { return _bSprinting; }

	UFUNCTION(BlueprintCallable)
	void SetSprint(bool bVal);

	UFUNCTION(BlueprintPure)
	FORCEINLINE bool IsDodging() { return _bDodging; }

	UFUNCTION(BlueprintCallable)
	void Dodge();

	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetCurPower() { return _CurPower; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetMaxPower() { return _MaxPower; }

	UFUNCTION(BlueprintCallable)
	void RecoverPower();

	UFUNCTION(BlueprintCallable)
	void HandleAnimNotify_DodgeEnd();

	UFUNCTION(BlueprintCallable)
	void HandleAnimNotify_DodgeChangeColliderBegin();

	UFUNCTION(BlueprintCallable)
	void HandleAnimNotify_DodgeChangeColliderEnd();

	bool bBegin;
	bool bEnd;

protected:
	UPROPERTY(VisibleAnywhere)
	bool _bReadyAtk;

	UPROPERTY(VisibleAnywhere)
	float _AtkRange;

	UPROPERTY(EditDefaultsOnly)
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

	UPROPERTY(VisibleAnywhere)
	float _DodgeCost;

	UPROPERTY(EditDefaultsOnly)
	float _RecoverPowerTime;

	UPROPERTY(VisibleAnywhere)
	FVector _OriginMeshRelativePos;

	UPROPERTY(VisibleAnywhere)
	float _DodgeCapsuleHalfHeight;

	UPROPERTY(VisibleAnywhere)
	bool _bDodging;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* _AnimMontage_Dodge;

	FTimerHandle _Timer_RecoverPower;

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
