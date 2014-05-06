#include "sceneTitle.h"
#include "lib\gplib.h"
#include "setting.h"

#include "sceneStageSelect.h"

//======================================
// CSceneTitle methods
#pragma region
// コンストラクタ
CSceneTitle::CSceneTitle()
{
	
}
CSceneTitle::~CSceneTitle()
{
}



// 処理
IScene* CSceneTitle::step()
{
	// 何かアクションを起こしてシーンが切り替わるとき
	if(CheckPush(KEY_BTN0))
	{
		return new CSceneStageSelect();
	}
	return this;
}

// 描画
void CSceneTitle::draw()
{
	//DSound_
	Draw_FontTextNC(100,200,0.5f,"タイトル画面",-1,0);
	Draw_FontTextNC(100,250,0.5f,"ボタン1でシーン切り替え",-1,0);
}


#pragma endregion
// CSceneTitle methods

