/**
* @file		ShadowCopyModeComponent.h
* @brief	�e�R�s�[���[�h����������R���|�[�l���g
* @author	22CU0219 ��ؗF��
* @date		23/12/10 �쐬
*			23/02/01 �ŏI�X�V
*/
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

// 22cu0327 ��t�����Y ---------------------------------------------------------
#include "ActorFollowingUserWidget.h"
#include "MouseUserWidget.h"
// -----------------------------------------------------------------------------

#include "ShadowCopyModeComponent.generated.h"

// �O���錾
class APlayerController;
class ACopyShadowObject;

// 22cu0327 ��t�����Y �ǋL�F2024/02/09-----------------------------------------
class UActorFollowingUserWidget;
class UMouseUserWidget;
// -----------------------------------------------------------------------------

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SHADOWTEST_API UShadowCopyModeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// �R���X�g���N�^
	UShadowCopyModeComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UFUNCTION(BlueprintCallable, Category = "ShadowCopyModeComponent")
		void Hold(bool _flag);

	UFUNCTION(BlueprintCallable, Category = "ShadowCopyModeComponent")
		bool IsHold() { return m_bIsHolding; }

	UFUNCTION(BlueprintCallable, Category = "ShadowCopyModeComponent")
		void AddYawRotation(float _deltaDegree);

	UFUNCTION(BlueprintCallable, Category = "ShadowCopyModeComponent")
		void ResetShadowPos();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		APlayerController* m_pPlayerController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShadowCopyModeComponent", meta = (AllowPrivateAccess = "true"))
		bool m_bIsHolding;

	// �J�[�\�����Ƀ��C���΂�����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShadowCopyModeComponent", meta = (AllowPrivateAccess = "true"))
		float m_UnderCursorTraceLength;

	// �e�I�u�W�F�N�g��Yaw������]
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShadowCopyModeComponent", meta = (AllowPrivateAccess = "true"))
		float m_addYaw;

	void GetUnderCursorShadow();

	void ProcStandardNormalBaseRotation(FVector _newNormal, FRotator _rotation);

	ACopyShadowObject* m_pShadowObj;	// �e�I�u�W�F�N�g

	FRotator m_holdingShadowRotation;	// 

	FVector m_prevNormal;	 // 1�t���[���O�̉�]��ێ�

	// 22cu0327 ��t�����Y ----------------------------------------------------------
	
	UActorFollowingUserWidget* m_pUI;
	UMouseUserWidget* m_pMouseUI;

	// Tick�ň�x�������s���邽�߂̃t���O
	bool m_bDoOnce_LeftClick;
	bool m_bDoOnce_Rotate;
	bool m_bDoOnce_Return;

	USoundBase* m_pPasteSound;
	USoundBase* m_pResetSound;

	// -----------------------------------------------------------------------------
};
