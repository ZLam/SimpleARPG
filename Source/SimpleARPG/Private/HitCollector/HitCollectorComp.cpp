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
	_bHitOnce = true;

	SetTickGroup(ETickingGroup::TG_PostPhysics);		//使用本帧的数据而不是上一帧
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

	for (auto &tTag : _ColliderTags)
	{
		auto tColliderArr = tOwner->GetComponentsByTag(UShapeComponent::StaticClass(), tTag);
		for (auto tCollider : tColliderArr)
		{
			auto comp_shape = Cast<UShapeComponent>(tCollider);
			_Colliders.Push(comp_shape);
			_HitOnceMap.Emplace(comp_shape, MakeShareable(new FHitOnceInfo));
		}
	}

	for (auto &tChannel : _HitChannels)
	{
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
	ResetHitOnceMap();
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

void UHitCollectorComp::ResetHitOnceMap()
{
	if (!_bHitOnce)
	{
		return;
	}
	for (auto elem : _HitOnceMap)
	{
		elem.Value->Reset();
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
		auto pHitOnceInfo = _HitOnceMap.Find(tCollider);

		if ((_bHitOnce) && (!pHitOnceInfo || !pHitOnceInfo->IsValid()))
		{
			continue;
		}
		
		TArray<FHitResult> hitResults;
		GetWorld()->SweepMultiByObjectType(
			hitResults,
			_LastPosArr[i],
			tCollider->GetComponentLocation(),
			tCollider->GetComponentQuat(),
			_ChannelsQueryParams,
			tCollider->GetCollisionShape(),
			_CollisionQueryParams
		);			//该方法凡是指定的Channels都会hit，唔理系Ignore，Overlap还是Block

		for (int32 j = 0; j < hitResults.Num(); ++j)
		{
			auto tHitResult = hitResults[j];

			if (tHitResult.Actor.IsValid() && tHitResult.Component.IsValid())
			{
				if (_bHitOnce)
				{
					bool bHited = (*pHitOnceInfo)->IsHited(tHitResult.Actor.Get());

					// WVLogI(TEXT("111 %s_%p_%d"), *(tHitResult.Actor->GetName()), tHitResult.Actor.Get(), bHited)
					
					if (bHited)
					{
						continue;
					}

					(*pHitOnceInfo)->Hited(tHitResult.Actor.Get());
					
					// WVLogI(TEXT("222 %s_%p_%d"), *(tHitResult.Actor->GetName()), tHitResult.Actor.Get(), 1)
				}

				if (HitCallback.IsBound())
				{
					HitCallback.Broadcast(hitResults[j], GetOwner(), tCollider);
				}
			}
		}
	}
}
