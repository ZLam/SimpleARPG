// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/PlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

APlayerCharacter::APlayerCharacter()
{
	_comp_springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("comp_springArm"));
	_comp_springArm->SetupAttachment(GetRootComponent());

	_comp_cam = CreateDefaultSubobject<UCameraComponent>(TEXT("comp_cam"));
	_comp_cam->SetupAttachment(_comp_springArm, USpringArmComponent::SocketName);
}
