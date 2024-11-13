//---------------------------------------------------------------------------
// �t�@�C����		�FSequencerPlayer.h
// �T�v				�F�v���C���[���I�[�o�[���b�v�����ۂɁA�ݒ肵���V�[�P���T�[�̃��[�r�[���Đ������{�b�N�X�R���W����
// �쐬��			�F22cu0127�@�O�� �V
// �X�V���e			�F2023/02/16�@�쐬
//---------------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SequencerPlayer.generated.h"

//�O���錾
class UBoxComponent;
class ULevelSequence;
class ALevelSequenceActor;

UCLASS()
class SHADOWTEST_API ASequencerPlayer : public AActor
{
	GENERATED_BODY()

public:
	//�R���X�g���N�^
	ASequencerPlayer();

protected:
	// �������ɌĂ΂�鏈��
	virtual void BeginPlay() override;

public:

	// �I�[�o�[���b�v�J�n���̏���
	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, Category = "Sequencer")
		ULevelSequence* m_pPlaySequencer;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UBoxComponent* m_pBoxComp;

	bool m_bOncePlay;
};