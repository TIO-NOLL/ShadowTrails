/**
* @file		ShadowCopyModeComponent.cpp
* @brief	影コピーモードを実装するコンポーネント
* @author	22CU0219 鈴木友也
* @date		23/12/10 作成
*			23/02/17 最終更新
*/


#include "ShadowCopyModeComponent.h"
#include "CopyShadowObject.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"
// 22cu0327 千葉涼太郎 追記：2024/02/09-----------------------------------------
#include "Blueprint/WidgetBlueprintLibrary.h"
// -----------------------------------------------------------------------------

// Sets default values for this component's properties
UShadowCopyModeComponent::UShadowCopyModeComponent()
	: m_pPlayerController(nullptr)
	, m_bIsHolding(false)
	, m_UnderCursorTraceLength(2000.f)
	, m_addYaw(0.f)
	, m_pShadowObj(nullptr)
	, m_holdingShadowRotation(FRotator::ZeroRotator)
	, m_prevNormal(FVector::ZeroVector)
	// 22cu0327 千葉涼太郎 ---------------------------------------------------------
	, m_pUI(nullptr)
	, m_pMouseUI(nullptr)
	, m_bDoOnce_LeftClick(false)
	, m_bDoOnce_Rotate(false)
	, m_bDoOnce_Return(false)
	, m_pPasteSound(nullptr)
	, m_pResetSound(nullptr)
	//------------------------------------------------------------------------------
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UShadowCopyModeComponent::BeginPlay()
{
	Super::BeginPlay();

	m_pPlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	/*if (m_pPlayerController != nullptr)
		m_pPlayerController->bShowMouseCursor = true;*/

	// 22cu0327 千葉涼太郎 ---------------------------------------------------------
	if (m_pUI == nullptr && m_pMouseUI == nullptr)
	{
		// ウィジェット取得
		TArray<UUserWidget*> findClass;
		UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), findClass, UActorFollowingUserWidget::StaticClass(), false);
		if (findClass.Num() == 1)
		{
			m_pUI = Cast<UActorFollowingUserWidget>(findClass[0]);
		}

		UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), findClass, UMouseUserWidget::StaticClass(), false);
		if (findClass.Num() == 1)
		{
			m_pMouseUI = Cast<UMouseUserWidget>(findClass[0]);
		}
	}

}


// Called every frame
void UShadowCopyModeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// 影を保持している時の処理
	if (m_bIsHolding)
	{
		FHitResult hitResult; // トレース結果
		FVector traceStart; // 始点座標
		FVector traceEnd; // 終点座標

		// マウスカーソル座標をワールド座標で取得(方向を一旦traceEndに入れておく)
		m_pPlayerController->DeprojectMousePositionToWorld(traceStart, traceEnd);
		// 長さ分伸ばした場所が終点
		traceEnd = traceStart + traceEnd * m_UnderCursorTraceLength;

		// ライントレース
		GetWorld()->LineTraceSingleByChannel(hitResult, traceStart, traceEnd, ECC_Visibility);
		// デバッグ確認
		//DrawDebugLine(GetWorld(), traceStart, traceEnd, FColor::Blue, false, .5f,0U,0.1f);

		// 影を貼れるオブジェクトかどうか判別
		bool isGroundHit = false;
		if (hitResult.bBlockingHit)
		{
			TWeakObjectPtr<AActor> hitActor = hitResult.GetActor();

			if (hitActor.IsValid())
			{
				auto responce = hitActor->GetComponentsCollisionResponseToChannel(ECC_GameTraceChannel2);
				
				if (responce == ECR_Block)
					isGroundHit = true;
				else
					isGroundHit = false;
			}
		}
		
		// 当たっていた時のみ処理
		if (hitResult.bBlockingHit && isGroundHit)
		{
			//// デバッグ確認
			//DrawDebugPoint(GetWorld(), hitResult.Location, 5.f, FColor::Blue, false, .5f);

			if (m_pShadowObj != nullptr)
			{
				// 影のオブジェクトをレイがヒットした位置に移動
				m_pShadowObj->SetActorLocation(hitResult.Location);

				// 法線方向を地面として回転
				FRotator defaultRot = m_pShadowObj->GetDefaultRotation();
				ProcStandardNormalBaseRotation(hitResult.Normal, defaultRot);

				// 法線を保持しておく
				m_prevNormal = hitResult.Normal;
			}

			// 22cu0327 千葉涼太郎 ---------------------------------------------------------
			if (m_pUI != nullptr)
			{
				if (m_bDoOnce_Rotate == false)
				{
					// RotateUIを表示
					m_pUI->VisibilityType(EUIType::RotateShadow);
					m_bDoOnce_Rotate = true;
					m_bDoOnce_LeftClick = false;
					m_bDoOnce_Return = false;

					// Cutしたときの音を鳴らす
					m_pUI->PlaySoundbyType(EShadowSoundType::Cut);
				}

				
			}
			//------------------------------------------------------------------------------
		}
		else
		{
			if (!m_prevNormal.IsZero())
			{
				FRotator defaultRot = m_pShadowObj->GetDefaultRotation();
				ProcStandardNormalBaseRotation(m_prevNormal, defaultRot);
			}
		}

		// 22cu0327 千葉涼太郎 ---------------------------------------------------------
		if (m_pMouseUI != nullptr)
		{
			m_pMouseUI->SetCursorType(ECursorType::Abnormal);
		}
		//------------------------------------------------------------------------------

	}
	else
	{
		// 影を保持していない時はカーソル下の影オブジェクトを取得
		GetUnderCursorShadow();

		// 22cu0327 千葉涼太郎 ---------------------------------------------------------
		if (m_pMouseUI != nullptr)
		{
			m_pMouseUI->SetCursorType(ECursorType::Normal);
		}
		//------------------------------------------------------------------------------
	}

}

void UShadowCopyModeComponent::ProcStandardNormalBaseRotation(FVector _newNormal, FRotator _rotation)
{
	// 回転
	if (m_pShadowObj == nullptr)
		return;

	m_pShadowObj->SetActorRotation(_rotation);
	//UE_LOG(LogTemp, Log, TEXT("hitNormal[X:%f,Y:%f,Z:%f"), _newNormal.X, _newNormal.Y, _newNormal.Z);

	// Case1---------------------------------------------------------------------------------------------
	// 壁を地面として向きを設定→最初の回転を足す
	FVector leftVector = m_pShadowObj->GetActorForwardVector() ^ m_pShadowObj->GetActorUpVector();
	FVector newForwardVector = _newNormal ^ leftVector;
	FVector newRightVector = _newNormal ^ newForwardVector;


	FRotator newRot = UKismetMathLibrary::MakeRotationFromAxes(newForwardVector, newRightVector, _newNormal);
	//UE_LOG(LogTemp, Log, TEXT("newRot[Pitch:%f,Yaw:%f,Roll:%f"), newRot.Pitch, newRot.Yaw, newRot.Roll);


	_rotation.Yaw += m_addYaw;
	FRotator fixRot = UKismetMathLibrary::ComposeRotators(_rotation, newRot);
	m_pShadowObj->SetActorRotation(fixRot);
	//UE_LOG(LogTemp, Log, TEXT("fixRot[Pitch:%f,Yaw:%f,Roll:%f"), fixRot.Pitch, fixRot.Yaw, fixRot.Roll);
}

/**
* 影の保持を切り替え
*/
void UShadowCopyModeComponent::Hold(bool _flag)
{
	if (_flag)
	{
		if (m_pShadowObj != nullptr)
		{
			if (!m_pShadowObj->IsMoved())
			{
				m_bIsHolding = true;
				m_pShadowObj->SetHold(true);
				m_pShadowObj->SetPawnCollisionEnable(false);
			}
		}
	}
	else
	{
		m_bIsHolding = false;
		m_pShadowObj->SetHighLightMode(false);
		m_pShadowObj->SetPawnCollisionEnable(true);
		m_pShadowObj->SetHold(false);
		m_pShadowObj->SetMoved(true);

		// 22cu0327 千葉涼太郎 ---------------------------------------------------------
		if (m_pUI != nullptr)
		{
			// 一度だけ実行
			if (m_bDoOnce_Return == false)
			{
				// ReturnUIを表示
				m_pUI->VisibilityType(EUIType::ReturnShadow);
				m_bDoOnce_Return = true;
				m_bDoOnce_LeftClick = false;
				m_bDoOnce_Rotate = false;

				// ペーストするときの音を鳴らす
				m_pUI->PlaySoundbyType(EShadowSoundType::Paste);
			}
		}
		//------------------------------------------------------------------------------
	}
}

/**
* マウス下の影オブジェクトを取得し、ハイライト状態にする
*/
void UShadowCopyModeComponent::GetUnderCursorShadow()
{
	ACopyShadowObject* pPrevObj = m_pShadowObj; // 切り替わった際にハイライト状態をオフにするため保持
	bool bHitShadow = false; // 影に当たったかどうか

	FHitResult hitResult; // トレース結果
	FVector traceStart; // 始点座標
	FVector traceEnd; // 終点座標

	// マウスカーソル座標をワールド座標で取得(方向を一旦traceEndに入れておく)
	m_pPlayerController->DeprojectMousePositionToWorld(traceStart, traceEnd);
	// 長さ分伸ばした場所が終点
	traceEnd = traceStart + traceEnd * m_UnderCursorTraceLength;

	// ライントレース
	GetWorld()->LineTraceSingleByChannel(hitResult, traceStart, traceEnd, ECC_GameTraceChannel1);
	// デバッグ確認
	//DrawDebugLine(GetWorld(), traceStart, traceEnd, FColor::Red, false, .5f,0U,0.1f);


	// ヒットした場合のみ処理
	if (hitResult.bBlockingHit)
	{
		//// デバッグ確認
		//DrawDebugPoint(GetWorld(), hitResult.Location, 5.f, FColor::Red, false, .5f);
		// ログ出力
		//UE_LOG(LogTemp, Log, TEXT("HitActor::%s"), *hitResult.GetActor()->GetName());

		// 影オブジェクトをタグで判別
		if (hitResult.GetActor()->ActorHasTag("Shadow"))
		{
			m_pShadowObj = Cast<ACopyShadowObject>(hitResult.GetActor()); // 変数に保持
			m_pShadowObj->SetHighLightMode(true); // ハイライト状態にする

			// 前フレームのオブジェクトと別の影オブジェクトの場合
			if (m_pShadowObj != pPrevObj)
			{
				// 前の影オブジェクトのハイライト状態を解除
				if (pPrevObj != nullptr)
					pPrevObj->SetHighLightMode(false);
			}
			bHitShadow = true;

			// 22cu0327 千葉涼太郎 ---------------------------------------------------------
			if (m_pUI != nullptr && m_pShadowObj != nullptr)
			{
				if (m_pShadowObj->IsMoved() == true)
				{
					if (m_bDoOnce_Return == false)
					{
						m_pUI->VisibilityType(EUIType::ReturnShadow);
						m_bDoOnce_Return = true;
						m_bDoOnce_LeftClick = false;
						m_bDoOnce_Rotate = false;
					}

				}
				else
				{
					if (m_bDoOnce_LeftClick == false)
					{
						m_pUI->VisibilityType(EUIType::LeftClick);
						m_bDoOnce_LeftClick = true;
						m_bDoOnce_Rotate = false;
						m_bDoOnce_Return = false;

						// 影にカーソルをあわせた時の音を鳴らす
						m_pUI->PlaySoundbyType(EShadowSoundType::Cursor);
					}
				}
			}
			//------------------------------------------------------------------------------
		}
	}

	// 影に当たっていなかった場合
	if (!bHitShadow)
	{
		// 現在保持している影オブジェクトを解放
		if (m_pShadowObj != nullptr)
		{
			m_pShadowObj->SetHighLightMode(false);
			m_pShadowObj = nullptr;
		}

		// 22cu0327 千葉涼太郎 ---------------------------------------------------------
		if (m_pUI != nullptr)
		{
			m_pUI->VisibilityType(EUIType::None);
			m_bDoOnce_LeftClick = false;
			m_bDoOnce_Rotate = false;
			m_bDoOnce_Return = false;
		}
		//------------------------------------------------------------------------------
	}
}


/**
* Yaw回転を加算して影を回転させる
*/
void UShadowCopyModeComponent::AddYawRotation(float _deltaDegree)
{
	// 影保持中でない場合は処理をスキップ
	if (!m_bIsHolding)
		return;

	m_addYaw += _deltaDegree;
}

/**
* 現在保持している影の位置を戻す
*/
void UShadowCopyModeComponent::ResetShadowPos()
{
	if (m_pShadowObj != nullptr)
	{
		// 動かされていた時のみ処理
		if (m_pShadowObj->IsMoved())
		{
			m_pShadowObj->ResetPos();
			m_pShadowObj->SetMoved(false);
		}

		// 22cu0327 千葉涼太郎 ---------------------------------------------------------
		// 影をリセットするときの音を鳴らす
		m_pUI->PlaySoundbyType(EShadowSoundType::Reset);
		//------------------------------------------------------------------------------
	}
}