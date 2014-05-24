#include "sceneEnd.h"
#include "define.h"

#include "sceneTitle.h"

#include "scoreMng.h"

//======================================
// CSceneEnd methods
#pragma region
// コンストラクタ
CSceneEnd::CSceneEnd():
IScene("img_end", "bgm_end")
{
	// スコアマネージャのみコピーを作り避難させておく
	auto scoreManager = std::dynamic_pointer_cast<CScoreMng>(gm()->GetObj(typeid(CScoreMng)));
	scoreManager->score(100);
	// メインゲーム内のオブジェクト消去
	gm()->init();
	// 再度必要なオブジェクトの追加
	InsertObject(ObjPtr(new CScoreMng(*scoreManager)));
}
CSceneEnd::~CSceneEnd()
{
}

// 描画
void CSceneEnd::draw()
{
	font::Draw_FontTextNC(100, 200, 0.5f, "エンド画面", -1, 0);

	// 背景
	//__super::draw();

}

// 処理
bool CSceneEnd::update()
{

	// 何かアクションを起こしてシーンが切り替わるとき
	if (input::CheckPush(input::KEY_BTN0))
	{
		auto& sm = std::dynamic_pointer_cast<CScoreMng>(gm()->GetObj(typeid(CScoreMng)));
		sm->RegisterRanking("MAON");
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
