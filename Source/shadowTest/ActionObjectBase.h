//-----------------------------------------------------------------------------------
//�t�@�C�����FActionObjectBase.h
// �������e	�FPlayer������A�N�V�������N�������Ƃ̂ł���I�u�W�F�N�g�̃x�[�X�N���X
// �쐬��	�F�O��V
// �X�V��	�F12/11		�쐬
//-----------------------------------------------------------------------------------

//�C���N���[�h�K�[�h
#pragma once

//�C���N���[�h
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ActionObjectBase.generated.h"

//�O���錾
class UStaticMeshComponent;
class UBoxComponent;

UCLASS()
class SHADOWTEST_API AActionObjectBase : public AActor
{
	GENERATED_BODY()
	
public:	
	//�R���X�g���N�^
	AActionObjectBase();

protected:
	// �������̏���
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime)override;

public:
	//�G�f�B�^���ŕҏW�ł��郁���o�ϐ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* m_pObjectMeshComp;		//�I�u�W�F�N�g�̃��b�V��

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UBoxComponent* m_pBoxComp;		//�R���W����

protected:
	UPROPERTY()
		USceneComponent* m_pBase;		//�x�[�X�R���|�[�l���g

public:
	//�v���C���[���A�N�V�����R�}���h�����s�����Ƃ��ɌĂ΂�鏈��
	UFUNCTION()
		virtual void OnActionthisObject();

};