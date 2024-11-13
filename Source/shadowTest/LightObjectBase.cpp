//-----------------------------------------------------------------------------------
//ファイル名：LightObjectBase.h
// 処理内容	：ライトとそれに関するデカールを管理するオブジェクトベースクラス
// 作成者	：丹代天
// 更新日	：02/11		作成
//-----------------------------------------------------------------------------------

#include "LightObjectBase.h"
#include "Engine/SpotLight.h"
#include "BoxDecal.h"

ALightObjectBase::ALightObjectBase()
	: m_pLightComp(NULL)
	, m_pDecalComp(NULL)
{
}

void ALightObjectBase::BeginPlay()
{
	Super::BeginPlay();

	PrimaryActorTick.bCanEverTick = true;
}

void ALightObjectBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}