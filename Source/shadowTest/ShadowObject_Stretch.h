/**
* @file		ShadowObject_Stretch.cpp
* @brief	�L�яk�݂���e�I�u�W�F�N�g
* @author	22cu0219 ��ؗF��
* @date		24/02/08 �쐬
*			24/02/17 �ŏI�X�V
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
	// �R���X�g���N�^
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
