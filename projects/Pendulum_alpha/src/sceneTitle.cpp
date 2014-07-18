#ifdef _DEBUG
//#define D_SCALE_TEST	// 拡大、カメラ移動テスト
#define D_EFFECT_TEST	// エフェクト生成テスト
//#define D_PICKUP_TEST	//  ピックアップアイテムテスト
#endif

//=================================================================================
// include 記述
//=================================================================================
#include "sceneTitle.h"
#include "define.h"

#include "setting.h"

#include "sceneDescription.h"

#include "easing.h"

#include "fade.h"


#ifdef D_EFFECT_TEST
#include "effectExplosion.h"
#include "effectSlash.h"
#endif

#ifdef D_PICKUP_TEST
#include "pickupJewely.h"
#endif

#ifdef _DEBUG

namespace titleDebug
{
	mymath::Vec3f pt;

}
using namespace titleDebug;

#endif


//=================================================================================
// 変数定数定義
//=================================================================================
#pragma region 変数定数定義
//ロゴ関連
const float CSceneTitle::TITLELOGO_X = 640.f;		//(WINW/2.f)	
const float CSceneTitle::TITLELOGO_Y = 240.f;		//(WINH/3.f)

//ロゴアニメ関連
const float CSceneTitle::LOGO_A_X = 0.f;
const float CSceneTitle::LOGO_A_Y = 480.f;			//(WINH*2/3.f)
const float CSceneTitle::LOGO_A_SIZE = 3.5f;
const float CSceneTitle::LOGO_A_STARTTIME = 0.5f;

const float CSceneTitle::LOGO_B_X = 1280.f;			//WINW
const float CSceneTitle::LOGO_B_Y = 480.f;			//WINH
const float CSceneTitle::LOGO_B_SIZE = 4.f;
const float CSceneTitle::LOGO_B_STARTTIME = 1.4f;

const float CSceneTitle::LOGO_C_X = 640.f;			//WINW/2
const float CSceneTitle::LOGO_C_Y = 720.f;			//WINH
const float CSceneTitle::LOGO_C_SIZE = 5.f;
const float CSceneTitle::LOGO_C_STARTTIME = 2.3f;

const float CSceneTitle::LOGO_TIME = 0.8f;

//斬撃関連
const float CSceneTitle::SLASH_A_X = 600.f;
const float CSceneTitle::SLASH_A_Y = 360.f;
const float CSceneTitle::SLASH_A_DEG = 30.f;
const float CSceneTitle::SLASH_B_X = 700.f;
const float CSceneTitle::SLASH_B_Y = 360.f;
const float CSceneTitle::SLASH_B_DEG = 120.f;
const float CSceneTitle::SLASH_NEXT_TIME = 0.3f;


//クリック関連
const float CSceneTitle::CLICK_ON = 1.f;
//const int CSceneTitle::CLICK_DEG_ALPHA = 5;
//const int CSceneTitle::CLICK_MIN_ALPHA = 50;

//表示時間
const float CSceneTitle::TITLETIME = 30.f;
const float CSceneTitle::DEMOTIME = 0.f;

#pragma endregion // 変数定数定義
//=================================================================================



//=================================================================================
// CSceneTitle methods
#pragma region CSceneTitle methods

// コンストラクタ
//BGM管理名、フェードイン時間、フェードアウト時間の初期化

CSceneTitle::CSceneTitle() :
IScene("img_title","bgm_title")
, phase_(Phase::TITLE)
, fadeState_(State::MAIN)
//, titleBack_(0.f, 0.f, 1.f, 0.f, 0.f, "img_title", system::WINW, system::WINH, 1.f, 1.f, 0.f, 0, 0)
, titleLogo_(TITLELOGO_X, TITLELOGO_Y, 0.5f, 0.f, 0.5f, "img_logo", 400, 200, 1.f, 1.f, 0.f, 0, 0)
, titleAnim_(LOGO_A_X, LOGO_A_Y, 0.4f, 0.f, 0.f, "img_logo", 400, 200, 1.f, 1.f, 0.f, 0, 1)
, slashAnim_(9, 0.9f)
, titleSlash_(SLASH_A_X, SLASH_A_Y, 0.f, 0.f, 0.f, "img_titleSlash", 240, 360, 2.f, 3.f, 0.f, 0, 0)
{
	camera::SetLookAt(system::WINW / 2.f, system::WINH / 2.f);
	CFade::ChangeColor(0xffffffff);
	TitleInit();

}

CSceneTitle::~CSceneTitle()
{

}



//======================================
// 描画、処理、初期化
//======================================


// 処理
bool CSceneTitle::update()
{
	//タイトルデモ間のフェードアウト、イン
	switch (fadeState_)
	{

	case IScene::State::INNING:
		//フェードイン処理
		if (CFade::FadeIn(255.f / 0.3f * system::FrameTime))
		{
			fadeState_ = State::MAIN;

		}
		break;

	case IScene::State::MAIN:
		switch (phase_)
		{
		case CSceneTitle::Phase::TITLE:

			// タイトル画面表示中
			TitleStep();
			// シーン変化
			if (input::CheckPush(input::KEY_MOUSE_LBTN))
			{
				bgm::DShow_Stop("bgm_title");
				se::DSound_Play("se_enter");
				return true;
			}
			break;
		case CSceneTitle::Phase::DEMO:
			// デモプレイ表示中
			DemoStep();
			break;
		}
		break;

	case IScene::State::OUTING:

		if (CFade::FadeOut(255.f / 0.3f * system::FrameTime))
		{
			fadeState_ = IScene::State::INNING;
			CFade::StartFadeIn();
			if (phase_ == Phase::DEMO)
			{
				phase_ = Phase::TITLE;
				TitleInit();
			}
			else if (phase_ == Phase::TITLE)
			{
				phase_ = Phase::DEMO;
				DemoInit();
			}
		}
		break;
	}



#ifdef D_EFFECT_TEST
	if (input::CheckPress(input::KEY_SHIFT))
	{
		if (input::CheckPush(input::KEY_BTN0))
			gm()->AddObject(ObjPtr(new CEffectSlash(gm()->GetCursorPos(), math::GetRandom(0.f, 360.f))));
	}
	else
	{
		if (input::CheckPush(input::KEY_BTN0))
			gm()->AddObject(ObjPtr(new CEffectSlash(gm()->GetCursorPos())));
		if (input::CheckPush(input::KEY_BTN1))
			gm()->AddObject(ObjPtr(new CEffectExplosion(gm()->GetCursorPos())));
	}
#endif



	return false;
}

IScene* CSceneTitle::NextScene()
{
	return new CSceneDescription();
}



//-------------------------
//Draw
//-------------------------

// 描画
void CSceneTitle::draw()
{

	//-------------------------
	//Title
	//-------------------------

	if (phase_ == Phase::TITLE)
	{

		//背景
		__super::draw();

#ifdef D_SCALE_TEST
		auto& point = camera::GetLookAt();
		graph::Draw_Line(point.x - 3, point.y, point.x + 3, point.y, 0.f, -1, 1);
		graph::Draw_Line(point.x, point.y - 3, point.x, point.y + 3, 0.f, -1, 1);
		std::stringstream ss;
		ss << "(" << point.x << ", " << point.y << ")";
		font::Draw_TextXY(point.x, point.y, ss.str(), -1);


		graph::Draw_GraphicsLeftTop(0, 0, 0.8f,
			BACK_RESNAME,
			0, 0, 1200, 800);
		graph::Draw_GraphicsLeftTop(0, 0, 9.f,
			BACK_RESNAME,
			0, 0, 1200, 800, 0, 0, 2.f, 2.f, 128);
		graph::Draw_GraphicsLeftTop(0, 0, 0.9f,
			BACK_RESNAME,
			0, 0, 1200, 800, 0, 0, 5.f, 5.f, 128);
		graph::Draw_GraphicsLeftTop(0, 0, 1.f,
			BACK_RESNAME,
			0, 0, 1200, 800, 0, 0, 10.f, 6.2f, 128);
#endif

		//titleBack_.draw(charabase::CharBase::LeftTop);

		//ロゴ
		if (titleLogo_.CheckUse())
			titleLogo_.draw(charabase::CharBase::Center);

		//Pleas Click
		if (clickPhase_ != ClickPhase::HIDDEN)
			font::Draw_FontTextNC(system::WINW / 2 - 80, system::WINH / 3 * 2, 0.5f,
			"Please Click!", ARGB(clickAlpha_, 0, 0, 0), setting::GetFontID("font_MSGOSICK20"));

		//玉アニメーション
		if (titleAnim_.CheckUse())
			titleAnim_.draw(charabase::CharBase::Center);


		//剣アニメーション
		if (titleSlash_.CheckUse())
			titleSlash_.draw(charabase::CharBase::Center);

		//debug
		//debug::Dbg_DisplayToMessage(100, 100, "%f", titleAnim_.scale.x);
		debug::SToM(100, 100, "%f", titleAnim_.scale.x);
	}

	//-------------------------
	//Demo
	//-------------------------
	else if (phase_ == Phase::DEMO)
	{

	}

}


//-------------------------
//Title
//-------------------------
//タイトル初期化
void CSceneTitle::TitleInit()
{
	//アニメーション初期化
	titleLogo_.src(0, 0, 0);
	titleAnim_.pos(LOGO_A_X, LOGO_A_Y);
	titleAnim_.add(0.f);
	titleAnim_.src(0, 1, 0);
	titleAnim_.alpha = 255;
	clickAlpha_ = 255;

	titleSlash_.pos(SLASH_A_X, SLASH_A_Y, 0.f);
	titleSlash_.angle = SLASH_A_DEG;

	//フェーズ初期化
	logoPhase_ = LogoPhase::RED;
	animPhase_ = AnimPhase::LOGO;
	clickPhase_ = ClickPhase::HIDDEN;
	slashPhase_ = SlashPhase::HIDDEN1;
	flashPhase_ = FlashPhase::FADEOUT;

	//タイマ初期化
	phaseTime_ = 0.f;
	offTime_ = 0.f;
	nextTime_ = 0.f;

	//初期化フラグ

	sePlaying_ = true;
	flashFlag_ = true;

	//可視不可視
	//titleBack_.SetUse(true);
	titleLogo_.SetUse(true);
	titleAnim_.SetUse(false);
	titleSlash_.SetUse(false);



}

//タイトル処理
void CSceneTitle::TitleStep()
{
	phaseTime_ += system::FrameTime;

	switch (animPhase_)
	{
	case CSceneTitle::AnimPhase::LOGO:
		LogoAnime();
		break;

	case CSceneTitle::AnimPhase::SLASH:
		SlashAnime();
		break;

	case CSceneTitle::AnimPhase::FLASH:
		TitleFlash();
		break;

	case CSceneTitle::AnimPhase::WAIT:
		PleaseClick();
		break;

	default:

		break;
	}


	//const float demoTime = 20.f;
	if (phaseTime_ >= TITLETIME)
	{
		ChangePhase();
	}


#ifdef D_PICKUP_TEST

	if (input::CheckPush(input::KEY_BTN0))
	{
		gm()->AddObject(ObjPtr(new CPickupJewely(gm()->GetCursorPos())));
	}

#endif



#ifdef D_SCALE_TEST
	using namespace input;
	struct
	{
		input::CODE_KEY code;
		POINT move;
	} a[4] =
	{
		{ KEY_UP, 0, -3 },
		{ KEY_DOWN, 0, +3 },
		{ KEY_LEFT, -3, 0 },
		{ KEY_RIGHT, +3, 0 },
	};
	for (auto& aa : a)
	{
		if (input::CheckPress(aa.code))
		{
			pt.x += aa.move.x;
			pt.y += aa.move.y;
		}
	}
	camera::SetLookAt(pt.x, pt.y);
#endif


}


//-------------------------
//Demo
//-------------------------

//デモ初期化
void CSceneTitle::DemoInit()
{
	phaseTime_ = 0.f;

}

//デモ処理
void CSceneTitle::DemoStep()
{
	phaseTime_ += system::FrameTime;



	// デモプレイ終了
	if (input::CheckPush(input::KEY_MOUSE_LBTN))
	{
		phaseTime_ = DEMOTIME;
	}


	if (phaseTime_ >= DEMOTIME)
	{
		ChangePhase();
	}
}





//======================================
//関数
//======================================

//ロゴアニメーション
void CSceneTitle::LogoAnime()
{
	switch (logoPhase_)
	{
	case CSceneTitle::LogoPhase::RED:
		if (phaseTime_ <= LOGO_A_STARTTIME)
			return;

		if (sePlaying_)
		{
			se::DSound_Play("se_drain");
			sePlaying_ = false;
		}

		titleAnim_.SetUse(true);			//可視

		titleAnim_.pos.x = Easing::QuadInOut(phaseTime_ - LOGO_A_STARTTIME, LOGO_A_X, (TITLELOGO_X - LOGO_A_X), LOGO_TIME);
		titleAnim_.pos.y = Easing::QuadInOut(phaseTime_ - LOGO_A_STARTTIME, LOGO_A_Y, (TITLELOGO_Y - LOGO_A_Y), LOGO_TIME);
		titleAnim_.alpha -= (200.f / LOGO_TIME)*system::FrameTime;		//透明化

		titleAnim_.scale.x = titleAnim_.scale.y = LogoSize(LOGO_A_SIZE, LOGO_A_Y);

		//フェーズ移動
		if (titleAnim_.pos.y <= TITLELOGO_Y)
		{

			titleAnim_.SetUse(false);
			titleAnim_.pos(LOGO_B_X, LOGO_B_Y);
			titleAnim_.src.y = 2;
			titleAnim_.alpha = 255;
			titleAnim_.scale(1.f, 1.f, 1.f);

			sePlaying_ = true;
			logoPhase_ = LogoPhase::GREEN;

		}
		break;

	case CSceneTitle::LogoPhase::GREEN:

		if (phaseTime_ <= LOGO_B_STARTTIME)
			return;

		if (sePlaying_)
		{
			se::DSound_Play("se_drain");
			sePlaying_ = false;
		}

		titleAnim_.SetUse(true);
		titleAnim_.pos.x = Easing::QuadInOut(phaseTime_ - LOGO_B_STARTTIME, LOGO_B_X, (TITLELOGO_X - LOGO_B_X), LOGO_TIME);
		titleAnim_.pos.y = Easing::QuadInOut(phaseTime_ - LOGO_B_STARTTIME, LOGO_B_Y, (TITLELOGO_Y - LOGO_B_Y), LOGO_TIME);
		titleAnim_.alpha -= (200.f / LOGO_TIME)*system::FrameTime;

		titleAnim_.scale.x = titleAnim_.scale.y = LogoSize(LOGO_B_SIZE, LOGO_B_Y);

		if (titleAnim_.pos.y <= TITLELOGO_Y)
		{
			titleAnim_.SetUse(false);
			titleAnim_.pos(LOGO_C_X, LOGO_C_Y);
			titleAnim_.alpha = 255;
			titleAnim_.src.y = 3;
			titleAnim_.scale(1.f, 1.f, 1.f);

			sePlaying_ = true;
			logoPhase_ = LogoPhase::BLUE;

		}
		break;

	case CSceneTitle::LogoPhase::BLUE:

		if (phaseTime_ <= LOGO_C_STARTTIME)
			return;

		if (sePlaying_)
		{
			se::DSound_Play("se_drain");
			sePlaying_ = false;
		}

		titleAnim_.SetUse(true);
		titleAnim_.pos.y = Easing::SineInOut(phaseTime_ - LOGO_C_STARTTIME, LOGO_C_Y, (TITLELOGO_Y - LOGO_C_Y), LOGO_TIME);
		titleAnim_.alpha -= (200 / LOGO_TIME)*system::FrameTime;

		titleAnim_.scale.x = titleAnim_.scale.y = LogoSize(LOGO_C_SIZE, LOGO_C_Y);

		if (titleAnim_.pos.y <= TITLELOGO_Y)
		{
			titleAnim_.SetUse(false);
			animPhase_ = AnimPhase::SLASH;

		}
		break;

	}

	//titleAnim_.Move();

}

//ロゴサイズ変更
float CSceneTitle::LogoSize(float firstsize, float firsty)
{

	//------------------------------------------------------------
	// 数式
	//
	// 現在サイズ = 最終サイズ値 + サイズ変化量* | 移動量/距離 |
	// NowSize = 1.0f + (⊿Size) * |(X0- X)/ (X0-Xf)|
	//
	//------------------------------------------------------------

	return firstsize - (firstsize - 1.0f) * fabsf((firsty - titleAnim_.pos.y) / (firsty - TITLELOGO_Y));

}

//プリーズクリックアニメ
void CSceneTitle::PleaseClick()
{
	//クリック表示アニメ
	switch (clickPhase_)
	{
	case CSceneTitle::ClickPhase::HIDDEN:

		clickPhase_ = ClickPhase::SHOW;
		break;

	case CSceneTitle::ClickPhase::SHOW:
		//オフになる時間
		offTime_ += system::FrameTime;

		//フェード判定
		if (offTime_ > CLICK_ON)
		{
			offTime_ = 0.f;
			clickPhase_ = ClickPhase::FADEOUT;
		}

		break;

	case CSceneTitle::ClickPhase::FADEOUT:

		clickAlpha_ -= CLICK_DEG_ALPHA;

		if (clickAlpha_ <= CLICK_MIN_ALPHA)
		{
			clickAlpha_ = CLICK_MIN_ALPHA;
			clickPhase_ = ClickPhase::FADEIN;
		}

		break;


	case CSceneTitle::ClickPhase::FADEIN:

		clickAlpha_ += CLICK_DEG_ALPHA;
		if (clickAlpha_ >= 255)
		{
			clickAlpha_ = 255;
			clickPhase_ = ClickPhase::SHOW;
		}

		break;


	default:
		break;
	}


}

//タイトル、デモ移動時のフェードアウト
void CSceneTitle::ChangePhase()
{
	fadeState_ = State::OUTING;
	CFade::StartFadeOut();
}

//斬撃アニメーション
void CSceneTitle::SlashAnime()
{
	//斬撃アニメ
	switch (slashPhase_)
	{
		//初期待機
	case CSceneTitle::SlashPhase::HIDDEN1:

		slashPhase_ = SlashPhase::SLASH1;
		sePlaying_ = true;
		break;

		//第一斬撃
	case CSceneTitle::SlashPhase::SLASH1:

		//可視化
		titleSlash_.SetUse(true);
		if (sePlaying_)
		{
			se::DSound_Play("se_slash");
			sePlaying_ = false;
		}
		if (slashAnim_.step())
		{
			titleSlash_.SetUse(false);
			slashPhase_ = SlashPhase::HIDDEN2;
		}
		titleSlash_.src.x = slashAnim_.no;
		break;

		//中間停止
	case CSceneTitle::SlashPhase::HIDDEN2:

		nextTime_ += system::FrameTime;

		//時間で次のフェーズへ
		if (nextTime_ > SLASH_NEXT_TIME)
		{
			//第二斬撃の初期化
			sePlaying_ = true;
			titleSlash_.pos(SLASH_B_X, SLASH_B_Y, 0.f);
			titleSlash_.angle = SLASH_B_DEG;
			slashPhase_ = SlashPhase::SLASH2;
		}

		break;

		//第二斬撃
	case CSceneTitle::SlashPhase::SLASH2:

		//可視化
		titleSlash_.SetUse(true);

		if (sePlaying_)
		{
			se::DSound_Play("se_slash");
			sePlaying_ = false;
		}

		if (slashAnim_.step())
		{
			titleSlash_.SetUse(false);
			animPhase_ = AnimPhase::FLASH;
		}
		titleSlash_.src.x = slashAnim_.no;

		break;

	}
}

//フラッシュアニメーション
void CSceneTitle::TitleFlash()
{
	switch (flashPhase_)
	{
	case CSceneTitle::FlashPhase::FADEOUT:
		if (flashFlag_)
		{
			CFade::StartFadeOut();
			flashFlag_ = false;
		}
		if (CFade::FadeOut(255.f / 0.7f * system::FrameTime))
		{
			flashFlag_ = true;
			flashPhase_ = FlashPhase::FADEIN;
		}

		break;
	case CSceneTitle::FlashPhase::FADEIN:

		if (flashFlag_)
		{
			CFade::StartFadeIn();
			flashFlag_ = false;
		}
		if (CFade::FadeIn(255.f / 0.5f * system::FrameTime))
		{
			animPhase_ = AnimPhase::WAIT;

		}

		break;

	default:
		break;
	}

}



#pragma endregion	// CSceneTitle methods
//=================================================================================


/*
//======================================
// CSceneTitle methods
#pragma region CSceneTitle methods
// コンストラクタ
CSceneTitle::CSceneTitle() :
IScene("img_title", "bgm_title")
, Phase_(Phase::TITLE)
{
	CFade::ChangeColor(-1);
#ifdef D_SCALE_TEST
	pt = camera::GetLookAt();
#endif
}
CSceneTitle::~CSceneTitle()
{
	
}


// 描画
void CSceneTitle::draw()
{
	font::Draw_FontTextNC(100, 200, 0.5f, "タイトル画面", -1, 0);
	font::Draw_FontTextNC(100, 250, 0.5f, "ボタン1でシーン切り替え", -1, 0);
	// 背景
	__super::draw();
#ifdef D_SCALE_TEST
	auto& point = camera::GetLookAt();
	graph::Draw_Line(point.x - 3, point.y, point.x + 3, point.y, 0.f, -1, 1);
	graph::Draw_Line(point.x, point.y - 3, point.x, point.y + 3, 0.f, -1, 1);
	std::stringstream ss;
	ss << "(" << point.x << ", " << point.y << ")";
	font::Draw_TextXY(point.x, point.y, ss.str(), -1);

	
	graph::Draw_GraphicsLeftTop(0, 0, 0.8f,
		BACK_RESNAME,
		0, 0, 1200, 800);
	graph::Draw_GraphicsLeftTop(0, 0, 9.f,
		BACK_RESNAME,
		0, 0, 1200, 800, 0, 0, 2.f, 2.f, 128);
	graph::Draw_GraphicsLeftTop(0, 0, 0.9f,
		BACK_RESNAME,
		0, 0, 1200, 800, 0, 0, 5.f, 5.f, 128);
	graph::Draw_GraphicsLeftTop(0, 0, 1.f,
		BACK_RESNAME,
		0, 0, 1200, 800,0,0,10.f,6.2f,128);
#endif
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
		Phase_ = Phase::DEMO;	// デモプレイ状態に
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
		Phase_ = Phase::TITLE;	// タイトル画面へ
	}
}

// 処理
bool CSceneTitle::update()
{
	switch (Phase_)
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

#ifdef D_EFFECT_TEST
	if (input::CheckPush(input::KEY_MOUSE_LBTN))
		gm()->AddObject(ObjPtr(new CEffectSlash(gm()->GetCursorPos())));
	if (input::CheckPush(input::KEY_MOUSE_RBTN))
		gm()->AddObject(ObjPtr(new CEffectExplosion(gm()->GetCursorPos())));
#endif

	// 何かアクションを起こしてシーンが切り替わるとき
	if (input::CheckPush(input::KEY_BTN0))
	{
		return true;
	}

#ifdef D_SCALE_TEST
	using namespace input;
	struct
	{
		input::CODE_KEY code;
		POINT move;
	} a[4] = 
	{
		{ KEY_UP, 0, -3 },
		{ KEY_DOWN, 0, +3 },
		{ KEY_LEFT, -3, 0 },
		{ KEY_RIGHT, +3, 0 },
	};
	for (auto& aa : a)
	{
		if (input::CheckPress(aa.code))
		{
			pt.x += aa.move.x;
			pt.y += aa.move.y;
		}
	}
	camera::SetLookAt(pt.x, pt.y);
#endif

	return false;
}

IScene* CSceneTitle::NextScene()
{
	return new CSceneStageSelect();
}


#pragma endregion	// CSceneTitle methods

//*/