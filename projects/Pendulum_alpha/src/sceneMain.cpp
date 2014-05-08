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
CSceneMain::CSceneMain()
{
	InsertObject(ObjPtr(new CCollision()));
	CFade::StartFadeIn();
	bgm::DShow_Play("bgm_main");
}
CSceneMain::~CSceneMain()
{
}

// 処理
IScene* CSceneMain::step()
{
	switch(state_)
	{
	case State::INNING:
		if(CFade::FadeIn(255.f/20.f))
		{
			state_ = State::MAIN;
		}
		break;
	case State::MAIN:
		// 何かアクションを起こしてシーンが切り替わるとき
		if(input::CheckPush(input::KEY_BTN0))
		{
			state_ = State::OUTING;
		}
		break;
	case State::OUTING:
		const float end_time = 0.4f;	// 終わり秒
		// BGMフェードアウト
		bgmVolum_ -= static_cast<int>(100.f / end_time * system::FrameTime);
		bgm::DShow_VolumeControl("bgm_main", bgmVolum_);
		if(bgmVolum_ <= 0)
		{
			bgmVolum_ = 0;
			return new CSceneEnd();
		}
		break;
	}
	

	return this;
}

// 描画
void CSceneMain::draw()
{
	//Draw_FontText(100,200,0.5f,"メイン画面",-1,0);
}


#pragma endregion
// CSceneMain methods

