//------------------------------------------------
// ファイル名：MouseUserWidget.h
// 処理内容	：マウスのUIを表示する
// 作成者	：22cu0327 千葉涼太郎
// 更新日	：02/13
//------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MouseUserWidget.generated.h"

// 表示するUIのタイプ
UENUM(BlueprintType)
enum class ECursorType :uint8
{
	None = 0	UMETA(DisplayName = "None"),
	Normal = 1	UMETA(DisplayName = "Normal"),
	Abnormal = 2	UMETA(DisplayName = "Abnormal"),
};

/**
 *
 */
UCLASS()
class SHADOWTEST_API UMouseUserWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:


	// コピーモードに入るときに呼び出す（ブループリントで処理を編集する）
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "MouseCursor")
		void SetCursorType(ECursorType _type);
};
