// Fill out your copyright notice in the Description page of Project Settings.


#include "RestartSettingBox.h"
#include "shadowTestGameMode.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"

ARestartSettingBox::ARestartSettingBox()
	: m_pPlayerStart(nullptr)
	, m_bOverlapped(false)
{
	this->OnActorBeginOverlap.AddDynamic(this, &ARestartSettingBox::OnActorOverlapBegin);
	this->OnActorEndOverlap.AddDynamic(this, &ARestartSettingBox::OnActorOverlapEnd);
}


void ARestartSettingBox::OnActorOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	// プレイヤーと当たっていて、オーバーラップ済みでない場合は更新する
	if (OtherActor->ActorHasTag("Player") && !m_bOverlapped)
	{
		/*GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, "CheckPoint");*/
		auto gameMode = Cast<AshadowTestGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

		if (m_pPlayerStart != nullptr)
		{
			gameMode->SetRestartPoint(Cast<AActor>(m_pPlayerStart.Get()));

			m_bOverlapped = true;
		}
		else
		{
			// ログ出力
			UE_LOG(LogTemp, Error, TEXT("null Reference::[%s]"), *GetName());
		}
	}
}

void ARestartSettingBox::OnActorOverlapEnd(AActor* OverlappedActor, AActor* OtherActor)
{

}
