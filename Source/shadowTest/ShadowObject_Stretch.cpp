/**
* @file		ShadowObject_Stretch.cpp
* @brief	�L�яk�݂���e�I�u�W�F�N�g
* @author	22cu0219 ��ؗF��
* @date		24/02/08 �쐬
*			24/02/17 �ŏI�X�V
*/

#include "ShadowObject_Stretch.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/DecalComponent.h"
#include "Components/BoxComponent.h"

/**
* �R���X�g���N�^
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
* �J�n���Ɉ��Ă΂��֐�
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

	// �X�P�[�����镝
	FTransform scaletrans;
	float scale = FMath::Lerp(1.0f, m_stretchScale, m_scaleAlpha);
	scaletrans.SetScale3D(FVector(1.0f, 1.0f, scale));

	// �X�P�[������ʒu�����炷�g�����X�t�H�[��
	FTransform pivotTrans;
	pivotTrans.SetLocation(FVector(0.f,0.f,32.f));

	// �X�P�[������ʒu���猴�_�֖߂��g�����X�t�H�[��
	FTransform fixtrans;
	fixtrans.SetLocation(FVector(0.f, 0.f, -32.f));

	auto newtrans = (pivotTrans * scaletrans) * fixtrans;

	// ��������Ă���or�ێ����͐L�яk�݂��Ȃ�
	if (!m_bIsMoved && !m_bIsHold)
		m_pBoxComponent->SetRelativeTransform(newtrans);
	//UE_LOG(LogTemp, Warning, TEXT("%s"), *newtrans.ToString());
}