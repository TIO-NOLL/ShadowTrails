//-----------------------------------------------------------------------------------
//�t�@�C�����FLightObjectBase.cpp
// �������e	�F���C�g�Ƃ���Ɋւ���f�J�[�����Ǘ�����I�u�W�F�N�g�x�[�X�N���X
// �쐬��	�F�O��V
// �X�V��	�F02/11		�쐬
//-----------------------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "ActionObjectBase.h"
#include "LightObjectBase.generated.h"

//�O���錾
class ASpotLight;
class ABoxDecal;

UCLASS()
class SHADOWTEST_API ALightObjectBase : public AActionObjectBase
{
	GENERATED_BODY()

public:
	//�R���X�g���N�^
	ALightObjectBase();

protected:
	// �������̏���
	virtual void BeginPlay() override;

	//�t���[�����Ƃ̏���
	virtual void Tick(float DeltaTime)override;

public:
	//�G�f�B�^���ŕҏW�ł��郁���o�ϐ�
	UPROPERTY(EditAnywhere, Category = "Light")
		AActor* m_pLightComp;		//���C�g

	UPROPERTY(EditAnywhere, Category = "Light")
		AActor* m_pDecalComp;		//�f�J�[��

};
