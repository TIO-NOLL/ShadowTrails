//---------------------------------------------------------------------------
// �t�@�C����		�FEffectComponent.cpp
// �T�v				�F�G�t�F�N�g��ێ�����R���|�[�l���g
// �쐬��			�F22cu0127�@�O�� �V
// �X�V���e			�F2023/01/14�@�쐬
//---------------------------------------------------------------------------

#include "EffectComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"


// �R���X�g���N�^
UEffectComponent::UEffectComponent()
	: ParticleEffect(NULL)
	, NiagaraEffect(NULL)
	, m_ActiveEffectComponent(NULL)
	, m_ActiveNiagaraComponent(NULL)
{
	// Tick���Ă΂�邩�ǂ��������߂鏈��
	PrimaryComponentTick.bCanEverTick = true;
}


// �������ɌĂ΂�鏈��
void UEffectComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

}


// �t���[�����ƂɌĂ΂�鏈��
void UEffectComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

/*
* �G�t�F�N�g���Đ�����֐�
*/
void UEffectComponent::PlayEffect(USceneComponent* _rootComponent, FVector _spawnVector, FRotator _spawnRotator, bool _autoDestoroy)
{
	// �p�[�e�B�N���G�t�F�N�g���Đ�
	if (ParticleEffect)
	{
		m_ActiveEffectComponent = UGameplayStatics::SpawnEmitterAttached(
			ParticleEffect,  // �X�|�[������G�t�F�N�g
			_rootComponent,  // �A�^�b�`���[��
			FName("SocketName"),  // �A�^�b�`����\�P�b�g�̖��O
			_spawnVector,  // �X�|�[���ʒu
			_spawnRotator,  // �X�|�[����]
			EAttachLocation::SnapToTarget,  // �A�^�b�`�̃^�C�v
			_autoDestoroy  // �I�[�g�f�X�g���C
		);
	}

	// �i�C�A�K���G�t�F�N�g���Đ�
	if (NiagaraEffect)
	{
		m_ActiveNiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
			NiagaraEffect,  // �X�|�[������G�t�F�N�g
			_rootComponent,  // �A�^�b�`���[��
			FName("SocketName"),  // �A�^�b�`����\�P�b�g�̖��O
			_spawnVector,  // �X�|�[���ʒu
			_spawnRotator,  // �X�|�[����]
			EAttachLocation::SnapToTarget,  // �A�^�b�`�̃^�C�v
			_autoDestoroy  // �I�[�g�f�X�g���C
		);
	}
}

/*
* �G�t�F�N�g��j������֐�
*/
void UEffectComponent::StopEffect()
{
	//���݃p�[�e�B�N���G�t�F�N�g���Đ����������ꍇ
	if (m_ActiveEffectComponent)
	{
		//�p�[�e�B�N���G�t�F�N�g��j��
		m_ActiveEffectComponent->Deactivate();
		//nullptr����
		m_ActiveEffectComponent = nullptr;
	}

	//���݃i�C�A�K���G�t�F�N�g���Đ����������ꍇ
	if (m_ActiveNiagaraComponent)
	{
		//�i�C�A�K���G�t�F�N�g��j��
		m_ActiveNiagaraComponent->Deactivate();
		//nullptr����
		m_ActiveNiagaraComponent = nullptr;
	}
}