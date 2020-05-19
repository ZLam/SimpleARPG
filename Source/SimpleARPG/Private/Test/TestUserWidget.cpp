﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleARPG/Public/Test/TestUserWidget.h"
#include "Components/Button.h"
#include "SimpleARPG/Public/Test/TestActor.h"
#include "WVModule/Public/Logger/WVLog.h"
#include "Kismet/GameplayStatics.h"
#include "WVModule/Public/ConfigUtil/WVConfigUtil.h"

void UTestUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	auto btn_test1 = Cast<UButton>(GetWidgetFromName(TEXT("btn_test1")));
	FScriptDelegate event_btn_test1;
	event_btn_test1.BindUFunction(this, TEXT("Btn_Test1_Callback"));
	btn_test1->OnClicked.Add(event_btn_test1);

	auto btn_test2 = Cast<UButton>(GetWidgetFromName(TEXT("btn_test2")));
	FScriptDelegate event_btn_test2;
	event_btn_test2.BindUFunction(this, TEXT("Btn_Test2_Callback"));
	btn_test2->OnClicked.Add(event_btn_test2);
}

void UTestUserWidget::Btn_Test1_Callback()
{
	WVLogI(TEXT("UTestUserWidget::Btn_Test1_Callback"));

	/***
	 *	目前未搞清StaticClass()，GetClass()具体有咩区别
	 *	目前理解UClass的主要作用是存储UObject的类型信息，用以实现UE4里面的类型系统（Property System）
	 *	类型系统里还有UProperty，UEnum，UStruct，UFunction等等等
	 *	有了类型系统才能实现反射系统
	 *
		//TestActor的c++版本拖进level
		//StaticClass() == GetClass()
		//StaticClass()->GetDefaultObject() == GetClass()->GetDefaultObject()
		WVLogger: Display: [18:14:17:677]ATestActor Class Begin
		WVLogger: Display: [18:14:17:677]000002789F1BB380
		WVLogger: Display: [18:14:17:677]00000278ACF79600
		WVLogger: Display: [18:14:17:677]000002789F1BB380
		WVLogger: Display: [18:14:17:677]00000278ACF79600
		WVLogger: Display: [18:14:17:677]ATestActor Class End

		//TestActor的bp版本拖进level
		//StaticClass()跟cpp的一样，StaticClass()的CDO也跟cpp的一样
		//但bp版本的GetClass() != StaticClass()，自然GetClass()的CDO也不一样了
		WVLogger: Display: [18:14:17:677]ATestActor Class Begin
		WVLogger: Display: [18:14:17:677]000002789F1BB380
		WVLogger: Display: [18:14:17:677]00000278ACF79600
		WVLogger: Display: [18:14:17:677]00000278C30C5D00
		WVLogger: Display: [18:14:17:677]00000278C2EA0400
		WVLogger: Display: [18:14:17:677]ATestActor Class End

		//SpawnActor出嚟的TestActor
		//跟上面c++版本拖进level的情况一样，完全可以理解
		//TestActor的类型信息就只有一份嘛，那么CDO也只有一个
		//那GetClass()干嘛用的呢？？？
		//StaticClass()是UE4的反射机制生成的，最后那些宏替换最终到ObjectMacro.h里 DECLARE_CLASS
		WVLogger: Display: [18:14:20:647]ATestActor Class Begin
		WVLogger: Display: [18:14:20:647]000002789F1BB380
		WVLogger: Display: [18:14:20:647]00000278ACF79600
		WVLogger: Display: [18:14:20:647]000002789F1BB380
		WVLogger: Display: [18:14:20:647]00000278ACF79600
		WVLogger: Display: [18:14:20:647]ATestActor Class End

		//呢个先不理
		WVLogger: Display: [18:14:20:647]======loadObj
		WVLogger: Display: [18:14:20:648]00000278C30C5600
		WVLogger: Display: [18:14:20:648]00000278E0191D00
		WVLogger: Display: [18:14:20:648]00000278E06B9D00

		//这里又很神奇，loadClass加载TestActor的bp
		//出来的UClass地址跟bp的GetClass()一样，CDO也跟bp的CDO一样
		//莫非GetClass()就是在bp的处理里会有不同？？？
		WVLogger: Display: [18:14:20:648]======loadClass
		WVLogger: Display: [18:14:20:648]00000278C30C5D00
		WVLogger: Display: [18:14:20:648]00000278C2EA0400
	 */
	// GetWorld()->SpawnActor(ATestActor::StaticClass(), &FVector::ZeroVector, &FRotator::ZeroRotator);
	// WVLogI(TEXT("======loadObj"));
	// auto t = LoadObject<UObject>(nullptr, TEXT("Blueprint'/Game/Test/BP_TestActor.BP_TestActor'"));
	// if (t)
	// {
	// 	WVLogI(TEXT("%p"), t);
	// 	WVLogI(TEXT("%p"), t->StaticClass());
	// 	WVLogI(TEXT("%p"), t->GetClass());
	// }
	// WVLogI(TEXT("======loadClass"));
	// auto t2 = LoadClass<AActor>(nullptr, TEXT("Blueprint'/Game/Test/BP_TestActor.BP_TestActor_C'"));//加_C后才能加载
	// if (t2)
	// {
	// 	WVLogI(TEXT("%p"), t2);
	// 	WVLogI(TEXT("%p"), t2->GetDefaultObject());
	// }




	
	// for (TFieldIterator<UProperty> ite(ATestActor::StaticClass()); ite; ++ite)
	// {
	// 	UProperty *property = *ite;
	// 	FString name = property->GetNameCPP();
	// 	FString type = property->GetCPPType();
	// 	WVLogI(TEXT("%s : %s"), *name, *type)
	// }



	
	TArray<AActor*> tArr;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATestActor::StaticClass(), tArr);
	if (tArr.Num() > 0)
	{
		ATestActor* tmpActor = Cast<ATestActor>(tArr[0]);

		/**
		 * 下面3种形式都可以读写到某对象的属性
		 * 但注意 ATestActor::StaticClass() != tC
		 * 实际上LoadClass这种形式更能体现到反射机制
		 * 反射机制有个特点，一般都能通过字符串创建对象（指定个类名或者文件名类似...），而UE4里一般要创建UObject，AActor都要UClass，
		 * 而UClass一般也可以用资源引用路径来创建，有了UClass就是有了类型信息，那么就是一般只要有了UClass，就能创建对象，并且利用类型信息
		 * 使用字符串来操作属性和方法。总结就是从创建对象到使用该对象都可以通过字符串来操作。这是反射的一个特点，也是运行时类型系统带来的特点
		 * 所以使用UE4的c++时，要利用好这个反射的特点
		 */
		// UProperty *var_tmpStr = tmpActor->StaticClass()->FindPropertyByName(TEXT("TmpStr"));
		// UProperty *var_tmpStr = ATestActor::StaticClass()->FindPropertyByName(TEXT("TmpStr"));
		UClass *tC = LoadClass<AActor>(nullptr, TEXT("Blueprint'/Game/Test/BP_TestActor.BP_TestActor_C'"));
		UProperty *var_tmpStr = tC->FindPropertyByName(TEXT("TmpStr"));
		
		if (var_tmpStr)
		{
			WVLogI(*var_tmpStr->GetFullName());
			UStrProperty *tVar = Cast<UStrProperty>(var_tmpStr);
			if (tVar)
			{
				void* p = tVar->ContainerPtrToValuePtr<uint8>(tmpActor);
				auto r = tVar->GetPropertyValue(p);
				WVLogI(TEXT("get!!! %s"), *r);
				tVar->SetPropertyValue(p, TEXT("fuxk fuxk fuxk"));
			}
		}
	}

	
}

void UTestUserWidget::Btn_Test2_Callback()
{
	WVLogI(TEXT("UTestUserWidget::Btn_Test1_Callback"));
	
	// GetWorld()->SpawnActor(ATestActor::StaticClass(), &FVector::ZeroVector, &FRotator::ZeroRotator);

	uint8 n1 = static_cast<uint8>(EWVConfigName::Item);
	uint8 n2 = static_cast<uint8>(EWVConfigName::Shop);

	UWVConfigUtil::GetInstance()->ForeachRow<FWVConfig_ItemRow>(
		EWVConfigName::Item,
		[](const FName& key, const FWVConfig_ItemRow& value)
		{
			WVLogI(
				TEXT("%s_%d_%s_%s_%s"),
				*key.ToString(),
				value.id,
				*value.name,
				*value.desc,
				*value.icon.ToSoftObjectPath().ToString()
			)
		}
	);

	WVLogI(TEXT("%d, %d"), n1, n2);
}
