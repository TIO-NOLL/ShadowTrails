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

	// ���X�^�[�g�����ۂɌĂяo�����BP��̃C�x���g
	UFUNCTION(BlueprintImplementableEvent, Category = "shadowTestGameMode")
		void Restart();

	// �v���C���[�����X�^�[�g
	UFUNCTION(BlueprintCallable, Category = "shadowTestGameMode")
		void RestartPlayerToCurrentPoint(class APlayerCharacter* _player);

	UFUNCTION(BlueprintCallable, Category = "shadowTestGameMode")
		AActor* GetCurrentRestart() { return m_pCurrentPlayerRestart.Get(); };

	UFUNCTION(BlueprintCallable, Category = "shadowTestGameMode")
		void SwitchMenu();

protected:
	// �G�f�B�^��ŃE�B�W�F�b�g��ݒ肷�邽�߂̕ϐ�
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Menu Widget")
		TSubclassOf<class UUserWidget> m_MenuClass;

	// �쐬�����E�B�W�F�b�g��ێ�����
	UPROPERTY()
		class UUserWidget* m_pMenuWidget;

	UPROPERTY()
	TArray<AActor*> m_PlayerStartArray;

	UPROPERTY(VisibleAnywhere, Category = "Menu Widget")
		TSoftObjectPtr<AActor> m_pCurrentPlayerRestart;


	UPROPERTY(VisibleAnywhere)
		TArray<class ACopyShadowObject*> m_ShadowArray;
};



