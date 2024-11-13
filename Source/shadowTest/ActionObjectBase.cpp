//-----------------------------------------------------------------------------------
//�t�@�C�����FActionObjectBase.cpp
// �������e	�FPlayer������A�N�V�������N�������Ƃ̂ł���I�u�W�F�N�g�̃x�[�X�N���X
// �쐬��	�F�O��V
// �X�V��	�F12/11		�쐬
//-----------------------------------------------------------------------------------

#include "ActionObjectBase.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Components/BoxComponent.h"

//�R���X�g���N�^
AActionObjectBase::AActionObjectBase()
{
	PrimaryActorTick.bCanEverTick = true;

	//���[�g�ɒu���x�[�X�I�u�W�F�N�g����
	m_pBase = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
	RootComponent = m_pBase;

	//���b�V���R���|�[�l���g����
	m_pObjectMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ObjectMesh"));
	m_pObjectMeshComp->SetupAttachment(RootComponent);

	//�{�b�N�X�R���W��������
	m_pBoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponant"));
	m_pBoxComp->SetupAttachment(RootComponent);
}

// �t���[�����Ƃ̏���
void AActionObjectBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AActionObjectBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//�v���C���[���A�N�V�����R�}���h�����s�����ۂɌĂ΂�鏈��
void AActionObjectBase::OnActionthisObject()
{
	//���s�������C�x���g�������B
}

