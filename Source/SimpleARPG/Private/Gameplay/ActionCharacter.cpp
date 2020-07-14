// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/ActionCharacter.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include "TimerManager.h"
#include "DrawDebugHelpers.h"
#include "WVModule/Public/EventSys/WVEventDispatcher.h"
#include "WVModule/Public/ConfigUtil/WVConfigUtil.h"
#include "WVModule/Public/Logger/WVLog.h"
#include "WVModule/Public/WVBlueprintFunctionLibrary.h"
#include "ComboSys/ComboMachineComp.h"
#include "ComboSys/ComboNode.h"
#include "Gameplay/Equipment.h"
#include "Gameplay/MyPlayerController.h"

// Sets default values
AActionCharacter::AActionCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	_State = EWVActionCharacterState::ReadyAtk;
	IsAI = false;
	
	_AtkRange = 1000.0f;
	
	_bSprinting = false;
	_bDodging = false;
	_bDodgeChangeColliderBegin = false;
	_bDodgeChangeColliderEnd = false;
	_bLockDodge = false;
	_bStrafe = false;
	_bLockMove = false;

	_bSuperArmor = false;
	_HurtedRotAngle = 90.0f;
	_DownResumeTime = 1.0f;
	_DieCountDownTime = 5.0f;

	_AnimMontage_Dodge = nullptr;
	_AnimMontage_Straight_Back_F = nullptr;
	_AnimMontage_Straight_Back_B = nullptr;
	_AnimMontage_Straight_Back_L = nullptr;
	_AnimMontage_Straight_Back_R = nullptr;
	_AnimMontage_Straight_Bounce_F = nullptr;
	_AnimMontage_Straight_Bounce_B = nullptr;

	_Comp_ComboMachine = CreateDefaultSubobject<UComboMachineComp>(TEXT("Comp_ComboMachine"));

	bShowDebug_Direction = false;
	bShowDebug_Velocity = false;
	bShowDebug_LastMovementInputVector = false;
}

// Called when the game starts or when spawned
void AActionCharacter::BeginPlay()
{
	Super::BeginPlay();

	auto cfg_locomotion = UWVConfigUtil::GetInstance()->GetConfigRowData<FWVConfig_LocomotionRow>(EWVConfigName::Locomotion, _CharactorName);
	
	if (cfg_locomotion)
	{
		_RunSpeed = cfg_locomotion->RunSpeed;
		_SprintSpeed = cfg_locomotion->SprintSpeed;
		_SprintCostPerSecond = cfg_locomotion->SprintCostPerSecond;
		_DodgeCost = cfg_locomotion->DodgeCost;
		_RecoverPowerTime = cfg_locomotion->RecoverPowerTime;
		_DodgeCapsuleHalfHeight = cfg_locomotion->DodgeCapsuleHalfHeight;

		auto comp_move = GetCharacterMovement();
		if (comp_move)
		{
			comp_move->MaxWalkSpeed = _RunSpeed;
		}
		else
		{
			WVLogW(TEXT("not found move component"))
		}
	}
	else
	{
		WVLogW(TEXT("not found locomotion cfg"))
	}

	auto cfg_attrib = UWVConfigUtil::GetInstance()->GetConfigRowData<FWVConfig_AttributeRow>(EWVConfigName::Attribute, _CharactorName);
	
	if (cfg_attrib)
	{
		_MaxPower = cfg_attrib->Power;
		_CurPower = _MaxPower;
		_MaxHP = cfg_attrib->HP;
		_CurHP = _MaxHP;
		_MaxStraight = cfg_attrib->Straight;
		_CurStraight = 0.0f;
		_MaxDown = cfg_attrib->Down;
		_CurDown = 0.0f;
	}
	else
	{
		WVLogW(TEXT("not found attrib cfg"))
	}

	_OriginMeshRelativePos = GetMesh()->GetRelativeLocation();

	for (auto &tEquipInfo : _EquipInfos)
	{
		const USkeletalMeshSocket *sock = GetMesh()->GetSocketByName(tEquipInfo.SocketName);
		
		if (!sock)
		{
			WVLogW(TEXT("create equip fail, not fount socket '%s'"), *(tEquipInfo.SocketName.ToString()))
		}

		if (_EquipMap.Contains(tEquipInfo.EquipName))
		{
			WVLogW(TEXT("create equip fail, EquipName repeat '%s'"), *(tEquipInfo.EquipName.ToString()))
		}

		FActorSpawnParameters tParams;
		tParams.Owner = this;

		auto tEquipActor = GetWorld()->SpawnActor(tEquipInfo.Class_Equip, nullptr, nullptr, tParams);
		if (tEquipActor)
		{
			sock->AttachActor(tEquipActor, GetMesh());
			_EquipMap.Add(tEquipInfo.EquipName, Cast<AEquipment>(tEquipActor));
		}
		else
		{
			WVLogW(TEXT("create equip fail, cant spawn"))
		}
	}
}

void AActionCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (_EquipMap.Num() > 0)
	{
		for (auto elem : _EquipMap)
		{
			elem.Value->Destroy();
		}
	}
	
	auto &timerMgr = GetWorldTimerManager();
	timerMgr.ClearAllTimersForObject(this);
	
	UWVEventDispatcher::GetInstance()->RemoveAllListener(this);
}

// Called every frame
void AActionCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (_bSprinting && !_bDodging)
	{
		if (_CurPower <= 0)
		{
			SetSprint(false);
		}
		else
		{
			_CurPower -= (_SprintCostPerSecond * DeltaTime);
			if (_CurPower < 0)
			{
				_CurPower = 0;
			}
		}
		UWVEventDispatcher::GetInstance()->FireEvent_SP(EWVEventCategory::Inner, EWVEventName::PlayerPowerChange);
	}

	if (_bDodgeChangeColliderBegin)
	{
		auto comp_capsule = GetCapsuleComponent();
		if (comp_capsule)
		{
			float defaultHalfHeight = GetDefaultHalfHeight();
			float curHalfHeight = comp_capsule->GetUnscaledCapsuleHalfHeight();
			FVector curMeshPos = GetMesh()->GetRelativeLocation();
			FVector toMeshPos = _OriginMeshRelativePos + FVector(0.0f, 0.0f, FMath::Abs(defaultHalfHeight - _DodgeCapsuleHalfHeight));
			auto tHalfHeight = FMath::FInterpConstantTo(curHalfHeight, _DodgeCapsuleHalfHeight, DeltaTime, 250);
			auto tMeshPos = FMath::VInterpConstantTo(curMeshPos, toMeshPos, DeltaTime, 250);

			// WVLogI(TEXT("begin : %f_%f_%f"), curHalfHeight, _DodgeCapsuleHalfHeight, tHalfHeight)

			comp_capsule->SetCapsuleHalfHeight(tHalfHeight, true);
			GetMesh()->SetRelativeLocation(tMeshPos);

			if (tHalfHeight <= _DodgeCapsuleHalfHeight)
			{
				// WVLogI(TEXT("begin done"))
				_bDodgeChangeColliderBegin = false;
			}
		}
	}
	else if (_bDodgeChangeColliderEnd)
	{
		auto comp_capsule = GetCapsuleComponent();
		if (comp_capsule)
		{
			float defaultHalfHeight = GetDefaultHalfHeight();
			float curHalfHeight = comp_capsule->GetUnscaledCapsuleHalfHeight();
			FVector curMeshPos = GetMesh()->GetRelativeLocation();
			auto tHalfHeight = FMath::FInterpConstantTo(curHalfHeight, defaultHalfHeight, DeltaTime, 250);
			auto tMeshPos = FMath::VInterpConstantTo(curMeshPos, _OriginMeshRelativePos, DeltaTime, 250);

			// WVLogI(TEXT("end : %f_%f_%f"), curHalfHeight, defaultHalfHeight, tHalfHeight)

			comp_capsule->SetCapsuleHalfHeight(tHalfHeight, true);
			GetMesh()->SetRelativeLocation(tMeshPos);

			if (tHalfHeight >= defaultHalfHeight)
			{
				// WVLogI(TEXT("end done"))
				_bDodgeChangeColliderEnd = false;
			}
		}
	}

	//应该要做个Tween 或者 像Cocos2dx的Action 之类的插件来做补间动画的功能。之后要看下！商城有插件，开源也有一个叫iTween UE4的
	//LatentActionManager 在Delay()里有个这个东东 参考下？

	if (_ToHurtedRot.IsValid())
	{
		auto nextRot = FMath::RInterpTo(GetActorRotation(), *_ToHurtedRot, DeltaTime, 20);

		SetRotation_EX(nextRot);
		
		if (nextRot.Equals(*_ToHurtedRot, 1.0f))
		{
			SetRotation_EX(*_ToHurtedRot);
			_ToHurtedRot.Reset();
		}
	}

	if (_ToHurtedOffset.IsValid())
	{
		auto nextPos = FMath::VInterpTo(GetActorLocation(), *_ToHurtedOffset, DeltaTime, 5);
		
		SetActorLocation(nextPos);
		
		if (nextPos.Equals(*_ToHurtedOffset, 1.0f))
		{
			SetActorLocation(*_ToHurtedOffset);
			_ToHurtedOffset.Reset();
		}
	}

	ShowDebug_Direction();
	ShowDebug_Velocity();
	ShowDebug_LastMovementInputVector();
}

// Called to bind functionality to input
void AActionCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AActionCharacter::SetSprint(bool bVal)
{
	if (_bLockMove)
	{
		return;
	}
	
	if (bVal == _bSprinting)
	{
		return;
	}

	if (GetVelocity().Size() <= 0)
	{
		return;
	}
	
	auto comp_move = GetCharacterMovement();

	_bSprinting = bVal;

	if (_bSprinting)
	{
		comp_move->MaxWalkSpeed = _SprintSpeed;

		auto &timerMgr = GetWorldTimerManager();
		if (timerMgr.TimerExists(_Timer_RecoverPower))
		{
			timerMgr.ClearTimer(_Timer_RecoverPower);
		}
	}
	else
	{
		comp_move->MaxWalkSpeed = _RunSpeed;

		if (_CurPower < _MaxPower)
		{
			RecoverPower();
		}
	}
}

void AActionCharacter::Dodge()
{
	if (_bLockDodge)
	{
		return;
	}
	
	if (!_AnimMontage_Dodge)
	{
		return;
	}

	if (_CurPower < _DodgeCost)
	{
		return;
	}

	// 目前lock了 先不用这个
	// if (_State == EWVActionCharacterState::Straight ||
	// 	_State == EWVActionCharacterState::Down ||
	// 	_State == EWVActionCharacterState::Die)
	// {
	// 	return;
	// }

	auto comp_move = GetMovementComponent();

	if (comp_move && comp_move->IsFalling())
	{
		return;
	}

	auto mesh = GetMesh();

	if (!mesh)
	{
		return;
	}

	auto animInst = mesh->GetAnimInstance();

	if (!animInst)
	{
		return;
	}

	if (animInst->Montage_IsPlaying(_AnimMontage_Dodge))
	{
		return;
	}

	auto &timerMgr = GetWorldTimerManager();
	_CurPower -= _DodgeCost;
	if (timerMgr.TimerExists(_Timer_RecoverPower))
	{
		timerMgr.ClearTimer(_Timer_RecoverPower);
	}

	_bDodging = true;

	auto lastInputVector = GetLastMovementInputVector_EX();

	SetRotation_EX(lastInputVector.Rotation());

	animInst->Montage_Play(_AnimMontage_Dodge, 1.5f);

	if (_Comp_ComboMachine)
	{
		if (_Comp_ComboMachine->IsRunning())
		{
			_Comp_ComboMachine->Resume();
		}
	}

	UWVEventDispatcher::GetInstance()->FireEvent_SP(EWVEventCategory::Inner, EWVEventName::PlayerPowerChange);
}

void AActionCharacter::RecoverPower()
{
	auto &timerMgr = GetWorldTimerManager();
	if (!timerMgr.TimerExists(_Timer_RecoverPower))
	{
		FTimerDelegate callback;
		callback.BindLambda(
			[this]()
			{
				_CurPower = _MaxPower;
				UWVEventDispatcher::GetInstance()->FireEvent_SP(EWVEventCategory::Inner, EWVEventName::PlayerPowerChange);
				GetWorldTimerManager().ClearTimer(_Timer_RecoverPower);
			}
		);
		timerMgr.SetTimer(_Timer_RecoverPower, callback, _RecoverPowerTime, false);
	}
}

void AActionCharacter::HandleAnimNotify_DodgeEnd()
{
	_bDodging = false;
	if (!_bSprinting)
	{
		RecoverPower();
	}
}

void AActionCharacter::HandleAnimNotify_DodgeChangeColliderBegin()
{
	_bDodgeChangeColliderBegin = true;
}

void AActionCharacter::HandleAnimNotify_DodgeChangeColliderEnd()
{
	_bDodgeChangeColliderEnd = true;
}

void AActionCharacter::HandleAnimNotify_DieEnd()
{
	auto animInst = GetMesh()->GetAnimInstance();

	animInst->bNeedsUpdate = false;
	
	auto &timerMgr = GetWorldTimerManager();

	if (!timerMgr.TimerExists(_Timer_DieCountdown))
	{
		FTimerDelegate callback;
		callback.BindLambda(
			[this]()
			{
				if (_TmpEventInstigatorForDie)
				{
					auto killer = Cast<AActionCharacter>(_TmpEventInstigatorForDie->GetCharacter());
					if (killer)
					{
						killer->_KillSomeone(this);
					}
				}
				Destroy();
			}
		);
		timerMgr.SetTimer(_Timer_DieCountdown, callback, _DieCountDownTime, false);
	}
}

void AActionCharacter::SetState(EWVActionCharacterState InState)
{
	if (_State == InState)
	{
		return;
	}

	_State = InState;

	switch (_State)
	{
		case EWVActionCharacterState::Relax:
		{
			//@TODO
			break;
		}
		case EWVActionCharacterState::ReadyAtk:
		{
			SetLockMove(false);
			SetLockDodge(false);
			break;
		}
		case EWVActionCharacterState::Atking:
		{
			SetLockMove(true);
			break;
		}
		case EWVActionCharacterState::Straight:
		{
			SetLockMove(true);
			SetLockDodge(true);
			break;
		}
		case EWVActionCharacterState::Down:
		{
			SetLockMove(true);
			SetLockDodge(true);
			break;
		}
		case EWVActionCharacterState::Die:
		{
			SetLockMove(true);
			SetLockDodge(true);
			break;
		}
		default:
		{
			WVLogW(TEXT("Unknow State %s!!!"), *(UEnum::GetDisplayValueAsText<EWVActionCharacterState>(_State).ToString()))
		}
	}
}

void AActionCharacter::ResumeStraight()
{
	if (_State == EWVActionCharacterState::Straight)
	{
		SetState(EWVActionCharacterState::ReadyAtk);
	}
	else if (_State == EWVActionCharacterState::Down)
	{
		if (_DownResumeTime <= 0)
		{
			_CurDown = 0;
			_CurStraight = 0;
			SetState(EWVActionCharacterState::ReadyAtk);
		}
		else
		{
			auto &timerMgr = GetWorldTimerManager();
			timerMgr.ClearTimer(_Timer_ResumeDown);
			FTimerDelegate callback;
			callback.BindLambda(
				[this]()
				{
					_CurDown = 0;
					_CurStraight = 0;
					SetState(EWVActionCharacterState::ReadyAtk);
					GetWorldTimerManager().ClearTimer(_Timer_ResumeDown);
				}
			);
			timerMgr.SetTimer(_Timer_ResumeDown, callback, _DownResumeTime, false);

			SetLockMove(false);		//Down起身后马上解锁lockmove，lockdodge，不受Down Delay时间影响
			SetLockDodge(false);
		}
	}
}

void AActionCharacter::SetRotationMode(ECharacterRotationMode InMode)
{
	if (InMode == ECharacterRotationMode::FollowCtrlYaw)
	{
		bUseControllerRotationYaw = true;
		GetCharacterMovement()->bOrientRotationToMovement = false;
	}
	else
	{
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
	}
}

void AActionCharacter::SetRotation_EX(const FRotator& InRotation)
{
	if (bUseControllerRotationYaw ||
		bUseControllerRotationPitch ||
		bUseControllerRotationRoll)
	{
		auto ctrl = GetController();
		ctrl->SetControlRotation(InRotation);
	}
	else
	{
		SetActorRotation(InRotation);
	}
}

float AActionCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
                                   AActor* DamageCauser)
{
	auto ret = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	//Down 无敌; Die 不处理
	if (_State == EWVActionCharacterState::Down || _State == EWVActionCharacterState::Die)
	{
		return ret;
	}

	//自己攻击中受到伤害
	if (_State == EWVActionCharacterState::Atking)
	{
		if (!_bSuperArmor)
		{
			_Comp_ComboMachine->Resume();
		}
	}

	//目前只处理内置的点攻击@TODO
	if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
	{
		AActionCharacter *Attacker = Cast<AActionCharacter>(EventInstigator->GetCharacter());

		if (Attacker)
		{
			auto comboMachine = Attacker->GetComboMachineComp();
			auto curComboNode = comboMachine->GetCurNode();

			if (curComboNode)
			{
				auto comboActionName = curComboNode->GetComboActionName();

				if (!comboActionName.IsEmpty())
				{
					//伤害处理

					auto cfg_comboAction = UWVConfigUtil::GetInstance()->GetConfigRowData<FWVConfig_ComboActionRow>(EWVConfigName::ComboAction, Attacker->GetActionCharacterName());

					if (cfg_comboAction)
					{
						auto comboActionData = cfg_comboAction->ComboActionInfoMap.Find(comboActionName);

						if (comboActionData)
						{
							_CurHP -= comboActionData->Damage;

							if (_CurHP <= 0)
							{
								//死亡

								_CurHP = 0;
								Die(EventInstigator, DamageCauser);
							}
							else
							{
								//是否霸体
								if (!_bSuperArmor)
								{
									auto curStraightType = comboActionData->StraightData.StraightType;
									_CurDown += comboActionData->StraightData.AddDown;
									_CurStraight += comboActionData->StraightData.AddStraight;
									
									if (_CurDown >= _MaxDown)
									{
										//爆Down		特殊的硬直表现
										
										if (curStraightType == EWVStraightType::Back)
										{
											//强制设为弹开
											curStraightType = EWVStraightType::Bounce;
										}

										//Down状态
										SetState(EWVActionCharacterState::Down);
									}
									else if (_CurStraight >= _MaxStraight)
									{
										//爆硬直

										// WVLogI(TEXT("%f_%f"), _CurStraight, _MaxStraight)

										_CurStraight = (int32)_CurStraight % (int32)_MaxStraight;

										// WVLogI(TEXT("%f"), _CurStraight)
										
										//硬直状态
										SetState(EWVActionCharacterState::Straight);
									}

									//处理硬直逻辑
									if (_State == EWVActionCharacterState::Straight || _State == EWVActionCharacterState::Down)
									{
										_HandleStraight(Attacker, curStraightType, comboActionData->StraightData);
									}
								}
							}

							UWVEventDispatcher::GetInstance()->FireEvent_SP(EWVEventCategory::Inner, EWVEventName::PlayerHPChange);
						}
					}
				}
			}
		}
	}

	return ret;
}

void AActionCharacter::_HandleStraight(AActionCharacter *Attacker, EWVStraightType curStraightType, const FWVStraightData &StraightData)
{	
	FVector selfForward = GetActorForwardVector();
	FVector attackerForward = Attacker->GetActorForwardVector();
	float tAngle = UWVBlueprintFunctionLibrary::GetAngleBetween2Vector(selfForward, attackerForward);
	int32 roughlyDir = 0;		//1（大致背击），2（大致正面击），3（大致侧面左击），4（大致侧面右击）

	//受击转向
	if (tAngle <= (_HurtedRotAngle / 2.0f))
	{
		//大致背击
		_ToHurtedRot = MakeShareable(new FRotator(attackerForward.ToOrientationRotator()));
		roughlyDir = 1;
	}
	else if (tAngle >= (180.0f - _HurtedRotAngle / 2.0f))
	{
		//大致正面击
		_ToHurtedRot = MakeShareable(new FRotator((-attackerForward).ToOrientationRotator()));
		roughlyDir = 2;
	}
	else
	{
		int32 nDir = UWVBlueprintFunctionLibrary::GetRelativeDirection(this, Attacker);
		float tRotAngle = 0;

		if (nDir == 3)
		{
			//大致侧面左击

			if (curStraightType == EWVStraightType::Bounce)
			{
				//弹开的话，转回朝向attacker
				tRotAngle = -180;
			}
			else
			{
				tRotAngle = -90;
			}
			roughlyDir = 3;
		}
		else if (nDir == 4)
		{
			//大致侧面右击

			if (curStraightType == EWVStraightType::Bounce)
			{
				//弹开的话，转回朝向attacker
				tRotAngle = 180;
			}
			else
			{
				tRotAngle = 90;
			}
			roughlyDir = 4;
		}

		auto tForward = UKismetMathLibrary::RotateAngleAxis(attackerForward, tRotAngle, FVector::UpVector);
		auto toRot = tForward.ToOrientationRotator();

		_ToHurtedRot = MakeShareable(new FRotator(toRot));
	}

	//受击位移
	if (curStraightType == EWVStraightType::Back)
	{
		auto toPos = GetActorLocation() + Attacker->GetActorForwardVector() * StraightData.BackDistance;
		_ToHurtedOffset = MakeShareable(new FVector(toPos));
	}
	else if (curStraightType == EWVStraightType::Bounce)
	{
		auto toPos = GetActorLocation() + Attacker->GetActorForwardVector() * StraightData.BounceDistance;
		_ToHurtedOffset = MakeShareable(new FVector(toPos));
	}
	else if (curStraightType == EWVStraightType::Soar)
	{
		//@TODO
	}

	//硬直表现
	auto animInst = GetMesh()->GetAnimInstance();
	if (animInst)
	{
		switch (roughlyDir)
		{
			case 1:
			{
				if (curStraightType == EWVStraightType::Back)
				{
					if (_AnimMontage_Straight_Back_B)
					{
						animInst->Montage_Play(_AnimMontage_Straight_Back_B);
					}
				}
				else if (curStraightType == EWVStraightType::Bounce)
				{
					if (_AnimMontage_Straight_Bounce_B)
					{
						animInst->Montage_Play(_AnimMontage_Straight_Bounce_B);
					}
				}
				break;
			}
			case 2:
			{
				if (curStraightType == EWVStraightType::Back)
				{
					if (_AnimMontage_Straight_Back_F)
					{
						animInst->Montage_Play(_AnimMontage_Straight_Back_F);
					}
				}
				else if (curStraightType == EWVStraightType::Bounce)
				{
					if (_AnimMontage_Straight_Bounce_F)
					{
						animInst->Montage_Play(_AnimMontage_Straight_Bounce_F);
					}
				}
				break;
			}
			case 3:
			{
				if (curStraightType == EWVStraightType::Back)
				{
					if (_AnimMontage_Straight_Back_L)
					{
						animInst->Montage_Play(_AnimMontage_Straight_Back_L);
					}
				}
				else if (curStraightType == EWVStraightType::Bounce)
				{
					if (_AnimMontage_Straight_Bounce_F)
					{
						animInst->Montage_Play(_AnimMontage_Straight_Bounce_F);
					}
				}
				break;
			}
			case 4:
			{
				if (curStraightType == EWVStraightType::Back)
				{
					if (_AnimMontage_Straight_Back_R)
					{
						animInst->Montage_Play(_AnimMontage_Straight_Back_R);
					}
				}
				else if (curStraightType == EWVStraightType::Bounce)
				{
					if (_AnimMontage_Straight_Bounce_F)
					{
						animInst->Montage_Play(_AnimMontage_Straight_Bounce_F);
					}
				}
				break;
			}
		}
	}
}

void AActionCharacter::Die(AController* EventInstigator, AActor* DamageCauser)
{
	SetState(EWVActionCharacterState::Die);
	_TmpEventInstigatorForDie = EventInstigator;
	_TmpDamageCauserForDie = DamageCauser;
	
	auto animInst = GetMesh()->GetAnimInstance();
	
	if (_AnimMontage_Die && animInst)
	{
		animInst->Montage_Play(_AnimMontage_Die);
	}
	else
	{
		HandleAnimNotify_DieEnd();
	}
}

FVector AActionCharacter::GetLastMovementInputVector_EX()
{
	auto v = GetLastMovementInputVector();
	if (v.IsZero())
	{
		return GetActorForwardVector();
	}
	return v;
}

void AActionCharacter::_KillSomeone(AActor* InSomeone)
{

}

void AActionCharacter::ShowDebug_Direction()
{
	if (bShowDebug_Direction)
	{
		auto comp_capsule = GetCapsuleComponent();
		auto startPos = comp_capsule->GetComponentLocation();
		float capsuleRadius;
		float capsuleHalfHeight;
		comp_capsule->GetScaledCapsuleSize(capsuleRadius, capsuleHalfHeight);
		startPos.Z -= (0.9 * capsuleHalfHeight);
		auto endPos = startPos + GetActorForwardVector().GetSafeNormal() * 100;
		DrawDebugDirectionalArrow(GetWorld(), startPos, endPos, 100, FColor::Cyan, false, -1, 0, 5);
	}
}

void AActionCharacter::ShowDebug_Velocity()
{
	if (bShowDebug_Velocity)
	{
		auto comp_capsule = GetCapsuleComponent();
		auto startPos = comp_capsule->GetComponentLocation();
		float capsuleRadius;
		float capsuleHalfHeight;
		comp_capsule->GetScaledCapsuleSize(capsuleRadius, capsuleHalfHeight);
		startPos.Z -= (0.8 * capsuleHalfHeight);
		auto endPos = startPos + GetVelocity().GetSafeNormal() * 100;
		DrawDebugDirectionalArrow(GetWorld(), startPos, endPos, 100, FColor::Purple, false, -1, 0, 5);
	}
}

void AActionCharacter::ShowDebug_LastMovementInputVector()
{
	if (bShowDebug_LastMovementInputVector)
	{
		auto comp_capsule = GetCapsuleComponent();
		auto startPos = comp_capsule->GetComponentLocation();
		float capsuleRadius;
		float capsuleHalfHeight;
		comp_capsule->GetScaledCapsuleSize(capsuleRadius, capsuleHalfHeight);
		startPos.Z -= (0.7 * capsuleHalfHeight);
		auto endPos = startPos + GetLastMovementInputVector_EX().GetSafeNormal() * 100;
		DrawDebugDirectionalArrow(GetWorld(), startPos, endPos, 100, FColor::Orange, false, -1, 0, 5);
	}
}
