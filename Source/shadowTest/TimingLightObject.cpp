//-----------------------------------------------------------------------------------
//ファイル名：TimingLightObject.cpp
// 処理内容	：点灯と消灯を一定の周期で繰り返すライトを管理するおbジェクト
// 作成者	：丹代天
// 更新日	：02/11		作成
//-----------------------------------------------------------------------------------


#include "TimingLightObject.h"
#include "Components/LightComponent.h"
#include "Components/DecalComponent.h"
#include "Engine/SpotLight.h"
#include "BoxDecal.h"
#include "EffectComponent.h"

//コンストラクタ(クラス)
ATimingLightObject::ATimingLightObject()
	: m_pEffectComponent(NULL)
	, m_State(LIGHT_STATE::TURN_OFF)
	, m_FSwitchTime({ 3.f, 3.f, 3.f, 3, 0.f, 0.f, 0 })
{
	//コンポーネントアタッチ
	m_pEffectComponent = CreateDefaultSubobject<UEffectComponent>(TEXT("EffectComponent"));
}

void ATimingLightObject::BeginPlay()
{
	Super::BeginPlay();

	//ライト------------------------------------------------------------------------------------
	//NULLチェック
	if (m_pLightComp)
	{
		//m_CanOperationLights[i]のコンポーネントを取得する
		ULightComponent* LightComponent = m_pLightComp->FindComponentByClass<ULightComponent>();

		//NULLチェック
		if (LightComponent)
		{
			//LightComponentの可視性を反転する
			LightComponent->SetVisibility(false);
		}
	}
	//------------------------------------------------------------------------------------------

	//デカール----------------------------------------------------------------------------------
	//NULLチェック
	if (m_pDecalComp)
	{
		// 可視性を反転する
		m_pDecalComp->SetActorHiddenInGame(false);

		// StaticMeshコンポーネントから当たり判定の無効化
		UStaticMeshComponent* StaticmeshComponent = m_pDecalComp->FindComponentByClass<UStaticMeshComponent>();
		//NULLチェック
		if (StaticmeshComponent)
		{
			StaticmeshComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
		}

		// Decalコンポーネントからデカールの無効化
		// StaticMeshコンポーネントから当たり判定の無効化
		UDecalComponent* DecalComponent = m_pDecalComp->FindComponentByClass<UDecalComponent>();
		//NULLチェック
		if (DecalComponent)
		{
			DecalComponent->SetVisibility(false);
		}

	}
	//------------------------------------------------------------------------------------------

	PrimaryActorTick.bCanEverTick = true;
}

void ATimingLightObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	switch (m_State)
	{
	case ATimingLightObject::TURN_OFF:

		//カウントを加算
		m_FSwitchTime.TimeCnt += DeltaTime;

		//設定した時間と比較(消灯～準備)
		if (m_FSwitchTime.m_OfftoPreparation < m_FSwitchTime.TimeCnt)
		{
			//ステートを切り替える
			m_State = LIGHT_STATE::PREPARATION;

			//カウントをリセット
			m_FSwitchTime.TimeCnt = 0.f;
		}

		break;
	case ATimingLightObject::PREPARATION:


		//警告
		if ((m_FSwitchTime.m_PreparationtoOn / (float)(m_FSwitchTime.m_Cavatefrequency + 1) * (float)m_FSwitchTime.PlayNum) <= m_FSwitchTime.CavateCnt)
		{
				if (m_pEffectComponent)
				{
					//箱のメッシュコンポーネントを取得
					UStaticMeshComponent* pObjectMesh = m_pObjectMeshComp;

					//箱のメッシュサイズを取得する。
					FVector ObjectMeshSize = pObjectMesh->Bounds.GetBox().GetSize() * GetActorScale();

					//このオブジェクトを基準に前方にエフェクトのスポーン位置を計算する
					FVector Dist = GetActorForwardVector();

					//エフェクトを再生
					m_pEffectComponent->PlayEffect(RootComponent, FVector(0.f, 20.f, 0.f), FRotator(0.f, 0.f, 0.f), true);
				}

			//カウントをリセット
			m_FSwitchTime.CavateCnt = 0.f;

			//回数を加算
			m_FSwitchTime.PlayNum++;
		}

		//カウントを加算
		m_FSwitchTime.TimeCnt += DeltaTime;
		m_FSwitchTime.CavateCnt += DeltaTime;

		//設定した時間と比較(準備～点灯)
		if (m_FSwitchTime.m_PreparationtoOn < m_FSwitchTime.TimeCnt)
		{
			//ステートを切り替える
			m_State = LIGHT_STATE::TURN_ON;

			//ライトをオンする
			LightSwitch(true);

			//カウントをリセット
			m_FSwitchTime.TimeCnt = 0.f;
			m_FSwitchTime.PlayNum = 0;
		}

		break;
	case ATimingLightObject::TURN_ON:

		//カウントを加算
		m_FSwitchTime.TimeCnt += DeltaTime;

		//設定した時間と比較(点灯～消灯)
		if (m_FSwitchTime.m_OntoOff < m_FSwitchTime.TimeCnt)
		{
			//ステートを切り替える
			m_State = LIGHT_STATE::TURN_OFF;

			//ライトをオフする
			LightSwitch(false);

			//カウントをリセット
			m_FSwitchTime.TimeCnt = 0.f;
		}
		break;
	}


}

//オンオフを切り替える処理
void ATimingLightObject::LightSwitch(bool LightState)
{
	//ライト------------------------------------------------------------------------------------
	//NULLチェック
	if (m_pLightComp)
	{
		//m_CanOperationLights[i]のコンポーネントを取得する
		ULightComponent* LightComponent = m_pLightComp->FindComponentByClass<ULightComponent>();

		//NULLチェック
		if (LightComponent)
		{
			//LightComponentの可視性を反転する
			LightComponent->SetVisibility(LightState);
		}
	}
	//------------------------------------------------------------------------------------------

	//デカール----------------------------------------------------------------------------------
	//NULLチェック
	if (m_pDecalComp)
	{
		// 可視性を反転する
		m_pDecalComp->SetActorHiddenInGame(!LightState);

		// StaticMeshコンポーネントから当たり判定の無効化
		UStaticMeshComponent* StaticmeshComponent = m_pDecalComp->FindComponentByClass<UStaticMeshComponent>();
		//NULLチェック
		if (StaticmeshComponent)
		{
			if (LightState)
				StaticmeshComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
			else
				StaticmeshComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

		}

		// Decalコンポーネントからデカールの無効化
		UDecalComponent* DecalComponent = m_pDecalComp->FindComponentByClass<UDecalComponent>();
		//NULLチェック
		if (DecalComponent)
		{
			DecalComponent->SetVisibility(LightState);
		}
	}
	//------------------------------------------------------------------------------------------
}