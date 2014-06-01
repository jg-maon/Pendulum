#ifndef DEF_SCENEBASE_HPP
#define DEF_SCENEBASE_HPP

#ifndef DEF_FADE_H
#include "fade.h"
#endif

#ifndef DEF_DEFINE_H
#include "define.h"
#endif

#ifndef DEF_GAMEMANAGER_H
#include "gameManager.h"
#endif

#include <string>

#include <memory>

class IScene
{
private:
	static CGameManager* gm_;			// ゲームマネージャを扱うためのクラス

	const float FADE_IN_TIME;			// フェードインにかける時間
	const float FADE_OUT_TIME;			// フェードアウトにかける時間

protected:

	const std::string BACK_RESNAME;		// 背景
	const std::string BGM_RESNAME;		// BGM音源

	enum class State		// シーン内部状態
	{
		INNING,		// シーン移り始め(INだとdefineされているので区別)
		MAIN,		// シーンのメイン処理
		OUTING,		// シーン移り終わり(OUTだとdefineされているので区別)
	};
	State state_;

	int bgmVolum_;		// BGM音量
	//std::string name_;

protected:
	/*
		@brief	ゲームマネージャの取得
		@return	ゲームマネージャ
	*/
	CGameManager* gm() const
	{
		return gm_;
	}
	/*
		@brief	ゲームマネージャの設定
		@return	なし
	*/
	void gm(CGameManager* gm)
	{
		gm_ = gm;
	}



	/*
		@brief	メイン更新処理
		@return	メイン終了か
		@retval	true	メイン終了(フェードイン開始)
		@retval	false	メイン処理中
	*/
	virtual bool update() = 0{ return false; }

	/*
		@brief	次のシーンに切り替わる瞬間に呼ばれる
		@return	次のシーン
	*/
	virtual IScene* NextScene() = 0{ return this; }

public:
	/*
		@param	[in]	back		背景画像管理名
		@param	[in]	bgm			BGM管理名
		@param	[in]	fadeInTime	フェードインにかける時間(デフォルト0.3秒)
		@param	[in]	fadeOutTime	フェードアウトにかける時間(デフォルト0.3秒)
	*/
	IScene(const std::string& back, const std::string& bgm, float fadeInTime = 0.3f, float fadeOutTime = 0.3f) :
		BACK_RESNAME(back)
		, BGM_RESNAME(bgm)
		, FADE_IN_TIME(fadeInTime)
		, FADE_OUT_TIME(fadeOutTime)
		, state_(IScene::State::INNING)
		, bgmVolum_(100)
	{
		bgm::DShow_SetStartPos(BGM_RESNAME);
		bgm::DShow_VolumeControl(BGM_RESNAME, bgmVolum_);
		bgm::DShow_Play(BGM_RESNAME);
		CFade::StartFadeIn();
	}
	IScene() :
		BACK_RESNAME("")
		, BGM_RESNAME("")
		, FADE_IN_TIME(0.3f)
		, FADE_OUT_TIME(0.3f)
	{}
	//IScene(const std::string& name):name_(name){}
	
	virtual ~IScene() =0 {}

	virtual IScene* step()
	{
		switch (state_)
		{
		case IScene::State::INNING:
			if (CFade::FadeIn(255.f / FADE_IN_TIME*system::FrameTime))
			{
				state_ = State::MAIN;
			}
			break;
		case IScene::State::MAIN:
			if (update())
			{
				// フェードアウト開始
				CFade::StartFadeOut();
				state_ = State::OUTING;
			}
			break;
		case IScene::State::OUTING:
			//--------------------------------------
			// BGMフェードアウト
			bgmVolum_ -= static_cast<int>(100.f / FADE_OUT_TIME * system::FrameTime);
			if (bgmVolum_ <= 0)
				bgmVolum_ = 0;
			bgm::DShow_VolumeControl(BGM_RESNAME, bgmVolum_);
			//--------------------------------------
			if (CFade::FadeOut(255.f / FADE_OUT_TIME * system::FrameTime))
			{
				return NextScene();
			}
			break;
		}
		return this;
	}

	virtual void draw()
	{
		SIZE size = graph::Draw_GetImageSize2(BACK_RESNAME);
		graph::Draw_GraphicsLeftTopNC(
			0, 0, 1.f,
			BACK_RESNAME,
			0, 0,
			size.cx, size.cy,
			0, nullptr,
			static_cast<float>(system::WINW) / size.cx,
			static_cast<float>(system::WINH) / size.cy);
	}
	
};


class TempScene : public IScene
{
protected:
	virtual bool update() override{ return false; }
	virtual IScene* NextScene() override{ return this; }
public:
	TempScene(CGameManager *game) :
		IScene()
	{
		gm(game);
	}
	~TempScene(){}
};


#endif