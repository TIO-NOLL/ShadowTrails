//---------------------------------------------------------------------------
// �t�@�C����		�FEffectComponent.h
// �T�v				�F�G�t�F�N�g��ێ�����R���|�[�l���g
// �쐬��			�F22cu0127�@�O�� �V
// �X�V���e			�F2023/01/14�@�쐬
//---------------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EffectComponent.generated.h"

//�O���錾
class UParticleSystem;
class UNiagaraSystem;
class UParticleSystemComponent;
class UNiagaraComponent;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SHADOWTEST_API UEffectComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	//�R���X�g���N�^
	UEffectComponent();

protected:
	// �������̏���
	virtual void BeginPlay() override;

public:
	// �t���[�����ƂɌĂ΂�鏈��
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects")
		UParticleSystem* ParticleEffect;		//�Đ�����G�t�F�N�g

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects")
		UNiagaraSystem* NiagaraEffect;

private:
	UParticleSystemComponent* m_ActiveEffectComponent;		//���������p�[�e�B�N���G�t�F�N�g��ێ�����ϐ�

	UNiagaraComponent* m_ActiveNiagaraComponent;			//��������Niagara�G�t�F�N�g��ێ�����ϐ�

public:
	//�G�t�F�N�g���Đ�����֐�
	void PlayEffect(USceneComponent* _rootComponent, FVector _spawnVector, FRotator _spawnRotator, bool _autoDestoroy);
	//�G�t�F�N�g���~����֐�
	void StopEffect();
};
