// Copyright Epic Games, Inc. All Rights Reserved.

#include "shadowTestGameMode.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/PlayerStart.h"
#include "Blueprint/UserWidget.h"
#include "PlayerCharacter.h"
#include "shadowTestCharacter.h"
#include "CopyShadowObject.h"
#include "Kismet/GameplayStatics.h"

AshadowTestGameMode::AshadowTestGameMode()
	: m_MenuClass(nullptr)
	, m_pMenuWidget(nullptr)
	, m_pCurrentPlayerRestart(nullptr)
{
	m_PlayerStartArray.Empty();
}

void AshadowTestGameMode::BeginPlay()
{
	Super::BeginPlay();

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), m_PlayerStartArray);

	// ウィジェット作成
	if (m_MenuClass != nullptr)
	{
		m_pMenuWidget = CreateWidget<UUserWidget>(GetWorld(), m_MenuClass);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("MenuWidgetClass is Empty"));
	}

	// 作成に成功したらAddToViewport
	if (m_pMenuWidget != nullptr)
	{
		m_pMenuWidget->AddToViewport();
		m_pMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Can not Create MenuWidget"));
	}

	// 影オブジェクトを全て取得しておく
	TArray<AActor*> outActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACopyShadowObject::StaticClass(), outActors);
	for (int i = 0; i < outActors.Num(); ++i)
	{
		ACopyShadowObject* shadow = Cast<ACopyShadowObject>(outActors[i]);
		if (shadow != nullptr)
			m_ShadowArray.Add(shadow);
	}


}


/**
* 復活地点を更新
*/
void AshadowTestGameMode::SetRestartPoint(AActor* _newPlayerStart)
{
	UE_LOG(LogTemp, Warning, TEXT("SetRestartPoint"));
	m_pCurrentPlayerRestart = _newPlayerStart;
}


/**
* プレイヤーを復活地点に移動させる
*/
void AshadowTestGameMode::RestartPlayerToCurrentPoint(APlayerCharacter* _player)
{
	if (m_pCurrentPlayerRestart == nullptr || _player == nullptr)
		return;

	// イベント呼び出し
	Restart();
	// プレイヤー状態をリセット
	_player->ResetPlayer();

	FVector restartLocation = m_pCurrentPlayerRestart->GetActorLocation();
	FRotator restartRotation = m_pCurrentPlayerRestart->GetActorRotation();

	_player->SetActorLocation(restartLocation);
	_player->SetPlayerRotation(restartRotation);

	// 影の位置を元に戻す
	for (int i = 0; i < m_ShadowArray.Num(); ++i)
	{
		if (m_ShadowArray[i] != nullptr)
		{
			if (m_ShadowArray[i]->IsMoved())
			{
				m_ShadowArray[i]->ResetPos();
				m_ShadowArray[i]->SetMoved(false);
			}
		}
	}

	//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Cyan, "Restart");
}


/**
* メニューの開閉を切り替え
*/
void AshadowTestGameMode::SwitchMenu()
{
	if (m_pMenuWidget->GetVisibility() != ESlateVisibility::Visible)
	{
		if (m_pMenuWidget != nullptr)
		{
			m_pMenuWidget->SetVisibility(ESlateVisibility::Visible);
			UGameplayStatics::SetGamePaused(GetWorld(), true);
			UGameplayStatics::GetPlayerController(GetWorld(), 0)->bShowMouseCursor = true;
		}
	}
	else
	{
		if (m_pMenuWidget != nullptr)
		{
			m_pMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
			UGameplayStatics::SetGamePaused(GetWorld(), false);
			UGameplayStatics::GetPlayerController(GetWorld(), 0)->bShowMouseCursor = false;
		}
	}
}