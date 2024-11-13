//---------------------------------------------------------------------------
// �t�@�C����		�FActionObject_LightSwitch.cpp
// �T�v				�F���C�g�̃X�C�b�`�A�A�N�V���������s�����ۂɂ͑Ή����郉�C�g�I�u�W�F�N�g�̕\����\����؂�ւ��邱�Ƃ��ł���
// �쐬��			�F22cu0127�@�O�� �V
// �X�V���e			�F2024/01/19�@�쐬
//---------------------------------------------------------------------------


#include "ActionObject_LightSwitch.h"
#include "Components/LightComponent.h"
#include "Components/DecalComponent.h"

//�R���X�g���N�^
AActionObject_LightSwitch::AActionObject_LightSwitch()
{
	
}

// �������̏���
void AActionObject_LightSwitch::BeginPlay()
{
	Super::BeginPlay();

	//m_CanOperationLights�̔z��̐��������s����
	for (int i = 0; i < m_StartOff_CanOperationLights.Num(); ++i)
	{
		//NULL�`�F�b�N
		if (m_StartOff_CanOperationLights[i])
		{
			//m_CanOperationLights[i]�̃R���|�[�l���g���擾����
			ULightComponent* LightComponent = m_StartOff_CanOperationLights[i]->FindComponentByClass<ULightComponent>();

			//LightComponent�̉�����؂�ւ���
			LightComponent->SetVisibility(false);
		}
	}

	//m_CanOperationLights�̔z��̐��������s����
	for (int i = 0; i < m_StartOff_CanOperationDecals.Num(); ++i)
	{
		//NULL�`�F�b�N
		if (m_StartOff_CanOperationDecals[i])
		{
			//m_StartOff_CanOperationDecals[i]�̃R���|�[�l���g���擾����
			UDecalComponent* DecalComponent = m_StartOff_CanOperationDecals[i]->FindComponentByClass<UDecalComponent>();

			//DecalComponent�̉�����؂�ւ���
			DecalComponent->SetVisibility(false);
		}
	}
}

//�A�N�V���������s���ꂽ���̏���
void AActionObject_LightSwitch::OnActionthisObject()
{
	//m_CanOperationLights_StartOn�̔z��̐��������s����
	for (int i = 0; i < m_StartOn_CanOperationLights.Num(); ++i)
	{
		//NULL�`�F�b�N
		if (m_StartOn_CanOperationLights[i])
		{
			//m_StartOn_CanOperationLights[i]�̃R���|�[�l���g���擾����
			ULightComponent* LightComponent = m_StartOn_CanOperationLights[i]->FindComponentByClass<ULightComponent>();

			//LightComponent�̉�����؂�ւ���
			LightComponent->SetVisibility(!LightComponent->IsVisible());
		}
	}

	//m_StartOff_CanOperationLights�̔z��̐��������s����
	for (int i = 0; i < m_StartOff_CanOperationLights.Num(); ++i)
	{
		//NULL�`�F�b�N
		if (m_StartOff_CanOperationLights[i])
		{
			//m_StartOff_CanOperationLights[i]�̃R���|�[�l���g���擾����
			ULightComponent* LightComponent = m_StartOff_CanOperationLights[i]->FindComponentByClass<ULightComponent>();

			//LightComponent�̉�����؂�ւ���
			LightComponent->SetVisibility(!LightComponent->IsVisible());
		}
	}

	//m_CanOperationDecals_StartOn�̔z��̐��������s����
	for (int i = 0; i < m_StartOn_CanOperationDecals.Num(); ++i)
	{
		//NULL�`�F�b�N
		if (m_StartOn_CanOperationDecals[i])
		{
			//m_StartOn_CanOperationDecals[i]�̃R���|�[�l���g���擾����
			UDecalComponent* DecalComponent = m_StartOn_CanOperationDecals[i]->FindComponentByClass<UDecalComponent>();

			//DecalComponent�̉�����؂�ւ���
			DecalComponent->SetVisibility(!DecalComponent->IsVisible());
		}
	}

	//m_CanOperationDecals_StartOff�̔z��̐��������s����
	for (int i = 0; i < m_StartOff_CanOperationDecals.Num(); ++i)
	{
		//NULL�`�F�b�N
		if (m_StartOff_CanOperationDecals[i])
		{
			//m_StartOff_CanOperationDecals[i]�̃R���|�[�l���g���擾����
			UDecalComponent* DecalComponent = m_StartOff_CanOperationDecals[i]->FindComponentByClass<UDecalComponent>();

			//DecalComponent�̉�����؂�ւ���
			DecalComponent->SetVisibility(!DecalComponent->IsVisible());
		}
	}

}