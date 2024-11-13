/**
* @file		CopyShadowObject.cpp
* @brief	影のコピーを実現するための、影オブジェクト
* @author	22CU0219 鈴木友也
* @date		23/12/10 作成
*			23/02/17 最終更新
*/

#include "CopyShadowObject.h"
#include "Components/DecalComponent.h"
#include "Components/BoxComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetSystemLibrary.h" 
//丹代天 追記：2024/01/31----------------------------------------------------------------
#include "EffectComponent.h"
#include "Kismet/GameplayStatics.h"
//--------------------------------------------------------------------------------------
/**
* コンストラクタ
*/
ACopyShadowObject::ACopyShadowObject()
	: m_pDecalMaterialInst(nullptr)
	, m_bHighLight(false)
	, m_emissiveValue(50.f)
	, m_emissiveColor(.05f, .05f, .5f, 1.f)
	, m_emissiveColor_Moved(1.f, .05f, .05f, 1.f)
	, m_bIsHold(false)
	, m_bIsMoved(false)
	, m_bIsPlayerHit(false)
	, m_DefaultTrans(FTransform::Identity)
	//丹代天 追記：2024/01/31----------------------------------------------------------------
	, m_bNowEffectVisibility(false)
	, m_pPlayer(NULL)
	, m_PlayerDistance(0.f)
	, m_ActivateRange(500.f)
	//--------------------------------------------------------------------------------------
{
	PrimaryActorTick.bCanEverTick = true;
	if (m_pBoxComponent != nullptr)
	{
		// バインド
		m_pBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ACopyShadowObject::OnOverlapBegin);
		m_pBoxComponent->OnComponentEndOverlap.AddDynamic(this, &ACopyShadowObject::OnOverlapEnd);

		// チャンネル[CopyShadow]のトレースを有効化
		m_pBoxComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Block);
	}

	//丹代天 追記：2024/01/31----------------------------------------------------------------
		//コンポーネントアタッチ
	EffectComponent = CreateDefaultSubobject<UEffectComponent>(TEXT("EffectComponent"));
	AddOwnedComponent(EffectComponent);
	//--------------------------------------------------------------------------------------

	// タグ追加
	Tags = { "Shadow" };
}


/**
* 開始時に一回呼ばれる関数
*/
void ACopyShadowObject::BeginPlay()
{
	Super::BeginPlay();

	// ダイナミックマテリアルインスタンスを作成
	m_pDecalMaterialInst = m_pDecalComponent->CreateDynamicMaterialInstance();
	m_DefaultTrans = GetActorTransform();

	//丹代天 追記：2024/01/31----------------------------------------------------------------
		//m_pPlayerが空の時のみ
	if (m_pPlayer == NULL)
	{
		//ゲーム全体へのActor検索処理(コスト高)
		TSubclassOf<AActor> findClass;
		findClass = AActor::StaticClass();
		TArray<AActor*> actors;
		UGameplayStatics::GetAllActorsOfClass(this->GetWorld(), findClass, actors);

		//検索の結果Actroがあれば
		if (actors.Num() > 0)
		{
			//Actorの中身を順番に検索していく
			for (int idx = 0; idx < actors.Num(); idx++)
			{
				AActor* pActor = Cast<AActor>(actors[idx]);
				//タグがプレイヤーだった時
				if (pActor->ActorHasTag("Player"))
				{
					m_pPlayer = pActor;
					break;
				}
			}
		}
	}
	//--------------------------------------------------------------------------------------
}


/**
* 毎フレーム呼び出される関数
*/
void ACopyShadowObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//丹代天 追記：2024/01/31----------------------------------------------------------------
	MeasurementToPlayer();
	//--------------------------------------------------------------------------------------
}

/**
* ハイライトモードを有効/無効化
*/
void ACopyShadowObject::SetHighLightMode(bool _flag)
{
	m_bHighLight = _flag;

	if (_flag)
	{
		if (m_pDecalMaterialInst != nullptr)
		{
			m_pDecalMaterialInst->SetScalarParameterValue(TEXT("EmissiveBlend"), m_emissiveValue);

			// 動かされた後で色を切り替え
			if (!m_bIsMoved)
			{
				m_pDecalMaterialInst->SetVectorParameterValue(TEXT("EmissiveColor"), m_emissiveColor);
			}
			else
			{
				m_pDecalMaterialInst->SetVectorParameterValue(TEXT("EmissiveColor"), m_emissiveColor_Moved);
			}
		}
	}
	else
	{
		if (m_pDecalMaterialInst != nullptr)
		{
			m_pDecalMaterialInst->SetScalarParameterValue(TEXT("EmissiveBlend"), 0.f);
		}
	}
}

/**
* 保持フラグを設定
*/
void ACopyShadowObject::SetHold(bool _flag)
{
	if (_flag)
	{
		m_bIsHold = true;
		//エフェクトを破棄する
		EffectComponent->StopEffect();
		//エフェクトを破棄したというフラグを上げておく
		m_bNowEffectVisibility = false;
	}
	else
	{
		m_bIsHold = false;
	}
}

/**
* 移動済みフラグを設定
*/
void ACopyShadowObject::SetMoved(bool _flag)
{
	if (_flag)
	{
		m_pDecalMaterialInst->SetScalarParameterValue(TEXT("EmissiveBlend"), 0.f);
		m_bIsMoved = true;
		//エフェクトを破棄する
		EffectComponent->StopEffect();
		//エフェクトを破棄したというフラグを上げておく
		m_bNowEffectVisibility = false;
	}
	else
	{
		m_bIsMoved = false;
	}
}

/**
* Pawnのオブジェクト応答を切り替え
*/
void ACopyShadowObject::SetPawnCollisionEnable(bool _flag)
{
	if (_flag)
		m_pBoxComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	else
		m_pBoxComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
}

/**
* OnComponentBeginOverlapにバインドする関数
*/
void ACopyShadowObject::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// プレイヤーとOverlapした時にフラグを立てる
	if (OtherActor->ActorHasTag("Player"))
	{
		m_pHitPlayer = OtherActor;
		m_bIsPlayerHit = true;
	}
}

/**
* OnComponentEndOverlapにバインドする関数
*/
void ACopyShadowObject::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// プレイヤーとOverlapEndした時にフラグを下す
	if (OtherActor->ActorHasTag("Player"))
	{
		m_bIsPlayerHit = false;
	}
}

//丹代天 追記：2024/01/31----------------------------------------------------------------
/*
* プレイヤーとの距離を測定しbool型の戻り値を返す関数
*/
void ACopyShadowObject::MeasurementToPlayer()
{
	//プレイヤーが格納できている場合
	if (m_pPlayer)
	{
		//プレイヤーとこのアクターの距離を算出する
		m_PlayerDistance = FVector::Dist(GetActorLocation(), m_pPlayer->GetActorLocation());

		//プレイヤーが指定の距離以内かつエフェクトが起動していない場合
		if (m_PlayerDistance < m_ActivateRange && m_bNowEffectVisibility == false)
		{

			// オブジェクトのForwardVectorを取得
			FVector ObjectForwardVector = GetActorForwardVector();

			FVector WorldUpVector;
			FRotator RotationToVertical;

			if (GetActorRotation().Roll >= GetActorRotation().Pitch)
			{
				WorldUpVector = FVector(0.f, 0.f, 1.f);

				// ForwardVectorとUpVectorの間の回転を計算
				RotationToVertical = FRotationMatrix::MakeFromXZ(ObjectForwardVector, WorldUpVector).Rotator();
			}
			else
			{
				WorldUpVector = FVector(1.f, 0.f, 0.f);
				RotationToVertical = FRotationMatrix::MakeFromYZ(ObjectForwardVector, WorldUpVector).Rotator();
			}

			// 動かされていない場合
			if (!m_bIsMoved && !m_bIsHold)
			{
				//エフェクトをスポーンさせる
				EffectComponent->PlayEffect(RootComponent, FVector::ZeroVector, RotationToVertical, false);
				//エフェクトを起動したというフラグを上げておく
				m_bNowEffectVisibility = true;
			}
		}
		//プレイヤーが指定の距離より外かつエフェクトが起動している場合
		else if (m_PlayerDistance > m_ActivateRange && m_bNowEffectVisibility == true)
		{
			//エフェクトを破棄する
			EffectComponent->StopEffect();
			//エフェクトを破棄したというフラグを上げておく
			m_bNowEffectVisibility = false;
		}
	}
}