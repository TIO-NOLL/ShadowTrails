/**
* @file		DecalLightObject.cpp
* @brief	���C�g�p�̋��`�R���W�����t���f�J�[���̃I�u�W�F�N�g
* @author	22CU0219 ��ؗF��
* @date		23/12/18 �쐬
*
*/


#include "DecalLightObject.h"
#include "Components/DecalComponent.h"
#include "Components/SphereComponent.h"


// Sets default values
ADecalLightObject::ADecalLightObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	// �f�J�[���R���|�[�l���g�̐����Ɛݒ�
	m_pDecalComponent = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComponent"));
	if (m_pDecalComponent != nullptr)
	{
		RootComponent = m_pDecalComponent;
	}

	// �X�t�B�A�R���|�[�l���g�̐����Ɛݒ�
	m_pStaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	if (m_pStaticMeshComponent != nullptr)
	{
		ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("/Game/Model/LitCylinder/SM_LitCollision.SM_LitCollision"));
		UStaticMesh* assetObj = MeshAsset.Object;
		m_pStaticMeshComponent->SetStaticMesh(assetObj);

		m_pStaticMeshComponent->SetupAttachment(RootComponent);
		m_pStaticMeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
		m_pStaticMeshComponent->bHiddenInGame = true;
		m_pStaticMeshComponent->SetRelativeScale3D(FVector(2.5f, 2.5f, 2.5f));
	}

	// �^�O�ǉ�
	Tags = { "DecalLight" };
}

// Called when the game starts or when spawned
void ADecalLightObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADecalLightObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

