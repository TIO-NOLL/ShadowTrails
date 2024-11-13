/**
* @file		DecalLightObject.h
* @brief	���C�g�p�̋��`�R���W�����t���f�J�[���̃I�u�W�F�N�g
* @author	22CU0219 ��ؗF��
* @date		23/12/18 �쐬
*
*/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DecalLightObject.generated.h"

UCLASS()
class SHADOWTEST_API ADecalLightObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADecalLightObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UDecalComponent* m_pDecalComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UStaticMeshComponent* m_pStaticMeshComponent;
};
