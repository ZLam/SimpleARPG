// Fill out your copyright notice in the Description page of Project Settings.


#include "LockTargetSys/LockTargetComp.h"
#include "GameFramework/Actor.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/WidgetComponent.h"
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

void ULockTargetComp::_UpdateLocking(float dt)
{
	if (!IsLocking())
	{
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
	FRotator toRotation = FRotationMatrix::MakeFromX(toPos - fromPos).Rotator();
	FRotator fromRotation = playerCtrl->GetControlRotation();

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

	UWidgetComponent *comp = NewObject<UWidgetComponent>(_CurTarget);
	comp->ComponentTags.Push(TEXT("LockTargetSys_TargetPoint"));
	comp->SetWidgetClass(Class_TargetPoint);
	comp->SetWidgetSpace(EWidgetSpace::Screen);
	comp->SetRelativeLocation(TargetPointRelativePos);
	comp->SetDrawSize(TargetPointDrawSize);
	comp->SetupAttachment(_CurTarget->GetRootComponent());
	comp->RegisterComponent();
}

void ULockTargetComp::_DelTargetPoint()
{
	if (!_CurTarget || !Class_TargetPoint)
	{
		return;
	}

	TArray<USceneComponent*> comps;
	_CurTarget->GetRootComponent()->GetChildrenComponents(true, comps);

	for (auto tComp : comps)
	{
		UWidgetComponent *comp_widget = Cast<UWidgetComponent>(tComp);
		if (comp_widget && comp_widget->ComponentHasTag(TEXT("LockTargetSys_TargetPoint")))
		{
			tComp->DestroyComponent();
		}
	}
}
