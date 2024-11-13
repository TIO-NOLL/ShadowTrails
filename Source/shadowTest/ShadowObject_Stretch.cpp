/**
* @file		ShadowObject_Stretch.cpp
* @brief	伸び縮みする影オブジェクト
* @author	22cu0219 鈴木友也
* @date		24/02/08 作成
*			24/02/17 最終更新
*/

#include "ShadowObject_Stretch.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/DecalComponent.h"
#include "Components/BoxComponent.h"

/**
* コンストラクタ
*/
AShadowObject_Stretch::AShadowObject_Stretch()
	: m_pSceneComponent(nullptr)
	, m_scaleAlpha(0.f)
	, m_cycleLength(1.f)
{
	m_pSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));

	if (m_pSceneComponent != nullptr)
	{
		RootComponent = m_pSceneComponent;

		if (m_pBoxComponent != nullptr)
		{
			m_pBoxComponent->SetupAttachment(RootComponent);

			if (m_pDecalComponent != nullptr)
			{
				m_pDecalComponent->SetupAttachment(m_pBoxComponent);
			}
		}
	}
}

/**
* 開始時に一回呼ばれる関数
*/
void AShadowObject_Stretch::BeginPlay()
{
	Super::BeginPlay();


}

void AShadowObject_Stretch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	m_timeCounter += DeltaTime;

	m_scaleAlpha = abs(sin(m_timeCounter * (PI / m_cycleLength)));

	// スケールする幅
	FTransform scaletrans;
	float scale = FMath::Lerp(1.0f, m_stretchScale, m_scaleAlpha);
	scaletrans.SetScale3D(FVector(1.0f, 1.0f, scale));

	// スケールする位置をずらすトランスフォーム
	FTransform pivotTrans;
	pivotTrans.SetLocation(FVector(0.f,0.f,32.f));

	// スケールする位置から原点へ戻すトランスフォーム
	FTransform fixtrans;
	fixtrans.SetLocation(FVector(0.f, 0.f, -32.f));

	auto newtrans = (pivotTrans * scaletrans) * fixtrans;

	// 動かされているor保持中は伸び縮みしない
	if (!m_bIsMoved && !m_bIsHold)
		m_pBoxComponent->SetRelativeTransform(newtrans);
	//UE_LOG(LogTemp, Warning, TEXT("%s"), *newtrans.ToString());
}