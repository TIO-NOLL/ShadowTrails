/**
* @file		CopyShadowObject.cpp
* @brief	�e�̃R�s�[���������邽�߂́A�e�I�u�W�F�N�g
* @author	22CU0219 ��ؗF��
* @date		23/12/10 �쐬
*			23/02/17 �ŏI�X�V
*/

#include "CopyShadowObject.h"
#include "Components/DecalComponent.h"
#include "Components/BoxComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetSystemLibrary.h" 
//�O��V �ǋL�F2024/01/31----------------------------------------------------------------
#include "EffectComponent.h"
#include "Kismet/GameplayStatics.h"
//--------------------------------------------------------------------------------------
/**
* �R���X�g���N�^
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
	//�O��V �ǋL�F2024/01/31----------------------------------------------------------------
	, m_bNowEffectVisibility(false)
	, m_pPlayer(NULL)
	, m_PlayerDistance(0.f)
	, m_ActivateRange(500.f)
	//--------------------------------------------------------------------------------------
{
	PrimaryActorTick.bCanEverTick = true;
	if (m_pBoxComponent != nullptr)
	{
		// �o�C���h
		m_pBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ACopyShadowObject::OnOverlapBegin);
		m_pBoxComponent->OnComponentEndOverlap.AddDynamic(this, &ACopyShadowObject::OnOverlapEnd);

		// �`�����l��[CopyShadow]�̃g���[�X��L����
		m_pBoxComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Block);
	}

	//�O��V �ǋL�F2024/01/31----------------------------------------------------------------
		//�R���|�[�l���g�A�^�b�`
	EffectComponent = CreateDefaultSubobject<UEffectComponent>(TEXT("EffectComponent"));
	AddOwnedComponent(EffectComponent);
	//--------------------------------------------------------------------------------------

	// �^�O�ǉ�
	Tags = { "Shadow" };
}


/**
* �J�n���Ɉ��Ă΂��֐�
*/
void ACopyShadowObject::BeginPlay()
{
	Super::BeginPlay();

	// �_�C�i�~�b�N�}�e���A���C���X�^���X���쐬
	m_pDecalMaterialInst = m_pDecalComponent->CreateDynamicMaterialInstance();
	m_DefaultTrans = GetActorTransform();

	//�O��V �ǋL�F2024/01/31----------------------------------------------------------------
		//m_pPlayer����̎��̂�
	if (m_pPlayer == NULL)
	{
		//�Q�[���S�̂ւ�Actor��������(�R�X�g��)
		TSubclassOf<AActor> findClass;
		findClass = AActor::StaticClass();
		TArray<AActor*> actors;
		UGameplayStatics::GetAllActorsOfClass(this->GetWorld(), findClass, actors);

		//�����̌���Actro�������
		if (actors.Num() > 0)
		{
			//Actor�̒��g�����ԂɌ������Ă���
			for (int idx = 0; idx < actors.Num(); idx++)
			{
				AActor* pActor = Cast<AActor>(actors[idx]);
				//�^�O���v���C���[��������
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
* ���t���[���Ăяo�����֐�
*/
void ACopyShadowObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//�O��V �ǋL�F2024/01/31----------------------------------------------------------------
	MeasurementToPlayer();
	//--------------------------------------------------------------------------------------
}

/**
* �n�C���C�g���[�h��L��/������
*/
void ACopyShadowObject::SetHighLightMode(bool _flag)
{
	m_bHighLight = _flag;

	if (_flag)
	{
		if (m_pDecalMaterialInst != nullptr)
		{
			m_pDecalMaterialInst->SetScalarParameterValue(TEXT("EmissiveBlend"), m_emissiveValue);

			// �������ꂽ��ŐF��؂�ւ�
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
* �ێ��t���O��ݒ�
*/
void ACopyShadowObject::SetHold(bool _flag)
{
	if (_flag)
	{
		m_bIsHold = true;
		//�G�t�F�N�g��j������
		EffectComponent->StopEffect();
		//�G�t�F�N�g��j�������Ƃ����t���O���グ�Ă���
		m_bNowEffectVisibility = false;
	}
	else
	{
		m_bIsHold = false;
	}
}

/**
* �ړ��ς݃t���O��ݒ�
*/
void ACopyShadowObject::SetMoved(bool _flag)
{
	if (_flag)
	{
		m_pDecalMaterialInst->SetScalarParameterValue(TEXT("EmissiveBlend"), 0.f);
		m_bIsMoved = true;
		//�G�t�F�N�g��j������
		EffectComponent->StopEffect();
		//�G�t�F�N�g��j�������Ƃ����t���O���グ�Ă���
		m_bNowEffectVisibility = false;
	}
	else
	{
		m_bIsMoved = false;
	}
}

/**
* Pawn�̃I�u�W�F�N�g������؂�ւ�
*/
void ACopyShadowObject::SetPawnCollisionEnable(bool _flag)
{
	if (_flag)
		m_pBoxComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	else
		m_pBoxComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
}

/**
* OnComponentBeginOverlap�Ƀo�C���h����֐�
*/
void ACopyShadowObject::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// �v���C���[��Overlap�������Ƀt���O�𗧂Ă�
	if (OtherActor->ActorHasTag("Player"))
	{
		m_pHitPlayer = OtherActor;
		m_bIsPlayerHit = true;
	}
}

/**
* OnComponentEndOverlap�Ƀo�C���h����֐�
*/
void ACopyShadowObject::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// �v���C���[��OverlapEnd�������Ƀt���O������
	if (OtherActor->ActorHasTag("Player"))
	{
		m_bIsPlayerHit = false;
	}
}

//�O��V �ǋL�F2024/01/31----------------------------------------------------------------
/*
* �v���C���[�Ƃ̋����𑪒肵bool�^�̖߂�l��Ԃ��֐�
*/
void ACopyShadowObject::MeasurementToPlayer()
{
	//�v���C���[���i�[�ł��Ă���ꍇ
	if (m_pPlayer)
	{
		//�v���C���[�Ƃ��̃A�N�^�[�̋������Z�o����
		m_PlayerDistance = FVector::Dist(GetActorLocation(), m_pPlayer->GetActorLocation());

		//�v���C���[���w��̋����ȓ����G�t�F�N�g���N�����Ă��Ȃ��ꍇ
		if (m_PlayerDistance < m_ActivateRange && m_bNowEffectVisibility == false)
		{

			// �I�u�W�F�N�g��ForwardVector���擾
			FVector ObjectForwardVector = GetActorForwardVector();

			FVector WorldUpVector;
			FRotator RotationToVertical;

			if (GetActorRotation().Roll >= GetActorRotation().Pitch)
			{
				WorldUpVector = FVector(0.f, 0.f, 1.f);

				// ForwardVector��UpVector�̊Ԃ̉�]���v�Z
				RotationToVertical = FRotationMatrix::MakeFromXZ(ObjectForwardVector, WorldUpVector).Rotator();
			}
			else
			{
				WorldUpVector = FVector(1.f, 0.f, 0.f);
				RotationToVertical = FRotationMatrix::MakeFromYZ(ObjectForwardVector, WorldUpVector).Rotator();
			}

			// ��������Ă��Ȃ��ꍇ
			if (!m_bIsMoved && !m_bIsHold)
			{
				//�G�t�F�N�g���X�|�[��������
				EffectComponent->PlayEffect(RootComponent, FVector::ZeroVector, RotationToVertical, false);
				//�G�t�F�N�g���N�������Ƃ����t���O���グ�Ă���
				m_bNowEffectVisibility = true;
			}
		}
		//�v���C���[���w��̋������O���G�t�F�N�g���N�����Ă���ꍇ
		else if (m_PlayerDistance > m_ActivateRange && m_bNowEffectVisibility == true)
		{
			//�G�t�F�N�g��j������
			EffectComponent->StopEffect();
			//�G�t�F�N�g��j�������Ƃ����t���O���グ�Ă���
			m_bNowEffectVisibility = false;
		}
	}
}