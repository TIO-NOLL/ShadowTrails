//------------------------------------------------
// �t�@�C�����FPlayerCharacter.h
// �������e	�FPlayer�̃L�����N�^�[�N���X
// �쐬��	�F�O��V
// �X�V��	�F12/11		�쐬
//------------------------------------------------
#pragma once

#include "CoreMinimal.h"
#include "PlayerCharacterBase.h"
#include "PlayerCharacter.generated.h"

/**
 *
 */
UCLASS()
class SHADOWTEST_API APlayerCharacter : public APlayerCharacterBase
{
	GENERATED_BODY()


public:
	//�R���X�g���N�^
	APlayerCharacter();

protected:
	//����������
	virtual void BeginPlay() override;

public:
	// �t���[�����Ƃ̏���
	virtual void Tick(float DeltaTime) override;

	// ���̓o�C���h����
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	// ============================================================================
	// �e
	// 22cu0219 ��؂��L�q
	// -----------------------------------------------------------
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UShadowCopyModeComponent* m_pShadowComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UEffectComponent* m_pDeadEffectComponent;

	UPROPERTY(VisibleAnywhere)
		class UMaterialInstanceDynamic* m_pMeshMaterial;

	UPROPERTY(EditAnywhere, Category = "Cut&Paste")
		float m_RotateDeglee;

	UPROPERTY(VisibleAnywhere, Category = "Cut&Paste")
		bool m_bClockwisePress;

	UPROPERTY(VisibleAnywhere, Category = "Cut&Paste")
		bool m_bCounterClockwisePress;


	//�y���̓o�C���h�z�e�J�b�g�E�y�[�X�g
	void CutPaste();

	//�y���̓o�C���h�z�e��Yaw���̎��v���ɉ�]�F�L�[��Down/Up�Ńt���O��؂�ւ���
	void SwapClockwisePressReleased() { m_bClockwisePress = !m_bClockwisePress; }

	//�y���̓o�C���h�z�e��Yaw���̔����v���ɉ�]�F�L�[��Down/Up�Ńt���O��؂�ւ���
	void SwapCounterClockwisePressReleased() { m_bCounterClockwisePress = !m_bCounterClockwisePress; }

	//�y���̓o�C���h�z�e�̉�]�F�}�E�X�z�C�[���̉�]
	void RotateShadow(float _axisValue);

	// �y���̓o�C���h�z�e�������ʒu�ɖ߂�
	void ResetShadowPos();

	// ======================================================================================
public:
	// ���S�L���������Z�b�g
	UFUNCTION(BlueprintCallable, Category = "PlayerCharacter")
		void ResetPlayer();
	
	UFUNCTION(BlueprintCallable, Category = "PlayerCharacter")
		void ResetDeadCounter() { m_DeadTimeCounter = 0.f; }
private:
	// ���S���t���O
	UPROPERTY(VisibleAnywhere, Category = "LightHit")
		bool m_bDead;

	//���ɓ������Ď��S����܂ł̕b��
	UPROPERTY(EditAnywhere, Category = "LightHit")
		float m_DeadTime;

	//���ɓ������Ď��S����܂ł̕b��
	UPROPERTY(EditAnywhere, Category = "LightHit")
		float m_DeadTimeCounter;

	//�ړ����x�����l
	//���ɓ������Ă���ԑ������Ă����A���񂾂�ƈړ����x���x���Ȃ�
	UPROPERTY(EditAnywhere, Category = "LightHit")
		float m_DownSpeed;

	// ���ɓ������Ă��邩�ǂ����̃t���O
	UPROPERTY(EditAnywhere, Category = "LightHit")
		uint8 m_hittingLightNum;

	// ���ݓ������Ă���e�̃J�E���g
	UPROPERTY(EditAnywhere, Category = "LightHit")
		uint8 m_hittingShadowNum;

	// 22cu0327 ��t�����Y 2024/02/08-------------------------------------------
	// �_���[�W���󂯂Ă��邩�ǂ����̃t���O
	UPROPERTY(EditAnywhere, Category = "LightHit")
		bool m_bDamage;
	// -------------------------------------------------------------------------

	// ----�c���@�L�q ------
	// �v���C���[�_�Ŏ��Ԍv�Z�p
	UPROPERTY(EditAnywhere, Category = "Flash")
		float m_FlashDuration;

	// �_�ŃC���^�[�o��
	UPROPERTY(EditAnywhere, Category = "Flash")
		float m_FlashInterval;

	// �v���C���[�\������Flag
	UPROPERTY(EditAnywhere, Category = "Flash")
		bool m_bPlayerVisible;

	// �v���C���[�_�ŏ���
	UFUNCTION()
		void PlayerFlashing(float DeltaTime);

	// �C�x���g�֐�
	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// 22cu0327 ��t�����Y 2024/02/08-------------------------------------------
	UFUNCTION(BlueprintCallable, Category = "Widget")
		bool GetbDamage();
	// -------------------------------------------------------------------------

	// 22cu0127 �O��V 2024/03/04-----------------------------------------------
	float m_ShadowRotateValue;
	// -------------------------------------------------------------------------
};
