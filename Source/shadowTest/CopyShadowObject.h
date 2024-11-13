/**
* @file		CopyShadowObject.h
* @brief	�e�̃R�s�[���������邽�߂́A�e�I�u�W�F�N�g
* @author	22CU0219 ��ؗF��
* @date		23/12/10 �쐬
*			23/02/01 �ŏI�X�V
*/

#pragma once

#include "CoreMinimal.h"
#include "BoxDecal.h"
#include "CopyShadowObject.generated.h"

//�O��V�F�ǋL 2024/01/31------------------------------------------------------
// �O���錾
class UEffectComponent;
//----------------------------------------------------------------------------

UCLASS()
class SHADOWTEST_API ACopyShadowObject : public ABoxDecal
{
	GENERATED_BODY()

public:
	// �R���X�g���N�^
	ACopyShadowObject();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

public:
	// �n�C���C�g��Ԃɐݒ�
	UFUNCTION(BlueprintCallable, Category = "CopyShadowObject")
		void SetHighLightMode(bool _flag);

	// �n�C���C�g��Ԃ��ǂ���
	UFUNCTION(BlueprintCallable, Category = "CopyShadowObject")
		bool GetHighLightMode() { return m_bHighLight; }

	// �ێ�����Ă��邩�ǂ����ݒ肷��
	UFUNCTION(BlueprintCallable, Category = "CopyShadowObject")
		void SetHold(bool _newFlag);

	// �������ꂽ���ǂ����ݒ�
	UFUNCTION(BlueprintCallable, Category = "CopyShadowObject")
		void SetMoved(bool _flag);

	// ���ɓ������ꂽ���ǂ���
	UFUNCTION(BlueprintCallable, Category = "CopyShadowObject")
		bool IsMoved() { return m_bIsMoved; }

	// �v���C���[�Ɠ������Ă��邩�ǂ���
	UFUNCTION(BlueprintCallable, Category = "CopyShadowObject")
		bool IsPlayerHit() { return m_bIsPlayerHit; }

	// �ʒu��BeginPlay�̎��ɖ߂�
	UFUNCTION(BlueprintCallable, Category = "CopyShadowObject")
		void ResetPos() { SetActorTransform(m_DefaultTrans); }

	UFUNCTION(BlueprintCallable, Category = "CopyShadowObject")
		FRotator GetDefaultRotation() { return m_DefaultTrans.GetRotation().Rotator(); }

	UFUNCTION(BlueprintCallable, Category = "CopyShadowObject")
		void SetPawnCollisionEnable(bool _flag);

	// �C�x���g�֐�
	UFUNCTION() // OverlapBegin
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION() // OverlapEnd
		void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	// �}�e���A���̃_�C�i�~�b�N�C���X�^���X
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UMaterialInstanceDynamic* m_pDecalMaterialInst;

	// �n�C���C�g��Ԃ�؂�ւ���t���O
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CopyShadow|HighlightMode")
		bool m_bHighLight;

	// �}�e���A���̃G�~�b�V�u�̃p�����[�^�ݒ�l
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CopyShadow|HighlightMode")
		float m_emissiveValue;

	// �}�e���A���̃G�~�b�V�u�̐F�p�����[�^�̐ݒ�l
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CopyShadow|HighlightMode")
		FLinearColor m_emissiveColor;

	// �}�e���A���̃G�~�b�V�u�̐F�p�����[�^�̐ݒ�l(�ړ���)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CopyShadow|HighlightMode")
		FLinearColor m_emissiveColor_Moved;

	// �ێ�����Ă��邩�ǂ���
	UPROPERTY(EditAnywhere, Category = "CopyShadow")
		bool m_bIsHold;

	// �������ꂽ���ǂ���
	UPROPERTY(EditAnywhere, Category = "CopyShadow")
		bool m_bIsMoved;

	// ���݃v���C���[�Ɠ������Ă��邩�ǂ���
	UPROPERTY(EditAnywhere, Category = "CopyShadow")
		bool m_bIsPlayerHit;

	// �Q�[���J�n���g�����X�t�H�[��
	UPROPERTY(VisibleAnywhere, Category = "CopyShadow")
		FTransform m_DefaultTrans;

	// �v���C���[�Q��
	AActor* m_pHitPlayer;

	//�O��V �ǋL�F2024/01/31----------------------------------------------------------------------------------
		//�R���|�[�l���g��ێ�����ϐ�
public:
	UPROPERTY(VisibleAnywhere, Category = "Components")
		UEffectComponent* EffectComponent;	//�R���|�[�l���g��ێ�����ϐ�

private:
	UPROPERTY(EditAnywhere, Category = "Range")
		float m_ActivateRange;		//�G�t�F�N�g���N�����鋗��

private:
	AActor* m_pPlayer;										//�v���C���[�̃|�C���^�i���W�擾�j

	bool m_bNowEffectVisibility;							//�G�t�F�N�g���Đ�����Ă��邩�ǂ����𔻒f����t���O

	float m_PlayerDistance;									//�v���C���[�Ƃ̋������i�[����ϐ�

	void MeasurementToPlayer();								//�v���C���[�Ƃ̋����𑪒肷��֐�
	//--------------------------------------------------------------------------------------------------------
};