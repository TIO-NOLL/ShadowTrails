//---------------------------------------------------------------------- -
//�t�@�C�����FTimingLightObject.h
// �������e	�F�_���Ə��������̎����ŌJ��Ԃ����C�g���Ǘ����邨b�W�F�N�g
// �쐬��	�F�O��V
// �X�V��	�F02/11		�쐬
//-----------------------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "LightObjectBase.h"
#include "TimingLightObject.generated.h"

//�O���錾
class UEffectComponent;
class UDecalComponent;

// �\���̂̒�`
USTRUCT(BlueprintType)
struct FSwitchTime
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, Category = "Time")
		float m_OfftoPreparation;		//������Ԃ��珀����Ԃ܂łɗv���鎞��

	UPROPERTY(EditAnywhere, Category = "Time")
		float m_PreparationtoOn;		//������Ԃ���_����Ԃ܂łɗv���鎞��

	UPROPERTY(EditAnywhere, Category = "Time")
		float m_OntoOff;		//�_����Ԃ��������Ԃ܂łɗv���鎞��

	UPROPERTY(EditAnywhere, Category = "Cavate")
		int m_Cavatefrequency;	//�x�������������

	float TimeCnt;				//��L�̎��ԂƔ�r����J�E���g
	float CavateCnt;			//�x�����Ɏg�p����J�E���g

	int PlayNum;				//�Đ�������
};

UCLASS()
class SHADOWTEST_API ATimingLightObject : public ALightObjectBase
{

	GENERATED_BODY()

public:
	ATimingLightObject();

	//�x��p�̃G�t�F�N�g
	UPROPERTY(VisibleAnywhere, Category = "Components")
		UEffectComponent* m_pEffectComponent;

protected:
	// �������̏���
	virtual void BeginPlay() override;

	//�t���[�����Ƃ̏���
	virtual void Tick(float DeltaTime)override;

	//�v���[���[�̃X�e�[�g
	enum LIGHT_STATE
	{
		TURN_OFF = 0,
		PREPARATION,
		TURN_ON
	};

	//�X�e�[�g�Ǘ�
	LIGHT_STATE m_State;

private:
	//���C�g�̃I���I�t��؂�ւ��鏈��
	void LightSwitch(bool Lightstate);

	//���Ԃ��i�[����\����
	UPROPERTY(EditAnywhere, Category = "Time")
		FSwitchTime m_FSwitchTime; // �\���̂��v���p�e�B�Ƃ��Ďg�p
};
