//------------------------------------------------
// �t�@�C�����FActorFollowingUserWidget.h
// �������e	�F����⏕��UI
// �쐬��	�F22cu0327 ��t�����Y
// �X�V��	�F02/09
//------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"
#include "ActorFollowingUserWidget.generated.h"

// �\������UI�̃^�C�v
UENUM(BlueprintType)
enum class EUIType :uint8
{
	None = 0	UMETA(DisplayName = "None"),
	LeftClick = 1	UMETA(DisplayName = "LeftClick"),
	Action = 2	UMETA(DisplayName = "Action"),
	RotateShadow = 3	UMETA(DisplayName = "RotateShadow"),
	ReturnShadow = 4	UMETA(DisplayName = "ResetSshadow"),
};


UENUM(BlueprintType)
enum class EShadowSoundType :uint8
{
	None = 0	UMETA(DisplayName = "None"),
	Cursor = 1	UMETA(DisplayName = "CursorSound"),
	Cut = 2	UMETA(DisplayName = "CutSound"),
	Paste = 3	UMETA(DisplayName = "PasteSound"),
	Reset = 4	UMETA(DisplayName = "ResetSound"),
};

/**
 *
 */
UCLASS()
class SHADOWTEST_API UActorFollowingUserWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;
	//virtual void NativeTick(const FGeometry& _g, float _deltaTime) override;

public:
	// ����⏕���K�v�ȏꏊ�ɂ���Ƃ��ɌĂяo��
	UFUNCTION(BlueprintCallable)
		void VisibilityType(EUIType _type);

	// �T�E���h��炷�Ƃ��ɌĂяo���i�u���[�v�����g�ŏ�����ҏW����j
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "ShadowSound")
		void PlaySoundbyType(EShadowSoundType _type);

	// UI�̃^�C�v
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
		EUIType m_UIType;
private:

	// UI
	UCanvasPanel* m_pLeftClick;
	UCanvasPanel* m_pAction;
	UCanvasPanel* m_pRotateShadow;
	UCanvasPanel* m_pResetShadow;

};
