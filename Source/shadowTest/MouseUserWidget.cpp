////------------------------------------------------
// �t�@�C�����FMouseUserWidget.h
// �������e	�F�}�E�X��UI��\������
// �쐬��	�F22cu0327 ��t�����Y
// �X�V��	�F02/13
//------------------------------------------------

#include "MouseUserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Image.h"
#include "Blueprint/WidgetLayoutLibrary.h"

// �R���X�g���N�^
void UMouseUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
	this->SetVisibility(ESlateVisibility::Visible);

}
