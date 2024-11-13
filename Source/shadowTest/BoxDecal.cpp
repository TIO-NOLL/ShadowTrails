/**
* @file		BoxDecal.cpp
* @brief	�R���W�����t���f�J�[���̃I�u�W�F�N�g
* @author	22CU0219 ��ؗF��
* @date		23/12/18 �쐬
*
*/


#include "BoxDecal.h"
#include "Components/DecalComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
ABoxDecal::ABoxDecal()
	: m_pDecalComponent(nullptr)
	, m_pBoxComponent(nullptr)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// �{�b�N�X�R���|�[�l���g�̐����Ɛݒ�
	m_pBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	if (m_pBoxComponent != nullptr)
	{
		RootComponent = m_pBoxComponent;
	}

	// �f�J�[���R���|�[�l���g�̐����Ɛݒ�
	m_pDecalComponent = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComponent"));
	if (m_pDecalComponent != nullptr)
	{
		m_pDecalComponent->SetupAttachment(RootComponent);
		m_pDecalComponent->DecalSize = m_pBoxComponent->GetScaledBoxExtent();
	}


	m_pEditMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EditMesh"));

	ConstructorHelpers::FObjectFinder<UStaticMesh>findAsset(TEXT("/Engine/BasicShapes/Cube.Cube"));
	UStaticMesh* assetObj = findAsset.Object;

	if (m_pEditMesh != nullptr)
	{
		m_pEditMesh->SetupAttachment(RootComponent);
		m_pEditMesh->SetStaticMesh(assetObj);
		m_pEditMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		m_pEditMesh->bHiddenInGame = true;
		m_pEditMesh->bReceivesDecals = false;
		m_pEditMesh->SetRelativeScale3D(FVector(0.1f, 0.1f, 0.1f));
	}
	
}

// Called when the game starts or when spawned
void ABoxDecal::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABoxDecal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

