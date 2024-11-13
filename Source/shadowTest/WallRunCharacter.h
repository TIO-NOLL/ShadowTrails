//------------------------------------------------
// ファイル名：WallRunCharacter.h
// 処理内容	：壁を走ることのできるキャラクター
// 作成者	：丹代天
// 更新日	：12/04		作成
//------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.h"
#include "WallRunCharacter.generated.h"

//前方宣言
class USpringArmComponent;
class UCameraComponent;
class APlayerCharacter;

UCLASS()
class SHADOWTEST_API AWallRunCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// コンストラクタ
	AWallRunCharacter();

protected:
	// 生成時に呼ばれる処理
	virtual void BeginPlay() override;

public:
	// フレームごとに呼ばれる処理
	virtual void Tick(float DeltaTime) override;

	// インプットキーとのバインド
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	//カメラの更新処理
	void UpdateCamera(float _deltaTime);
	//移動処理
	void UpdateMove(float _deltaTime);
	//レイを飛ばす処理
	void UpdateRay();

private:
	//【入力バインド】カメラ回転：Pitch(Y軸)
	void Cam_RotatePitch(float _axisValue);
	//【入力バインド】カメラ回転：Yaw(Z軸)
	void Cam_RotateYaw(float _axisValue);

	//【入力バインド】キャラクター移動：前後
	void Chara_MoveForward(float _axisValue);
	//【入力バインド】キャラクター移動：前後
	void Chara_MoveRight(float _axisValue);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		USpringArmComponent* m_pSpringArm;		//スプリングアーム

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		UCameraComponent* m_pCamera;			//カメラ

	FVector2D m_charaMoveInput;		//キャラクター移動量
	FVector2D m_cameraRotateInput;	//カメラ回転入力量


	UPROPERTY(EditAnywhere, Category = "Camera")
		FVector2D m_cameraPitchLimit;	//カメラのピッチ範囲
	UPROPERTY(EditAnywhere, Category = "Camera")
		float m_CameraSensitivity;		//カメラ感度
	UPROPERTY(EditAnywhere, Category = "Move")
		float m_moveSpeed;			//移動量

	UPROPERTY(EditAnywhere, Category = "Ray")
		float m_RayLength;			//レイを飛ばす距離

	UPROPERTY(EditAnywhere, Category = "Generate")
		TSubclassOf<APlayerCharacter> m_pGenerateCharacter;

	bool m_bCanControl;				//操作可能フラグ
};
