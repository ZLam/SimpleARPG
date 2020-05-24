// Fill out your copyright notice in the Description page of Project Settings.


#include "EventSys/WVEventHandler.h"
#include "EventSys/WVEventListener.h"
#include "Logger/WVLog.h"
#include "WVBlueprintFunctionLibrary.h"

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

EWVEventHandlerRet UWVEventHandler::FireEvent(void* params)
{
	EWVEventHandlerRet ret = EWVEventHandlerRet::Fail;
	
	if (_listeners.Num() <= 0)
	{
		ret = EWVEventHandlerRet::Success;
		return ret;
	}

	for (auto listener : _listeners)
	{
		if (listener.IsValid())
		{
			auto validType = listener->IsReady();
			if (validType == EWVEventListenerValid::FuncName)
			{
				listener->caller->ProcessEvent(listener->func, params);
				ret = EWVEventHandlerRet::Success;
			}
			else if (validType == EWVEventListenerValid::Delegate)
			{
				FWVEventDelegateParams_One params_one;
				params_one.dataInfoPtr = nullptr;
				params_one.dataPtr = params;
				listener->delegateOne.ExecuteIfBound(params_one);
				ret = EWVEventHandlerRet::Success;
			}
			else if (validType == EWVEventListenerValid::Fatal || validType == EWVEventListenerValid::None)
			{
				_toDeleteArr.Add(listener);
			}
		}
		else
		{
			_toDeleteArr.Add(listener);
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

	return ret;
}

EWVEventHandlerRet UWVEventHandler::FireEvent_BP(FWVEventParams_BP& params_bp)
{
	WVLogI(TEXT("UWVEventHandler::FireEvent_BP_%p_%p"), params_bp.propertyInfoPtr, params_bp.propertyPtr);
	
	EWVEventHandlerRet ret = EWVEventHandlerRet::Fail;

	if (_listeners.Num() <= 0)
	{
		ret = EWVEventHandlerRet::Success;
		return ret;
	}

	for (auto listener : _listeners)
	{
		if (listener.IsValid())
		{			
			auto validType = listener->IsReady();

			if (validType == EWVEventListenerValid::Fatal || validType == EWVEventListenerValid::None)
			{
				//该listener无用的了
				_toDeleteArr.Add(listener);
			}
			else if (validType == EWVEventListenerValid::Delegate)
			{
				//处理 delegate listener
				FWVEventDelegateParams_One params_one;
				params_one.dataInfoPtr = params_bp.propertyInfoPtr;
				params_one.dataPtr = params_bp.propertyPtr;
				listener->delegateOne.ExecuteIfBound(params_one);
				ret = EWVEventHandlerRet::Success;
			}
			else if (validType == EWVEventListenerValid::FuncName)
			{
				//处理 funcName listener
				
				if (!params_bp.propertyInfoPtr)
				{
					//无参数传递
					TArray<UProperty*> properties;
					UWVBlueprintFunctionLibrary::FuncParamsOfProperties(listener->func, properties);
					if (properties.Num() > 0)
					{
						WVLogW(TEXT("attempt send params to obj '%s' function '%s'"), *(listener->caller->GetName()), *(listener->funcName))
					}
					else
					{
						listener->caller->ProcessEvent(listener->func, nullptr);
						ret = EWVEventHandlerRet::Success;
					}
				}
				else if (params_bp.propertyInfoPtr->IsA(UBoolProperty::StaticClass()) &&
					listener->IsClassFuncParamsFirst(UBoolProperty::StaticClass()))
				{
					//bool参数
					UBoolProperty *tProperty = Cast<UBoolProperty>(params_bp.propertyInfoPtr);
					if (tProperty)
					{
						bool val = tProperty->GetPropertyValue(params_bp.propertyPtr);
						listener->caller->ProcessEvent(listener->func, &val);
						ret = EWVEventHandlerRet::Success;
					}
					else
					{
						WVLogW(TEXT("%s"), *WVLog_Location)
						WVLogW(TEXT("some thing wrong"))
					}
				}
				else if (params_bp.propertyInfoPtr->IsA(UNumericProperty::StaticClass()) &&
					listener->IsClassFuncParamsFirst(UNumericProperty::StaticClass()))
				{
					//number参数
					UNumericProperty *tProperty = Cast<UNumericProperty>(params_bp.propertyInfoPtr);
					if (tProperty)
					{
						if (tProperty->IsFloatingPoint())
						{
							//float
							double val = tProperty->GetFloatingPointPropertyValue(params_bp.propertyPtr);
							listener->caller->ProcessEvent(listener->func, &val);
						}
						else
						{
							//int , enum
							int64 val = tProperty->GetSignedIntPropertyValue(params_bp.propertyPtr);
							listener->caller->ProcessEvent(listener->func, &val);
						}
						ret = EWVEventHandlerRet::Success;
					}
					else
					{
						WVLogW(TEXT("%s"), *WVLog_Location)
						WVLogW(TEXT("some thing wrong"))
					}
				}
				else if (params_bp.propertyInfoPtr->IsA(UStrProperty::StaticClass()) &&
					listener->IsClassFuncParamsFirst(UStrProperty::StaticClass()))
				{
					//string参数
					UStrProperty *tProperty = Cast<UStrProperty>(params_bp.propertyInfoPtr);
					if (tProperty)
					{
						FString val = tProperty->GetPropertyValue(params_bp.propertyPtr);
						listener->caller->ProcessEvent(listener->func, &val);
						ret = EWVEventHandlerRet::Success;
					}
					else
					{
						WVLogW(TEXT("%s"), *WVLog_Location)
						WVLogW(TEXT("some thing wrong"))
					}
				}
				else if (params_bp.propertyInfoPtr->IsA(UObjectProperty::StaticClass()) &&
					listener->IsClassFuncParamsFirst(UObjectProperty::StaticClass()))
				{
					//uobject参数
					UObjectProperty *tProperty = Cast<UObjectProperty>(params_bp.propertyInfoPtr);
					if (tProperty)
					{
						UObject *val = tProperty->GetPropertyValue(params_bp.propertyPtr);
						listener->caller->ProcessEvent(listener->func, val);
						ret = EWVEventHandlerRet::Success;
					}
					else
					{
						WVLogW(TEXT("%s"), *WVLog_Location)
						WVLogW(TEXT("some thing wrong"))
					}
				}
				else if (params_bp.propertyInfoPtr->IsA(UStructProperty::StaticClass()) &&
					listener->IsClassFuncParamsFirst(UStructProperty::StaticClass()))
				{
					//struct参数
					UStructProperty *tProperty = Cast<UStructProperty>(params_bp.propertyInfoPtr);
					if (tProperty)
					{
						TArray<UProperty*> properties_params;
						UWVBlueprintFunctionLibrary::FuncParamsOfProperties(listener->func, properties_params);
						if (properties_params.Num() > 0)
						{
							UStructProperty *structParamsProp = Cast<UStructProperty>(properties_params[0]);
							if (structParamsProp)
							{
								if (tProperty->Struct == structParamsProp->Struct)
								{
									listener->caller->ProcessEvent(listener->func, params_bp.propertyPtr);
									ret = EWVEventHandlerRet::Success;
								}
								else
								{
									WVLogW(TEXT("%s"), *WVLog_Location)
									WVLogI(TEXT("struct not equal"));
								}
							}
							else
							{
								WVLogW(TEXT("%s"), *WVLog_Location)
								WVLogW(TEXT("some thing wrong"))
							}
						}
						else
						{
							WVLogW(TEXT("%s"), *WVLog_Location)
							WVLogI(TEXT("func not params???"));
						}
					}
					else
					{
						WVLogW(TEXT("%s"), *WVLog_Location)
						WVLogW(TEXT("some thing wrong"))
					}
				}
				else
				{
					WVLogW(TEXT("%s"), *WVLog_Location)
					WVLogI(TEXT("dont support this type '%s'"), *(params_bp.propertyInfoPtr->GetName()));
				}
			}
		}
		else
		{
			_toDeleteArr.Add(listener);
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

	return ret;
}
