// Fill out your copyright notice in the Description page of Project Settings.


#include "HitCollector/HitCollectorComp.h"
#include "GameFramework/Actor.h"
#include "Components/ShapeComponent.h"

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

	if (tOwner)
	{
		return;
	}

	for (auto &tTag : _ColliderTags)
	{
		auto tColliderArr = tOwner->GetComponentsByTag(UShapeComponent::StaticClass(), tTag);
		for (auto tCollider : tColliderArr)
		{
			_Colliders.Push(Cast<UShapeComponent>(tCollider));
		}
	}

	for (auto &tChannel : _HitChannels)
	{
		_ChannelsQueryParams.AddObjectTypesToQuery(tChannel);
	}

	_CollisionQueryParams.AddIgnoredActor(tOwner);
}


// Called every frame
void UHitCollectorComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
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
