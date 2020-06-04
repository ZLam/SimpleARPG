// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/PlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "LockTargetSys/LockTargetComp.h"
#include "Kismet/GameplayStatics.h"
#include "WVModule/Public/Logger/WVLog.h"

APlayerCharacter::APlayerCharacter()
{
	_bReadyAtk = true;
	
	_Comp_SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("comp_springArm"));
	_Comp_SpringArm->SetupAttachment(GetRootComponent());

	_Comp_Cam = CreateDefaultSubobject<UCameraComponent>(TEXT("comp_cam"));
	_Comp_Cam->SetupAttachment(_Comp_SpringArm, USpringArmComponent::SocketName);

	_Comp_LockTarget = CreateDefaultSubobject<ULockTargetComp>(TEXT("comp_lockTarget"));
}

void APlayerCharacter::TestLockTarget()
{
	if (Targets.Num() <= 0)
	{
		UGameplayStatics::GetAllActorsWithTag(this, FName(TEXT("enemy")), Targets);
		_Comp_LockTarget->Callback_Distance.BindUFunction(this, TEXT("TestLockTarget_Callback_Distance"));
	}

	if (Targets.Num() > 0)
	{
		if (Index_Target < 0)
		{
			Index_Target = 0;
		}
		else
		{
			Index_Target++;
			Index_Target = Index_Target % Targets.Num();
		}
		_Comp_LockTarget->LockTarget(Targets[Index_Target]);
	}
}

void APlayerCharacter::TestLockTarget_Callback_Distance(float dist)
{
	// WVLogI(TEXT("%f"), dist);
}
