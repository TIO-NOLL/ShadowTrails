//------------------------------------------------
// ファイル名：ActorFollowingUserWidget.cpp
// 処理内容	：操作補助のUI
// 作成者	：22cu0327 千葉涼太郎
// 更新日	：02/09
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

// 表示するUIと表示するかしないかを設定する関数
void UActorFollowingUserWidget::VisibilityType(EUIType _type)
{
	m_UIType = _type;

	this->SetVisibility(ESlateVisibility::Visible);

	// タイプに応じてUIを表示する
	switch (_type)
	{
		// 何も表示しない
	case EUIType::None:
	{
		this->SetVisibility(ESlateVisibility::Collapsed);
		break;
	}
	// LeftClickUI表示
	case EUIType::LeftClick:
	{
		m_pLeftClick->SetVisibility(ESlateVisibility::Visible);
		m_pAction->SetVisibility(ESlateVisibility::Collapsed);
		m_pRotateShadow->SetVisibility(ESlateVisibility::Collapsed);
		m_pResetShadow->SetVisibility(ESlateVisibility::Collapsed);
		break;
	}
	// ActionUI表示
	case EUIType::Action:
	{
		m_pAction->SetVisibility(ESlateVisibility::Visible);
		m_pLeftClick->SetVisibility(ESlateVisibility::Collapsed);
		m_pRotateShadow->SetVisibility(ESlateVisibility::Collapsed);
		m_pResetShadow->SetVisibility(ESlateVisibility::Collapsed);
		break;
	}
	// RotateUI表示
	case EUIType::RotateShadow:
	{
		m_pRotateShadow->SetVisibility(ESlateVisibility::Visible);
		m_pLeftClick->SetVisibility(ESlateVisibility::Collapsed);
		m_pAction->SetVisibility(ESlateVisibility::Collapsed);
		m_pResetShadow->SetVisibility(ESlateVisibility::Collapsed);
		break;
	}
	// ReturnUI表示
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