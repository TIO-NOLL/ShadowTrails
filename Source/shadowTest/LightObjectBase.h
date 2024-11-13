//-----------------------------------------------------------------------------------
//ファイル名：LightObjectBase.cpp
// 処理内容	：ライトとそれに関するデカールを管理するオブジェクトベースクラス
// 作成者	：丹代天
// 更新日	：02/11		作成
//-----------------------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "ActionObjectBase.h"
#include "LightObjectBase.generated.h"

//前方宣言
class ASpotLight;
class ABoxDecal;

UCLASS()
class SHADOWTEST_API ALightObjectBase : public AActionObjectBase
{
	GENERATED_BODY()

public:
	//コンストラクタ
	ALightObjectBase();

protected:
	// 生成時の処理
	virtual void BeginPlay() override;

	//フレームごとの処理
	virtual void Tick(float DeltaTime)override;

public:
	//エディタ側で編集できるメンバ変数
	UPROPERTY(EditAnywhere, Category = "Light")
		AActor* m_pLightComp;		//ライト

	UPROPERTY(EditAnywhere, Category = "Light")
		AActor* m_pDecalComp;		//デカール

};
