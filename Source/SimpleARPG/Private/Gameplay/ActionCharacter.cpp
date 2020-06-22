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
	
	_AtkRange = 1000.0f;
	
	_bSprinting = false;
	_bDodging = false;
	_bDodgeChangeColliderBegin = false;
	_bDodgeChangeColliderEnd = false;
	_bLockDodge = false;

	_bSuperArmor = false;
	_HurtedRotAngle = 90.0f;

	_AnimMontage_Dodge = nullptr;

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

	FScriptDelegate callback_comboMachine_start;
	callback_comboMachine_start.BindUFunction(this, TEXT("Callback_ComboMachine_Start"));
	_Comp_ComboMachine->Callback_Start.Add(callback_comboMachine_start);

	FScriptDelegate callback_comboMachine_resume;
	callback_comboMachine_resume.BindUFunction(this, TEXT("Callback_ComboMachine_Resume"));
	_Comp_ComboMachine->Callback_Resume.Add(callback_comboMachine_resume);
}

void AActionCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
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

	auto animIns = mesh->GetAnimInstance();

	if (!animIns)
	{
		return;
	}

	if (animIns->Montage_IsPlaying(_AnimMontage_Dodge))
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

	auto lastInputVector = GetLastMovementInputVector();
	SetActorRotation(lastInputVector.Rotation());

	animIns->Montage_Play(_AnimMontage_Dodge, 1.5f);

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
			[this, &timerMgr]()
			{
				timerMgr.ClearTimer(this->_Timer_RecoverPower);
				_CurPower = _MaxPower;
				UWVEventDispatcher::GetInstance()->FireEvent_SP(EWVEventCategory::Inner, EWVEventName::PlayerPowerChange);
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

float AActionCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	auto ret = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

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

					auto cfg_comboAction = UWVConfigUtil::GetInstance()->GetConfigRowData<FWVConfig_ComboActionRow>(EWVConfigName::ComboAction, _CharactorName);

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
								Die();
							}
							else
							{
								//是否霸体
								
								if (!_bSuperArmor)
								{
									//受击转向
									
									_HandleHurtedRot(Attacker);

									auto curStraightType = comboActionData->StraightData.StraightType;
									_CurDown += comboActionData->StraightData.AddDown;
									_CurStraight += comboActionData->StraightData.AddStraight;
									
									if (_CurDown >= _MaxDown)
									{
										//爆Down
										
										if (curStraightType == EWVStraightType::Back)
										{
											//强制设为弹开
											curStraightType = EWVStraightType::Bounce;
										}

										//Down状态
										_State = EWVActionCharacterState::Down;
									}
									else if (_CurStraight >= _MaxStraight)
									{
										//爆硬直

										_CurStraight = (int32)_CurStraight % (int32)_MaxStraight;
										
										//硬直状态
										_State = EWVActionCharacterState::Straight;
									}

									_HandleStraight(Attacker, curStraightType, comboActionData->StraightData);
								}
							}
						}
					}
				}
			}
		}
	}

	return ret;
}

void AActionCharacter::_HandleHurtedRot(AActionCharacter *Attacker)
{
	FVector selfForward = GetActorForwardVector();
	FVector attackerForward = Attacker->GetActorForwardVector();

	float tAngle = UWVBlueprintFunctionLibrary::GetAngleBetween2Vector(selfForward, attackerForward);

	if (tAngle <= (_HurtedRotAngle / 2.0f))
	{
		//大致背击

		WVLogI(TEXT("111"))

		auto toRot = attackerForward.ToOrientationRotator();
		SetActorRotation(toRot);
	}
	else if (tAngle >= (180.0f - _HurtedRotAngle / 2.0f))
	{
		//大致正面击

		WVLogI(TEXT("222"))

		auto toRot = (-attackerForward).ToOrientationRotator();
		SetActorRotation(toRot);
	}
	else
	{
		int32 nDir = UWVBlueprintFunctionLibrary::GetRelativeDirection(this, Attacker);
		float tRotAngle = 0;

		if (nDir == 3)
		{
			//大致侧面左击

			WVLogI(TEXT("333"))

			tRotAngle = -90;
		}
		else if (nDir == 4)
		{
			//大致侧面右击

			WVLogI(TEXT("444"))

			tRotAngle = 90;
		}

		auto tForward = UKismetMathLibrary::RotateAngleAxis(attackerForward, tRotAngle, FVector::UpVector);
		auto toRot = tForward.ToOrientationRotator();
		SetActorRotation(toRot);
	}
}

void AActionCharacter::_HandleStraight(AActionCharacter *Attacker, EWVStraightType curStraightType, const FWVStraightData &StraightData)
{
	// if (_State == EWVActionCharacterState::Straight)
	// {
	// 	//当前硬直状态，硬直的表现会有后退，弹开，击飞等等
	// }
	// else if (_State == EWVActionCharacterState::Down)
	// {
	// 	//当前Down状态，意在不能一直连击并拉开距离，只会有弹开，击飞等等之类的表现
	// }

	if (curStraightType == EWVStraightType::Back)
	{
		auto toPos = GetActorLocation() + Attacker->GetActorForwardVector() * StraightData.BackDistance;
		SetActorLocation(toPos);
	}
	else if (curStraightType == EWVStraightType::Bounce)
	{
		auto toPos = GetActorLocation() + Attacker->GetActorForwardVector() * StraightData.BounceDistance;
		SetActorLocation(toPos);
	}
	else if (curStraightType == EWVStraightType::Soar)
	{
		//@TODO
	}
}

void AActionCharacter::Die()
{
	WVLogI(TEXT("die die die"))
}

void AActionCharacter::Callback_ComboMachine_Start()
{
	if (_State == EWVActionCharacterState::ReadyAtk)
	{
		_State = EWVActionCharacterState::Atking;
	}
}

void AActionCharacter::Callback_ComboMachine_Resume()
{
	if (_State == EWVActionCharacterState::Atking)
	{
		_State = EWVActionCharacterState::ReadyAtk;
	}
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
		auto endPos = startPos + GetLastMovementInputVector().GetSafeNormal() * 100;
		DrawDebugDirectionalArrow(GetWorld(), startPos, endPos, 100, FColor::Orange, false, -1, 0, 5);
	}
}
