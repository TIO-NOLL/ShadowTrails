//------------------------------------------------
// �t�@�C�����FMouseUserWidget.h
// �������e	�F�}�E�X��UI��\������
// �쐬��	�F22cu0327 ��t�����Y
// �X�V��	�F02/13
//------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MouseUserWidget.generated.h"

// �\������UI�̃^�C�v
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


	// �R�s�[���[�h�ɓ���Ƃ��ɌĂяo���i�u���[�v�����g�ŏ�����ҏW����j
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "MouseCursor")
		void SetCursorType(ECursorType _type);
};
