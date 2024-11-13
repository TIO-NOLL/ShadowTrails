//---------------------------------------------------------------------------
// ファイル名		：ActionObject_LightSwitch.cpp
// 概要				：ライトのスイッチ、アクションを実行した際には対応するライトオブジェクトの表示非表示を切り替えることができる
// 作成者			：22cu0127　丹代 天
// 更新内容			：2024/01/19　作成
//---------------------------------------------------------------------------


#include "ActionObject_LightSwitch.h"
#include "Components/LightComponent.h"
#include "Components/DecalComponent.h"

//コンストラクタ
AActionObject_LightSwitch::AActionObject_LightSwitch()
{
	
}

// 生成時の処理
void AActionObject_LightSwitch::BeginPlay()
{
	Super::BeginPlay();

	//m_CanOperationLightsの配列の数だけ実行する
	for (int i = 0; i < m_StartOff_CanOperationLights.Num(); ++i)
	{
		//NULLチェック
		if (m_StartOff_CanOperationLights[i])
		{
			//m_CanOperationLights[i]のコンポーネントを取得する
			ULightComponent* LightComponent = m_StartOff_CanOperationLights[i]->FindComponentByClass<ULightComponent>();

			//LightComponentの可視性を切り替える
			LightComponent->SetVisibility(false);
		}
	}

	//m_CanOperationLightsの配列の数だけ実行する
	for (int i = 0; i < m_StartOff_CanOperationDecals.Num(); ++i)
	{
		//NULLチェック
		if (m_StartOff_CanOperationDecals[i])
		{
			//m_StartOff_CanOperationDecals[i]のコンポーネントを取得する
			UDecalComponent* DecalComponent = m_StartOff_CanOperationDecals[i]->FindComponentByClass<UDecalComponent>();

			//DecalComponentの可視性を切り替える
			DecalComponent->SetVisibility(false);
		}
	}
}

//アクションが実行された時の処理
void AActionObject_LightSwitch::OnActionthisObject()
{
	//m_CanOperationLights_StartOnの配列の数だけ実行する
	for (int i = 0; i < m_StartOn_CanOperationLights.Num(); ++i)
	{
		//NULLチェック
		if (m_StartOn_CanOperationLights[i])
		{
			//m_StartOn_CanOperationLights[i]のコンポーネントを取得する
			ULightComponent* LightComponent = m_StartOn_CanOperationLights[i]->FindComponentByClass<ULightComponent>();

			//LightComponentの可視性を切り替える
			LightComponent->SetVisibility(!LightComponent->IsVisible());
		}
	}

	//m_StartOff_CanOperationLightsの配列の数だけ実行する
	for (int i = 0; i < m_StartOff_CanOperationLights.Num(); ++i)
	{
		//NULLチェック
		if (m_StartOff_CanOperationLights[i])
		{
			//m_StartOff_CanOperationLights[i]のコンポーネントを取得する
			ULightComponent* LightComponent = m_StartOff_CanOperationLights[i]->FindComponentByClass<ULightComponent>();

			//LightComponentの可視性を切り替える
			LightComponent->SetVisibility(!LightComponent->IsVisible());
		}
	}

	//m_CanOperationDecals_StartOnの配列の数だけ実行する
	for (int i = 0; i < m_StartOn_CanOperationDecals.Num(); ++i)
	{
		//NULLチェック
		if (m_StartOn_CanOperationDecals[i])
		{
			//m_StartOn_CanOperationDecals[i]のコンポーネントを取得する
			UDecalComponent* DecalComponent = m_StartOn_CanOperationDecals[i]->FindComponentByClass<UDecalComponent>();

			//DecalComponentの可視性を切り替える
			DecalComponent->SetVisibility(!DecalComponent->IsVisible());
		}
	}

	//m_CanOperationDecals_StartOffの配列の数だけ実行する
	for (int i = 0; i < m_StartOff_CanOperationDecals.Num(); ++i)
	{
		//NULLチェック
		if (m_StartOff_CanOperationDecals[i])
		{
			//m_StartOff_CanOperationDecals[i]のコンポーネントを取得する
			UDecalComponent* DecalComponent = m_StartOff_CanOperationDecals[i]->FindComponentByClass<UDecalComponent>();

			//DecalComponentの可視性を切り替える
			DecalComponent->SetVisibility(!DecalComponent->IsVisible());
		}
	}

}