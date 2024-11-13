/**
* @file		BoxDecal.cpp
* @brief	コリジョン付きデカールのオブジェクト
* @author	22CU0219 鈴木友也
* @date		23/12/18 作成
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

	// ボックスコンポーネントの生成と設定
	m_pBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	if (m_pBoxComponent != nullptr)
	{
		RootComponent = m_pBoxComponent;
	}

	// デカールコンポーネントの生成と設定
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

