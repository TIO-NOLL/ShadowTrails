//---------------------------------------------------------------------------
// ファイル名		：ActionObject_Box.h
// 概要				：箱型の被アクションオブジェクト、アクションコマンドが実行された際には動かすことができる
// 作成者			：22cu0127　丹代 天
// 更新内容			：2023/12/14　作成
//---------------------------------------------------------------------------

//インクルードガード
#pragma once

//インクルード
#include "CoreMinimal.h"
#include "ActionObjectBase.h"		//ActionObjectBase継承
#include "ActionObject_Box.generated.h"

//前方宣言
class USkeletalMeshComponent;
class UStaticMesh;

UCLASS()
class SHADOWTEST_API AActionObject_Box : public AActionObjectBase
{
	GENERATED_BODY()

public:
	//コンストラクタ
	AActionObject_Box();

protected:
	//ゲームスタート時、または生成時に呼ばれる処理
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

private:
	//プレイヤーに押されている間のみ、プレイヤーの行動に合わせて動かす関数
	void IsPushed(float _deltaTime);

private:
	//プレイヤーのアドレスを検索する関数
	void GetPlayerActor(FVector PlayerPos);

public:
	//AActionObjectBaseの仮想関数をオーバーライドする
	//共通してPlayer側から呼び出される
	virtual void OnActionthisObject()override;

	//このオブジェクトとプレイヤーオブジェクトの相対角度を、Xを基線として求め、それを返す関数
	//Player側から呼び出される
	float GetAnglePlayer(FVector PlayerPos);

private:
	//位置補正中か判断する変数
	bool m_bNowCorrect;
	//このオブジェクトが動かされているかどうか判断するフラグ
	bool m_bIsPushed;

	AActor* m_pPlayer;				//プレイヤーのポインタ
	UStaticMesh* m_pStaticMesh;		//スタティックメッシュ
	FVector m_meshSize;				//スタティックメッシュのサイズ

	////起動する時間を受け取る変数
	//float m_CorrectTime;
	////加算し、上記の変数と比較するための変数
	//float m_CorrectCnt;

public:
	/*UFUNCTION()
		void SetTime(float _CorrectTime);*/
};
