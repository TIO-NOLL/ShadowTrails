////------------------------------------------------
// ファイル名：MouseUserWidget.h
// 処理内容	：マウスのUIを表示する
// 作成者	：22cu0327 千葉涼太郎
// 更新日	：02/13
//------------------------------------------------

#include "MouseUserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Image.h"
#include "Blueprint/WidgetLayoutLibrary.h"

// コンストラクタ
void UMouseUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
	this->SetVisibility(ESlateVisibility::Visible);

}
