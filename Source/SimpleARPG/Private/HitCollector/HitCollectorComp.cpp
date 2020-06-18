// Fill out your copyright notice in the Description page of Project Settings.


#include "HitCollector/HitCollectorComp.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "Components/ShapeComponent.h"
#include "WVModule/Public/Logger/WVLog.h"

// Sets default values for this component's properties
UHitCollectorComp::UHitCollectorComp()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...

	_bEnable = false;

	SetTickGroup(ETickingGroup::TG_PostPhysics);		//
}


// Called when the game starts
void UHitCollectorComp::BeginPlay()
{
	Super::BeginPlay();

	// ...

	auto tOwner = GetOwner();
	
	if (!tOwner)
	{
		return;
	}

	WVLogI(TEXT("111 %s"), *(tOwner->GetName()))

	for (auto &tTag : _ColliderTags)
	{
		WVLogI(TEXT("222"))
		
		auto tColliderArr = tOwner->GetComponentsByTag(UShapeComponent::StaticClass(), tTag);
		for (auto tCollider : tColliderArr)
		{
			WVLogI(TEXT("333"))
			
			_Colliders.Push(Cast<UShapeComponent>(tCollider));
		}
	}

	for (auto &tChannel : _HitChannels)
	{
		WVLogI(TEXT("444"))
		
		_ChannelsQueryParams.AddObjectTypesToQuery(tChannel);
	}

	_CollisionQueryParams.AddIgnoredActor(tOwner);
	auto tOwnerCharacter = GetOwnerCharacter();
	if (tOwnerCharacter)
	{
		_CollisionQueryParams.AddIgnoredActor(tOwnerCharacter);
	}
}


// Called every frame
void UHitCollectorComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

	HandleHit();
}

void UHitCollectorComp::Enable()
{
	if (_bEnable)
	{
		return;
	}

	_bEnable = true;

	ResetLastPosArr();
}

void UHitCollectorComp::Disable()
{
	if (!_bEnable)
	{
		return;
	}

	_bEnable = false;
}

ACharacter* UHitCollectorComp::GetOwnerCharacter()
{
	return _GetOwnerCharacter(GetOwner());
}

ACharacter* UHitCollectorComp::_GetOwnerCharacter(AActor* InOwner)
{
	if (!InOwner)
	{
		return nullptr;
	}
	ACharacter *character = Cast<ACharacter>(InOwner);
	if (character)
	{
		return  character;
	}
	return _GetOwnerCharacter(InOwner->GetOwner());
}

void UHitCollectorComp::ResetLastPosArr()
{
	_LastPosArr.Empty();
	for (auto tCollider : _Colliders)
	{
		_LastPosArr.Push(tCollider->GetComponentLocation());
	}
}

void UHitCollectorComp::HandleHit()
{
	if (!_bEnable)
	{
		return;
	}
	
	for (int32 i = 0; i < _Colliders.Num(); ++i)
	{
		auto tCollider = _Colliders[i];
		
		TArray<FHitResult> hitResults;
		GetWorld()->SweepMultiByObjectType(
			hitResults,
			_LastPosArr[i],
			tCollider->GetComponentLocation(),
			tCollider->GetComponentQuat(),
			_ChannelsQueryParams,
			tCollider->GetCollisionShape(),
			_CollisionQueryParams
		);

		for (int32 j = 0; j < hitResults.Num(); ++j)
		{
			if (HitCallback.IsBound())
			{
				HitCallback.Broadcast(hitResults[j], GetOwner(), tCollider);
			}
		}
	}
}
