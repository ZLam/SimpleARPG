// Fill out your copyright notice in the Description page of Project Settings.


#include "InputBufferSys/InputBufferController.h"
#include "InputBufferSys/InputBufferComp.h"
#include "Logger/WVLog.h"

AInputBufferController::AInputBufferController()
{
	Comp_InputBuffer = CreateDefaultSubobject<UInputBufferComp>(TEXT("Comp_InputBuffer"));

	//0_0_0_1_0
	// WVLogI(TEXT("%d_%d_%d_%d_%d"),
	// 	EKeys::Gamepad_FaceButton_Left.IsDeprecated(),
	// 	EKeys::Gamepad_FaceButton_Left.IsFloatAxis(),
	// 	EKeys::Gamepad_FaceButton_Left.IsVectorAxis(),
	// 	EKeys::Gamepad_FaceButton_Left.IsGamepadKey(),
	// 	EKeys::Gamepad_FaceButton_Left.IsModifierKey());

	//0_1_0_1_0
	// WVLogI(TEXT("%d_%d_%d_%d_%d"),
	// 	EKeys::Gamepad_LeftX.IsDeprecated(),
	// 	EKeys::Gamepad_LeftX.IsFloatAxis(),
	// 	EKeys::Gamepad_LeftX.IsVectorAxis(),
	// 	EKeys::Gamepad_LeftX.IsGamepadKey(),
	// 	EKeys::Gamepad_LeftX.IsModifierKey());
}

void AInputBufferController::PostProcessInput(const float DeltaTime, const bool bGamePaused)
{
	Super::PostProcessInput(DeltaTime, bGamePaused);

	Comp_InputBuffer->HandlePostProcessInput(DeltaTime, bGamePaused, PlayerInput);
	
	// Comp_InputBuffer->PrintLog();
}

void AInputBufferController::HandlePostInput_Implementation()
{
	
}

