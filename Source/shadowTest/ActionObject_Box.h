//---------------------------------------------------------------------------
// �t�@�C����		�FActionObject_Box.h
// �T�v				�F���^�̔�A�N�V�����I�u�W�F�N�g�A�A�N�V�����R�}���h�����s���ꂽ�ۂɂ͓��������Ƃ��ł���
// �쐬��			�F22cu0127�@�O�� �V
// �X�V���e			�F2023/12/14�@�쐬
//---------------------------------------------------------------------------

//�C���N���[�h�K�[�h
#pragma once

//�C���N���[�h
#include "CoreMinimal.h"
#include "ActionObjectBase.h"		//ActionObjectBase�p��
#include "ActionObject_Box.generated.h"

//�O���錾
class USkeletalMeshComponent;
class UStaticMesh;

UCLASS()
class SHADOWTEST_API AActionObject_Box : public AActionObjectBase
{
	GENERATED_BODY()

public:
	//�R���X�g���N�^
	AActionObject_Box();

protected:
	//�Q�[���X�^�[�g���A�܂��͐������ɌĂ΂�鏈��
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

private:
	//�v���C���[�ɉ�����Ă���Ԃ̂݁A�v���C���[�̍s���ɍ��킹�ē������֐�
	void IsPushed(float _deltaTime);

private:
	//�v���C���[�̃A�h���X����������֐�
	void GetPlayerActor(FVector PlayerPos);

public:
	//AActionObjectBase�̉��z�֐����I�[�o�[���C�h����
	//���ʂ���Player������Ăяo�����
	virtual void OnActionthisObject()override;

	//���̃I�u�W�F�N�g�ƃv���C���[�I�u�W�F�N�g�̑��Ίp�x���AX������Ƃ��ċ��߁A�����Ԃ��֐�
	//Player������Ăяo�����
	float GetAnglePlayer(FVector PlayerPos);

private:
	//�ʒu�␳�������f����ϐ�
	bool m_bNowCorrect;
	//���̃I�u�W�F�N�g����������Ă��邩�ǂ������f����t���O
	bool m_bIsPushed;

	AActor* m_pPlayer;				//�v���C���[�̃|�C���^
	UStaticMesh* m_pStaticMesh;		//�X�^�e�B�b�N���b�V��
	FVector m_meshSize;				//�X�^�e�B�b�N���b�V���̃T�C�Y

	////�N�����鎞�Ԃ��󂯎��ϐ�
	//float m_CorrectTime;
	////���Z���A��L�̕ϐ��Ɣ�r���邽�߂̕ϐ�
	//float m_CorrectCnt;

public:
	/*UFUNCTION()
		void SetTime(float _CorrectTime);*/
};
