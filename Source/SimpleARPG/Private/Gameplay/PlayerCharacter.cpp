// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/PlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "LockTargetSys/LockTargetComp.h"
#include "Kismet/GameplayStatics.h"
#include "WVModule/Public/Logger/WVLog.h"
#include "LockTargetSys/TargetPointWidget.h"

APlayerCharacter::APlayerCharacter()
{
	_Index_Target = -1;
	
	_Comp_SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("comp_springArm"));
	_Comp_SpringArm->SetupAttachment(GetRootComponent());

	_Comp_Cam = CreateDefaultSubobject<UCameraComponent>(TEXT("comp_cam"));
	_Comp_Cam->SetupAttachment(_Comp_SpringArm, USpringArmComponent::SocketName);

	_Comp_LockTarget = CreateDefaultSubobject<ULockTargetComp>(TEXT("comp_lockTarget"));
	_Comp_LockTarget->Callback_Distance.BindUFunction(this, TEXT("LockTarget_Callback_Distance"));
}

void APlayerCharacter::ScanTargets(const TArray<FName> &tagNames)
{
	CleanupTargets();
	
	for (const FName &tagName : tagNames)
	{
		UGameplayStatics::GetAllActorsWithTag(this, tagName, _Targets);
	}
}

void APlayerCharacter::CleanupTargets()
{
	UnlockTarget();
	_Targets.Empty();
}

void APlayerCharacter::LockTarget()
{
	if (_Targets.Num() > 0)
	{
		if (_Index_Target < 0)
		{
			_Index_Target = 0;
		}
		else
		{
			_Index_Target++;
			_Index_Target = _Index_Target % _Targets.Num();
		}

		auto target = _Targets[_Index_Target];
		_Comp_LockTarget->LockTarget(target);
	}
}

void APlayerCharacter::UnlockTarget()
{
	_Comp_LockTarget->UnlockTarget();
	_Index_Target = -1;
}

void APlayerCharacter::LockTarget_Callback_Distance(float dist)
{
	// WVLogI(TEXT("%f"), dist);

	auto targetPoint = Cast<UTargetPointWidget>(_Comp_LockTarget->GetTargetPointWidget());

	if (targetPoint)
	{
		if (dist <= _AtkRange)
		{
			targetPoint->Show(ETargetPointType::Red);
		}
		else
		{
			targetPoint->Show(ETargetPointType::Green);
		}
	}
}
