// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WVModule/Public/Gameplay/WVCharacter.h"
#include "WVModule/Public/WVGameTypes.h"
#include "ActionCharacter.generated.h"

class UAnimMontage;
class UComboMachineComp;
class AEquipment;

UENUM(BlueprintType)
enum class ECharacterRotationMode : uint8
{
	FollowCtrlYaw		UMETA(DisplayName = "FollowCtrlYaw"),
	OrientByMove		UMETA(DisplayName = "OrientByMove"),
};

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
class SIMPLEARPG_API AActionCharacter : public AWVCharacter
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	bool bShowDebug_Direction;

	UPROPERTY(EditAnywhere)
	bool bShowDebug_Velocity;

	UPROPERTY(EditAnywhere)
	bool bShowDebug_LastMovementInputVector;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool IsAI;
	
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

	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetCurHP() { return _CurHP; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetMaxHP() { return _MaxHP; }

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

	UFUNCTION(BlueprintCallable)
	void HandleAnimNotify_DieEnd();

	UFUNCTION(BlueprintPure)
	UComboMachineComp* GetComboMachineComp() { return _Comp_ComboMachine; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE bool IsLockDodge() { return _bLockDodge; }

	UFUNCTION(BlueprintCallable)
	void SetLockDodge(bool val) { _bLockDodge = val; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE bool IsLockMove() { return _bLockMove; }

	UFUNCTION(BlueprintCallable)
	void SetLockMove(bool val) { _bLockMove = val; }

	UFUNCTION(BlueprintPure)
	EWVActionCharacterState GetState() { return _State; }

	UFUNCTION(BlueprintCallable)
	void SetState(EWVActionCharacterState InState);

	UFUNCTION(BlueprintPure)
	AEquipment* GetEquipment(const FName &InEquipName) { return _EquipMap.FindRef(InEquipName); }

	UFUNCTION(BlueprintCallable)
	void ResumeStraight();

	UFUNCTION(BlueprintPure)
	FVector GetLastMovementInputVector_EX();

	UFUNCTION(BlueprintPure)
	FORCEINLINE bool IsStrafe() { return _bStrafe; }

	UFUNCTION(BlueprintCallable)
	void SetStrafe(bool val) { _bStrafe = val; }

	UFUNCTION(BlueprintCallable)
	void SetRotationMode(ECharacterRotationMode InMode);

	/**
	 * 暂时这样处理，为什么呢，因为AI果边要SetFocus，而敌人是有情况是使用controller的角度的，比如敌人设左bUseControllerRotationYaw = true，
	 * 然后又需要SetFocus某个Actor。那么当敌人受到攻击时，不能直接设敌人Actor的角度，而应该设controller的角度。因为当前敌人设了使用controller的
	 * 角度！！！
	 */
	UFUNCTION(BlueprintCallable)
	void SetRotation_EX(const FRotator &InRotation);

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
	bool _bLockMove;

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

	UPROPERTY(VisibleAnywhere)
	bool _bStrafe;

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

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* _AnimMontage_Die;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UComboMachineComp *_Comp_ComboMachine;

	UPROPERTY(EditDefaultsOnly)
	TArray<FCharacterEquipInfo> _EquipInfos;

	UPROPERTY(VisibleAnywhere)
	TMap<FName, AEquipment*> _EquipMap;

	UPROPERTY(EditDefaultsOnly)
	float _DownResumeTime;

	UPROPERTY(EditDefaultsOnly)
	float _DieCountDownTime;

	UPROPERTY()
	float _HurtedRotAngle;

	bool _bDodgeChangeColliderBegin;

	bool _bDodgeChangeColliderEnd;

	FTimerHandle _Timer_RecoverPower;

	FTimerHandle _Timer_ResumeDown;

	FTimerHandle _Timer_DieCountdown;

	TSharedPtr<FRotator> _ToHurtedRot;

	TSharedPtr<FVector> _ToHurtedOffset;

	AController *_TmpEventInstigatorForDie;

	AActor *_TmpDamageCauserForDie;

	/**
	 * 硬直状态，硬直的表现会有后退，弹开，击飞等等
	 * Down状态（特殊的硬直），意在不能一直被连击并拉开距离，只会有弹开，击飞等等之类的表现
	 */
	UFUNCTION()
	void _HandleStraight(AActionCharacter *Attacker, EWVStraightType curStraightType, const FWVStraightData &StraightData);

	UFUNCTION()
	void Die(AController* EventInstigator, AActor* DamageCauser);

	virtual void _KillSomeone(AActor* InSomeone);

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
