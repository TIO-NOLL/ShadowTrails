
//------------------------------------------------
// ファイル名：PlayerCharacterBase.h
// 処理内容	：Playerの移動とカメラの操作をするベースキャラクタークラス
// 作成者	：丹代天
// 更新日	：12/08		作成
//			　12/10		カメラの制御、移動処理、操作可能フラグ切り替え、状態切り替えを追加
//------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ActionObjectBase.h"
#include "PlayerCharacterBase.generated.h"

//クラスの前方宣言
class USpringArmComponent;
class UCameraComponent;
class AWallRunCharacter;

UCLASS()
class SHADOWTEST_API APlayerCharacterBase : public ACharacter
{
	//プレイヤー状態
	enum PLAYER_STATE
	{
		normal = 0,		//通常時
		damage,			//光に当たっているとき
		dead			//死亡時
	};

	//レイにヒットしたActorの情報を格納し、戻り値として返すための構造体
	struct RayInfo
	{
		FHitResult HitActor;
		FVector NormalVector;
		FVector HitPoint;
		bool HitFlag;
	};


	GENERATED_BODY()

public:
	// コンストラクタ
	APlayerCharacterBase();

protected:
	// 生成時の処理
	virtual void BeginPlay() override;

public:
	// フレームごとの処理
	virtual void Tick(float DeltaTime) override;

	// インプットバインド
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	//カメラの更新処理
	void UpdateCamera(float _deltaTime);
	//移動の更新処理
	void UpdateMove(float _deltaTime);
	//箱を押す際、箱の手前まで移動する関数
	void PositionCorrect();

private:
	//【入力バインド】カメラ回転：Pitch(Y軸)
	void Cam_RotatePitch(float _axisValue);
	//【入力バインド】カメラ回転：Yaw(Z軸)
	void Cam_RotateYaw(float _axisValue);
	//【入力バインド】カメラをロックする
	void Cam_MoveLock();
	//【入力バインド】カメラのロックを解除する
	void Cam_MoveUnlock();

	//【入力バインド】キャラクター移動：前後
	void Chara_MoveForward(float _axisValue);
	//【入力バインド】キャラクター移動：前後
	void Chara_MoveRight(float _axisValue);

	//【入力バインド】アクション：ギミック起動
	void Chara_Action();
	//【入力バインド】アクション：壁走り
	void Chara_WallRun();


public:
	//プレイヤーの状態を変更する関数
	//normal
	UFUNCTION(BlueprintCallable, Category = "PlayerCharacterBase")
		void StateChangeNormal();
	//damage
	UFUNCTION(BlueprintCallable, Category = "PlayerCharacterBase")
		void StateChangeDamage();
	//dead
	UFUNCTION(BlueprintCallable, Category = "PlayerCharacterBase")
		void StateChangeDead();

	//操作可能フラグを切り替える関数
	UFUNCTION(BlueprintCallable, Category = "PlayerCharacterBase")
		void SetCanMove(bool _flag);

	// 22cu0222 田中 追記：2024/02/13--------------------------
	// プレイヤーの移動量を減少させる量を格納する関数
	UFUNCTION(BlueprintCallable, Category = "PlayerCharacterBase")
		void ChangeSpeed(float m_Speed);
	// -------------------------------------------------------

	// 22CU0219 鈴木 追記:2024/02/16-------------------
	// コンポーネント相対回転をリセットする関数
	UFUNCTION(BlueprintCallable, Category = "PlayerCharacterBase")
		void ResetRelativeRotation();

	UFUNCTION(BlueprintCallable, Category = "PlayerCharacterBase")
		void SetPlayerRotation(FRotator _rotation);
	//-------------------------------------------------

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
	UPROPERTY(EditAnywhere, Category = "Move")
		float m_DecreaseRateofMove;		//モノを押している際の移動量減少値
	UPROPERTY(EditAnywhere, Category = "Move")
		float m_InterpolationSpeed;		//回転速度

	// 22cu0222 田中 追記：2024/02/13 -----------------------------------
	UPROPERTY(EditAnywhere, Category = "Move")
		float m_ChangeSpeed;        // 変更した移動量を格納
	// -----------------------------------------------------------------

	//UPROPERTY(EditAnywhere, Category = "Action")
	//	float m_CorrectTime;				//補正のために移動する時間
	//
	//float m_CorrectCnt;						//補正のために移動する時間(加算)
	//float m_Distance;						//補正のために１フレーム当たりでの移動距離

	UPROPERTY(EditAnywhere, Category = "Action")
		float m_ActionForwardLength;		//ギミック起動用レイを飛ばす距離

	FVector m_PlayerCorrectPosition;	//箱を押す際に移動する座標
	//FVector m_CorrectPositionVector;	//箱を押す際に移動するベクトル
	//FVector m_ToCorrectPositionDistnce;	//移動距離を求めるための変数

	AActionObjectBase* m_pActionObject;		//起動可能なギミックの情報を格納する変数

	bool m_bCanControl;				//操作可能な状態か
	bool m_bCanWallRunning;			//壁走りが可能かどうかを知らせるためのフラグ

	UPROPERTY(EditAnywhere, Category = "Generate")
		TSubclassOf<AWallRunCharacter> m_pGenerateCharacter;		//生成するキャラクター

	PLAYER_STATE m_state;			//プレイヤーの状態

	//レイを飛ばし、レイに触れたアクターを返す関数
	RayInfo FlyRay();

	bool m_bCameraLock;

public:
	bool m_bIsBoxPush;				//現在箱を押しているか
};
