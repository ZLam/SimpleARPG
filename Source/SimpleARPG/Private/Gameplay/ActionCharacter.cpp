// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/ActionCharacter.h"
#include "WVModule/Public/EventSys/WVEventDispatcher.h"
#include "WVModule/Public/ConfigUtil/WVConfigUtil.h"
#include "WVModule/Public/Logger/WVLog.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"
#include "TimerManager.h"
#include "DrawDebugHelpers.h"
#include "ComboSys/ComboMachineComp.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Gameplay/Equipment.h"

// Sets default values
AActionCharacter::AActionCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	_bReadyAtk = true;
	_AtkRange = 1000.0f;
	
	_bSprinting = false;
	_bDodging = false;
	_bDodgeChangeColliderBegin = false;
	_bDodgeChangeColliderEnd = false;
	_bLockDodge = false;
	
	bShowDebug_Direction = false;
	bShowDebug_Velocity = false;
	bShowDebug_LastMovementInputVector = false;

	_AnimMontage_Dodge = nullptr;

	_Comp_ComboMachine = CreateDefaultSubobject<UComboMachineComp>(TEXT("Comp_ComboMachine"));
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
