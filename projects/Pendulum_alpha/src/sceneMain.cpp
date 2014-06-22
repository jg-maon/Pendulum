#include "sceneMain.h"
#include "define.h"

//#include "Collision.h"		// Collision検出用
#include "enemyMng.h"
#include "setting.h"
#include "scoreMng.h"
#include "Fade.h"

#include "stageMng.h"

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
	// 何もしない(ステージマネージャーのdrawに任せる)
}

// 処理
bool CSceneMain::update()
{
	const auto& sm = CStageMng::GetPtr();

	// 何かアクションを起こしてシーンが切り替わるとき
	if (sm->isEndStage())
	{
		return true;
	}
	return false;
}

IScene* CSceneMain::NextScene()
{
	// メイン内で使ったオブジェクトを停止させる
	auto& objs = gm()->GetObjects("StageMng Collision Player EnemyMng", ' ');
	for (auto& obj : objs)
		obj->stop();
	// スコアマネージャにスコアを追加
	auto& sm = std::dynamic_pointer_cast<CScoreMng>(gm()->GetObj(typeid(CScoreMng)));
	sm->score(100);
	return new CSceneEnd();
}



#pragma endregion
// CSceneMain methods

