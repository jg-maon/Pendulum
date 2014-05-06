#include "sceneEnd.h"
#include "define.h"

#include "sceneTitle.h"

//======================================
// CSceneEnd methods
#pragma region
// コンストラクタ
CSceneEnd::CSceneEnd()
{
}
CSceneEnd::~CSceneEnd()
{
}


// 処理
IScene* CSceneEnd::step()
{
	// 何かアクションを起こしてシーンが切り替わるとき
	if(input::CheckPush(input::KEY_BTN0))
	{
		return new CSceneTitle();
	}
	return this;
}


// 描画
void CSceneEnd::draw()
{
	font::Draw_FontText(100,200,0.5f,"エンド画面",-1,0);
}

#pragma endregion
// CSceneEnd methods
