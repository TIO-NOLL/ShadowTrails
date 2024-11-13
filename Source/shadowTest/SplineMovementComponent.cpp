/**
* @file		SplineMovementComponent.cpp
* @brief	アクタをスプラインに沿って移動させるコンポーネント
* @author	22cu0219　鈴木友也
*/

#include "SplineMovementComponent.h"
#include "Components/SplineComponent.h"

// Sets default values for this component's properties
USplineMovementComponent::USplineMovementComponent()
	: m_pSplineActor(nullptr)
	, m_moveSpeed(100.f)
	, m_interpSpeed(100.f)
	, m_currentMovedDistance(0.f)
	, m_bLoop(true)
	, m_bReturn(false)
	, m_bReverse(false)
	, m_returnWaitSecond(0.f)
	, m_waitCounter(0.f)
	, m_pSplineComponent(nullptr)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void USplineMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	// スプラインコンポーネントを取得
	if(m_pSplineActor != nullptr)
		m_pSplineComponent = m_pSplineActor->FindComponentByClass<USplineComponent>();

	// 始点へ移動
	if (m_pSplineComponent != nullptr)
	{
		FVector startLocation = m_pSplineComponent->GetLocationAtDistanceAlongSpline(0.f, ESplineCoordinateSpace::World);
		AActor* ownerActor = GetOwner();
		ownerActor->SetActorLocation(startLocation);
	}
}


// Called every frame
void USplineMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// スプラインコンポーネントが無効だった場合抜ける
	if (!m_pSplineComponent.IsValid())
		return;

	AActor* ownerActor = GetOwner();
	if (ownerActor == nullptr)
		return;

	// 今回加算する距離
	float addDistance = DeltaTime * m_moveSpeed;
	// 逆行フラグがtrueならマイナスにする
	if (m_bReverse)
		addDistance *= -1;
	// スプラインの全長
	float splineLength = m_pSplineComponent->GetSplineLength();

	if(0.f <= m_currentMovedDistance && m_currentMovedDistance <= splineLength)
		m_currentMovedDistance += addDistance;
	else
		m_waitCounter += DeltaTime;


	// Loopしない場合超えていたら長さを代入
	if (!m_bLoop)
	{
		if (m_currentMovedDistance > splineLength)
			m_currentMovedDistance = splineLength;
	}
	
	

	if (m_waitCounter > m_returnWaitSecond)
	{
		if (!m_bReturn)
			m_bReverse = !m_bReverse;
		else
			m_currentMovedDistance = 0.f;

		if (0.f > m_currentMovedDistance)
			m_currentMovedDistance = 0.f;
		else if(m_currentMovedDistance > splineLength)
			m_currentMovedDistance = splineLength;

		m_waitCounter = 0.f;
	}


	// 移動距離からスプライン上のワールド座標を算出
	FVector TargetLocation = m_pSplineComponent->GetLocationAtDistanceAlongSpline(m_currentMovedDistance, ESplineCoordinateSpace::World);


	// 所持アクターに計算した座標に移動させる
	ownerActor->SetActorLocation(TargetLocation);
}

