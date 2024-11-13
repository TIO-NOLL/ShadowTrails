//---------------------------------------------------------------------------
// ファイル名		：ActionObject_Box.cpp
// 概要				：箱型の被アクションオブジェクト、アクションコマンドが実行された際には動かすことができる
// 作成者			：22cu0127　丹代 天
// 更新内容			：2023/12/14　作成
//---------------------------------------------------------------------------

//インクルード
#include "ActionObject_Box.h"
#include "Kismet/GamePlayStatics.h"
#include "DrawDebugHelpers.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Character.h"


AActionObject_Box::AActionObject_Box()
	: m_pPlayer(NULL)
	, m_bNowCorrect(false)
	, m_pStaticMesh(NULL)
	, m_bIsPushed(false)
	, m_meshSize(FVector::ZeroVector)
	/*, m_CorrectTime(0.f)
	, m_CorrectCnt(0.f)*/
{
	
}

void AActionObject_Box::BeginPlay()
{
	Super::BeginPlay();

	PrimaryActorTick.bCanEverTick = true;

	////プレイヤーのポインタを取得する
	//GetPlayerActor();

	if (m_pObjectMeshComp != NULL)
	{
		//メッシュの中身を取得
		m_pStaticMesh = m_pObjectMeshComp->GetStaticMesh();
	}

	//メッシュのNULLチェック
	if (m_pStaticMesh != NULL)
	{
		//メッシュのバウンディングボックスサイズを取得する
		m_meshSize = m_pStaticMesh->GetBounds().GetBox().GetSize();

		//オブジェクトのActorのサイズを加味する
		m_meshSize *= GetActorScale();
	}

	//Tick処理を無効する
	SetActorTickEnabled(false);
}

void AActionObject_Box::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	////デバッグ用ログ
	//UE_LOG(LogTemp, Warning, TEXT("m_CorrectTime: %f, m_CorrectCnt: %f"), m_CorrectTime, m_CorrectCnt);

	//if (m_CorrectTime + 2.0f > m_CorrectCnt)
	//{
	//	//現在のFPSを測定
	//	float fps = 1.0f / DeltaTime;

	//	//処理落ちしても、正確に移動するように補正
	//	float fpsCorrection = 60.f / fps;

	//	//加算
	//	m_CorrectCnt += fpsCorrection;
	//}
	//else 
	//{
	//	//押されている最中の座標更新処理
	//	IsPushed(DeltaTime);
	//}

	//押されている最中の座標更新処理
	IsPushed(DeltaTime);
}

//押されている状態の時、プレイヤーの動きに合わせて移動する関数
void AActionObject_Box::IsPushed(float _deltaTime)
{
	//プレイヤーのメッシュ情報を取得
	USkeletalMeshComponent* pPlayerMesh = Cast<ACharacter>(m_pPlayer)->GetMesh();

	//プレイヤーのメッシュサイズ取得
	FVector PlayerMeshSize = pPlayerMesh->Bounds.GetBox().GetSize() * m_pPlayer->GetActorScale();

	//プレイヤーの座標を取得
	FVector PlayerLocation = pPlayerMesh->GetComponentLocation();

	//新しくプレイヤーと、このオブジェクトのメッシュサイズを考慮した座礁を設定する。
	FVector NewLocation = PlayerLocation + pPlayerMesh->GetForwardVector() * (m_meshSize + PlayerMeshSize) / 2;

	//Z軸に関してはプレイヤーの座標を考慮しない
	NewLocation.Z = GetActorLocation().Z;

	//オブジェクトの座標を設定
	SetActorLocation(NewLocation);

	//運び状態でないときオブジェクトTickを切る
	if (m_bIsPushed == false)
	{
		//Tickを無効にする
		SetActorTickEnabled(false);
		/*if (m_CorrectCnt != 0.0f)
		{
			m_CorrectCnt = 0.0f;
		}*/
	}
}

//プレイヤー側から呼び出される関数
void AActionObject_Box::OnActionthisObject()
{
	//フラグの切り替え
	m_bIsPushed = !m_bIsPushed;

	//デバッグ用ログ
	UE_LOG(LogTemp, Warning, TEXT("Pushed"));

	if (m_bIsPushed == true) 
	{ 
		//Tickを有効にする
		SetActorTickEnabled(true);
	}
}

//プレイヤーのアドレスを取得する関数
void AActionObject_Box::GetPlayerActor(FVector PlayerPos)
{
	////m_pPlayerが空の時のみ
	//if (m_pPlayer == NULL)
	//{
	//	//ゲーム全体へのActor検索処理(コスト高、一回だけ)
	//	TSubclassOf<AActor> findClass;
	//	findClass = AActor::StaticClass();
	//	TArray<AActor*> actors;
	//	UGameplayStatics::GetAllActorsOfClass(this->GetWorld(), findClass, actors);

	//	//検索の結果Actroがあれば
	//	if (actors.Num() > 0)
	//	{
	//		//Actorの中身を順番に検索していく
	//		for (int idx = 0; idx < actors.Num(); idx++)
	//		{
	//			AActor* pActor = Cast<AActor>(actors[idx]);
	//			//タグがプレイヤーだった時
	//			if (pActor->ActorHasTag("Player"))
	//			{
	//				m_pPlayer = pActor;
	//				break;
	//			}
	//		}
	//	}
	//}

	//レイを飛ばす
		//始点 Start：Actor
	FVector Start = GetActorLocation();
	//終点 End：Startから前方向m_ActionForwardLengthの距離th;
	FVector End = PlayerPos;

	////デバッグ用にラインを描画
	//DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 1.f);

	//コリジョン判定で無視する項目を指定
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

	//レイにヒットしたオブジェクトを格納する変数
	FHitResult RayHit;

	//ヒットした際にtrueになるフラグ,
	bool isHit = GetWorld()->LineTraceSingleByChannel(RayHit, Start, End, ECC_WorldStatic, CollisionParams);

	//ヒットしたオブジェクトがある場合
	if (isHit)
	{
		//ヒットしたActorのタグを調べる
		//アクションを起こすことのできるオブジェクトだった場合
		if (RayHit.GetActor()->ActorHasTag("Player"))
		{
			////デバッグ確認用
			//FString str = RayHit.GetActor()->GetName();
			//UE_LOG(LogTemp, Warning, TEXT("HitActor [ActionObject]ObjectName : %s"), *str);

			//ActionObjectBaseにキャスト
			m_pPlayer = Cast<ACharacter>(RayHit.GetActor());
		}
		else
		{
			//アクションを起こすオブジェクト以外の場合はActionObjectの中身を空にしておく
			m_pPlayer = NULL;
		}
	}
}

/*
* このオブジェクトのフォワードベクトルとプレイヤーの座標までのベクトルの角度を求める関数
*
*	引数：PlayerPos		プレイヤーの座標
*/
float AActionObject_Box::GetAnglePlayer(FVector PlayerPos)
{
	//対象と自分の座標を照合
	FVector diff = PlayerPos - this->GetActorLocation();
	//数値をもとにラジアン値を求める
	float rad = atan2(diff.Y, diff.X);
	//ラジアン値から角度に変換する
	float degree = FMath::RadiansToDegrees(rad);

	//プレイヤー方向にレイを飛ばし、触れたプレイヤーのアドレスを取得する
	GetPlayerActor(PlayerPos);


	//角度を返す(-180°～180°)
	return degree;
}

////時間を設定する関数
//void AActionObject_Box::SetTime(float _CorrectTime)
//{
//	m_CorrectTime = _CorrectTime;
//}