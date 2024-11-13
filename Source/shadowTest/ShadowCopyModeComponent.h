/**
* @file		ShadowCopyModeComponent.h
* @brief	影コピーモードを実装するコンポーネント
* @author	22CU0219 鈴木友也
* @date		23/12/10 作成
*			23/02/01 最終更新
*/
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

// 22cu0327 千葉涼太郎 ---------------------------------------------------------
#include "ActorFollowingUserWidget.h"
#include "MouseUserWidget.h"
// -----------------------------------------------------------------------------

#include "ShadowCopyModeComponent.generated.h"

// 前方宣言
class APlayerController;
class ACopyShadowObject;

// 22cu0327 千葉涼太郎 追記：2024/02/09-----------------------------------------
class UActorFollowingUserWidget;
class UMouseUserWidget;
// -----------------------------------------------------------------------------

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SHADOWTEST_API UShadowCopyModeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// コンストラクタ
	UShadowCopyModeComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UFUNCTION(BlueprintCallable, Category = "ShadowCopyModeComponent")
		void Hold(bool _flag);

	UFUNCTION(BlueprintCallable, Category = "ShadowCopyModeComponent")
		bool IsHold() { return m_bIsHolding; }

	UFUNCTION(BlueprintCallable, Category = "ShadowCopyModeComponent")
		void AddYawRotation(float _deltaDegree);

	UFUNCTION(BlueprintCallable, Category = "ShadowCopyModeComponent")
		void ResetShadowPos();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		APlayerController* m_pPlayerController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShadowCopyModeComponent", meta = (AllowPrivateAccess = "true"))
		bool m_bIsHolding;

	// カーソル下にレイを飛ばす長さ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShadowCopyModeComponent", meta = (AllowPrivateAccess = "true"))
		float m_UnderCursorTraceLength;

	// 影オブジェクトのYaw方向回転
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShadowCopyModeComponent", meta = (AllowPrivateAccess = "true"))
		float m_addYaw;

	void GetUnderCursorShadow();

	void ProcStandardNormalBaseRotation(FVector _newNormal, FRotator _rotation);

	ACopyShadowObject* m_pShadowObj;	// 影オブジェクト

	FRotator m_holdingShadowRotation;	// 

	FVector m_prevNormal;	 // 1フレーム前の回転を保持

	// 22cu0327 千葉涼太郎 ----------------------------------------------------------
	
	UActorFollowingUserWidget* m_pUI;
	UMouseUserWidget* m_pMouseUI;

	// Tickで一度だけ実行するためのフラグ
	bool m_bDoOnce_LeftClick;
	bool m_bDoOnce_Rotate;
	bool m_bDoOnce_Return;

	USoundBase* m_pPasteSound;
	USoundBase* m_pResetSound;

	// -----------------------------------------------------------------------------
};
