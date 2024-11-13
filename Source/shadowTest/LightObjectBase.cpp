//-----------------------------------------------------------------------------------
//�t�@�C�����FLightObjectBase.h
// �������e	�F���C�g�Ƃ���Ɋւ���f�J�[�����Ǘ�����I�u�W�F�N�g�x�[�X�N���X
// �쐬��	�F�O��V
// �X�V��	�F02/11		�쐬
//-----------------------------------------------------------------------------------

#include "LightObjectBase.h"
#include "Engine/SpotLight.h"
#include "BoxDecal.h"

ALightObjectBase::ALightObjectBase()
	: m_pLightComp(NULL)
	, m_pDecalComp(NULL)
{
}

void ALightObjectBase::BeginPlay()
{
	Super::BeginPlay();

	PrimaryActorTick.bCanEverTick = true;
}

void ALightObjectBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}