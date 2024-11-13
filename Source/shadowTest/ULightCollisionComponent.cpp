//--------------------------
// ファイル： 　ULightCollisionComponent.cpp
// 概要:　      ライトとプレイヤーとの当たり判定
// 作成者:      田中滉太
// 作成日:      12/14


#include "ULightCollisionComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Components/SpotLightComponent.h"
#include "Engine/World.h"
#include "Engine/Engine.h"

// Sets default values
AULightCollisionComponent::AULightCollisionComponent()
	:LightCnt(0.0f)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
	m_pSpotLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("SpotLightComponent"));
	m_pSpotLight->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AULightCollisionComponent::BeginPlay()
{
	Super::BeginPlay();

	//m_pSpotLight->OnToggle.BindUFunction(this, FName("CheckIllumination"));

	m_pPlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	if (m_pPlayerCharacter)
	{/*
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("Player found"));*/
	}
	else
	{/*
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Player not found"));*/
	}
}

// Called every frame
void AULightCollisionComponent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsWithinDistance())
	{
		CheckIllumination();
	}
}
	

// プレイヤーとライトの当たり判定
void AULightCollisionComponent::CheckIllumination()
{
	FVector StartLocation = m_pSpotLight->GetComponentLocation();
	FVector EndLocation = m_pPlayerCharacter->GetActorLocation();

	FCollisionQueryParams TraceParams(FName(TEXT("SpotLightTrace")), true, nullptr);
	FHitResult HitResult;
	
	// レイキャストを行う
	GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, TraceParams);

	if (HitResult.bBlockingHit)
	{

	}
	else
	{
		/*DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, -1.0f, 0, 3.0f);*/
	}
}

bool AULightCollisionComponent::IsWithinDistance()
{
	if (m_pPlayerCharacter)
	{
		FVector SpotLightLocation = GetActorLocation();
		FVector PlayerLocation = m_pPlayerCharacter->GetActorLocation();

		float Distance = FVector::DistXY(SpotLightLocation, PlayerLocation);

		// 一定の範囲内か
		if (Distance <= 5.0f)
		{
			return true;
		}
	}
	return false;
}

