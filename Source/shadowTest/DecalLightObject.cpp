/**
* @file		DecalLightObject.cpp
* @brief	ライト用の球形コリジョン付きデカールのオブジェクト
* @author	22CU0219 鈴木友也
* @date		23/12/18 作成
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


	// デカールコンポーネントの生成と設定
	m_pDecalComponent = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComponent"));
	if (m_pDecalComponent != nullptr)
	{
		RootComponent = m_pDecalComponent;
	}

	// スフィアコンポーネントの生成と設定
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

	// タグ追加
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

