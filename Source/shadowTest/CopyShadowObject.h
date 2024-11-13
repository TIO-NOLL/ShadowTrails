/**
* @file		CopyShadowObject.h
* @brief	影のコピーを実現するための、影オブジェクト
* @author	22CU0219 鈴木友也
* @date		23/12/10 作成
*			23/02/01 最終更新
*/

#pragma once

#include "CoreMinimal.h"
#include "BoxDecal.h"
#include "CopyShadowObject.generated.h"

//丹代天：追記 2024/01/31------------------------------------------------------
// 前方宣言
class UEffectComponent;
//----------------------------------------------------------------------------

UCLASS()
class SHADOWTEST_API ACopyShadowObject : public ABoxDecal
{
	GENERATED_BODY()

public:
	// コンストラクタ
	ACopyShadowObject();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

public:
	// ハイライト状態に設定
	UFUNCTION(BlueprintCallable, Category = "CopyShadowObject")
		void SetHighLightMode(bool _flag);

	// ハイライト状態かどうか
	UFUNCTION(BlueprintCallable, Category = "CopyShadowObject")
		bool GetHighLightMode() { return m_bHighLight; }

	// 保持されているかどうか設定する
	UFUNCTION(BlueprintCallable, Category = "CopyShadowObject")
		void SetHold(bool _newFlag);

	// 動かされたかどうか設定
	UFUNCTION(BlueprintCallable, Category = "CopyShadowObject")
		void SetMoved(bool _flag);

	// 既に動かされたかどうか
	UFUNCTION(BlueprintCallable, Category = "CopyShadowObject")
		bool IsMoved() { return m_bIsMoved; }

	// プレイヤーと当たっているかどうか
	UFUNCTION(BlueprintCallable, Category = "CopyShadowObject")
		bool IsPlayerHit() { return m_bIsPlayerHit; }

	// 位置をBeginPlayの時に戻す
	UFUNCTION(BlueprintCallable, Category = "CopyShadowObject")
		void ResetPos() { SetActorTransform(m_DefaultTrans); }

	UFUNCTION(BlueprintCallable, Category = "CopyShadowObject")
		FRotator GetDefaultRotation() { return m_DefaultTrans.GetRotation().Rotator(); }

	UFUNCTION(BlueprintCallable, Category = "CopyShadowObject")
		void SetPawnCollisionEnable(bool _flag);

	// イベント関数
	UFUNCTION() // OverlapBegin
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION() // OverlapEnd
		void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	// マテリアルのダイナミックインスタンス
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UMaterialInstanceDynamic* m_pDecalMaterialInst;

	// ハイライト状態を切り替えるフラグ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CopyShadow|HighlightMode")
		bool m_bHighLight;

	// マテリアルのエミッシブのパラメータ設定値
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CopyShadow|HighlightMode")
		float m_emissiveValue;

	// マテリアルのエミッシブの色パラメータの設定値
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CopyShadow|HighlightMode")
		FLinearColor m_emissiveColor;

	// マテリアルのエミッシブの色パラメータの設定値(移動後)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CopyShadow|HighlightMode")
		FLinearColor m_emissiveColor_Moved;

	// 保持されているかどうか
	UPROPERTY(EditAnywhere, Category = "CopyShadow")
		bool m_bIsHold;

	// 動かされたかどうか
	UPROPERTY(EditAnywhere, Category = "CopyShadow")
		bool m_bIsMoved;

	// 現在プレイヤーと当たっているかどうか
	UPROPERTY(EditAnywhere, Category = "CopyShadow")
		bool m_bIsPlayerHit;

	// ゲーム開始時トランスフォーム
	UPROPERTY(VisibleAnywhere, Category = "CopyShadow")
		FTransform m_DefaultTrans;

	// プレイヤー参照
	AActor* m_pHitPlayer;

	//丹代天 追記：2024/01/31----------------------------------------------------------------------------------
		//コンポーネントを保持する変数
public:
	UPROPERTY(VisibleAnywhere, Category = "Components")
		UEffectComponent* EffectComponent;	//コンポーネントを保持する変数

private:
	UPROPERTY(EditAnywhere, Category = "Range")
		float m_ActivateRange;		//エフェクトが起動する距離

private:
	AActor* m_pPlayer;										//プレイヤーのポインタ（座標取得）

	bool m_bNowEffectVisibility;							//エフェクトが再生されているかどうかを判断するフラグ

	float m_PlayerDistance;									//プレイヤーとの距離を格納する変数

	void MeasurementToPlayer();								//プレイヤーとの距離を測定する関数
	//--------------------------------------------------------------------------------------------------------
};