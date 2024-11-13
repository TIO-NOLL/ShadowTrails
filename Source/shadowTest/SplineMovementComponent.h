/**
* @file		SplineMovementComponent.h
* @brief	�A�N�^���X�v���C���ɉ����Ĉړ�������R���|�[�l���g
* @author	22cu0219�@��ؗF��
*/

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SplineMovementComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHADOWTEST_API USplineMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// �R���X�g���N�^
	USplineMovementComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void SetMoveSpeed(float _newSpeed) { m_moveSpeed = _newSpeed; }

protected:
	// �X�v���C���A�N�^�Q��
	UPROPERTY(EditAnywhere)
		TWeakObjectPtr<AActor> m_pSplineActor;

	// �ړ����x
	UPROPERTY(EditAnywhere)
		float m_moveSpeed;

	// ��ԑ��x
	UPROPERTY(EditAnywhere)
		float m_interpSpeed;

	// ���݂̃X�v���C����̈ړ�����
	UPROPERTY(VisibleAnywhere)
		float m_currentMovedDistance;

	// ���[�v���邩�ǂ���
	UPROPERTY(EditAnywhere)
		bool m_bLoop;

	// �I�_�ɓ��B�����Ƃ��n�_�ɖ߂邩�ǂ���
	UPROPERTY(EditAnywhere)
		bool m_bReturn;

	// �t�s�����ǂ���
	UPROPERTY(VisibleAnywhere)
		bool m_bReverse;

	UPROPERTY(EditAnywhere)
		float m_returnWaitSecond;

	UPROPERTY(VisibleAnywhere)
		float m_waitCounter;

	// �X�v���C���R���|�[�l���g�̎�Q��
	TWeakObjectPtr<class USplineComponent> m_pSplineComponent;

};
