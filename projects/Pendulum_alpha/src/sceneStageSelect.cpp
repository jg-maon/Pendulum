#include "sceneStageSelect.h"
#include "define.h"

#include "sceneMain.h"
#include "Fade.h"

#include "stageMng.h"

extern CGameManager* gm;

//======================================
// CSceneStageSelect methods
#pragma region CSceneStageSelect methods
// コンストラクタ
CSceneStageSelect::CSceneStageSelect() :
IScene("bgm_stageSelect")
{
	InsertObject(ObjPtr(new CStageMng()));
}
CSceneStageSelect::~CSceneStageSelect()
{
}

// 描画
void CSceneStageSelect::draw()
{
	font::Draw_FontTextNC(100, 200, 0.5f, "ステージセレクト画面", -1, 0);
}


// 処理
bool CSceneStageSelect::update()
{
	// 何かアクションを起こしてシーンが切り替わるとき
	if (input::CheckPush(input::KEY_BTN0))
	{
		CFade::ChangeColor(255, 255, 255);
		return true;
	}
	return false;
}

IScene* CSceneStageSelect::NextScene()
{
	const auto& sm = gm->GetObjects("StageMng");
	if (!sm.empty())
	{
		std::dynamic_pointer_cast<CStageMng>(sm[0])->LoadStage("stage01");
	}
	else
	{
		ObjPtr stage(new CStageMng());
		InsertObject(stage);
		std::dynamic_pointer_cast<CStageMng>(stage)->LoadStage("stage01");
	}
	return new CSceneMain();
}



#pragma endregion CSceneStageSelect methods
// CSceneStageSelect methods

