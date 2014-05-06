#include "sceneTitle.h"
#include "define.h"

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
	if(input::CheckPush(input::KEY_BTN0))
	{
		return new CSceneStageSelect();
	}
	return this;
}

// 描画
void CSceneTitle::draw()
{
	//DSound_
	font::Draw_FontTextNC(100,200,0.5f,"タイトル画面",-1,0);
	font::Draw_FontTextNC(100,250,0.5f,"ボタン1でシーン切り替え",-1,0);
}


#pragma endregion
// CSceneTitle methods

