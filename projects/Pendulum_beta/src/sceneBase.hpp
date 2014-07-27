#ifndef DEF_SCENEBASE_HPP
#define DEF_SCENEBASE_HPP

#ifndef DEF_OBJ_H
#include "obj.h"
#endif

#ifndef DEF_FADE_H
#include "fade.h"
#endif



#include <string>

#include <memory>


class IScene : public Base
{
private:

	const float FADE_IN_TIME;			// フェードインにかける時間
	const float FADE_OUT_TIME;			// フェードアウトにかける時間

protected:

	const std::string BACK_RESNAME;		// 背景
	std::string bgmResname_;			// BGM音源

	enum class State		// シーン内部状態
	{
		INNING,		// シーン移り始め(INだとdefineされているので区別)
		MAIN,		// シーンのメイン処理
		OUTING,		// シーン移り終わり(OUTだとdefineされているので区別)
		END,		// シーン終了
	};
	State state_;

	int bgmVolume_;		// BGM音量
	//std::string name_;

protected:

	/*
		@brief	メイン更新処理
		@return	メイン終了か
		@retval	true	メイン終了(フェードアウト開始)
		@retval	false	メイン処理中
	*/
	virtual bool update() = 0;


public:
	/*
		@brief	シーン生成
		@param	[in]	name		名前
		@param	[in]	back		背景画像管理名
		@param	[in]	bgm			BGM管理名
		@param	[in]	fadeInTime	フェードインにかける時間(デフォルト0.3秒)
		@param	[in]	fadeOutTime	フェードアウトにかける時間(デフォルト0.3秒)
	*/
	IScene(const std::string& name, const std::string& back, const std::string& bgm, float fadeInTime = 0.3f, float fadeOutTime = 0.3f) :
		Base(name)
		, BACK_RESNAME(back)
		, bgmResname_(bgm)
		, FADE_IN_TIME(fadeInTime)
		, FADE_OUT_TIME(fadeOutTime)
	{
		//start();
	}
	/*
		@brief	空シーン生成
		@param	[in]	name		名前
	*/
	IScene(const std::string& name) :
		Base(name)
		, BACK_RESNAME("")
		, bgmResname_("")
		, FADE_IN_TIME(0.3f)
		, FADE_OUT_TIME(0.3f)
	{}
	//IScene(const std::string& name):name_(name){}
	
	virtual ~IScene() =0 {}

	virtual void step() override
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
			bgmVolume_ -= static_cast<int>(100.f / FADE_OUT_TIME * system::FrameTime);
			if (bgmVolume_ <= 0)
				bgmVolume_ = 0;
			if (!bgmResname_.empty())
				bgm::DShow_VolumeControl(bgmResname_, bgmVolume_);
			//--------------------------------------
			if (CFade::FadeOut(255.f / FADE_OUT_TIME * system::FrameTime))
			{
				state_ = State::END;
			}
			break;
		case IScene::State::END:
			break;
		}
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
	/*
		@brief		シーンの初期化
		@attention	派生先で必ず__supter::start()を呼ぶこと
		@return		なし
	*/
	virtual void start() = 0
	{
		__super::start();
		state_ = (IScene::State::INNING);
		bgmVolume_ = (100);
		if (!bgmResname_.empty())
		{
			bgm::DShow_SetStartPos(bgmResname_);
			bgm::DShow_VolumeControl(bgmResname_, bgmVolume_);
			bgm::DShow_Play(bgmResname_);
		}
		CFade::StartFadeIn();
	}



	/*
		@brief	シーンを変えるか
		@return	シーンを変えるか
		@retval	true	シーンを変える
		@retval	false	シーンを変えない
	*/
	bool isChangeScene() const
	{
		return state_ == State::END;
	}
	
	/*
		@brief	次のシーンに切り替わる瞬間に呼ばれる
		@return	次のシーン
	*/
	virtual int NextScene() const = 0;
	
};
typedef std::shared_ptr<IScene> ScenePtr;

#endif