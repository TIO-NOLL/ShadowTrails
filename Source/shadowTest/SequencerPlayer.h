//---------------------------------------------------------------------------
// ファイル名		：SequencerPlayer.h
// 概要				：プレイヤーがオーバーラップした際に、設定したシーケンサーのムービーが再生されるボックスコリジョン
// 作成者			：22cu0127　丹代 天
// 更新内容			：2023/02/16　作成
//---------------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SequencerPlayer.generated.h"

//前方宣言
class UBoxComponent;
class ULevelSequence;
class ALevelSequenceActor;

UCLASS()
class SHADOWTEST_API ASequencerPlayer : public AActor
{
	GENERATED_BODY()

public:
	//コンストラクタ
	ASequencerPlayer();

protected:
	// 生成時に呼ばれる処理
	virtual void BeginPlay() override;

public:

	// オーバーラップ開始時の処理
	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, Category = "Sequencer")
		ULevelSequence* m_pPlaySequencer;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UBoxComponent* m_pBoxComp;

	bool m_bOncePlay;
};