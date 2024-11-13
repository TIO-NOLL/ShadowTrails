//--------------------------
// �t�@�C���F �@ULightCollisionComponent.h
// �T�v:�@      ���C�g�ƃv���C���[�Ƃ̓����蔻��
// �쐬��:      �c���䑾
// �쐬��:      12/14

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "ULightCollisionComponent.generated.h"

//#include "Components/SpotLightComponent.h"

class APlayerController;

UCLASS()
class SHADOWTEST_API AULightCollisionComponent : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AULightCollisionComponent();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "LightCollisionComponent")
		float GetLight() const { return LightCnt; }

private:
	UPROPERTY(EditAnywhere)
		class AActor* m_pPlayerCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		class USpotLightComponent* m_pSpotLight;

	UPROPERTY(VisibleAnywhere)
		class USceneComponent* Root;

	UPROPERTY(EditAnywhere)
		float LightCnt;

	UFUNCTION()
	void CheckIllumination();

	bool IsWithinDistance();

	void LightWorning(float LightDistance);
	
};
