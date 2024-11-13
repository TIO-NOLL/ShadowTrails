//------------------------------------------------
// �t�@�C�����FWallRunCharacter.h
// �������e	�F�ǂ𑖂邱�Ƃ̂ł���L�����N�^�[
// �쐬��	�F�O��V
// �X�V��	�F12/04		�쐬
//------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.h"
#include "WallRunCharacter.generated.h"

//�O���錾
class USpringArmComponent;
class UCameraComponent;
class APlayerCharacter;

UCLASS()
class SHADOWTEST_API AWallRunCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// �R���X�g���N�^
	AWallRunCharacter();

protected:
	// �������ɌĂ΂�鏈��
	virtual void BeginPlay() override;

public:
	// �t���[�����ƂɌĂ΂�鏈��
	virtual void Tick(float DeltaTime) override;

	// �C���v�b�g�L�[�Ƃ̃o�C���h
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	//�J�����̍X�V����
	void UpdateCamera(float _deltaTime);
	//�ړ�����
	void UpdateMove(float _deltaTime);
	//���C���΂�����
	void UpdateRay();

private:
	//�y���̓o�C���h�z�J������]�FPitch(Y��)
	void Cam_RotatePitch(float _axisValue);
	//�y���̓o�C���h�z�J������]�FYaw(Z��)
	void Cam_RotateYaw(float _axisValue);

	//�y���̓o�C���h�z�L�����N�^�[�ړ��F�O��
	void Chara_MoveForward(float _axisValue);
	//�y���̓o�C���h�z�L�����N�^�[�ړ��F�O��
	void Chara_MoveRight(float _axisValue);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		USpringArmComponent* m_pSpringArm;		//�X�v�����O�A�[��

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		UCameraComponent* m_pCamera;			//�J����

	FVector2D m_charaMoveInput;		//�L�����N�^�[�ړ���
	FVector2D m_cameraRotateInput;	//�J������]���͗�


	UPROPERTY(EditAnywhere, Category = "Camera")
		FVector2D m_cameraPitchLimit;	//�J�����̃s�b�`�͈�
	UPROPERTY(EditAnywhere, Category = "Camera")
		float m_CameraSensitivity;		//�J�������x
	UPROPERTY(EditAnywhere, Category = "Move")
		float m_moveSpeed;			//�ړ���

	UPROPERTY(EditAnywhere, Category = "Ray")
		float m_RayLength;			//���C���΂�����

	UPROPERTY(EditAnywhere, Category = "Generate")
		TSubclassOf<APlayerCharacter> m_pGenerateCharacter;

	bool m_bCanControl;				//����\�t���O
};
