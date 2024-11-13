//-----------------------------------------------------------------------------------
//ファイル名：ActionObjectBase.cpp
// 処理内容	：Player側からアクションを起こすことのできるオブジェクトのベースクラス
// 作成者	：丹代天
// 更新日	：12/11		作成
//-----------------------------------------------------------------------------------

#include "ActionObjectBase.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Components/BoxComponent.h"

//コンストラクタ
AActionObjectBase::AActionObjectBase()
{
	PrimaryActorTick.bCanEverTick = true;

	//ルートに置くベースオブジェクト生成
	m_pBase = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
	RootComponent = m_pBase;

	//メッシュコンポーネント生成
	m_pObjectMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ObjectMesh"));
	m_pObjectMeshComp->SetupAttachment(RootComponent);

	//ボックスコリジョン生成
	m_pBoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponant"));
	m_pBoxComp->SetupAttachment(RootComponent);
}

// フレームごとの処理
void AActionObjectBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AActionObjectBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//プレイヤーがアクションコマンドを実行した際に呼ばれる処理
void AActionObjectBase::OnActionthisObject()
{
	//実行したいイベントを書く。
}

