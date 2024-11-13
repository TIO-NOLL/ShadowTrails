/**
* @file		ShadowCopyModeComponent.cpp
* @brief	�e�R�s�[���[�h����������R���|�[�l���g
* @author	22CU0219 ��ؗF��
* @date		23/12/10 �쐬
*			23/02/17 �ŏI�X�V
*/


#include "ShadowCopyModeComponent.h"
#include "CopyShadowObject.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"
// 22cu0327 ��t�����Y �ǋL�F2024/02/09-----------------------------------------
#include "Blueprint/WidgetBlueprintLibrary.h"
// -----------------------------------------------------------------------------

// Sets default values for this component's properties
UShadowCopyModeComponent::UShadowCopyModeComponent()
	: m_pPlayerController(nullptr)
	, m_bIsHolding(false)
	, m_UnderCursorTraceLength(2000.f)
	, m_addYaw(0.f)
	, m_pShadowObj(nullptr)
	, m_holdingShadowRotation(FRotator::ZeroRotator)
	, m_prevNormal(FVector::ZeroVector)
	// 22cu0327 ��t�����Y ---------------------------------------------------------
	, m_pUI(nullptr)
	, m_pMouseUI(nullptr)
	, m_bDoOnce_LeftClick(false)
	, m_bDoOnce_Rotate(false)
	, m_bDoOnce_Return(false)
	, m_pPasteSound(nullptr)
	, m_pResetSound(nullptr)
	//------------------------------------------------------------------------------
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UShadowCopyModeComponent::BeginPlay()
{
	Super::BeginPlay();

	m_pPlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	/*if (m_pPlayerController != nullptr)
		m_pPlayerController->bShowMouseCursor = true;*/

	// 22cu0327 ��t�����Y ---------------------------------------------------------
	if (m_pUI == nullptr && m_pMouseUI == nullptr)
	{
		// �E�B�W�F�b�g�擾
		TArray<UUserWidget*> findClass;
		UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), findClass, UActorFollowingUserWidget::StaticClass(), false);
		if (findClass.Num() == 1)
		{
			m_pUI = Cast<UActorFollowingUserWidget>(findClass[0]);
		}

		UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), findClass, UMouseUserWidget::StaticClass(), false);
		if (findClass.Num() == 1)
		{
			m_pMouseUI = Cast<UMouseUserWidget>(findClass[0]);
		}
	}

}


// Called every frame
void UShadowCopyModeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// �e��ێ����Ă��鎞�̏���
	if (m_bIsHolding)
	{
		FHitResult hitResult; // �g���[�X����
		FVector traceStart; // �n�_���W
		FVector traceEnd; // �I�_���W

		// �}�E�X�J�[�\�����W�����[���h���W�Ŏ擾(��������UtraceEnd�ɓ���Ă���)
		m_pPlayerController->DeprojectMousePositionToWorld(traceStart, traceEnd);
		// �������L�΂����ꏊ���I�_
		traceEnd = traceStart + traceEnd * m_UnderCursorTraceLength;

		// ���C���g���[�X
		GetWorld()->LineTraceSingleByChannel(hitResult, traceStart, traceEnd, ECC_Visibility);
		// �f�o�b�O�m�F
		//DrawDebugLine(GetWorld(), traceStart, traceEnd, FColor::Blue, false, .5f,0U,0.1f);

		// �e��\���I�u�W�F�N�g���ǂ�������
		bool isGroundHit = false;
		if (hitResult.bBlockingHit)
		{
			TWeakObjectPtr<AActor> hitActor = hitResult.GetActor();

			if (hitActor.IsValid())
			{
				auto responce = hitActor->GetComponentsCollisionResponseToChannel(ECC_GameTraceChannel2);
				
				if (responce == ECR_Block)
					isGroundHit = true;
				else
					isGroundHit = false;
			}
		}
		
		// �������Ă������̂ݏ���
		if (hitResult.bBlockingHit && isGroundHit)
		{
			//// �f�o�b�O�m�F
			//DrawDebugPoint(GetWorld(), hitResult.Location, 5.f, FColor::Blue, false, .5f);

			if (m_pShadowObj != nullptr)
			{
				// �e�̃I�u�W�F�N�g�����C���q�b�g�����ʒu�Ɉړ�
				m_pShadowObj->SetActorLocation(hitResult.Location);

				// �@��������n�ʂƂ��ĉ�]
				FRotator defaultRot = m_pShadowObj->GetDefaultRotation();
				ProcStandardNormalBaseRotation(hitResult.Normal, defaultRot);

				// �@����ێ����Ă���
				m_prevNormal = hitResult.Normal;
			}

			// 22cu0327 ��t�����Y ---------------------------------------------------------
			if (m_pUI != nullptr)
			{
				if (m_bDoOnce_Rotate == false)
				{
					// RotateUI��\��
					m_pUI->VisibilityType(EUIType::RotateShadow);
					m_bDoOnce_Rotate = true;
					m_bDoOnce_LeftClick = false;
					m_bDoOnce_Return = false;

					// Cut�����Ƃ��̉���炷
					m_pUI->PlaySoundbyType(EShadowSoundType::Cut);
				}

				
			}
			//------------------------------------------------------------------------------
		}
		else
		{
			if (!m_prevNormal.IsZero())
			{
				FRotator defaultRot = m_pShadowObj->GetDefaultRotation();
				ProcStandardNormalBaseRotation(m_prevNormal, defaultRot);
			}
		}

		// 22cu0327 ��t�����Y ---------------------------------------------------------
		if (m_pMouseUI != nullptr)
		{
			m_pMouseUI->SetCursorType(ECursorType::Abnormal);
		}
		//------------------------------------------------------------------------------

	}
	else
	{
		// �e��ێ����Ă��Ȃ����̓J�[�\�����̉e�I�u�W�F�N�g���擾
		GetUnderCursorShadow();

		// 22cu0327 ��t�����Y ---------------------------------------------------------
		if (m_pMouseUI != nullptr)
		{
			m_pMouseUI->SetCursorType(ECursorType::Normal);
		}
		//------------------------------------------------------------------------------
	}

}

void UShadowCopyModeComponent::ProcStandardNormalBaseRotation(FVector _newNormal, FRotator _rotation)
{
	// ��]
	if (m_pShadowObj == nullptr)
		return;

	m_pShadowObj->SetActorRotation(_rotation);
	//UE_LOG(LogTemp, Log, TEXT("hitNormal[X:%f,Y:%f,Z:%f"), _newNormal.X, _newNormal.Y, _newNormal.Z);

	// Case1---------------------------------------------------------------------------------------------
	// �ǂ�n�ʂƂ��Č�����ݒ聨�ŏ��̉�]�𑫂�
	FVector leftVector = m_pShadowObj->GetActorForwardVector() ^ m_pShadowObj->GetActorUpVector();
	FVector newForwardVector = _newNormal ^ leftVector;
	FVector newRightVector = _newNormal ^ newForwardVector;


	FRotator newRot = UKismetMathLibrary::MakeRotationFromAxes(newForwardVector, newRightVector, _newNormal);
	//UE_LOG(LogTemp, Log, TEXT("newRot[Pitch:%f,Yaw:%f,Roll:%f"), newRot.Pitch, newRot.Yaw, newRot.Roll);


	_rotation.Yaw += m_addYaw;
	FRotator fixRot = UKismetMathLibrary::ComposeRotators(_rotation, newRot);
	m_pShadowObj->SetActorRotation(fixRot);
	//UE_LOG(LogTemp, Log, TEXT("fixRot[Pitch:%f,Yaw:%f,Roll:%f"), fixRot.Pitch, fixRot.Yaw, fixRot.Roll);
}

/**
* �e�̕ێ���؂�ւ�
*/
void UShadowCopyModeComponent::Hold(bool _flag)
{
	if (_flag)
	{
		if (m_pShadowObj != nullptr)
		{
			if (!m_pShadowObj->IsMoved())
			{
				m_bIsHolding = true;
				m_pShadowObj->SetHold(true);
				m_pShadowObj->SetPawnCollisionEnable(false);
			}
		}
	}
	else
	{
		m_bIsHolding = false;
		m_pShadowObj->SetHighLightMode(false);
		m_pShadowObj->SetPawnCollisionEnable(true);
		m_pShadowObj->SetHold(false);
		m_pShadowObj->SetMoved(true);

		// 22cu0327 ��t�����Y ---------------------------------------------------------
		if (m_pUI != nullptr)
		{
			// ��x�������s
			if (m_bDoOnce_Return == false)
			{
				// ReturnUI��\��
				m_pUI->VisibilityType(EUIType::ReturnShadow);
				m_bDoOnce_Return = true;
				m_bDoOnce_LeftClick = false;
				m_bDoOnce_Rotate = false;

				// �y�[�X�g����Ƃ��̉���炷
				m_pUI->PlaySoundbyType(EShadowSoundType::Paste);
			}
		}
		//------------------------------------------------------------------------------
	}
}

/**
* �}�E�X���̉e�I�u�W�F�N�g���擾���A�n�C���C�g��Ԃɂ���
*/
void UShadowCopyModeComponent::GetUnderCursorShadow()
{
	ACopyShadowObject* pPrevObj = m_pShadowObj; // �؂�ւ�����ۂɃn�C���C�g��Ԃ��I�t�ɂ��邽�ߕێ�
	bool bHitShadow = false; // �e�ɓ����������ǂ���

	FHitResult hitResult; // �g���[�X����
	FVector traceStart; // �n�_���W
	FVector traceEnd; // �I�_���W

	// �}�E�X�J�[�\�����W�����[���h���W�Ŏ擾(��������UtraceEnd�ɓ���Ă���)
	m_pPlayerController->DeprojectMousePositionToWorld(traceStart, traceEnd);
	// �������L�΂����ꏊ���I�_
	traceEnd = traceStart + traceEnd * m_UnderCursorTraceLength;

	// ���C���g���[�X
	GetWorld()->LineTraceSingleByChannel(hitResult, traceStart, traceEnd, ECC_GameTraceChannel1);
	// �f�o�b�O�m�F
	//DrawDebugLine(GetWorld(), traceStart, traceEnd, FColor::Red, false, .5f,0U,0.1f);


	// �q�b�g�����ꍇ�̂ݏ���
	if (hitResult.bBlockingHit)
	{
		//// �f�o�b�O�m�F
		//DrawDebugPoint(GetWorld(), hitResult.Location, 5.f, FColor::Red, false, .5f);
		// ���O�o��
		//UE_LOG(LogTemp, Log, TEXT("HitActor::%s"), *hitResult.GetActor()->GetName());

		// �e�I�u�W�F�N�g���^�O�Ŕ���
		if (hitResult.GetActor()->ActorHasTag("Shadow"))
		{
			m_pShadowObj = Cast<ACopyShadowObject>(hitResult.GetActor()); // �ϐ��ɕێ�
			m_pShadowObj->SetHighLightMode(true); // �n�C���C�g��Ԃɂ���

			// �O�t���[���̃I�u�W�F�N�g�ƕʂ̉e�I�u�W�F�N�g�̏ꍇ
			if (m_pShadowObj != pPrevObj)
			{
				// �O�̉e�I�u�W�F�N�g�̃n�C���C�g��Ԃ�����
				if (pPrevObj != nullptr)
					pPrevObj->SetHighLightMode(false);
			}
			bHitShadow = true;

			// 22cu0327 ��t�����Y ---------------------------------------------------------
			if (m_pUI != nullptr && m_pShadowObj != nullptr)
			{
				if (m_pShadowObj->IsMoved() == true)
				{
					if (m_bDoOnce_Return == false)
					{
						m_pUI->VisibilityType(EUIType::ReturnShadow);
						m_bDoOnce_Return = true;
						m_bDoOnce_LeftClick = false;
						m_bDoOnce_Rotate = false;
					}

				}
				else
				{
					if (m_bDoOnce_LeftClick == false)
					{
						m_pUI->VisibilityType(EUIType::LeftClick);
						m_bDoOnce_LeftClick = true;
						m_bDoOnce_Rotate = false;
						m_bDoOnce_Return = false;

						// �e�ɃJ�[�\�������킹�����̉���炷
						m_pUI->PlaySoundbyType(EShadowSoundType::Cursor);
					}
				}
			}
			//------------------------------------------------------------------------------
		}
	}

	// �e�ɓ������Ă��Ȃ������ꍇ
	if (!bHitShadow)
	{
		// ���ݕێ����Ă���e�I�u�W�F�N�g�����
		if (m_pShadowObj != nullptr)
		{
			m_pShadowObj->SetHighLightMode(false);
			m_pShadowObj = nullptr;
		}

		// 22cu0327 ��t�����Y ---------------------------------------------------------
		if (m_pUI != nullptr)
		{
			m_pUI->VisibilityType(EUIType::None);
			m_bDoOnce_LeftClick = false;
			m_bDoOnce_Rotate = false;
			m_bDoOnce_Return = false;
		}
		//------------------------------------------------------------------------------
	}
}


/**
* Yaw��]�����Z���ĉe����]������
*/
void UShadowCopyModeComponent::AddYawRotation(float _deltaDegree)
{
	// �e�ێ����łȂ��ꍇ�͏������X�L�b�v
	if (!m_bIsHolding)
		return;

	m_addYaw += _deltaDegree;
}

/**
* ���ݕێ����Ă���e�̈ʒu��߂�
*/
void UShadowCopyModeComponent::ResetShadowPos()
{
	if (m_pShadowObj != nullptr)
	{
		// ��������Ă������̂ݏ���
		if (m_pShadowObj->IsMoved())
		{
			m_pShadowObj->ResetPos();
			m_pShadowObj->SetMoved(false);
		}

		// 22cu0327 ��t�����Y ---------------------------------------------------------
		// �e�����Z�b�g����Ƃ��̉���炷
		m_pUI->PlaySoundbyType(EShadowSoundType::Reset);
		//------------------------------------------------------------------------------
	}
}