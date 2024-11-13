/**
* @file		SplineMovementComponent.cpp
* @brief	�A�N�^���X�v���C���ɉ����Ĉړ�������R���|�[�l���g
* @author	22cu0219�@��ؗF��
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

	// �X�v���C���R���|�[�l���g���擾
	if(m_pSplineActor != nullptr)
		m_pSplineComponent = m_pSplineActor->FindComponentByClass<USplineComponent>();

	// �n�_�ֈړ�
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

	// �X�v���C���R���|�[�l���g�������������ꍇ������
	if (!m_pSplineComponent.IsValid())
		return;

	AActor* ownerActor = GetOwner();
	if (ownerActor == nullptr)
		return;

	// ������Z���鋗��
	float addDistance = DeltaTime * m_moveSpeed;
	// �t�s�t���O��true�Ȃ�}�C�i�X�ɂ���
	if (m_bReverse)
		addDistance *= -1;
	// �X�v���C���̑S��
	float splineLength = m_pSplineComponent->GetSplineLength();

	if(0.f <= m_currentMovedDistance && m_currentMovedDistance <= splineLength)
		m_currentMovedDistance += addDistance;
	else
		m_waitCounter += DeltaTime;


	// Loop���Ȃ��ꍇ�����Ă����璷������
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


	// �ړ���������X�v���C����̃��[���h���W���Z�o
	FVector TargetLocation = m_pSplineComponent->GetLocationAtDistanceAlongSpline(m_currentMovedDistance, ESplineCoordinateSpace::World);


	// �����A�N�^�[�Ɍv�Z�������W�Ɉړ�������
	ownerActor->SetActorLocation(TargetLocation);
}

