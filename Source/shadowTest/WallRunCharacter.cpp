//------------------------------------------------
// ファイル名：WallRunCharacter.cpp
// 処理内容	：壁を走ることのできるキャラクター
// 作成者	：丹代天
// 更新日	：12/04		作成
//------------------------------------------------


#include "WallRunCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "shadowTestGameMode.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerCharacter.h"
#include "DrawDebugHelpers.h"

// コンストラクタ
AWallRunCharacter::AWallRunCharacter()
	: m_pSpringArm(NULL)
	, m_pCamera(NULL)
	, m_pGenerateCharacter(ACharacter::StaticClass())
	, m_charaMoveInput(FVector::ZeroVector)
	, m_cameraRotateInput(FVector::ZeroVector)
	, m_cameraPitchLimit(FVector2D(-70.f, -15.f))
	, m_CameraSensitivity(0.f)
	, m_moveSpeed(10.f)
	, m_RayLength(20.f)
	, m_bCanControl(true)
{
	// このアクターのティックを回すかどうか
	PrimaryActorTick.bCanEverTick = true;

	//スプリングアームのオブジェクト生成
	m_pSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("m_pSpringArm"));
	if (m_pSpringArm != NULL)
	{
		m_pSpringArm->SetupAttachment(RootComponent);
		//カメラのコリジョンテストを行うか設定
		m_pSpringArm->bDoCollisionTest = true;
		//カメラ追従ラグを使うか設定
		m_pSpringArm->bEnableCameraLag = false;
		//カメラ追従ラグの速度を設定
		m_pSpringArm->CameraLagSpeed = 10.f;
		//カメラ回転ラグを使うか設定
		m_pSpringArm->bEnableCameraRotationLag = false;
		//カメラ回転ラグの速度を設定
		m_pSpringArm->CameraRotationLagSpeed = 10.f;
	}

	//カメラのオブジェクトを生成
	m_pCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("m_pCamera"));
	if ((m_pCamera != NULL) && (m_pSpringArm != NULL))
	{
		//カメラをスプリングアームにアタッチ
		m_pCamera->SetupAttachment(m_pSpringArm, USpringArmComponent::SocketName);
	}
}

// 生成時に呼ばれる処理
void AWallRunCharacter::BeginPlay()
{
	Super::BeginPlay();

	//キャラの移動方法を「浮遊」にする
	GetCharacterMovement()->SetMovementMode(MOVE_Flying);
}

// フレームごとに呼ばれる処理
void AWallRunCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateMove(DeltaTime);

	UpdateCamera(DeltaTime);

	UpdateRay();
}

// インプットキーとのバインド
void AWallRunCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//カメラ回転
	InputComponent->BindAxis("CameraPitch", this, &AWallRunCharacter::Cam_RotatePitch);
	InputComponent->BindAxis("CameraYaw", this, &AWallRunCharacter::Cam_RotateYaw);

	//移動
	InputComponent->BindAxis("MoveForward", this, &AWallRunCharacter::Chara_MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AWallRunCharacter::Chara_MoveRight);
}

//カメラ更新処理
void AWallRunCharacter::UpdateCamera(float _deltaTime)
{
	//ルートオブジェクトを中心に、スプリングアームについているカメラを回転させる
	if (m_pSpringArm != NULL)
	{
		//現在のFPSを測定
		float fps = 1.0f / _deltaTime;

		//処理落ちしても、一定速度でカメラが周るように補正
		float rotateCorrection = 60.f / fps;

		//カメラの新しい角度を求める
		//現在の角度を取得
		FRotator NewRotation = m_pSpringArm->GetRelativeRotation();

		//Yawは入力した分回す
		NewRotation.Yaw += m_cameraRotateInput.X * rotateCorrection;

		//Pitchを上下制限角度の範囲内で切る
		NewRotation.Pitch = FMath::Clamp(NewRotation.Pitch + (m_cameraRotateInput.Y * rotateCorrection), m_cameraPitchLimit.X, m_cameraPitchLimit.Y);

		//新しい角度を反映
		m_pSpringArm->SetRelativeRotation(NewRotation);
	}
}

//移動処理
void AWallRunCharacter::UpdateMove(float _deltaTime)
{
	//移動処理がある場合
	if (!m_charaMoveInput.IsZero())
	{
		//コントロール可能な場合にのみ
		if (m_bCanControl == false) { return; }

		//入力に合わせて、Actorを前後左右に移動
		if (m_pSpringArm != NULL)
		{
			FVector NewLocation = GetActorLocation();

			//キャラクターの移動
			{
				//ほかのオブジェクトに影響されず、Z軸方向と平行に動く
				const FVector upVec = FVector(0.f, 0.f, 1.f);
				AddMovementInput(upVec, m_charaMoveInput.Y * m_moveSpeed);
				const FVector rightVec = GetActorRightVector();
				AddMovementInput(rightVec, m_charaMoveInput.X * m_moveSpeed);
			}

			////移動するキャラクターの回転
			//{
			//	//角度はDegreeの角度の範囲で表記(-180°～180°)
			//	USkeletalMeshComponent* pMeshComp = GetMesh();
			//	if (pMeshComp != NULL)
			//	{
			//		//アークタンジェントを使ってコントローラーの入力方向がなす角度を求める
			//		float angle = atan2(m_charaMoveInput.X, m_charaMoveInput.Y);
			//		//Radian値をDegreeに変換
			//		float angleDeg = FMath::RadiansToDegrees(angle);

			//		//入力した角度 + メッシュの回転速度 + Actorに対して回転しているSpringArmの相対角度
			//		float newYaw = angleDeg + GetBaseRotationOffsetRotator().Yaw + m_pSpringArm->GetRelativeRotation().Yaw;

			//		pMeshComp->SetRelativeRotation(FRotator(pMeshComp->GetRelativeRotation().Pitch, newYaw, pMeshComp->GetRelativeRotation().Roll));
			//	}
			//}
		}
	}
}

//レイを飛ばす関数
void AWallRunCharacter::UpdateRay()
{
	//コントロール可能な場合のみ
	if (m_bCanControl == false) { return; }

	//このオブジェクトのメッシュを取得する
	USkeletalMeshComponent* pMeshComp = GetMesh();

	if (pMeshComp != NULL)
	{
		//プレイヤーのメッシュサイズを取得する
		USkeletalMeshComponent* pPlayerMesh = this->GetMesh();
		FVector playerMeshSize = pPlayerMesh->Bounds.GetBox().GetSize() * this->GetActorScale();

		//レイを飛ばす
		//始点 Start：Actorの背中
		FVector Start = GetActorLocation() + (-GetActorUpVector()  * (playerMeshSize / 2));
		//終点 End：Startから前方向m_ActionForwardLengthの距離
		FVector End = GetActorLocation() + (-GetActorUpVector() * (playerMeshSize / 2)) + pMeshComp->GetUpVector() * -1.f * m_RayLength;

		//デバッグ用にラインを描画
		DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 1.f);

		//コリジョン判定で無視する項目を指定
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(this);

		//レイにヒットしたオブジェクトを格納する変数
		FHitResult RayHit;

		//ヒットした際にtrueになるフラグ,
		bool isHit = GetWorld()->LineTraceSingleByChannel(RayHit, Start, End, ECC_WorldStatic, CollisionParams);

		//ヒットしたオブジェクトがなくなった場合
		if (!isHit)
		{
			// スポーン位置を指定
			FVector SpawnLocation = GetActorLocation();

			// スポーン時の回転を指定
			FRotator SpawnRotation(GetActorRotation());

			// スポーンするアクターのTransformを作成
			FTransform SpawnTransform(SpawnRotation, SpawnLocation);

			//このアクターの座標にアクターを生成する
			APlayerCharacter* SpawnActor = GetWorld()->SpawnActor<APlayerCharacter>(m_pGenerateCharacter, SpawnTransform); // スポーン処理

			if (SpawnActor)
			{
				//ShadowGameMode内のSwitchCharacter関数を呼び出し、プレイヤーキャラクターを切り替える
				AGameModeBase* CurrentGameMode = UGameplayStatics::GetGameMode(GetWorld());
				AshadowTestGameMode* shadowTestGameMode = Cast<AshadowTestGameMode>(CurrentGameMode);

				if (shadowTestGameMode)
				{
					//shadowTestGameMode->SwitchCharacter(SpawnActor);
				}
				else
				{
					// ゲームモードが取得できなかった場合のエラーハンドリング
					UE_LOG(LogTemp, Error, TEXT("Failed to get AShadowGameMode"));
				}
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Failed to Generate SpawnActor"));
			}
		}
	}
}

//【入力バインド】カメラ回転：Pitch(Y軸)
void AWallRunCharacter::Cam_RotatePitch(float _axisValue)
{
	//コントロール可能な場合のみ
	if (m_bCanControl == false) { return; }

	m_cameraRotateInput.Y = _axisValue * m_CameraSensitivity;
}

//【入力バインド】カメラ回転：Yaw(Z軸)
void AWallRunCharacter::Cam_RotateYaw(float _axisValue)
{
	//コントロール可能な場合のみ
	if (m_bCanControl == false) { return; }

	m_cameraRotateInput.X = _axisValue * m_CameraSensitivity;
}

//【入力バインド】キャラ移動：上下
void AWallRunCharacter::Chara_MoveForward(float _axisValue)
{
	//コントロール可能な場合のみ
	if (m_bCanControl == false) { return; }

	m_charaMoveInput.Y = FMath::Clamp(_axisValue, -1.0f, 1.0f) * 0.5f;
}

//【入力バインド】キャラ移動：左右
void AWallRunCharacter::Chara_MoveRight(float _axisValue)
{
	//コントロール可能な場合または箱を押していないとき
	if (m_bCanControl == false) { return; }

	m_charaMoveInput.X = FMath::Clamp(_axisValue, -1.0f, 1.0f) * 0.5f;
}