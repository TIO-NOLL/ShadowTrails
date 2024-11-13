//------------------------------------------------
// �t�@�C�����FActorFollowingUserWidget.cpp
// �������e	�F����⏕��UI
// �쐬��	�F22cu0327 ��t�����Y
// �X�V��	�F02/09
//------------------------------------------------


#include "ActorFollowingUserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetLayoutLibrary.h"

void UActorFollowingUserWidget::NativeConstruct()
{
	m_pLeftClick = Cast<UCanvasPanel>(GetWidgetFromName(TEXT("CanvasPanel_LeftClick")));
	m_pAction = Cast<UCanvasPanel>(GetWidgetFromName(TEXT("CanvasPanel_Action")));
	m_pRotateShadow = Cast<UCanvasPanel>(GetWidgetFromName(TEXT("CanvasPanel_Rotate")));
	m_pResetShadow = Cast<UCanvasPanel>(GetWidgetFromName(TEXT("CanvasPanel_Reset")));

	Super::NativeConstruct();
	this->SetVisibility(ESlateVisibility::Collapsed);
}

// �\������UI�ƕ\�����邩���Ȃ�����ݒ肷��֐�
void UActorFollowingUserWidget::VisibilityType(EUIType _type)
{
	m_UIType = _type;

	this->SetVisibility(ESlateVisibility::Visible);

	// �^�C�v�ɉ�����UI��\������
	switch (_type)
	{
		// �����\�����Ȃ�
	case EUIType::None:
	{
		this->SetVisibility(ESlateVisibility::Collapsed);
		break;
	}
	// LeftClickUI�\��
	case EUIType::LeftClick:
	{
		m_pLeftClick->SetVisibility(ESlateVisibility::Visible);
		m_pAction->SetVisibility(ESlateVisibility::Collapsed);
		m_pRotateShadow->SetVisibility(ESlateVisibility::Collapsed);
		m_pResetShadow->SetVisibility(ESlateVisibility::Collapsed);
		break;
	}
	// ActionUI�\��
	case EUIType::Action:
	{
		m_pAction->SetVisibility(ESlateVisibility::Visible);
		m_pLeftClick->SetVisibility(ESlateVisibility::Collapsed);
		m_pRotateShadow->SetVisibility(ESlateVisibility::Collapsed);
		m_pResetShadow->SetVisibility(ESlateVisibility::Collapsed);
		break;
	}
	// RotateUI�\��
	case EUIType::RotateShadow:
	{
		m_pRotateShadow->SetVisibility(ESlateVisibility::Visible);
		m_pLeftClick->SetVisibility(ESlateVisibility::Collapsed);
		m_pAction->SetVisibility(ESlateVisibility::Collapsed);
		m_pResetShadow->SetVisibility(ESlateVisibility::Collapsed);
		break;
	}
	// ReturnUI�\��
	case EUIType::ReturnShadow:
	{
		m_pResetShadow->SetVisibility(ESlateVisibility::Visible);
		m_pLeftClick->SetVisibility(ESlateVisibility::Collapsed);
		m_pAction->SetVisibility(ESlateVisibility::Collapsed);
		m_pRotateShadow->SetVisibility(ESlateVisibility::Collapsed);
		break;
	}
	}
}