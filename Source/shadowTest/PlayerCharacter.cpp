//------------------------------------------------
// �t�@�C�����FPlayerCharacter.h
// �������e	�FPlayer�̃L�����N�^�[�N���X
// �쐬��	�F�O��V
// �X�V��	�F12/11		�쐬
//------------------------------------------------

#include "PlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ShadowCopyModeComponent.h"
#include "DecalLightObject.h"
#include "EffectComponent.h"

APlayerCharacter::APlayerCharacter()
	: m_pShadowComponent(nullptr)
	, m_pDeadEffectComponent(nullptr)
	, m_pMeshMaterial(nullptr)
	, m_RotateDeglee(.5f)
	, m_bClockwisePress(false)
	, m_bCounterClockwisePress(false)
	, m_bDead(false)
	, m_DeadTime(3.f)
	, m_DeadTimeCounter(0.f)
	, m_DownSpeed(0.f)
	, m_hittingLightNum(0)
	, m_hittingShadowNum(0)
	, m_bPlayerVisible(true)
	, m_FlashInterval(0.3f)
	, m_FlashDuration(0.0f)
	, m_ShadowRotateValue(0.f)
{
	// Tick()�𓮂������ǂ�����؂�ւ�
	PrimaryActorTick.bCanEverTick = true;

	// �e�R�s�[�p�̃R���|�[�l���g����
	m_pShadowComponent = CreateDefaultSubobject<UShadowCopyModeComponent>(TEXT("m_pShadowCopyComponent"));
	AddOwnedComponent(m_pShadowComponent);

	// ���S���G�t�F�N�g�̃R���|�[�l���g���쐬
	m_pDeadEffectComponent = CreateDefaultSubobject<UEffectComponent>(TEXT("DeadEffect"));
	AddOwnedComponent(m_pDeadEffectComponent);

	UCapsuleComponent* pCapsuleComp = GetCapsuleComponent();
	// �f���Q�[�g
	pCapsuleComp->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnOverlapBegin);
	pCapsuleComp->OnComponentEndOverlap.AddDynamic(this, &APlayerCharacter::OnOverlapEnd);

	// �^�O�̒ǉ�
	Tags = { "Player" };
}

// �������̏���
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// 22cu0219 ��ؗF�� �L�q:24/02/15
	USkeletalMeshComponent* mesh = GetMesh();
	m_pMeshMaterial = mesh->CreateDynamicMaterialInstance(0);
}

void APlayerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// 22cu0219 ��؂��L�q-----------------
	//�e�J�b�g�E�y�[�X�g
	InputComponent->BindAction("CutPaste", IE_Pressed, this, &APlayerCharacter::CutPaste);

	// ���v����]���{�^��������Ԃɂ���Đ؂�ւ�
	InputComponent->BindAction("ClockwiseRotate", IE_Pressed, this, &APlayerCharacter::SwapClockwisePressReleased);
	InputComponent->BindAction("ClockwiseRotate", IE_Released, this, &APlayerCharacter::SwapClockwisePressReleased);

	// �����v����]���{�^��������Ԃɂ���Đ؂�ւ�
	InputComponent->BindAction("CounterClockwiseRotate", IE_Pressed, this, &APlayerCharacter::SwapCounterClockwisePressReleased);
	InputComponent->BindAction("CounterClockwiseRotate", IE_Released, this, &APlayerCharacter::SwapCounterClockwisePressReleased);

	// �ړ��ς݉e�ɃJ�[�\�������킹�ă{�^���������ƈʒu���Z�b�g
	InputComponent->BindAction("ResetPos", IE_Pressed, this, &APlayerCharacter::ResetShadowPos);
	//------------------------------------

	InputComponent->BindAxis("ShadowRotate", this, &APlayerCharacter::RotateShadow);

}

// �t���[�����Ƃ̏���
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// �_���[�W����
	if (m_hittingLightNum && !m_hittingShadowNum && !m_bDead)
	{
		m_DeadTimeCounter += DeltaTime;

		// 22cu0222 �c�� �ǋL�F2024/02/13---------------------------------------
		// �ړ��ʂ�����������ʂ����Z
		m_DownSpeed += 0.05f;

		StateChangeDamage();  // �X�e�[�^�X���_���[�W�ɕύX
		ChangeSpeed(m_DownSpeed); // �ړ��ύX

			// �v���C���[�_�ŏ���
		PlayerFlashing(DeltaTime);
		// --------------------------------------------------------------------

		// 22cu0327 ��t�����Y �ǋL�F2024/02/08-------------------------------------
		m_bDamage = true;
		// -------------------------------------------------------------------------

		/*GEngine->AddOnScreenDebugMessage(-1, .1f, FColor::Orange, "Light Hit");*/
	}
	// 22cu0327 ��t�����Y �ǋL�F2024/02/08-------------------------------------
	else
	{
		m_bDamage = false;

		// 22cu0222 �c�� �ǋL�F2024/02/13---------------------------------------
		m_DownSpeed = 0;
		SetActorHiddenInGame(false);
		StateChangeNormal();
		// --------------------------------------------------------------------
	}
	// -------------------------------------------------------------------------

	//-------------------------------------------------------------
	// 22cu0219��ؗF�� �ǋL:24/02/15
	//=============================================================
	// �e�̒��ɂ���ꍇ�}�e���A������������

	if (m_hittingShadowNum > 0)
		m_pMeshMaterial->SetScalarParameterValue(TEXT("BlackBlend"), 0.2f);
	else
		m_pMeshMaterial->SetScalarParameterValue(TEXT("BlackBlend"), 1.0f);

	//-------------------------------------------------------------------------

	// ���S������
	if (m_DeadTime < m_DeadTimeCounter)
	{
		if (!m_bDead)
		{
			// ���b�V���擾
			USkeletalMeshComponent* mesh = GetMesh();
			mesh->SetVisibility(false);

			// ���S�������Ă����e�͌��̈ʒu�ɖ߂�
			if (m_pShadowComponent != nullptr)
			{
				if (m_pShadowComponent->IsHold())
				{
					m_pShadowComponent->Hold(false);
					m_pShadowComponent->ResetShadowPos();
				}
			}

			// ���S�G�t�F�N�g���Đ�
			m_pDeadEffectComponent->PlayEffect(GetRootComponent(), GetActorLocation(), GetActorRotation(), true);
			SetCanMove(false); // �ړ�������

			m_bDead = true;
			StateChangeDead();  // �X�e�[�^�X��Dead�ɕύX

			//// ���O�o��
			//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, "Dead");
		}
	}


	// 22cu0219 ��؂��L�q----
	if (m_pShadowComponent != nullptr)
	{
		//// �e�̉�]
		//// ���v����]
		//if (m_bClockwisePress)
		//	m_pShadowComponent->AddYawRotation(m_RotateDeglee);

		//// �����v����]
		//if (m_bCounterClockwisePress)
		//	m_pShadowComponent->AddYawRotation(-1 * m_RotateDeglee);

		//22Cu0127 �O��V 2024/03/04------------------------------
		m_pShadowComponent->AddYawRotation(m_ShadowRotateValue);
		//--------------------------------------------------------
	}
	//-----------------------
}
/*
* 22cu0127 �O��V 2024/03/04
* �e����]����֐�
*/
void APlayerCharacter::RotateShadow(float _AxisValue)
{
	m_ShadowRotateValue = _AxisValue * m_RotateDeglee;
}

/**
* @author	22cu0219 ��ؗF��
* @brief	���S��Ԃ̃v���C���[�����ɖ߂�
*/
void APlayerCharacter::ResetPlayer()
{
	// ���b�V���擾
	USkeletalMeshComponent* mesh = GetMesh();
	mesh->SetVisibility(true); // ���b�V��������

	// �J�E���^�[��������
	m_DeadTimeCounter = 0.f;

	m_bDead = false;
	StateChangeNormal(); // �X�e�[�g�؂�ւ�
	SetCanMove(true); // �ړ��L����
	ResetRelativeRotation();
}

/**
* @author	22cu0219 ��ؗF��
* @brief	�y���̓o�C���h�z�e�J�b�g�y�[�X�g��؂�ւ���
*/
void APlayerCharacter::CutPaste()
{
	if (m_pShadowComponent != nullptr)
	{
		m_pShadowComponent->Hold(!m_pShadowComponent->IsHold());
	}
}

/**
* @author	22cu0219 ��ؗF��
* @brief	�y���̓o�C���h�z�e�������ʒu�ɖ߂�
*/
void APlayerCharacter::ResetShadowPos()
{
	if (m_pShadowComponent != nullptr)
	{
		m_pShadowComponent->ResetShadowPos();
	}
}

// 22cu0222 �c���@�ǋL�F2024/02/13 -----------------------------
// �v���C���[�_�ŏ���
void APlayerCharacter::PlayerFlashing(float DeltaTime)
{
	//FLinearColor Color;
	//m_pMaterialInstanceDynamic->GetVectorParameterValue(TEXT("ColorParaneterName"), Color);

	m_FlashDuration += DeltaTime;

	if (m_FlashDuration >= m_FlashInterval)
	{
		if (m_bPlayerVisible)
		{
			SetActorHiddenInGame(true);
			//Color.R = 1.0f;
			m_FlashDuration = 0.0f;
			m_bPlayerVisible = false;
		}
		else
		{
			SetActorHiddenInGame(false);
			//Color.R = 0.0f;
			m_FlashDuration = 0.0f;
			m_bPlayerVisible = true;
		}
	}
}
// -------------------------------------------------------------


void APlayerCharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("DecalLight"))
	{
		m_hittingLightNum++;
	}
	else if (OtherActor->ActorHasTag("Shadow"))
	{
		m_hittingShadowNum++;
	}
}


void APlayerCharacter::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->ActorHasTag("DecalLight"))
	{
		m_hittingLightNum--;
	}
	else if (OtherActor->ActorHasTag("Shadow"))
	{
		m_hittingShadowNum--;
	}
}

// 22cu0327 ��t�����Y �ǋL�F2024/02/08-------------------------------------
bool APlayerCharacter::GetbDamage()
{
	return m_bDamage;
}
//--------------------------------------------------------------------------