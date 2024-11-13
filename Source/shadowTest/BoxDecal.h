/**
* @file		BoxDecal.h
* @brief	�R���W�����t���f�J�[���̃I�u�W�F�N�g
* @author	22CU0219 ��ؗF��
* @date		23/12/18 �쐬
*
*/
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BoxDecal.generated.h"

UCLASS()
class SHADOWTEST_API ABoxDecal : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABoxDecal();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* m_pEditMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UDecalComponent* m_pDecalComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UBoxComponent* m_pBoxComponent;

};
