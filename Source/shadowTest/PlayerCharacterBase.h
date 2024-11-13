
//------------------------------------------------
// �t�@�C�����FPlayerCharacterBase.h
// �������e	�FPlayer�̈ړ��ƃJ�����̑��������x�[�X�L�����N�^�[�N���X
// �쐬��	�F�O��V
// �X�V��	�F12/08		�쐬
//			�@12/10		�J�����̐���A�ړ������A����\�t���O�؂�ւ��A��Ԑ؂�ւ���ǉ�
//------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ActionObjectBase.h"
#include "PlayerCharacterBase.generated.h"

//�N���X�̑O���錾
class USpringArmComponent;
class UCameraComponent;
class AWallRunCharacter;

UCLASS()
class SHADOWTEST_API APlayerCharacterBase : public ACharacter
{
	//�v���C���[���
	enum PLAYER_STATE
	{
		normal = 0,		//�ʏ펞
		damage,			//���ɓ������Ă���Ƃ�
		dead			//���S��
	};

	//���C�Ƀq�b�g����Actor�̏����i�[���A�߂�l�Ƃ��ĕԂ����߂̍\����
	struct RayInfo
	{
		FHitResult HitActor;
		FVector NormalVector;
		FVector HitPoint;
		bool HitFlag;
	};


	GENERATED_BODY()

public:
	// �R���X�g���N�^
	APlayerCharacterBase();

protected:
	// �������̏���
	virtual void BeginPlay() override;

public:
	// �t���[�����Ƃ̏���
	virtual void Tick(float DeltaTime) override;

	// �C���v�b�g�o�C���h
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	//�J�����̍X�V����
	void UpdateCamera(float _deltaTime);
	//�ړ��̍X�V����
	void UpdateMove(float _deltaTime);
	//���������ہA���̎�O�܂ňړ�����֐�
	void PositionCorrect();

private:
	//�y���̓o�C���h�z�J������]�FPitch(Y��)
	void Cam_RotatePitch(float _axisValue);
	//�y���̓o�C���h�z�J������]�FYaw(Z��)
	void Cam_RotateYaw(float _axisValue);
	//�y���̓o�C���h�z�J���������b�N����
	void Cam_MoveLock();
	//�y���̓o�C���h�z�J�����̃��b�N����������
	void Cam_MoveUnlock();

	//�y���̓o�C���h�z�L�����N�^�[�ړ��F�O��
	void Chara_MoveForward(float _axisValue);
	//�y���̓o�C���h�z�L�����N�^�[�ړ��F�O��
	void Chara_MoveRight(float _axisValue);

	//�y���̓o�C���h�z�A�N�V�����F�M�~�b�N�N��
	void Chara_Action();
	//�y���̓o�C���h�z�A�N�V�����F�Ǒ���
	void Chara_WallRun();


public:
	//�v���C���[�̏�Ԃ�ύX����֐�
	//normal
	UFUNCTION(BlueprintCallable, Category = "PlayerCharacterBase")
		void StateChangeNormal();
	//damage
	UFUNCTION(BlueprintCallable, Category = "PlayerCharacterBase")
		void StateChangeDamage();
	//dead
	UFUNCTION(BlueprintCallable, Category = "PlayerCharacterBase")
		void StateChangeDead();

	//����\�t���O��؂�ւ���֐�
	UFUNCTION(BlueprintCallable, Category = "PlayerCharacterBase")
		void SetCanMove(bool _flag);

	// 22cu0222 �c�� �ǋL�F2024/02/13--------------------------
	// �v���C���[�̈ړ��ʂ�����������ʂ��i�[����֐�
	UFUNCTION(BlueprintCallable, Category = "PlayerCharacterBase")
		void ChangeSpeed(float m_Speed);
	// -------------------------------------------------------

	// 22CU0219 ��� �ǋL:2024/02/16-------------------
	// �R���|�[�l���g���Ή�]�����Z�b�g����֐�
	UFUNCTION(BlueprintCallable, Category = "PlayerCharacterBase")
		void ResetRelativeRotation();

	UFUNCTION(BlueprintCallable, Category = "PlayerCharacterBase")
		void SetPlayerRotation(FRotator _rotation);
	//-------------------------------------------------

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
	UPROPERTY(EditAnywhere, Category = "Move")
		float m_DecreaseRateofMove;		//���m�������Ă���ۂ̈ړ��ʌ����l
	UPROPERTY(EditAnywhere, Category = "Move")
		float m_InterpolationSpeed;		//��]���x

	// 22cu0222 �c�� �ǋL�F2024/02/13 -----------------------------------
	UPROPERTY(EditAnywhere, Category = "Move")
		float m_ChangeSpeed;        // �ύX�����ړ��ʂ��i�[
	// -----------------------------------------------------------------

	//UPROPERTY(EditAnywhere, Category = "Action")
	//	float m_CorrectTime;				//�␳�̂��߂Ɉړ����鎞��
	//
	//float m_CorrectCnt;						//�␳�̂��߂Ɉړ����鎞��(���Z)
	//float m_Distance;						//�␳�̂��߂ɂP�t���[��������ł̈ړ�����

	UPROPERTY(EditAnywhere, Category = "Action")
		float m_ActionForwardLength;		//�M�~�b�N�N���p���C���΂�����

	FVector m_PlayerCorrectPosition;	//���������ۂɈړ�������W
	//FVector m_CorrectPositionVector;	//���������ۂɈړ�����x�N�g��
	//FVector m_ToCorrectPositionDistnce;	//�ړ����������߂邽�߂̕ϐ�

	AActionObjectBase* m_pActionObject;		//�N���\�ȃM�~�b�N�̏����i�[����ϐ�

	bool m_bCanControl;				//����\�ȏ�Ԃ�
	bool m_bCanWallRunning;			//�Ǒ��肪�\���ǂ�����m�点�邽�߂̃t���O

	UPROPERTY(EditAnywhere, Category = "Generate")
		TSubclassOf<AWallRunCharacter> m_pGenerateCharacter;		//��������L�����N�^�[

	PLAYER_STATE m_state;			//�v���C���[�̏��

	//���C���΂��A���C�ɐG�ꂽ�A�N�^�[��Ԃ��֐�
	RayInfo FlyRay();

	bool m_bCameraLock;

public:
	bool m_bIsBoxPush;				//���ݔ��������Ă��邩
};
