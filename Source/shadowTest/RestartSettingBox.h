// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "RestartSettingBox.generated.h"

/**
 *
 */
UCLASS()
class SHADOWTEST_API ARestartSettingBox : public ATriggerBox
{
	GENERATED_BODY()

public:
	ARestartSettingBox();

	UPROPERTY(EditAnywhere)
		TWeakObjectPtr<class APlayerStart> m_pPlayerStart;

private:
	// ÉCÉxÉìÉgä÷êî
	UFUNCTION()
		void OnActorOverlapBegin(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
		void OnActorOverlapEnd(AActor* OverlappedActor, AActor* OtherActor);

	bool m_bOverlapped;
};
