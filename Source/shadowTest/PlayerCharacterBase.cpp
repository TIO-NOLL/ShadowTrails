//------------------------------------------------
// ファイル名：PlayerCharacterBase.h
// 処理内容	：Playerの移動とカメラの操作をするベースキャラクタークラス
// 作成者	：丹代天
// 更新日	：12/08		作成
//			　12/10		カメラの制御、移動処理、操作可能フラグ切り替え、状態切り替えを追加
//------------------------------------------------

#include "PlayerCharacterBase.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "ActionObject_Box.h"
#include "shadowTestGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "WallRunCharacter.h"

// コンストラクタ
APlayerCharacterBase::APlayerCharacterBase()
	: m_pSpringArm(NULL)
	, m_pCamera(NULL)
	, m_pActionObject(NULL)
	, m_charaMoveInput(FVector2D::ZeroVector)
	, m_cameraRotateInput(FVector2D::ZeroVector)
	, m_PlayerCorrectPosition(FVector::ZeroVector)
	, m_cameraPitchLimit(FVector2D(-70.f, -15.f))
	, m_moveSpeed(10.f)
	, m_ActionForwardLength(30.f)
	, m_CameraSensitivity(5.f)
	, m_DecreaseRateofMove(3.f)
	, m_InterpolationSpeed(5.f)
	, m_bCanControl(true)
	, m_bIsBoxPush(false)
	, m_bCanWallRunning(false)
	, m_bCameraLock(true)
	, m_state(normal)
	, m_ChangeSpeed(0.0f)
{
	// Tick()を動かすかどうかを切り替え
	PrimaryActorTick.bCanEverTick = true;

	//デフォルトプレイヤーとして設定
	AutoPossessPlayer = EAutoReceiveInput::Player0;

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

// 生成時の処理
void APlayerCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

// フレームごとの処理
void APlayerCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//カメラ更新処理
	UpdateCamera(DeltaTime);

	//移動処理
	UpdateMove(DeltaTime);
}

// インプットキーとのバインド
void APlayerCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//カメラ回転
	InputComponent->BindAxis("CameraPitch", this, &APlayerCharacterBase::Cam_RotatePitch);
	InputComponent->BindAxis("CameraYaw", this, &APlayerCharacterBase::Cam_RotateYaw);

	//カメラのロックを解除する
	InputComponent->BindAction("CameraUnLock", IE_Pressed, this, &APlayerCharacterBase::Cam_MoveUnlock);
	InputComponent->BindAction("CameraUnLock", IE_Released, this, &APlayerCharacterBase::Cam_MoveLock);

	//移動
	InputComponent->BindAxis("MoveForward", this, &APlayerCharacterBase::Chara_MoveForward);
	InputComponent->BindAxis("MoveRight", this, &APlayerCharacterBase::Chara_MoveRight);

	//アクション
	InputComponent->BindAction("Action", IE_Pressed, this, &APlayerCharacterBase::Chara_Action);

	//壁走り
	InputComponent->BindAction("WallRun", IE_Pressed, this, &APlayerCharacterBase::Chara_WallRun);


}

//カメラ更新処理
void APlayerCharacterBase::UpdateCamera(float _deltaTime)
{
	//コントロール可能な場合にのみ
	if (m_bCanControl == false) { return; }

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
void APlayerCharacterBase::UpdateMove(float _deltaTime)
{
	//コントロール可能な場合にのみ
	if (m_bCanControl == false) { return; }

	//移動処理がある場合
	if (!m_charaMoveInput.IsZero())
	{
		//入力に合わせて、Actorを前後左右に移動
		if (m_pSpringArm != NULL)
		{
			FVector NewLocation = GetActorLocation();

			//キャラクターの移動
			{
				//もし箱を押していた場合
				if (m_bIsBoxPush == true)
				{
					//プレイヤーのメッシュ取得
					USkeletalMeshComponent* pMeshComp = GetMesh();

					//減少率に合わせて移動量を減らす
					m_charaMoveInput.Y /= m_DecreaseRateofMove;
					AddMovementInput(pMeshComp->GetForwardVector(), m_charaMoveInput.Y * m_moveSpeed);
				}
				else
				{
					// 22cu0222 田中 追記：2024/02/13---------------------------------
					if (m_state == damage)
					{
						m_charaMoveInput.Y /= m_ChangeSpeed;
						m_charaMoveInput.X /= m_ChangeSpeed;
					}
					// --------------------------------------------------------------

					//SpringArmが向く方向に、入力による移動量をPawnMovementComponentに渡
					const FVector forwardVec = m_pSpringArm->GetForwardVector();
					AddMovementInput(forwardVec, m_charaMoveInput.Y * m_moveSpeed);
					const FVector rightVec = m_pSpringArm->GetRightVector();
					AddMovementInput(rightVec, m_charaMoveInput.X * m_moveSpeed);
				}
			}

			//移動するキャラクターの回転
			if (m_bIsBoxPush == false)
			{
				//角度はDegreeの角度の範囲で表記(-180°～180°)
				USkeletalMeshComponent* pMeshComp = GetMesh();
				if (pMeshComp != NULL)
				{
					//現在の回転を保持
					FRotator CurrentRotation = pMeshComp->GetRelativeRotation();

					//アークタンジェントを使ってコントローラーの入力方向がなす角度を求める
					float angle = atan2(m_charaMoveInput.X, m_charaMoveInput.Y);
					//Radian値をDegreeに変換
					float angleDeg = FMath::RadiansToDegrees(angle);

					//入力した角度 + メッシュの回転速度 + Actorに対して回転しているSpringArmの相対角度
					float newYaw = angleDeg + GetBaseRotationOffsetRotator().Yaw + m_pSpringArm->GetRelativeRotation().Yaw;

					//新しい回転を保持
					FRotator NewRotation = FRotator(pMeshComp->GetRelativeRotation().Pitch, newYaw, pMeshComp->GetRelativeRotation().Roll);

					if (_deltaTime == 0.f || CurrentRotation == NewRotation) { return; }

					if (m_InterpolationSpeed <= 0.f) { pMeshComp->SetRelativeRotation(NewRotation); }

					//回転速度を計算
					float DeltaInterpSpeed = m_InterpolationSpeed * _deltaTime;
					//回転量の最短を求める
					const FRotator DeltaRotate = (NewRotation - CurrentRotation).GetNormalized();

					if (DeltaRotate.IsNearlyZero()) { pMeshComp->SetRelativeRotation(NewRotation); }

					//一フレームの回転量（最短の回転量 × 回転速度[0～1]）
					const FRotator DeltaMove = DeltaRotate * FMath::Clamp<float>(DeltaInterpSpeed, 0.f, 1.f);

					//現在の回転に一フレームの回転量を足した物を返す
					pMeshComp->SetRelativeRotation((CurrentRotation + DeltaMove).GetNormalized());
				}
			}
		}
	}
}

//レイを飛ばす関数
APlayerCharacterBase::RayInfo APlayerCharacterBase::FlyRay()
{
	//コントロール可能な場合のみ
	if (m_bCanControl == false) { return { FHitResult(),FVector::ZeroVector, FVector::ZeroVector, false }; }

	//このオブジェクトのメッシュを取得する
	USkeletalMeshComponent* pMeshComp = GetMesh();

	if (pMeshComp != NULL)
	{
		//レイを飛ばす
		//始点 Start：Actor
		FVector Start = GetActorLocation();
		//終点 End：Startから前方向m_ActionForwardLengthの距離
		FVector End = GetActorLocation() + pMeshComp->GetForwardVector() * m_ActionForwardLength;

		////デバッグ用にラインを描画
		//DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 1.f);

		//コリジョン判定で無視する項目を指定
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(this);

		//レイにヒットしたオブジェクトを格納する変数
		FHitResult RayHit;

		//ヒットしたサーフェイスの法線ベクトルを格納する変数
		FVector RayHitNormal;

		//ヒットしているポイントの座標を格納する変数
		FVector RayHitPoint;

		//ヒットした際にtrueになるフラグ,
		bool isHit = GetWorld()->LineTraceSingleByChannel(RayHit, Start, End, ECC_WorldStatic, CollisionParams);

		if (isHit)
		{
			RayHitNormal = RayHit.Normal;
			RayHitPoint = RayHit.ImpactPoint;
		}

		RayInfo NowHitInfo = { RayHit, RayHitNormal, RayHitPoint, isHit };

		return NowHitInfo;
	}

	return { FHitResult(),FVector::ZeroVector, FVector::ZeroVector, false };
}
//箱を移動する際、プレイヤーの座標を補正する関数
void APlayerCharacterBase::PositionCorrect()
{
	////デバッグ用ログ
	//UE_LOG(LogTemp, Warning, TEXT("CorrectTime = %f"), m_CorrectTime);
	//UE_LOG(LogTemp, Warning, TEXT("CorrectCnt = %f"), m_CorrectCnt);

	////デバッグ用ログ
	//UE_LOG(LogTemp, Log, TEXT("Distance = %f"), m_Distance);

	//設定した座標に移動
	SetActorLocation(m_PlayerCorrectPosition);

	////デバッグ用ログ
	//UE_LOG(LogTemp, Warning, TEXT("Correct Now"));

	////レイを飛ばす
	////始点 Start：Actor
	//FVector Start = GetActorLocation();
	////終点 End：Startから前方向m_ActionForwardLengthの距離
	//FVector End = m_PlayerCorrectPosition;

	////デバッグ用にラインを描画
	//DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1.f);

	//デバッグ用ログ
	UE_LOG(LogTemp, Warning, TEXT("Correct End"));
}

//【入力バインド】カメラ回転：Pitch(Y軸)
void APlayerCharacterBase::Cam_RotatePitch(float _axisValue)
{
	//コントロール可能な場合のみ
	if (m_bCanControl == false) { return; }

	m_cameraRotateInput.Y = _axisValue * m_CameraSensitivity;

	if (m_bCameraLock == true) { m_cameraRotateInput.Y = 0.f; }
}

//【入力バインド】カメラ回転：Yaw(Z軸)
void APlayerCharacterBase::Cam_RotateYaw(float _axisValue)
{
	//コントロール可能な場合のみ
	if (m_bCanControl == false) { return; }

	m_cameraRotateInput.X = _axisValue * m_CameraSensitivity;

	if (m_bCameraLock == true) { m_cameraRotateInput.X = 0.f; }
}

//【入力バインド】カメラをロックする
void APlayerCharacterBase::Cam_MoveLock()
{
	m_bCameraLock = true;
}

//【入力バインド】カメラのロックを解除する
void APlayerCharacterBase::Cam_MoveUnlock()
{
	m_bCameraLock = false;
}

//【入力バインド】キャラ移動：前後
void APlayerCharacterBase::Chara_MoveForward(float _axisValue)
{
	//コントロール可能な場合のみ
	if (m_bCanControl == false) { return; }

	m_charaMoveInput.Y = FMath::Clamp(_axisValue, -1.0f, 1.0f) * 0.5f;
}

//【入力バインド】キャラ移動：左右
void APlayerCharacterBase::Chara_MoveRight(float _axisValue)
{
	//コントロール可能な場合のみ
	if (m_bCanControl == false || m_bIsBoxPush == true) { return; }

	m_charaMoveInput.X = FMath::Clamp(_axisValue, -1.0f, 1.0f) * 0.5f;
}

//【入力バインド】アクション：ギミック起動
void APlayerCharacterBase::Chara_Action()
{
	//コントロール可能な場合のみ
	if (m_bCanControl == false) { return; }

	RayInfo RayHit = FlyRay();

	//ヒットしたオブジェクトがある場合
	if (RayHit.HitFlag)
	{
		//ヒットしたActorのタグを調べる
		//アクションを起こすことのできるオブジェクトだった場合
		if (RayHit.HitActor.GetActor()->ActorHasTag("ActionObject"))
		{
			////デバッグ確認用
			//FString str = RayHit.GetActor()->GetName();
			//UE_LOG(LogTemp, Warning, TEXT("HitActor [ActionObject]ObjectName : %s"), *str);

			//ActionObjectBaseにキャスト
			m_pActionObject = Cast<AActionObjectBase>(RayHit.HitActor.GetActor());
		}
		else
		{
			//アクションを起こすオブジェクト以外の場合はActionObjectの中身を空にしておく
			m_pActionObject = NULL;
		}
	}
	else
	{
		//何もヒットしていない場合はActionObjectの中身を空にしておく
		m_pActionObject = NULL;
	}


	if (m_pActionObject != NULL)
	{
		//Actor側の関数を呼び出す
		m_pActionObject->OnActionthisObject();
	}

	if (m_bIsBoxPush)
	{
		m_bIsBoxPush = false;
		m_pActionObject = NULL;
		UE_LOG(LogTemp, Warning, TEXT("FALSE"));
	}

	if (m_pActionObject != NULL && m_bCanControl == true)
	{
		//レイに触れているオブジェクトが押せる箱だった時
		if (m_pActionObject->ActorHasTag("Box"))
		{
			//箱のメッシュコンポーネントを取得
			UStaticMeshComponent* pObjectMesh = m_pActionObject->m_pObjectMeshComp;


			//プレイヤーを回転させる(レイにヒットしているサーフェイスの法線の逆ベクトルを参照し回転として変換、その後代入する)
			FVector InvertedNormal = -RayHit.NormalVector;
			FRotator NowRotation = InvertedNormal.Rotation();

			//メッシュコンポーネントの取得
			USkeletalMeshComponent* pMeshComp = GetMesh();

			pMeshComp->SetWorldRotation(NowRotation);

			UE_LOG(LogTemp, Warning, TEXT("Rotation.Roll = %f"), NowRotation.Roll);
			UE_LOG(LogTemp, Warning, TEXT("Rotation.Yaw = %f"), NowRotation.Yaw);
			UE_LOG(LogTemp, Warning, TEXT("Rotation.Pitch = %f"), NowRotation.Pitch);

			//箱のメッシュサイズを取得する。
			FVector ObjectMeshSize = pObjectMesh->Bounds.GetBox().GetSize() * m_pActionObject->GetActorScale();

			////デバッグ用ログ
			//UE_LOG(LogTemp, Warning, TEXT("ObjectSize = %f"), ObjectMeshSize.X);
			//UE_LOG(LogTemp, Warning, TEXT("ObjectSize = %f"), ObjectMeshSize.Y);
			//UE_LOG(LogTemp, Warning, TEXT("ObjectSize = %f"), ObjectMeshSize.Z);

			//プレイヤーのメッシュサイズを取得する
			USkeletalMeshComponent* pPlayerMesh = this->GetMesh();
			FVector playerMeshSize = pPlayerMesh->Bounds.GetBox().GetSize() * this->GetActorScale();

			////デバッグ用ログ
			//UE_LOG(LogTemp, Warning, TEXT("ObjectSize = %f"), playerMeshSize.X);
			//UE_LOG(LogTemp, Warning, TEXT("ObjectSize = %f"), playerMeshSize.Y);
			//UE_LOG(LogTemp, Warning, TEXT("ObjectSize = %f"), playerMeshSize.Z);

			//箱の関数を呼び出しプレイヤー座標との内積を求める
			AActionObject_Box* pActionBox = Cast<AActionObject_Box>(m_pActionObject);
			float angle = pActionBox->GetAnglePlayer(this->GetActorLocation());
			////時間を設定する
			//pActionBox->SetTime(m_CorrectTime);

			//プレイヤーの位置を補正するための、ブロックに対する方向ベクトルを格納する変数
			FVector PlayerDirection = FVector(0.f, 0.f, 0.f);

			//内積の数値に応じてプレイヤーをプレイヤーのメッシュと箱のメッシュのサイズを加味した位置まで移動する。
			if (angle >= -45.f && angle <= 45.f)
			{
				PlayerDirection = m_pActionObject->GetActorForwardVector();
			}
			else if (angle > 45.f && angle <= 135.f)
			{
				PlayerDirection = m_pActionObject->GetActorRightVector();
			}
			else if (angle > 135.f || angle < -135.f)
			{
				PlayerDirection = m_pActionObject->GetActorForwardVector();
				PlayerDirection = PlayerDirection * -1.0f;
			}
			else if (angle >= -135.f && angle < -45.f)
			{
				PlayerDirection = m_pActionObject->GetActorRightVector();
				PlayerDirection = PlayerDirection * -1.0f;
			}

			////デバッグ用ログ
			//UE_LOG(LogTemp, Warning, TEXT("PlayerDirection.X = %f"), PlayerDirection.X);
			//UE_LOG(LogTemp, Warning, TEXT("PlayerDirection.Y = %f"), PlayerDirection.Y);
			//UE_LOG(LogTemp, Warning, TEXT("PlayerDirection.Z = %f"), PlayerDirection.Z);

			//補正する座標を計算する。
			FVector CorrecPos = PlayerDirection * (PlayerDirection * ((ObjectMeshSize + playerMeshSize) / 2.f));
			//オブジェクトの座標 + 方向ベクトル * ((オブジェクトのサイズ + プレイヤーメッシュのサイズ / 2))
			if (PlayerDirection.X == 1.0f || PlayerDirection.Y == 1.0f)
			{
				m_PlayerCorrectPosition = m_pActionObject->GetActorLocation() + CorrecPos;
			}
			else if (PlayerDirection.X == -1.0f || PlayerDirection.Y == -1.0f)
			{
				m_PlayerCorrectPosition = m_pActionObject->GetActorLocation() - CorrecPos;
			}

			//Z軸においてはプレイヤーの座標そのまま
			m_PlayerCorrectPosition.Z = GetActorLocation().Z;

			//デバッグ用ログ
			UE_LOG(LogTemp, Warning, TEXT("CorrectPosition.X = %f"), m_PlayerCorrectPosition.X);
			UE_LOG(LogTemp, Warning, TEXT("CorrectPosition.Y = %f"), m_PlayerCorrectPosition.Y);

			////進む方向ベクトルを計算する
			//FVector Direction = m_PlayerCorrectPosition - this->GetActorLocation();
			//m_CorrectPositionVector = Direction.GetSafeNormal();

			////進む総距離を計算する
			//m_ToCorrectPositionDistnce = m_PlayerCorrectPosition - GetActorLocation();
			////プレイヤー現在地から移動する値を計算する
			////m_CorrectPositionから現在のプレイヤーの座標を引き、三平方の定理を使うことで移動距離を求める
			//m_Distance = (float)sqrt(m_ToCorrectPositionDistnce.X * m_ToCorrectPositionDistnce.X) + (m_ToCorrectPositionDistnce.Y * m_ToCorrectPositionDistnce.Y);
			////デバッグ用ログ
			//UE_LOG(LogTemp, Log, TEXT("Distance1 = %f"), m_Distance);

			//m_Distance = m_Distance / m_CorrectTime;

			////デバッグ用ログ
			//UE_LOG(LogTemp, Warning, TEXT("CorrectPositionVector.X = %f"), m_CorrectPositionVector.X);
			//UE_LOG(LogTemp, Warning, TEXT("CorrectPositionVector.Y = %f"), m_CorrectPositionVector.Y);
			//UE_LOG(LogTemp, Warning, TEXT("CorrectPositionVector.Z = %f"), m_CorrectPositionVector.Z);

			//箱のそばまで移動する
			PositionCorrect();

			//箱を押していることを通達するフラグを立てる
			m_bIsBoxPush = true;

		}
	}
}
//【入力バインド】アクション：壁走り
void APlayerCharacterBase::Chara_WallRun()
{
	//レイを飛ばす
	RayInfo RayHit = FlyRay();

	//ヒットしたオブジェクトがある場合
	if (RayHit.HitFlag)
	{
		//内積を求めるためのZ軸に平行な単位ベクトル
		FVector Zaxis = FVector(0.f, 0.f, 1.f);

		////確認用ログ
		//UE_LOG(LogTemp, Error, TEXT("ImpactNormal.X = %f"), RayHit.ImpactNormal.X);
		//UE_LOG(LogTemp, Error, TEXT("ImpactNormal.Y = %f"), RayHit.ImpactNormal.Y);
		//UE_LOG(LogTemp, Error, TEXT("ImpactNormal.Z = %f"), RayHit.ImpactNormal.Z);
		// 
		//ヒットしたオブジェクトのサーフェイス法線を取得し、Z軸との内積を求める
		float product = FVector::DotProduct(RayHit.NormalVector, Zaxis);

		////デバッグ用ログ
		//UE_LOG(LogTemp, Error, TEXT("product = %f"), product);

		//内積がある程度小さかった場合そこを壁とみなす
		if (product < 0.05f && product > -0.05f)
		{
			//壁走り可能フラグを立てる
			m_bCanWallRunning = true;

			////デバッグ用ログ
			//UE_LOG(LogTemp, Warning, TEXT("m_GeneratePos.X = %f"), m_GeneratePosition.X);
			//UE_LOG(LogTemp, Warning, TEXT("m_GeneratePos.Y = %f"), m_GeneratePosition.Y);
			//UE_LOG(LogTemp, Warning, TEXT("m_GeneratePos.Z = %f"), m_GeneratePosition.Z);
		}
	}
	else
	{
		//何もヒットしていない場合、壁走り可能フラグをおろしておく
		m_bCanWallRunning = false;
	}

	if (m_bCanWallRunning == true)
	{
		////デバッグ用ログ
		//UE_LOG(LogTemp, Warning, TEXT("WallRun_On"));

		// スポーン時の回転を指定(レイにヒットしているサーフェイスの法線の逆ベクトルを参照し回転として変換、その後代入する)
		FVector InvertedNormal = -RayHit.NormalVector;
		FRotator SpawnRotation = InvertedNormal.Rotation();

		// スポーンするアクターのTransformを作成
		FTransform SpawnTransform(SpawnRotation, RayHit.HitPoint);

		//レイが触れている座標に別アクターを生成する
		AWallRunCharacter* SpawnActor = GetWorld()->SpawnActor<AWallRunCharacter>(m_pGenerateCharacter, SpawnTransform); // スポーン処理

		if (SpawnActor)
		{
			//// 成功した場合の処理
			//SpawnActor->SetActorLocation(m_GeneratePosition); // 座標を設定

			//// ShadowGameMode内のSwitchCharacter関数を呼び出し、プレイヤーキャラクターを切り替える
			//AShadowGameMode* GameMode = Cast<AShadowGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
			//if (GameMode)
			//{
			//	GameMode->SwitchCharacter(SpawnActor);
			//}

			//現在設定しているゲームモードを取得
			AGameModeBase* CurrentGameMode = UGameplayStatics::GetGameMode(GetWorld());
			//キャストしする
			AshadowTestGameMode* ShadowGameMode = Cast<AshadowTestGameMode>(CurrentGameMode);

			//NULLチェック
			if (ShadowGameMode)
			{
				//関数呼び出し
				//ShadowGameMode->SwitchCharacter(SpawnActor);
			}
			else
			{
				// ゲームモードが取得できなかった場合のエラーハンドリング
				UE_LOG(LogTemp, Error, TEXT("Failed to get AShadowGameMode"));
			}
		}
		else
		{
			// 失敗した場合の処理
			UE_LOG(LogTemp, Warning, TEXT("Failed to spawn character."));
		}
	}
}


//プレイヤーの状態を切り替える関数
//normal
void APlayerCharacterBase::StateChangeNormal()
{
	m_state = normal;
}
//damage
void APlayerCharacterBase::StateChangeDamage()
{
	m_state = damage;
}
//dead
void APlayerCharacterBase::StateChangeDead()
{
	m_state = dead;
}

//操作可能フラグを切り替える関数
void APlayerCharacterBase::SetCanMove(bool _flag)
{
	m_bCanControl = _flag;
}

// 22cu0222 田中 追記：2024/02/13
// プレイヤーの移動量を減少させる量を格納する関数
void APlayerCharacterBase::ChangeSpeed(float m_Speed)
{
	m_ChangeSpeed = m_Speed;
}

/**
* コンポーネントの相対回転をリセットして、アクタとメッシュが同じ方向を向くようにする
*/
void APlayerCharacterBase::ResetRelativeRotation()
{
	if (m_pSpringArm != NULL)
	{
		m_pSpringArm->SetRelativeRotation(FRotator::ZeroRotator);
	}

	// メッシュの相対回転をリセット
	USkeletalMeshComponent* pMeshComponent = GetMesh();
	if (pMeshComponent != NULL)
	{
		pMeshComponent->SetRelativeRotation(FRotator(0.f, 270.f, 0.f));
	}

}

void APlayerCharacterBase::SetPlayerRotation(FRotator _rotation)
{
	m_pSpringArm->SetRelativeRotation(_rotation);

	USkeletalMeshComponent* pMeshComp = GetMesh();
	_rotation.Yaw += 270.f;
	pMeshComp->SetRelativeRotation(_rotation);
}