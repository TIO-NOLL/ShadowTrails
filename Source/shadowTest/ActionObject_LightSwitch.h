//---------------------------------------------------------------------------
// �t�@�C����		�FActionObject_LightSwitch.h
// �T�v				�F���C�g�̃X�C�b�`�A�A�N�V���������s�����ۂɂ͑Ή����郉�C�g�I�u�W�F�N�g�̕\����\����؂�ւ��邱�Ƃ��ł���
// �쐬��			�F22cu0127�@�O�� �V
// �X�V���e			�F2024/01/19�@�쐬
//---------------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "ActionObjectBase.h"		//ActionBase�p��
#include "ActionObject_LightSwitch.generated.h"

/**
 * 
 */
UCLASS()
class SHADOWTEST_API AActionObject_LightSwitch : public AActionObjectBase
{
	GENERATED_BODY()

private:
	//�R���X�g���N�^
	AActionObject_LightSwitch();

protected:
	// �������̏���
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = "Light")
		TArray<AActor*> m_StartOn_CanOperationLights;		//�J�n���ɉ���Ԃ̃��C�g�̃I�u�W�F�N�g���i�[����z��
	UPROPERTY(EditAnywhere, Category = "Light")
		TArray<AActor*> m_StartOff_CanOperationLights;		//�J�n���ɕs����Ԃ̃��C�g�̃I�u�W�F�N�g���i�[����z��

	UPROPERTY(EditAnywhere, Category = "Decal")
		TArray<AActor*> m_StartOn_CanOperationDecals;		//�J�n���ɉ���Ԃ̃f�J�[���̃I�u�W�F�N�g�i�[����z��
	UPROPERTY(EditAnywhere, Category = "Decal")
		TArray<AActor*> m_StartOff_CanOperationDecals;		//�J�n���ɕs����Ԃ̃f�J�[���̃I�u�W�F�N�g���i�[����z��
private:
	//�A�N�V���������s���ꂽ�ۂɌĂ΂��֐�
	void OnActionthisObject()override;
};
