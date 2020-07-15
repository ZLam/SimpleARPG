#pragma once

#include "CoreMinimal.h"
#include "WVGameTypes.generated.h"

/**
 * gameplay 回调，无参数
 */
DECLARE_DYNAMIC_DELEGATE(FWVCallback_Null);

/**
 * gameplay 回调，单个bool参数
 */
DECLARE_DYNAMIC_DELEGATE_OneParam(FWVCallback_Bool, bool, bVal);

/**
 * gameplay 回调，单个int32参数
 */
DECLARE_DYNAMIC_DELEGATE_OneParam(FWVCallback_Int32, int32, num);

/**
 * gameplay 回调，单个int32参数
 */
DECLARE_DYNAMIC_DELEGATE_OneParam(FWVCallback_Float, float, num);

/**
 * gameplay 玩法里的资源类型枚举
 */
UENUM(BlueprintType)
enum class EWVGameResType : uint8
{
	Money		UMETA(DisplayName = "Money"),	//货币
	Item		UMETA(DisplayName = "Item"),	//道具
	MAX,
};

/**
 * gameplay 道具类型枚举
 */
UENUM(BlueprintType)
enum class EWVItemType : uint8
{
	Food			UMETA(DisplayName = "Food"),			//食物
	Medicine		UMETA(DisplayName = "Medicine"),		//药物
	Importantce		UMETA(DisplayName = "Importantce"),		//重要的东西
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

	/**
	 * 匹配时
	 * 1，当前时间和轴的LastTime的差在有效范围
	 * 2，当前轴的值和 -1 或 1 的差在有效范围，说明当前轴在适当范围确实指向F
	 * 1 && 2 就ok
	 */
	AxisF_WithLastTime			UMETA(DisplayName = "AxisF_WithLastTime"),
	AxisB_WithLastTime			UMETA(DisplayName = "AxisB_WithLastTime"),
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ValidDiff_AxisF;		//0.0 ~ 1.0

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ValidDiff_AxisB;		//0.0 ~ 1.0
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
 * gameplay 攻击动作slomo信息
 */
USTRUCT(BlueprintType)
struct FWVComboActionSlomoData
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Dilation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Duration;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Delay;
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bShake;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FWVComboActionSlomoData SlomoData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bNeedBounceSign;
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

/**
 * gameplay 玩法里的资源数据
 */
USTRUCT(BlueprintType)
struct FWVGameResData
{
	GENERATED_USTRUCT_BODY()
public:
	FWVGameResData() = default;

	FWVGameResData(EWVGameResType InGameResType, int32 InId = 0, int32 InNum = 0):
	GameResType(InGameResType),
	Id(InId),
	Num(InNum)
	{}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWVGameResType GameResType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Num;
};

/**
 * gameplay 创建MakeSureDialog传递的参数
 */
USTRUCT(BlueprintType)
struct FWVParams_CreateMakeSureDialog
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FWVCallback_Bool Callback_IsOk;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText TxtDesc;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText TxtOk;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText TxtCancel;
};

/**
 * gameplay 创建ConfirmDialog传递的参数
 */
USTRUCT(BlueprintType)
struct FWVParams_CreateConfirmDialog
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FWVCallback_Null Callback_Confirm;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText TxtDesc;
};

/**
 * gameplay 创建ChooseNumDialog传递的参数
 */
USTRUCT(BlueprintType)
struct FWVParams_CreateChooseNumDialog
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FWVCallback_Float Callback_ChooseNum;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MinNum;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxNum;
};

/**
 * gameplay Combo的每一步会发一下事件，暂时拿来看看按键而已
 */
USTRUCT(BlueprintType)
struct FWVParams_ComboStepExecute
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FKey> ComboMatchKeys;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<EWVInputMatchStyle> ComboMatchStyles;
};
