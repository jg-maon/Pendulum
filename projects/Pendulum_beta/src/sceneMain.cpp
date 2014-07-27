#include "sceneMain.h"
#include "define.h"

//#include "Collision.h"		// Collision検出用
#include "enemyMng.h"
#include "setting.h"
#include "scoreMng.h"
#include "Fade.h"

#include "stageMng.h"




//======================================
// CSceneMain methods
#pragma region
// コンストラクタ
CSceneMain::CSceneMain() :
IScene("SceneMain")
{
}


CSceneMain::~CSceneMain()
{
}
void CSceneMain::start()
{

	// メイン内で使うオブジェクトを始動させる
	// プレイヤーや敵はStageクラスがやってくれる
	auto& objs = gm()->GetObjects("StageMng Collision ScoreMng", ' ');
	for (auto& obj : objs)
		obj->start();

	// ステージマネージャより、現在ステージのBGM情報を取得
	auto& sm = std::dynamic_pointer_cast<CStageMng>(gm()->GetObj(typeid(CStageMng)));
	bgmResname_ = sm->getStageBGM();

	__super::start();
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

int CSceneMain::NextScene() const
{
	// メイン内で使ったオブジェクトを停止させる
	{
		auto& objs = gm()->GetObjects("StageMng Collision ScoreMng Player EnemyMng", ' ');
		for (auto& obj : objs)
			obj->stop();
	}
	// 遠距離攻撃を消去する
	{
		auto& objs = gm()->GetObjects("Atk_");
		for (auto& obj : objs)
			obj->kill();
	}

	// ボーナス計算
	gm()->scoreMng()->CalcBonusScore();

	return CSceneMng::Scene::END;
}



#pragma endregion
// CSceneMain methods

