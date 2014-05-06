#include "sceneMain.h"
#include "lib\gplib.h"

#include "Obj.h"			// InsertObject,MargeObjects
#include "Collision.h"		// Collision検出用
#include "player.h"
#include "enemyMng.h"
#include "setting.h"

#include "Fade.h"

#include "sceneEnd.h"		// シーン遷移


//======================================
// CSceneMain methods
#pragma region
// コンストラクタ
CSceneMain::CSceneMain()
{
	InsertObject(ObjPtr(new CCollision(*(shareRes_->stage))));
	InsertObject(ObjPtr(new CEnemyMng()));
	CFade::StartFadeIn();
	DShow_Play(BGM::MAIN);
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
		for(auto& obj : objs)
		{
			obj->step();
		}

		MargeObjects();
	
		shareRes_->Update();

		// 何かアクションを起こしてシーンが切り替わるとき
		if(CheckPush(KEY_BTN0))
		{
			state_ = State::OUTING;
		}
		break;
	case State::OUTING:
		const float end_time = 0.4f;	// 終わり秒
		// BGMフェードアウト
		bgmVolum_ -= static_cast<int>(100.f / end_time * FrameTime);
		DShow_VolumeControl(BGM::MAIN,bgmVolum_);
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
	// ObjectBase描画
	shareRes_->Draw();
	for(auto& obj : objs)
	{
		obj->draw();
	}
}


#pragma endregion
// CSceneMain methods

