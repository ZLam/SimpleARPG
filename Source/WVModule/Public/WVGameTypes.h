#pragma once

#include "CoreMinimal.h"
#include "WVGameTypes.generated.h"

/**
 * gameplay 道具类型枚举
 */
UENUM(BlueprintType)
enum class EWVItemType : uint8
{
	Money		UMETA(DisplayName = "Money"),	//货币
	Item		UMETA(DisplayName = "Item"),	//道具
	MAX,
};

/**
 * gameplay 品质枚举
 */
UENUM(BlueprintType)
enum class EWVQualityType : uint8
{
	White		UMETA(DisplayName = "White"),
	Green		UMETA(DisplayName = "Green"),
	Blue		UMETA(DisplayName = "Blue"),
	Purple		UMETA(DisplayName = "Purple"),
	Orange		UMETA(DisplayName = "Orange"),
	MAX,
};

/**
 * gameplay InputBuffer各种匹配方式
 */
UENUM(BlueprintType)
enum class EWVInputMatchStyle : uint8
{
	/**
	 * 匹配时，只要当前时间和按键的LastTime的差在有效范围就ok
	 */
	OnlyLastTime				UMETA(DisplayName = "OnlyLastTime"),

	/**
	 * 匹配时
	 * 1，当前时间和轴的LastTime的差在有效范围
	 * 2，当前轴的值和 -1 或 1 的差在有效范围，说明当前轴在适当范围确实指向L或R
	 * 1 && 2 就ok
	 */
	AxisLR_WithLastTime			UMETA(DisplayName = "AxisLR_WithLastTime"),
};

/**
 * gameplay InputBuffer匹配会用到的数据
 */
USTRUCT(BlueprintType)
struct FWVInputMatchData
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ValidDiff_LastTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ValidDiff_AxisL;		//0.0 ~ 1.0

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ValidDiff_AxisR;		//0.0 ~ 1.0
};

/**
 * gameplay 硬直类型枚举
 */
UENUM(BlueprintType)
enum class EWVStraightType : uint8
{
	Back		UMETA(DisplayName = "Back"),	//后退
	Bounce		UMETA(DisplayName = "Bounce"),	//弹开
	Soar		UMETA(DisplayName = "Soar"),	//击飞		//@TODO
	MAX,
};

/**
 * gameplay 硬直信息
 */
USTRUCT(BlueprintType)
struct FWVStraightData
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWVStraightType StraightType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AddStraight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AddDown;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BackDistance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BounceDistance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SoarDistance;
};

/**
 * gameplay 攻击动作信息
 */
USTRUCT(BlueprintType)
struct FWVComboActionData
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FWVStraightData StraightData;
};

/**
 * gameplay 角色状态枚举
 */
UENUM(BlueprintType)
enum class EWVActionCharacterState : uint8
{
	Relax			UMETA(DisplayName = "Relax"),		//@TODO
	ReadyAtk		UMETA(DisplayName = "ReadyAtk"),	//准备好攻击
	Atking			UMETA(DisplayName = "Atking"),		//攻击中
	Straight		UMETA(DisplayName = "Straight"),	//硬直状态
	Down			UMETA(DisplayName = "Down"),		//Down状态
	Die				UMETA(DisplayName = "Die"),			//死亡
};
