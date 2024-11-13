//-----------------------------------------------------------------------------------
//ファイル名：ActionObjectBase.h
// 処理内容	：Player側からアクションを起こすことのできるオブジェクトのベースクラス
// 作成者	：丹代天
// 更新日	：12/11		作成
//-----------------------------------------------------------------------------------

//インクルードガード
#pragma once

//インクルード
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ActionObjectBase.generated.h"

//前方宣言
class UStaticMeshComponent;
class UBoxComponent;

UCLASS()
class SHADOWTEST_API AActionObjectBase : public AActor
{
	GENERATED_BODY()
	
public:	
	//コンストラクタ
	AActionObjectBase();

protected:
	// 生成時の処理
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime)override;

public:
	//エディタ側で編集できるメンバ変数
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* m_pObjectMeshComp;		//オブジェクトのメッシュ

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UBoxComponent* m_pBoxComp;		//コリジョン

protected:
	UPROPERTY()
		USceneComponent* m_pBase;		//ベースコンポーネント

public:
	//プレイヤーがアクションコマンドを実行したときに呼ばれる処理
	UFUNCTION()
		virtual void OnActionthisObject();

};