//---------------------------------------------------------------------------
// ファイル名		：EffectComponent.h
// 概要				：エフェクトを保持するコンポーネント
// 作成者			：22cu0127　丹代 天
// 更新内容			：2023/01/14　作成
//---------------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EffectComponent.generated.h"

//前方宣言
class UParticleSystem;
class UNiagaraSystem;
class UParticleSystemComponent;
class UNiagaraComponent;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SHADOWTEST_API UEffectComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	//コンストラクタ
	UEffectComponent();

protected:
	// 生成時の処理
	virtual void BeginPlay() override;

public:
	// フレームごとに呼ばれる処理
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects")
		UParticleSystem* ParticleEffect;		//再生するエフェクト

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects")
		UNiagaraSystem* NiagaraEffect;

private:
	UParticleSystemComponent* m_ActiveEffectComponent;		//生成したパーティクルエフェクトを保持する変数

	UNiagaraComponent* m_ActiveNiagaraComponent;			//生成したNiagaraエフェクトを保持する変数

public:
	//エフェクトを再生する関数
	void PlayEffect(USceneComponent* _rootComponent, FVector _spawnVector, FRotator _spawnRotator, bool _autoDestoroy);
	//エフェクトを停止する関数
	void StopEffect();
};
