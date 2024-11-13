/**
* @file		DecalLightObject.h
* @brief	ライト用の球形コリジョン付きデカールのオブジェクト
* @author	22CU0219 鈴木友也
* @date		23/12/18 作成
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
