// Fill out your copyright notice in the Description page of Project Settings.


#include "InputBufferSys/InputBufferComp.h"
#include "GameFramework/PlayerInput.h"
#include "Engine/World.h"
#include "InputBufferSys/InputBufferController.h"
#include "Logger/WVLog.h"

// Sets default values for this component's properties
UInputBufferComp::UInputBufferComp()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	// PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UInputBufferComp::BeginPlay()
{
	Super::BeginPlay();

	// ...

	for (const FKey &key : RegistKey)
	{
		_CurInputInfoMap.Emplace(key);
	}
}


// Called every frame
void UInputBufferComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UInputBufferComp::HandlePostProcessInput(const float DeltaTime, const bool bGamePaused, UPlayerInput *InPlayerInput)
{
	if (bGamePaused || !InPlayerInput)
	{
		return;
	}

	bool hasChanged = false;

	for (auto &elem : _CurInputInfoMap)
	{
		FKey &tInputKey = elem.Key;
		FInputInfo &tInputInfo = elem.Value;
		FKeyState *keyState = InPlayerInput->GetKeyState(tInputKey);
		
		if (keyState)
		{
			if (tInputKey.IsFloatAxis())
			{
				if (tInputInfo.KeyState == EInputKeyState::None || tInputInfo.KeyState == EInputKeyState::Released)
				{
					if (!keyState->Value.IsNearlyZero())
					{
						tInputInfo.IsValid = true;
						tInputInfo.KeyState = EInputKeyState::Held;
						tInputInfo.StartTime = GetWorld()->GetRealTimeSeconds();
						tInputInfo.LastTime = tInputInfo.StartTime;
						tInputInfo.AxisValue = keyState->Value;

						hasChanged = true;
					}
				}
				else if (tInputInfo.KeyState == EInputKeyState::Held)
				{
					tInputInfo.KeyState = keyState->Value.IsNearlyZero() ? EInputKeyState::Released : EInputKeyState::Held;
					tInputInfo.LastTime = GetWorld()->GetRealTimeSeconds();
					tInputInfo.AxisValue = keyState->Value.IsNearlyZero() ? FVector(0.0f) : keyState->Value;

					hasChanged = true;
				}
			}
			else
			{
				if (tInputInfo.KeyState == EInputKeyState::None)
				{
					if (keyState->bDown)
					{
						tInputInfo.IsValid = true;
						tInputInfo.KeyState = EInputKeyState::Pressed;
						tInputInfo.StartTime = GetWorld()->GetRealTimeSeconds();
						tInputInfo.LastTime = tInputInfo.StartTime;

						hasChanged = true;
					}
				}
				else if (tInputInfo.KeyState == EInputKeyState::Pressed || tInputInfo.KeyState == EInputKeyState::Held)
				{
					tInputInfo.KeyState = keyState->bDown ? EInputKeyState::Held : EInputKeyState::Released;
					tInputInfo.LastTime = GetWorld()->GetRealTimeSeconds();

					hasChanged = true;
				}
				else if (tInputInfo.KeyState == EInputKeyState::Released)
				{
					if (keyState->bDown)
					{
						tInputInfo.KeyState = EInputKeyState::Pressed;
						tInputInfo.StartTime = GetWorld()->GetRealTimeSeconds();
						tInputInfo.LastTime = tInputInfo.StartTime;

						hasChanged = true;
					}
				}
			}
		}
	}

	if (hasChanged)
	{
		AInputBufferController *inputCtrl = Cast<AInputBufferController>(GetOwner());
		if (inputCtrl)
		{
			inputCtrl->HandlePostInput();
		}
	}
}

bool UInputBufferComp::Match(FKey InKey, EWVInputMatchStyle InMatchStyle, const FWVInputMatchData& InMatchData, bool bNeedInvalid)
{
	bool ret = false;
	const FInputInfo *tInputInfo = _CurInputInfoMap.Find(InKey);
	if (!tInputInfo)
	{
		return ret;
	}
	switch (InMatchStyle)
	{
		case EWVInputMatchStyle::OnlyLastTime:
		{
			ret = GetWorld()->GetRealTimeSeconds() - tInputInfo->LastTime <= InMatchData.ValidDiff_LastTime;
			break;
		}
		case EWVInputMatchStyle::AxisLR_WithLastTime:
		{
			if (InKey.IsFloatAxis())
			{
				bool bLastTime = GetWorld()->GetRealTimeSeconds() - tInputInfo->LastTime <= InMatchData.ValidDiff_LastTime;
				bool bAxisL = FMath::Abs(tInputInfo->AxisValue.X - -1) <= InMatchData.ValidDiff_AxisL;
				bool bAxisR = FMath::Abs(tInputInfo->AxisValue.X - 1) <= InMatchData.ValidDiff_AxisR;
				
				ret = (bLastTime) && (bAxisL || bAxisR);
			}
			break;
		}
	}
	if (bNeedInvalid)
	{
		Invalid();
	}
	return ret;
}

bool UInputBufferComp::MatchAll(const TArray<FKey>& InKeys, const TArray<EWVInputMatchStyle>& InMatchStyles,
	const TArray<FWVInputMatchData>& InMatchDataArr, bool bNeedInvalid)
{
	for (int32 i = 0; i < InKeys.Num(); i++)
	{
		if (!Match(InKeys[i], InMatchStyles[i], InMatchDataArr[i], bNeedInvalid))
		{
			return false;
		}
	}
	return true;
}

void UInputBufferComp::Invalid()
{
	for (auto &elem : _CurInputInfoMap)
	{
		elem.Value.IsValid = false;
	}
}

void UInputBufferComp::PrintLog()
{
	for (auto &elem : _CurInputInfoMap)
	{
		auto str = FString::Printf(
			TEXT("%s_%d_%s_%f_%f"),
			*(elem.Key.ToString()),
			elem.Value.KeyState,
			*(elem.Value.AxisValue.ToString()),
			elem.Value.StartTime,
			elem.Value.LastTime
		);
		WVLogI(*str)
	}
}
