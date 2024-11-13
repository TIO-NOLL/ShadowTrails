//------------------------------------------------
// ファイル名：PlayerCharacter.h
// 処理内容	：Playerのキャラクタークラス
// 作成者	：丹代天
// 更新日	：12/11		作成
//------------------------------------------------

#include "PlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ShadowCopyModeComponent.h"
#include "DecalLightObject.h"
#include "EffectComponent.h"

APlayerCharacter::APlayerCharacter()
	: m_pShadowComponent(nullptr)
	, m_pDeadEffectComponent(nullptr)
	, m_pMeshMaterial(nullptr)
	, m_RotateDeglee(.5f)
	, m_bClockwisePress(false)
	, m_bCounterClockwisePress(false)
	, m_bDead(false)
	, m_DeadTime(3.f)
	, m_DeadTimeCounter(0.f)
	, m_DownSpeed(0.f)
	, m_hittingLightNum(0)
	, m_hittingShadowNum(0)
	, m_bPlayerVisible(true)
	, m_FlashInterval(0.3f)
	, m_FlashDuration(0.0f)
	, m_ShadowRotateValue(0.f)
{
	// Tick()を動かすかどうかを切り替え
	PrimaryActorTick.bCanEverTick = true;

	// 影コピー用のコンポーネント生成
	m_pShadowComponent = CreateDefaultSubobject<UShadowCopyModeComponent>(TEXT("m_pShadowCopyComponent"));
	AddOwnedComponent(m_pShadowComponent);

	// 死亡時エフェクトのコンポーネントを作成
	m_pDeadEffectComponent = CreateDefaultSubobject<UEffectComponent>(TEXT("DeadEffect"));
	AddOwnedComponent(m_pDeadEffectComponent);

	UCapsuleComponent* pCapsuleComp = GetCapsuleComponent();
	// デリゲート
	pCapsuleComp->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnOverlapBegin);
	pCapsuleComp->OnComponentEndOverlap.AddDynamic(this, &APlayerCharacter::OnOverlapEnd);

	// タグの追加
	Tags = { "Player" };
}

// 生成時の処理
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// 22cu0219 鈴木友也 記述:24/02/15
	USkeletalMeshComponent* mesh = GetMesh();
	m_pMeshMaterial = mesh->CreateDynamicMaterialInstance(0);
}

void APlayerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// 22cu0219 鈴木が記述-----------------
	//影カット・ペースト
	InputComponent->BindAction("CutPaste", IE_Pressed, this, &APlayerCharacter::CutPaste);

	// 時計回り回転をボタン押下状態によって切り替え
	InputComponent->BindAction("ClockwiseRotate", IE_Pressed, this, &APlayerCharacter::SwapClockwisePressReleased);
	InputComponent->BindAction("ClockwiseRotate", IE_Released, this, &APlayerCharacter::SwapClockwisePressReleased);

	// 反時計回り回転をボタン押下状態によって切り替え
	InputComponent->BindAction("CounterClockwiseRotate", IE_Pressed, this, &APlayerCharacter::SwapCounterClockwisePressReleased);
	InputComponent->BindAction("CounterClockwiseRotate", IE_Released, this, &APlayerCharacter::SwapCounterClockwisePressReleased);

	// 移動済み影にカーソルを合わせてボタンを押すと位置リセット
	InputComponent->BindAction("ResetPos", IE_Pressed, this, &APlayerCharacter::ResetShadowPos);
	//------------------------------------

	InputComponent->BindAxis("ShadowRotate", this, &APlayerCharacter::RotateShadow);

}

// フレームごとの処理
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// ダメージ処理
	if (m_hittingLightNum && !m_hittingShadowNum && !m_bDead)
	{
		m_DeadTimeCounter += DeltaTime;

		// 22cu0222 田中 追記：2024/02/13---------------------------------------
		// 移動量を減少させる量を加算
		m_DownSpeed += 0.05f;

		StateChangeDamage();  // ステータスをダメージに変更
		ChangeSpeed(m_DownSpeed); // 移動変更

			// プレイヤー点滅処理
		PlayerFlashing(DeltaTime);
		// --------------------------------------------------------------------

		// 22cu0327 千葉涼太郎 追記：2024/02/08-------------------------------------
		m_bDamage = true;
		// -------------------------------------------------------------------------

		/*GEngine->AddOnScreenDebugMessage(-1, .1f, FColor::Orange, "Light Hit");*/
	}
	// 22cu0327 千葉涼太郎 追記：2024/02/08-------------------------------------
	else
	{
		m_bDamage = false;

		// 22cu0222 田中 追記：2024/02/13---------------------------------------
		m_DownSpeed = 0;
		SetActorHiddenInGame(false);
		StateChangeNormal();
		// --------------------------------------------------------------------
	}
	// -------------------------------------------------------------------------

	//-------------------------------------------------------------
	// 22cu0219鈴木友也 追記:24/02/15
	//=============================================================
	// 影の中にいる場合マテリアルを黒くする

	if (m_hittingShadowNum > 0)
		m_pMeshMaterial->SetScalarParameterValue(TEXT("BlackBlend"), 0.2f);
	else
		m_pMeshMaterial->SetScalarParameterValue(TEXT("BlackBlend"), 1.0f);

	//-------------------------------------------------------------------------

	// 死亡時処理
	if (m_DeadTime < m_DeadTimeCounter)
	{
		if (!m_bDead)
		{
			// メッシュ取得
			USkeletalMeshComponent* mesh = GetMesh();
			mesh->SetVisibility(false);

			// 死亡時持っていた影は元の位置に戻す
			if (m_pShadowComponent != nullptr)
			{
				if (m_pShadowComponent->IsHold())
				{
					m_pShadowComponent->Hold(false);
					m_pShadowComponent->ResetShadowPos();
				}
			}

			// 死亡エフェクトを再生
			m_pDeadEffectComponent->PlayEffect(GetRootComponent(), GetActorLocation(), GetActorRotation(), true);
			SetCanMove(false); // 移動無効化

			m_bDead = true;
			StateChangeDead();  // ステータスをDeadに変更

			//// ログ出力
			//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, "Dead");
		}
	}


	// 22cu0219 鈴木が記述----
	if (m_pShadowComponent != nullptr)
	{
		//// 影の回転
		//// 時計回り回転
		//if (m_bClockwisePress)
		//	m_pShadowComponent->AddYawRotation(m_RotateDeglee);

		//// 反時計回り回転
		//if (m_bCounterClockwisePress)
		//	m_pShadowComponent->AddYawRotation(-1 * m_RotateDeglee);

		//22Cu0127 丹代天 2024/03/04------------------------------
		m_pShadowComponent->AddYawRotation(m_ShadowRotateValue);
		//--------------------------------------------------------
	}
	//-----------------------
}
/*
* 22cu0127 丹代天 2024/03/04
* 影を回転する関数
*/
void APlayerCharacter::RotateShadow(float _AxisValue)
{
	m_ShadowRotateValue = _AxisValue * m_RotateDeglee;
}

/**
* @author	22cu0219 鈴木友也
* @brief	死亡状態のプレイヤーを元に戻す
*/
void APlayerCharacter::ResetPlayer()
{
	// メッシュ取得
	USkeletalMeshComponent* mesh = GetMesh();
	mesh->SetVisibility(true); // メッシュを可視化

	// カウンターを初期化
	m_DeadTimeCounter = 0.f;

	m_bDead = false;
	StateChangeNormal(); // ステート切り替え
	SetCanMove(true); // 移動有効化
	ResetRelativeRotation();
}

/**
* @author	22cu0219 鈴木友也
* @brief	【入力バインド】影カットペーストを切り替える
*/
void APlayerCharacter::CutPaste()
{
	if (m_pShadowComponent != nullptr)
	{
		m_pShadowComponent->Hold(!m_pShadowComponent->IsHold());
	}
}

/**
* @author	22cu0219 鈴木友也
* @brief	【入力バインド】影を初期位置に戻す
*/
void APlayerCharacter::ResetShadowPos()
{
	if (m_pShadowComponent != nullptr)
	{
		m_pShadowComponent->ResetShadowPos();
	}
}

// 22cu0222 田中　追記：2024/02/13 -----------------------------
// プレイヤー点滅処理
void APlayerCharacter::PlayerFlashing(float DeltaTime)
{
	//FLinearColor Color;
	//m_pMaterialInstanceDynamic->GetVectorParameterValue(TEXT("ColorParaneterName"), Color);

	m_FlashDuration += DeltaTime;

	if (m_FlashDuration >= m_FlashInterval)
	{
		if (m_bPlayerVisible)
		{
			SetActorHiddenInGame(true);
			//Color.R = 1.0f;
			m_FlashDuration = 0.0f;
			m_bPlayerVisible = false;
		}
		else
		{
			SetActorHiddenInGame(false);
			//Color.R = 0.0f;
			m_FlashDuration = 0.0f;
			m_bPlayerVisible = true;
		}
	}
}
// -------------------------------------------------------------


void APlayerCharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("DecalLight"))
	{
		m_hittingLightNum++;
	}
	else if (OtherActor->ActorHasTag("Shadow"))
	{
		m_hittingShadowNum++;
	}
}


void APlayerCharacter::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->ActorHasTag("DecalLight"))
	{
		m_hittingLightNum--;
	}
	else if (OtherActor->ActorHasTag("Shadow"))
	{
		m_hittingShadowNum--;
	}
}

// 22cu0327 千葉涼太郎 追記：2024/02/08-------------------------------------
bool APlayerCharacter::GetbDamage()
{
	return m_bDamage;
}
//--------------------------------------------------------------------------