/**
* @file		SplineMovementComponent.h
* @brief	アクタをスプラインに沿って移動させるコンポーネント
* @author	22cu0219　鈴木友也
*/

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SplineMovementComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHADOWTEST_API USplineMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// コンストラクタ
	USplineMovementComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void SetMoveSpeed(float _newSpeed) { m_moveSpeed = _newSpeed; }

protected:
	// スプラインアクタ参照
	UPROPERTY(EditAnywhere)
		TWeakObjectPtr<AActor> m_pSplineActor;

	// 移動速度
	UPROPERTY(EditAnywhere)
		float m_moveSpeed;

	// 補間速度
	UPROPERTY(EditAnywhere)
		float m_interpSpeed;

	// 現在のスプライン上の移動距離
	UPROPERTY(VisibleAnywhere)
		float m_currentMovedDistance;

	// ループするかどうか
	UPROPERTY(EditAnywhere)
		bool m_bLoop;

	// 終点に到達したとき始点に戻るかどうか
	UPROPERTY(EditAnywhere)
		bool m_bReturn;

	// 逆行中かどうか
	UPROPERTY(VisibleAnywhere)
		bool m_bReverse;

	UPROPERTY(EditAnywhere)
		float m_returnWaitSecond;

	UPROPERTY(VisibleAnywhere)
		float m_waitCounter;

	// スプラインコンポーネントの弱参照
	TWeakObjectPtr<class USplineComponent> m_pSplineComponent;

};
