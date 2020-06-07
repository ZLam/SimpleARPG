// Fill out your copyright notice in the Description page of Project Settings.


#include "LockTargetSys/LockTargetComp.h"
#include "GameFramework/Actor.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/WidgetComponent.h"
#include "LockTargetSys/LockTargetableInterface.h"
#include "DrawDebugHelpers.h"
#include "WVModule/Public/Logger/WVLog.h"

// Sets default values for this component's properties
ULockTargetComp::ULockTargetComp()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...

	_CurTarget = nullptr;
	Class_TargetPoint = nullptr;
	TargetPointRelativePos = FVector(0.0f);
	TargetPointDrawSize = FVector2D(32.0f, 32.0f);
	OffsetFromPos = FVector(0.0f);
	PitchLimit = FVector2D(-60, 60);
}


// Called when the game starts
void ULockTargetComp::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void ULockTargetComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

	_UpdateLocking(DeltaTime);
}

void ULockTargetComp::LockTarget(AActor* target)
{
	if (_CurTarget)
	{		
		UnlockTarget();
	}
	
	_CurTarget = target;

	_AddTargetPoint();
}

void ULockTargetComp::UnlockTarget()
{
	_DelTargetPoint();

	_CurTarget = nullptr;
}

bool ULockTargetComp::IsLocking()
{
	return _CurTarget != nullptr;
}

UUserWidget* ULockTargetComp::GetTargetPointWidget()
{
	if (_Comp_TargetPoint)
	{
		return _Comp_TargetPoint->GetUserWidgetObject();
	}
	return nullptr;
}

bool ULockTargetComp::IsCurTargetable()
{
	if (IsValid(_CurTarget) && _CurTarget->GetClass()->ImplementsInterface(ULockTargetableInterface::StaticClass()))
	{
		return ILockTargetableInterface::Execute_IsTargetable(_CurTarget);
	}
	return false;
}

void ULockTargetComp::_UpdateLocking(float dt)
{
	if (!IsLocking())
	{
		return;
	}

	if (!IsCurTargetable())
	{
		UnlockTarget();
		return;
	}

	AActor *player = GetOwner();

	UCameraComponent *comp_cam = player->FindComponentByClass<UCameraComponent>();
	if (!comp_cam)
	{
		return;
	}

	APlayerController *playerCtrl = UGameplayStatics::GetPlayerController(player, 0);
	if (!playerCtrl)
	{
		return;
	}

	FVector fromPos = player->GetActorLocation();
	FVector toPos = _CurTarget->GetActorLocation();

	fromPos += OffsetFromPos;

	//
	// DrawDebugLine(GetWorld(), fromPos, toPos, FColor::Cyan);
	//

	FRotator fromRotation = playerCtrl->GetControlRotation();
	FRotator toRotation = FRotationMatrix::MakeFromX(toPos - fromPos).Rotator();

	toRotation.Roll = fromRotation.Roll;
	if (toRotation.Pitch < PitchLimit.X)
	{
		toRotation.Pitch = PitchLimit.X;
	}
	else if (toRotation.Pitch > PitchLimit.Y)
	{
		toRotation.Pitch = PitchLimit.Y;
	}

	FRotator interpRotation = FMath::RInterpTo(fromRotation, toRotation, dt, 5.0f);

	playerCtrl->SetControlRotation(interpRotation);
	
	if (Callback_Distance.IsBound())
	{
		float dist = FVector::Dist(fromPos, toPos);
		Callback_Distance.Execute(dist);
	}
}

void ULockTargetComp::_AddTargetPoint()
{
	if (!_CurTarget || !Class_TargetPoint)
	{
		return;
	}

	if (_Comp_TargetPoint)
	{
		_DelTargetPoint();
	}

	_Comp_TargetPoint = NewObject<UWidgetComponent>(_CurTarget);
	_Comp_TargetPoint->ComponentTags.Push(TEXT("LockTargetSys_TargetPoint"));
	_Comp_TargetPoint->SetWidgetClass(Class_TargetPoint);
	_Comp_TargetPoint->SetWidgetSpace(EWidgetSpace::Screen);
	_Comp_TargetPoint->SetRelativeLocation(TargetPointRelativePos);
	_Comp_TargetPoint->SetDrawSize(TargetPointDrawSize);
	_Comp_TargetPoint->SetupAttachment(_CurTarget->GetRootComponent());
	_Comp_TargetPoint->RegisterComponent();
}

void ULockTargetComp::_DelTargetPoint()
{
	if (!_CurTarget || !Class_TargetPoint)
	{
		return;
	}

	//
	// TArray<USceneComponent*> comps;
	// _CurTarget->GetRootComponent()->GetChildrenComponents(true, comps);
	//
	// for (auto tComp : comps)
	// {
	// 	UWidgetComponent *comp_widget = Cast<UWidgetComponent>(tComp);
	// 	if (comp_widget && comp_widget->ComponentHasTag(TEXT("LockTargetSys_TargetPoint")))
	// 	{
	// 		tComp->DestroyComponent();
	// 	}
	// }
	//

	if (_Comp_TargetPoint)
	{
		_Comp_TargetPoint->DestroyComponent();
		_Comp_TargetPoint = nullptr;
	}
}
