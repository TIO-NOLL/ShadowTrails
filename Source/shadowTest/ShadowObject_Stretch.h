/**
* @file		ShadowObject_Stretch.cpp
* @brief	伸び縮みする影オブジェクト
* @author	22cu0219 鈴木友也
* @date		24/02/08 作成
*			24/02/17 最終更新
*/

#pragma once

#include "CoreMinimal.h"
#include "CopyShadowObject.h"
#include "ShadowObject_Stretch.generated.h"

/**
 *
 */
UCLASS()
class SHADOWTEST_API AShadowObject_Stretch : public ACopyShadowObject
{
	GENERATED_BODY()

public:
	// コンストラクタ
	AShadowObject_Stretch();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
		USceneComponent* m_pSceneComponent;

	UPROPERTY(VisibleAnywhere)
		float m_scaleAlpha;

	UPROPERTY(VisibleAnywhere)
		float m_timeCounter;

	UPROPERTY(EditAnywhere)
		float m_stretchScale;

	UPROPERTY(EditAnywhere)
		float m_cycleLength;
	
};
