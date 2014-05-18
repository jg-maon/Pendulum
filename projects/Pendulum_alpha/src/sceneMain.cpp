#include "sceneMain.h"
#include "define.h"

#include "Collision.h"		// Collision検出用
#include "enemyMng.h"
#include "setting.h"

#include "Fade.h"

#include "sceneEnd.h"		// シーン遷移

extern CGameManager* gm;


//======================================
// CSceneMain methods
#pragma region
// コンストラクタ
CSceneMain::CSceneMain():
IScene("bgm_main")
{
	InsertObject(ObjPtr(new CCollision()));
}
CSceneMain::~CSceneMain()
{
}

// 描画
void CSceneMain::draw()
{
	//Draw_FontText(100,200,0.5f,"メイン画面",-1,0);
}

// 処理
bool CSceneMain::update()
{
	// 何かアクションを起こしてシーンが切り替わるとき
	if (input::CheckPush(input::KEY_BTN0))
	{
		return true;
	}
	return false;
}

IScene* CSceneMain::NextScene()
{
	return new CSceneEnd();
}



#pragma endregion
// CSceneMain methods

