//---------------------------------------------------------------------------
// ファイル名		：SequencerPlayer.cpp
// 概要				：プレイヤーがオーバーラップした際に、設定したシーケンサーのムービーが再生されるボックスコリジョン
// 作成者			：22cu0127　丹代 天
// 更新内容			：2023/02/16　作成
//---------------------------------------------------------------------------

#include "SequencerPlayer.h"
#include "Components/BoxComponent.h"
#include "LevelSequence.h"
#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "MovieSceneSequencePlayer.h"

// コンストラクタ
ASequencerPlayer::ASequencerPlayer()
	: m_pPlaySequencer(NULL)
	, m_pBoxComp(NULL)
	, m_bOncePlay(false)
{
	// Tickを回すかどうか判定
	PrimaryActorTick.bCanEverTick = true;

	//ボックスコンポーネント生成
	m_pBoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	if (m_pBoxComp)
	{
		RootComponent = m_pBoxComp;
	}
}

// 生成時の処理
void ASequencerPlayer::BeginPlay()
{
	Super::BeginPlay();

	//オーバーラップ接触し始めたときに呼ばれる処理
	if (m_pBoxComp != NULL)
	{
		m_pBoxComp->OnComponentBeginOverlap.AddDynamic(this, &ASequencerPlayer::OnOverlapBegin);
	}
}

//オーバーラップ接触し始めた時の処理
void ASequencerPlayer::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//オーバーラップした際に実行したいイベント
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		//オーバーラップしたのがプレイヤーの時のみ反応させたい
		//PlayerCharaBP -> Actor -> Tagsに設定したタグ「Player」で、プレイヤーを識別する
		if (OtherActor->ActorHasTag("Player"))
		{
			//まだ一度も再生されていなければ
			if (m_bOncePlay == false)
			{
				m_bOncePlay = true;

				//再生するシーケンサーとそのアクターが設定されていれば
				if (m_pPlaySequencer)
				{
					// シーケンスアクターを生成
					ALevelSequenceActor* SequenceActor = GetWorld()->SpawnActor<ALevelSequenceActor>(ALevelSequenceActor::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
					if (SequenceActor)
					{
						// シーケンスアクターにシーケンスを設定
						SequenceActor->SetSequence(m_pPlaySequencer);

						// シーケンスプレイヤーを取得
						ULevelSequencePlayer* SequencePlayer = SequenceActor->GetSequencePlayer();
						if (SequencePlayer)
						{
							// シーケンスプレイヤーを再生
							SequencePlayer->Play();
						}
					}
				}
			}
		}
	}
}