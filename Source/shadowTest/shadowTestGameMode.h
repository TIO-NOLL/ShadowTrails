// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "shadowTestGameMode.generated.h"

UCLASS(minimalapi)
class AshadowTestGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AshadowTestGameMode();

protected:
	void BeginPlay() override;


public:
	UFUNCTION(BlueprintCallable, Category = "shadowTestGameMode")
		void SetRestartPoint(AActor* _newPlayerStart);

	// リスタートした際に呼び出されるBP上のイベント
	UFUNCTION(BlueprintImplementableEvent, Category = "shadowTestGameMode")
		void Restart();

	// プレイヤーをリスタート
	UFUNCTION(BlueprintCallable, Category = "shadowTestGameMode")
		void RestartPlayerToCurrentPoint(class APlayerCharacter* _player);

	UFUNCTION(BlueprintCallable, Category = "shadowTestGameMode")
		AActor* GetCurrentRestart() { return m_pCurrentPlayerRestart.Get(); };

	UFUNCTION(BlueprintCallable, Category = "shadowTestGameMode")
		void SwitchMenu();

protected:
	// エディタ上でウィジェットを設定するための変数
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Menu Widget")
		TSubclassOf<class UUserWidget> m_MenuClass;

	// 作成したウィジェットを保持する
	UPROPERTY()
		class UUserWidget* m_pMenuWidget;

	UPROPERTY()
	TArray<AActor*> m_PlayerStartArray;

	UPROPERTY(VisibleAnywhere, Category = "Menu Widget")
		TSoftObjectPtr<AActor> m_pCurrentPlayerRestart;


	UPROPERTY(VisibleAnywhere)
		TArray<class ACopyShadowObject*> m_ShadowArray;
};



