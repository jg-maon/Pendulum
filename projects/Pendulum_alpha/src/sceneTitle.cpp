#include "sceneTitle.h"
#include "define.h"

#include "setting.h"

#include "sceneStageSelect.h"

//======================================
// CSceneTitle methods
#pragma region
// コンストラクタ
CSceneTitle::CSceneTitle() :
IScene("bgm_title")
, phase_(Phase::TITLE)
{
	CFade::ChangeColor(-1);
}
CSceneTitle::~CSceneTitle()
{
	
}


// 描画
void CSceneTitle::draw()
{
	//DSound_
	font::Draw_FontTextNC(100, 200, 0.5f, "タイトル画面", -1, 0);
	font::Draw_FontTextNC(100, 250, 0.5f, "ボタン1でシーン切り替え", -1, 0);
}



void CSceneTitle::TitleInit()
{
	phaseTime_ = 0.f;
}


void CSceneTitle::TitleStep()
{
	phaseTime_ += system::FrameTime;
	const float demoTime = 2.f;	// デモプレイに移動するまでの時間
	if (phaseTime_ >= demoTime)
	{
		DemoInit();
		phase_ = Phase::DEMO;	// デモプレイ状態に
	}
}

void CSceneTitle::DemoInit()
{
	demo_.pos.x = 100.f;
}
void CSceneTitle::DemoStep()
{
	demo_.pos.x += 100.f * system::FrameTime;
	// デモプレイ終了
	if (demo_.pos.x >= 200.f)
	{
		TitleInit();
		phase_ = Phase::TITLE;	// タイトル画面へ
	}
}


// 処理
bool CSceneTitle::update()
{
	switch (phase_)
	{
	case Phase::TITLE:
		// タイトル画面表示中
		TitleStep();
		break;
	case Phase::DEMO:
		// デモプレイ表示中
		DemoStep();
		break;
	}

	// 何かアクションを起こしてシーンが切り替わるとき
	if (input::CheckPush(input::KEY_BTN0))
	{
		return true;
	}
	return false;
}

IScene* CSceneTitle::NextScene()
{
	return new CSceneStageSelect();
}


#pragma endregion
// CSceneTitle methods

