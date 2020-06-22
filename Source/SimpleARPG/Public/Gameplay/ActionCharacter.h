// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "WVModule/Public/WVGameTypes.h"
#include "ActionCharacter.generated.h"

class UAnimMontage;
class UComboMachineComp;
class AEquipment;

USTRUCT()
struct FCharacterEquipInfo
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditDefaultsOnly)
	FName SocketName;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AEquipment> Class_Equip;
	UPROPERTY(EditDefaultsOnly)
	FName EquipName;
};

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
	FORCEINLINE FName GetActionCharacterName() { return _CharactorName; }
	
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

	/**
	 * 滚动的时候处理变小capsule，滚动完后恢复capsule正常size。
	 * 既然要变小capsule，那么mesh也要偏移位置的。所以主要要修改2个值
	 * 一开始用直接设置值来做，发现表现上会闪了一下，效果很不好
	 * 然后换了插值的方式做，基本看不到闪了，效果还行
	 * 以后看看会不会有其他的方式做的
	 */
	UFUNCTION(BlueprintCallable)
	void HandleAnimNotify_DodgeChangeColliderBegin();

	UFUNCTION(BlueprintCallable)
	void HandleAnimNotify_DodgeChangeColliderEnd();

	UFUNCTION(BlueprintPure)
	UComboMachineComp* GetComboMachineComp() { return _Comp_ComboMachine; }

	UFUNCTION(BlueprintPure)
	bool IsLockDodge() { return _bLockDodge; }

	UFUNCTION(BlueprintCallable)
	void SetLockDodge(bool val) { _bLockDodge = val; }

	UFUNCTION(BlueprintPure)
	EWVActionCharacterState GetState() { return _State; }

	UFUNCTION(BlueprintPure)
	AEquipment* GetEquipment(const FName &InEquipName) { return _EquipMap.FindRef(InEquipName); }

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

protected:
	UPROPERTY(VisibleAnywhere)
	EWVActionCharacterState _State;
	
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

	UPROPERTY(VisibleAnywhere)
	float _RecoverPowerTime;

	UPROPERTY(VisibleAnywhere)
	FVector _OriginMeshRelativePos;

	UPROPERTY(VisibleAnywhere)
	float _DodgeCapsuleHalfHeight;

	UPROPERTY(VisibleAnywhere)
	bool _bDodging;

	UPROPERTY(VisibleAnywhere)
	bool _bLockDodge;

	UPROPERTY(VisibleAnywhere)
	float _MaxHP;

	UPROPERTY(VisibleAnywhere)
	float _CurHP;

	UPROPERTY(VisibleAnywhere)
	float _MaxStraight;

	UPROPERTY(VisibleAnywhere)
	float _CurStraight;

	UPROPERTY(VisibleAnywhere)
	float _MaxDown;

	UPROPERTY(VisibleAnywhere)
	float _CurDown;

	UPROPERTY(VisibleAnywhere)
	bool _bSuperArmor;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* _AnimMontage_Dodge;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* _AnimMontage_Straight_Back_F;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* _AnimMontage_Straight_Back_B;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* _AnimMontage_Straight_Back_L;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* _AnimMontage_Straight_Back_R;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* _AnimMontage_Straight_Bounce_F;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* _AnimMontage_Straight_Bounce_B;

	UPROPERTY(EditAnywhere)
	UComboMachineComp *_Comp_ComboMachine;

	UPROPERTY(EditDefaultsOnly)
	TArray<FCharacterEquipInfo> _EquipInfos;

	UPROPERTY(VisibleAnywhere)
	TMap<FName, AEquipment*> _EquipMap;

	UPROPERTY()
	float _HurtedRotAngle;

	bool _bDodgeChangeColliderBegin;

	bool _bDodgeChangeColliderEnd;

	FTimerHandle _Timer_RecoverPower;

	/**
	 * OutDir : 1（大致背击），2（大致正面击），3（大致侧面左击），4（大致侧面右击）
	 */
	UFUNCTION()
	void _HandleHurtedRot(AActionCharacter *Attacker);

	UFUNCTION()
	void _HandleStraight(AActionCharacter *Attacker, EWVStraightType curStraightType, const FWVStraightData &StraightData);

	UFUNCTION()
	void Die();

	UFUNCTION()
	void Callback_ComboMachine_Start();

	UFUNCTION()
	void Callback_ComboMachine_Resume();

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
