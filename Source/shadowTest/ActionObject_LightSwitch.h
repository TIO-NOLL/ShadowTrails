//---------------------------------------------------------------------------
// ファイル名		：ActionObject_LightSwitch.h
// 概要				：ライトのスイッチ、アクションを実行した際には対応するライトオブジェクトの表示非表示を切り替えることができる
// 作成者			：22cu0127　丹代 天
// 更新内容			：2024/01/19　作成
//---------------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "ActionObjectBase.h"		//ActionBase継承
#include "ActionObject_LightSwitch.generated.h"

/**
 * 
 */
UCLASS()
class SHADOWTEST_API AActionObject_LightSwitch : public AActionObjectBase
{
	GENERATED_BODY()

private:
	//コンストラクタ
	AActionObject_LightSwitch();

protected:
	// 生成時の処理
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = "Light")
		TArray<AActor*> m_StartOn_CanOperationLights;		//開始時に可視状態のライトのオブジェクトを格納する配列
	UPROPERTY(EditAnywhere, Category = "Light")
		TArray<AActor*> m_StartOff_CanOperationLights;		//開始時に不可視状態のライトのオブジェクトを格納する配列

	UPROPERTY(EditAnywhere, Category = "Decal")
		TArray<AActor*> m_StartOn_CanOperationDecals;		//開始時に可視状態のデカールのオブジェクト格納する配列
	UPROPERTY(EditAnywhere, Category = "Decal")
		TArray<AActor*> m_StartOff_CanOperationDecals;		//開始時に不可視状態のデカールのオブジェクトを格納する配列
private:
	//アクションが実行された際に呼ばれる関数
	void OnActionthisObject()override;
};
