#include "sceneEnd.h"
#include "define.h"

#include "sceneTitle.h"

//======================================
// CSceneEnd methods
#pragma region
// コンストラクタ
CSceneEnd::CSceneEnd():
IScene("bgm_end")
{
}
CSceneEnd::~CSceneEnd()
{
}

// 描画
void CSceneEnd::draw()
{
	font::Draw_FontText(100, 200, 0.5f, "エンド画面", -1, 0);
}


// 処理
bool CSceneEnd::update()
{

	// 何かアクションを起こしてシーンが切り替わるとき
	if (input::CheckPush(input::KEY_BTN0))
	{
		return true;
	}
	return false;
}

IScene* CSceneEnd::NextScene()
{
	return new CSceneTitle();
}




#pragma endregion
// CSceneEnd methods
