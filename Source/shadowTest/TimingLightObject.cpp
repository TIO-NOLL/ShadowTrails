//-----------------------------------------------------------------------------------
//�t�@�C�����FTimingLightObject.cpp
// �������e	�F�_���Ə��������̎����ŌJ��Ԃ����C�g���Ǘ����邨b�W�F�N�g
// �쐬��	�F�O��V
// �X�V��	�F02/11		�쐬
//-----------------------------------------------------------------------------------


#include "TimingLightObject.h"
#include "Components/LightComponent.h"
#include "Components/DecalComponent.h"
#include "Engine/SpotLight.h"
#include "BoxDecal.h"
#include "EffectComponent.h"

//�R���X�g���N�^(�N���X)
ATimingLightObject::ATimingLightObject()
	: m_pEffectComponent(NULL)
	, m_State(LIGHT_STATE::TURN_OFF)
	, m_FSwitchTime({ 3.f, 3.f, 3.f, 3, 0.f, 0.f, 0 })
{
	//�R���|�[�l���g�A�^�b�`
	m_pEffectComponent = CreateDefaultSubobject<UEffectComponent>(TEXT("EffectComponent"));
}

void ATimingLightObject::BeginPlay()
{
	Super::BeginPlay();

	//���C�g------------------------------------------------------------------------------------
	//NULL�`�F�b�N
	if (m_pLightComp)
	{
		//m_CanOperationLights[i]�̃R���|�[�l���g���擾����
		ULightComponent* LightComponent = m_pLightComp->FindComponentByClass<ULightComponent>();

		//NULL�`�F�b�N
		if (LightComponent)
		{
			//LightComponent�̉����𔽓]����
			LightComponent->SetVisibility(false);
		}
	}
	//------------------------------------------------------------------------------------------

	//�f�J�[��----------------------------------------------------------------------------------
	//NULL�`�F�b�N
	if (m_pDecalComp)
	{
		// �����𔽓]����
		m_pDecalComp->SetActorHiddenInGame(false);

		// StaticMesh�R���|�[�l���g���瓖���蔻��̖�����
		UStaticMeshComponent* StaticmeshComponent = m_pDecalComp->FindComponentByClass<UStaticMeshComponent>();
		//NULL�`�F�b�N
		if (StaticmeshComponent)
		{
			StaticmeshComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
		}

		// Decal�R���|�[�l���g����f�J�[���̖�����
		// StaticMesh�R���|�[�l���g���瓖���蔻��̖�����
		UDecalComponent* DecalComponent = m_pDecalComp->FindComponentByClass<UDecalComponent>();
		//NULL�`�F�b�N
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

		//�J�E���g�����Z
		m_FSwitchTime.TimeCnt += DeltaTime;

		//�ݒ肵�����ԂƔ�r(�����`����)
		if (m_FSwitchTime.m_OfftoPreparation < m_FSwitchTime.TimeCnt)
		{
			//�X�e�[�g��؂�ւ���
			m_State = LIGHT_STATE::PREPARATION;

			//�J�E���g�����Z�b�g
			m_FSwitchTime.TimeCnt = 0.f;
		}

		break;
	case ATimingLightObject::PREPARATION:


		//�x��
		if ((m_FSwitchTime.m_PreparationtoOn / (float)(m_FSwitchTime.m_Cavatefrequency + 1) * (float)m_FSwitchTime.PlayNum) <= m_FSwitchTime.CavateCnt)
		{
				if (m_pEffectComponent)
				{
					//���̃��b�V���R���|�[�l���g���擾
					UStaticMeshComponent* pObjectMesh = m_pObjectMeshComp;

					//���̃��b�V���T�C�Y���擾����B
					FVector ObjectMeshSize = pObjectMesh->Bounds.GetBox().GetSize() * GetActorScale();

					//���̃I�u�W�F�N�g����ɑO���ɃG�t�F�N�g�̃X�|�[���ʒu���v�Z����
					FVector Dist = GetActorForwardVector();

					//�G�t�F�N�g���Đ�
					m_pEffectComponent->PlayEffect(RootComponent, FVector(0.f, 20.f, 0.f), FRotator(0.f, 0.f, 0.f), true);
				}

			//�J�E���g�����Z�b�g
			m_FSwitchTime.CavateCnt = 0.f;

			//�񐔂����Z
			m_FSwitchTime.PlayNum++;
		}

		//�J�E���g�����Z
		m_FSwitchTime.TimeCnt += DeltaTime;
		m_FSwitchTime.CavateCnt += DeltaTime;

		//�ݒ肵�����ԂƔ�r(�����`�_��)
		if (m_FSwitchTime.m_PreparationtoOn < m_FSwitchTime.TimeCnt)
		{
			//�X�e�[�g��؂�ւ���
			m_State = LIGHT_STATE::TURN_ON;

			//���C�g���I������
			LightSwitch(true);

			//�J�E���g�����Z�b�g
			m_FSwitchTime.TimeCnt = 0.f;
			m_FSwitchTime.PlayNum = 0;
		}

		break;
	case ATimingLightObject::TURN_ON:

		//�J�E���g�����Z
		m_FSwitchTime.TimeCnt += DeltaTime;

		//�ݒ肵�����ԂƔ�r(�_���`����)
		if (m_FSwitchTime.m_OntoOff < m_FSwitchTime.TimeCnt)
		{
			//�X�e�[�g��؂�ւ���
			m_State = LIGHT_STATE::TURN_OFF;

			//���C�g���I�t����
			LightSwitch(false);

			//�J�E���g�����Z�b�g
			m_FSwitchTime.TimeCnt = 0.f;
		}
		break;
	}


}

//�I���I�t��؂�ւ��鏈��
void ATimingLightObject::LightSwitch(bool LightState)
{
	//���C�g------------------------------------------------------------------------------------
	//NULL�`�F�b�N
	if (m_pLightComp)
	{
		//m_CanOperationLights[i]�̃R���|�[�l���g���擾����
		ULightComponent* LightComponent = m_pLightComp->FindComponentByClass<ULightComponent>();

		//NULL�`�F�b�N
		if (LightComponent)
		{
			//LightComponent�̉����𔽓]����
			LightComponent->SetVisibility(LightState);
		}
	}
	//------------------------------------------------------------------------------------------

	//�f�J�[��----------------------------------------------------------------------------------
	//NULL�`�F�b�N
	if (m_pDecalComp)
	{
		// �����𔽓]����
		m_pDecalComp->SetActorHiddenInGame(!LightState);

		// StaticMesh�R���|�[�l���g���瓖���蔻��̖�����
		UStaticMeshComponent* StaticmeshComponent = m_pDecalComp->FindComponentByClass<UStaticMeshComponent>();
		//NULL�`�F�b�N
		if (StaticmeshComponent)
		{
			if (LightState)
				StaticmeshComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
			else
				StaticmeshComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

		}

		// Decal�R���|�[�l���g����f�J�[���̖�����
		UDecalComponent* DecalComponent = m_pDecalComp->FindComponentByClass<UDecalComponent>();
		//NULL�`�F�b�N
		if (DecalComponent)
		{
			DecalComponent->SetVisibility(LightState);
		}
	}
	//------------------------------------------------------------------------------------------
}