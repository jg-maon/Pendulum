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
	// 必要なオブジェクトの始動
	auto& objs = gm()->GetObjects("ScoreMng");
	for (auto& obj : objs)
		obj->start();

	state_ = CSceneEnd::State::NAME;

	selectChar_ = -1;
	registerAnimTime_ = 0.f;

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
	static float s = 0.f;
	for (int i = 0; i < 26; ++i)
	{
		graph::Draw_GraphicsNC(
			100 + i * 25,
			300,
			0.1f,
			"img_UIalphabet",
			i * 20, 0, 20, 40);
		graph::Draw_GraphicsNC(
			100 + i * 25,
			300,
			0.f,
			"img_UIalphabet",
			i * 20, 0, 20, 40,
			0, 0,
			1.f + std::sinf(s),
			1.f + std::sinf(s),
			static_cast<u_char>(255.f / 2.f + (255.f / 2.f * std::cosf(s) / 2.f)));
	}
	if ((s += math::Calc_DegreeToRad(2.f)) >= math::PI)
		s -= math::PI;
	
	

}

// 処理
bool CSceneEnd::update()
{
	switch (state_)
	{
	case CSceneEnd::State::NAME:
		DecideName();
		break;
	case CSceneEnd::State::REGISTER_ANIM:
		RegisterAnimation();
		break;
	case CSceneEnd::State::ANIM_END:
		auto& sm = std::dynamic_pointer_cast<CScoreMng>(gm()->GetObj(typeid(CScoreMng)));
		sm->RegisterRanking("MAON");
		return true;
	}

	// 何かアクションを起こしてシーンが切り替わるとき
	if (input::CheckPush(input::KEY_BTN0))
	{
		state_ = CSceneEnd::State::ANIM_END;
	}
	return false;
}

IScene* CSceneEnd::NextScene()
{
	return new CSceneTitle();
}


void CSceneEnd::DecideName()
{
	if (input::CheckPush(input::KEY_MOUSE_LBTN))
	{

	}
	if (input::CheckPush(input::KEY_MOUSE_RBTN))
	{
		state_ = CSceneEnd::State::REGISTER_ANIM;
	}
}

void CSceneEnd::RegisterAnimation()
{

}



#pragma endregion
// CSceneEnd methods
