#include "sceneStageSelect.h"
#include "lib\gplib.h"

#include "sceneMain.h"
#include "Fade.h"

//======================================
// CSceneStageSelect methods
#pragma region CSceneStageSelect methods
// コンストラクタ
CSceneStageSelect::CSceneStageSelect()
{
	state_ = State::MAIN;
}
CSceneStageSelect::~CSceneStageSelect()
{
}



// 処理
IScene* CSceneStageSelect::step()
{
	if(state_ == State::INNING)
	{
		if(!CFade::FadeIn(255.f/20.f))
		{
			return this;
		}
		else
		{
			state_ = State::MAIN;
		}
	}
	else if(state_ == State::OUTING)
	{
		if(CFade::FadeOut(255.f/20.f))
		{
			shareRes_->stage->LoadStage("stage01");
			return new CSceneMain();
		}
		/*
		const float end_time = 0.4f;	// 終わり秒
		// BGMフェードアウト
		bgmVolum_ -= static_cast<int>(100.f / end_time * FrameTime);
		DShow_VolumeControl(BGM::MAIN,bgmVolum_);
		if(bgmVolum_ <= 0)
		{
			bgmVolum_ = 0;
			return new CSceneEnd();
		}
		*/
	}
	else
	{
		// 何かアクションを起こしてシーンが切り替わるとき
		if(CheckPush(KEY_BTN0))
		{
			state_ = State::OUTING;
			if(!CFade::IsFadeOuting())
			{
				CFade::ChangeColor(255,255,255);
				CFade::StartFadeOut();
			}
		}
	}


	return this;
}

// 描画
void CSceneStageSelect::draw()
{
	Draw_FontTextNC(100,200,0.5f,"ステージセレクト画面",-1,0);
}


#pragma endregion CSceneStageSelect methods
// CSceneStageSelect methods

