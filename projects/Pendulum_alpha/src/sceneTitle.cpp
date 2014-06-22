#ifdef _DEBUG
//#define D_SCALE_TEST	// 拡大、カメラ移動テスト
//#define D_EFFECT_TEST	// エフェクト生成テスト
#endif

#include "sceneTitle.h"
#include "define.h"

#include "setting.h"

#include "sceneStageSelect.h"

#ifdef D_EFFECT_TEST
#include "effectExplosion.h"
#include "effectSlash.h"
#endif

#ifdef _DEBUG

namespace titleDebug
{
	mymath::Vec3f pt;
}
using namespace titleDebug;
#endif
//======================================
// CSceneTitle methods
#pragma region
// コンストラクタ
CSceneTitle::CSceneTitle() :
IScene("img_title", "bgm_title")
, phase_(Phase::TITLE)
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


#pragma endregion
// CSceneTitle methods

