#include "sceneMain.h"
#include "define.h"

//#include "Collision.h"		// Collision検出用
#include "enemyMng.h"
#include "setting.h"

#include "Fade.h"

#include "sceneEnd.h"		// シーン遷移



//======================================
// CSceneMain methods
#pragma region
// コンストラクタ
CSceneMain::CSceneMain():
IScene("","bgm_main")
{
	//InsertObject(ObjPtr(new CCollision()));

	// メイン内で使うオブジェクトを始動させる
	auto& objs = gm()->GetObjects("StageMng Collision Player EnemyMng", ' ');
	for (auto& obj : objs)
		obj->start();

}
CSceneMain::~CSceneMain()
{
}

// 描画
void CSceneMain::draw()
{
	// 何もしない(ステージマネージャーのdrawに任せ
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
	// メイン内で使うオブジェクトを停止させる
	auto& objs = gm()->GetObjects("StageMng Collision", ' ');
	for (auto& obj : objs)
		obj->stop();
	return new CSceneEnd();
}



#pragma endregion
// CSceneMain methods

