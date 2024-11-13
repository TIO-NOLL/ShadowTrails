//------------------------------------------------
// ファイル名：PlayerCharacter.h
// 処理内容	：Playerのキャラクタークラス
// 作成者	：丹代天
// 更新日	：12/11		作成
//------------------------------------------------
#pragma once

#include "CoreMinimal.h"
#include "PlayerCharacterBase.h"
#include "PlayerCharacter.generated.h"

/**
 *
 */
UCLASS()
class SHADOWTEST_API APlayerCharacter : public APlayerCharacterBase
{
	GENERATED_BODY()


public:
	//コンストラクタ
	APlayerCharacter();

protected:
	//生成時処理
	virtual void BeginPlay() override;

public:
	// フレームごとの処理
	virtual void Tick(float DeltaTime) override;

	// 入力バインド処理
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	// ============================================================================
	// 影
	// 22cu0219 鈴木が記述
	// -----------------------------------------------------------
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UShadowCopyModeComponent* m_pShadowComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UEffectComponent* m_pDeadEffectComponent;

	UPROPERTY(VisibleAnywhere)
		class UMaterialInstanceDynamic* m_pMeshMaterial;

	UPROPERTY(EditAnywhere, Category = "Cut&Paste")
		float m_RotateDeglee;

	UPROPERTY(VisibleAnywhere, Category = "Cut&Paste")
		bool m_bClockwisePress;

	UPROPERTY(VisibleAnywhere, Category = "Cut&Paste")
		bool m_bCounterClockwisePress;


	//【入力バインド】影カット・ペースト
	void CutPaste();

	//【入力バインド】影をYaw軸の時計回りに回転：キーのDown/Upでフラグを切り替える
	void SwapClockwisePressReleased() { m_bClockwisePress = !m_bClockwisePress; }

	//【入力バインド】影をYaw軸の反時計回りに回転：キーのDown/Upでフラグを切り替える
	void SwapCounterClockwisePressReleased() { m_bCounterClockwisePress = !m_bCounterClockwisePress; }

	//【入力バインド】影の回転：マウスホイールの回転
	void RotateShadow(float _axisValue);

	// 【入力バインド】影を初期位置に戻す
	void ResetShadowPos();

	// ======================================================================================
public:
	// 死亡キャラをリセット
	UFUNCTION(BlueprintCallable, Category = "PlayerCharacter")
		void ResetPlayer();
	
	UFUNCTION(BlueprintCallable, Category = "PlayerCharacter")
		void ResetDeadCounter() { m_DeadTimeCounter = 0.f; }
private:
	// 死亡時フラグ
	UPROPERTY(VisibleAnywhere, Category = "LightHit")
		bool m_bDead;

	//光に当たって死亡するまでの秒数
	UPROPERTY(EditAnywhere, Category = "LightHit")
		float m_DeadTime;

	//光に当たって死亡するまでの秒数
	UPROPERTY(EditAnywhere, Category = "LightHit")
		float m_DeadTimeCounter;

	//移動速度減少値
	//光に当たっている間増加していき、だんだんと移動速度が遅くなる
	UPROPERTY(EditAnywhere, Category = "LightHit")
		float m_DownSpeed;

	// 光に当たっているかどうかのフラグ
	UPROPERTY(EditAnywhere, Category = "LightHit")
		uint8 m_hittingLightNum;

	// 現在当たっている影のカウント
	UPROPERTY(EditAnywhere, Category = "LightHit")
		uint8 m_hittingShadowNum;

	// 22cu0327 千葉涼太郎 2024/02/08-------------------------------------------
	// ダメージを受けているかどうかのフラグ
	UPROPERTY(EditAnywhere, Category = "LightHit")
		bool m_bDamage;
	// -------------------------------------------------------------------------

	// ----田中　記述 ------
	// プレイヤー点滅時間計算用
	UPROPERTY(EditAnywhere, Category = "Flash")
		float m_FlashDuration;

	// 点滅インターバル
	UPROPERTY(EditAnywhere, Category = "Flash")
		float m_FlashInterval;

	// プレイヤー表示時のFlag
	UPROPERTY(EditAnywhere, Category = "Flash")
		bool m_bPlayerVisible;

	// プレイヤー点滅処理
	UFUNCTION()
		void PlayerFlashing(float DeltaTime);

	// イベント関数
	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// 22cu0327 千葉涼太郎 2024/02/08-------------------------------------------
	UFUNCTION(BlueprintCallable, Category = "Widget")
		bool GetbDamage();
	// -------------------------------------------------------------------------

	// 22cu0127 丹代天 2024/03/04-----------------------------------------------
	float m_ShadowRotateValue;
	// -------------------------------------------------------------------------
};
