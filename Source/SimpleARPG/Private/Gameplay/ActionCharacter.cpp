// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/ActionCharacter.h"
#include "WVModule/Public/EventSys/WVEventDispatcher.h"
#include "WVModule/Public/ConfigUtil/WVConfigUtil.h"
#include "WVModule/Public/Logger/WVLog.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"
#include "DrawDebugHelpers.h"

// Sets default values
AActionCharacter::AActionCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	_bReadyAtk = true;
	_AtkRange = 1000.0f;
	
	_bSprinting = false;
	
	bShowDebug_Direction = false;
	bShowDebug_Velocity = false;
	bShowDebug_LastMovementInputVector = false;

	_AnimMontage_Dodge = nullptr;
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
}

void AActionCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UWVEventDispatcher::GetInstance()->RemoveAllListener(this);
}

// Called every frame
void AActionCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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

	_bSprinting = bVal;
	auto comp_move = GetCharacterMovement();

	if (_bSprinting)
	{
		comp_move->MaxWalkSpeed = _SprintSpeed;
	}
	else
	{
		comp_move->MaxWalkSpeed = _RunSpeed;
	}
}

void AActionCharacter::Dodge()
{
	if (!_AnimMontage_Dodge)
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

	auto lastInputVector = GetLastMovementInputVector();
	SetActorRotation(lastInputVector.Rotation());

	animIns->Montage_Play(_AnimMontage_Dodge, 1.5f);
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
