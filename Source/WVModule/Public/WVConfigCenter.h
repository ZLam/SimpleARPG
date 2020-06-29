#pragma once

#include "CoreMinimal.h"
#include "InputCoreTypes.h"
#include "Engine/DataTable.h"
#include "WVGameTypes.h"
#include "WVConfigCenter.generated.h"

class UTexture2D;
class UAnimMontage;

/**
 * DataTable配置枚举
 * @REMIND 这里加了，UWVConfigUtil::Init也要加
 */
UENUM(BlueprintType)
enum class EWVConfigName : uint8
{
	Locomotion		UMETA(DisplayName = "Locomotion"),		//角色的基本运动
	Attribute		UMETA(DisplayName = "Attribute"),		//角色的属性
	Combo			UMETA(DisplayName = "Combo"),			//角色的连招
	ComboAction		UMETA(DisplayName = "ComboAction"),		//角色每个攻击动作的配置
	Item			UMETA(DisplayName = "Item"),			//道具
	Shop			UMETA(DisplayName = "Shop"),			//商店
	ShopGood		UMETA(DisplayName = "ShopGood"),		//商店商品
	MAX,
};

/**
 * DataTable配置表头
 */

/**
 * Locomotion配置表头
 */
USTRUCT(BlueprintType)
struct FWVConfig_LocomotionRow : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	FWVConfig_LocomotionRow():
	RunSpeed(600.0f),
	SprintSpeed(800.0f),
	SprintCostPerSecond(10.0f),
	DodgeCost(30.0f),
	RecoverPowerTime(1.0f),
	DodgeCapsuleHalfHeight(50.0f)
	{}
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RunSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SprintSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SprintCostPerSecond;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DodgeCost;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RecoverPowerTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DodgeCapsuleHalfHeight;
};

/**
 * Attribute配置表头
 */
USTRUCT(BlueprintType)
struct FWVConfig_AttributeRow : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	FWVConfig_AttributeRow():
	Power(100.0f),
	HP(100.0f),
	Straight(100.0f),
	Down(100.0f)
	{}
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Power;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Straight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Down;
};

/**
 * Combo配置表头
 */
USTRUCT(BlueprintType)
struct FWVConfig_ComboMatchKeys
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FKey> MatchKeys;
};

USTRUCT(BlueprintType)
struct FWVConfig_ComboMatchStyle
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<EWVInputMatchStyle> MatchStyles;
};

USTRUCT(BlueprintType)
struct FWVConfig_ComboMatchData
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FWVInputMatchData> MatchDatas;
};

USTRUCT(BlueprintType)
struct FWVConfig_ComboInfo
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FWVConfig_ComboMatchKeys> ComboKeys;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int32> ComboPrioritys;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UAnimMontage> ComboMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<float> Rates;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> Sections;								//Montage可以用Section或者StartPosition控制播放开始位置，两者选其一，优先Section
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<float> StartPositions;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FWVConfig_ComboMatchStyle> ComboMatchStyles;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FWVConfig_ComboMatchData> ComboMatchDatas;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FString> ComboActionNames;
};

USTRUCT(BlueprintType)
struct FWVConfig_ComboRow : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FString, FWVConfig_ComboInfo> ComboInfoMap;
};

/**
 * 角色每个攻击动作配置表头
 */
USTRUCT(BlueprintType)
struct FWVConfig_ComboActionRow : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FString, FWVComboActionData> ComboActionInfoMap;
};

/**
 * 道具配置表头
 */
USTRUCT(BlueprintType)
struct FWVConfig_ItemRow : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	FWVConfig_ItemRow():
	id(0),
	type(EWVItemType::Importantce),
	quality(EWVQualityType::White),
	icon(nullptr)
	{}
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWVItemType type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWVQualityType quality;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText desc;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UTexture2D> icon;
};

/**
 * 商店配置表头
 */
USTRUCT(BlueprintType)
struct FWVConfig_ShopRow : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> idArr;
};

/**
 * 商店商品配置表头
 */
USTRUCT(BlueprintType)
struct FWVConfig_ShopGoodsRow : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FWVGameResData ItemData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FWVGameResData costData;
};
