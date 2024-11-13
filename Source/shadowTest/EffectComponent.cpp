//---------------------------------------------------------------------------
// ファイル名		：EffectComponent.cpp
// 概要				：エフェクトを保持するコンポーネント
// 作成者			：22cu0127　丹代 天
// 更新内容			：2023/01/14　作成
//---------------------------------------------------------------------------

#include "EffectComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"


// コンストラクタ
UEffectComponent::UEffectComponent()
	: ParticleEffect(NULL)
	, NiagaraEffect(NULL)
	, m_ActiveEffectComponent(NULL)
	, m_ActiveNiagaraComponent(NULL)
{
	// Tickが呼ばれるかどうかを決める処理
	PrimaryComponentTick.bCanEverTick = true;
}


// 生成時に呼ばれる処理
void UEffectComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

}


// フレームごとに呼ばれる処理
void UEffectComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

/*
* エフェクトを再生する関数
*/
void UEffectComponent::PlayEffect(USceneComponent* _rootComponent, FVector _spawnVector, FRotator _spawnRotator, bool _autoDestoroy)
{
	// パーティクルエフェクトを再生
	if (ParticleEffect)
	{
		m_ActiveEffectComponent = UGameplayStatics::SpawnEmitterAttached(
			ParticleEffect,  // スポーンするエフェクト
			_rootComponent,  // アタッチルール
			FName("SocketName"),  // アタッチするソケットの名前
			_spawnVector,  // スポーン位置
			_spawnRotator,  // スポーン回転
			EAttachLocation::SnapToTarget,  // アタッチのタイプ
			_autoDestoroy  // オートデストロイ
		);
	}

	// ナイアガラエフェクトを再生
	if (NiagaraEffect)
	{
		m_ActiveNiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
			NiagaraEffect,  // スポーンするエフェクト
			_rootComponent,  // アタッチルール
			FName("SocketName"),  // アタッチするソケットの名前
			_spawnVector,  // スポーン位置
			_spawnRotator,  // スポーン回転
			EAttachLocation::SnapToTarget,  // アタッチのタイプ
			_autoDestoroy  // オートデストロイ
		);
	}
}

/*
* エフェクトを破棄する関数
*/
void UEffectComponent::StopEffect()
{
	//現在パーティクルエフェクトが再生中だった場合
	if (m_ActiveEffectComponent)
	{
		//パーティクルエフェクトを破棄
		m_ActiveEffectComponent->Deactivate();
		//nullptrを代入
		m_ActiveEffectComponent = nullptr;
	}

	//現在ナイアガラエフェクトが再生中だった場合
	if (m_ActiveNiagaraComponent)
	{
		//ナイアガラエフェクトを破棄
		m_ActiveNiagaraComponent->Deactivate();
		//nullptrを代入
		m_ActiveNiagaraComponent = nullptr;
	}
}