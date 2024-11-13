//---------------------------------------------------------------------------
// �t�@�C����		�FSequencerPlayer.cpp
// �T�v				�F�v���C���[���I�[�o�[���b�v�����ۂɁA�ݒ肵���V�[�P���T�[�̃��[�r�[���Đ������{�b�N�X�R���W����
// �쐬��			�F22cu0127�@�O�� �V
// �X�V���e			�F2023/02/16�@�쐬
//---------------------------------------------------------------------------

#include "SequencerPlayer.h"
#include "Components/BoxComponent.h"
#include "LevelSequence.h"
#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "MovieSceneSequencePlayer.h"

// �R���X�g���N�^
ASequencerPlayer::ASequencerPlayer()
	: m_pPlaySequencer(NULL)
	, m_pBoxComp(NULL)
	, m_bOncePlay(false)
{
	// Tick���񂷂��ǂ�������
	PrimaryActorTick.bCanEverTick = true;

	//�{�b�N�X�R���|�[�l���g����
	m_pBoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	if (m_pBoxComp)
	{
		RootComponent = m_pBoxComp;
	}
}

// �������̏���
void ASequencerPlayer::BeginPlay()
{
	Super::BeginPlay();

	//�I�[�o�[���b�v�ڐG���n�߂��Ƃ��ɌĂ΂�鏈��
	if (m_pBoxComp != NULL)
	{
		m_pBoxComp->OnComponentBeginOverlap.AddDynamic(this, &ASequencerPlayer::OnOverlapBegin);
	}
}

//�I�[�o�[���b�v�ڐG���n�߂����̏���
void ASequencerPlayer::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//�I�[�o�[���b�v�����ۂɎ��s�������C�x���g
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		//�I�[�o�[���b�v�����̂��v���C���[�̎��̂ݔ�����������
		//PlayerCharaBP -> Actor -> Tags�ɐݒ肵���^�O�uPlayer�v�ŁA�v���C���[�����ʂ���
		if (OtherActor->ActorHasTag("Player"))
		{
			//�܂���x���Đ�����Ă��Ȃ����
			if (m_bOncePlay == false)
			{
				m_bOncePlay = true;

				//�Đ�����V�[�P���T�[�Ƃ��̃A�N�^�[���ݒ肳��Ă����
				if (m_pPlaySequencer)
				{
					// �V�[�P���X�A�N�^�[�𐶐�
					ALevelSequenceActor* SequenceActor = GetWorld()->SpawnActor<ALevelSequenceActor>(ALevelSequenceActor::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
					if (SequenceActor)
					{
						// �V�[�P���X�A�N�^�[�ɃV�[�P���X��ݒ�
						SequenceActor->SetSequence(m_pPlaySequencer);

						// �V�[�P���X�v���C���[���擾
						ULevelSequencePlayer* SequencePlayer = SequenceActor->GetSequencePlayer();
						if (SequencePlayer)
						{
							// �V�[�P���X�v���C���[���Đ�
							SequencePlayer->Play();
						}
					}
				}
			}
		}
	}
}