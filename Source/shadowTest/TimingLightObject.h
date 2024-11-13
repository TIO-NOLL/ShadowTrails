//---------------------------------------------------------------------- -
//ファイル名：TimingLightObject.h
// 処理内容	：点灯と消灯を一定の周期で繰り返すライトを管理するおbジェクト
// 作成者	：丹代天
// 更新日	：02/11		作成
//-----------------------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "LightObjectBase.h"
#include "TimingLightObject.generated.h"

//前方宣言
class UEffectComponent;
class UDecalComponent;

// 構造体の定義
USTRUCT(BlueprintType)
struct FSwitchTime
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, Category = "Time")
		float m_OfftoPreparation;		//消灯状態から準備状態までに要する時間

	UPROPERTY(EditAnywhere, Category = "Time")
		float m_PreparationtoOn;		//準備状態から点灯状態までに要する時間

	UPROPERTY(EditAnywhere, Category = "Time")
		float m_OntoOff;		//点灯状態から消灯状態までに要する時間

	UPROPERTY(EditAnywhere, Category = "Cavate")
		int m_Cavatefrequency;	//警告が発生する回数

	float TimeCnt;				//上記の時間と比較するカウント
	float CavateCnt;			//警告時に使用するカウント

	int PlayNum;				//再生した回数
};

UCLASS()
class SHADOWTEST_API ATimingLightObject : public ALightObjectBase
{

	GENERATED_BODY()

public:
	ATimingLightObject();

	//警報用のエフェクト
	UPROPERTY(VisibleAnywhere, Category = "Components")
		UEffectComponent* m_pEffectComponent;

protected:
	// 生成時の処理
	virtual void BeginPlay() override;

	//フレームごとの処理
	virtual void Tick(float DeltaTime)override;

	//プレーヤーのステート
	enum LIGHT_STATE
	{
		TURN_OFF = 0,
		PREPARATION,
		TURN_ON
	};

	//ステート管理
	LIGHT_STATE m_State;

private:
	//ライトのオンオフを切り替える処理
	void LightSwitch(bool Lightstate);

	//時間を格納する構造体
	UPROPERTY(EditAnywhere, Category = "Time")
		FSwitchTime m_FSwitchTime; // 構造体をプロパティとして使用
};
