﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "EventSys/WVEventHandler.h"
#include "EventSys/WVEventListener.h"

void UWVEventHandler::Add(TSharedPtr<FWVEventListener> listener)
{
	_listeners.AddTail(listener);
}

void UWVEventHandler::Delete(UObject* inObj)
{
	auto ite = begin(_listeners);
	while (ite)
	{
		if ((*ite)->caller == inObj)
		{
			_listeners.RemoveNode(ite.GetNode());
		}
		++ite;
	}
}

void UWVEventHandler::Delete(TSharedPtr<FWVEventListener> listener)
{
	auto ite = begin(_listeners);
	while (ite)
	{
		if ((*ite) == listener)
		{
			_listeners.RemoveNode(ite.GetNode());
		}
		++ite;
	}
}

void UWVEventHandler::FireEvent()
{
	if (_listeners.Num() <= 0)
	{
		return;
	}

	for (auto listener : _listeners)
	{
		if (listener.IsValid())
		{
			auto validType = listener->IsValid();
			if (validType == EWVEventListenerValid::FuncName)
			{
				listener->caller->ProcessEvent(listener->func, nullptr);
			}
			else if (validType == EWVEventListenerValid::Delegate)
			{
				
			}
			else if (validType == EWVEventListenerValid::Fatal || validType == EWVEventListenerValid::None)
			{
				_toDeleteArr.Push(listener);
			}
		}
	}

	if (_toDeleteArr.Num() > 0)
	{
		for (auto listener : _toDeleteArr)
		{
			Delete(listener);
		}
		_toDeleteArr.Empty();
	}
}