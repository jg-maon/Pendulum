#include "sceneEnd.h"
#include "define.h"


#include "scoreMng.h"

#include "effectMoveStar.h"


#include "easing.h"

//======================================
// CSceneEnd methods

#pragma region


#if 0
const float CSceneEnd::BONUSPOINT_X = 1000.f;
const float CSceneEnd::BONUSINFO_X = 800.f;
const float CSceneEnd::BONUSPOINT_WIDTH = 50.f;


// コンストラクタ
CSceneEnd::CSceneEnd():
IScene("img_end", "bgm_end")
//, sceneEndBg_(0.f, 0.f, 1.f, 0.f, 0.f, "img_endbg", system::WINW, system::WINH, 1.f, 1.f, 0.f, 0, 0)
{

	//sceneEndBg_.SetUse(true);

	phase_ = CSceneEnd::Phase::CLEAR;

	PointDataInit();

	selectChar_ = -1;
	registerAnimTime_ = 0.f;

}
CSceneEnd::~CSceneEnd()
{
}

// 仮ポイント入力
// 外部からポイントを渡すようにすること
void CSceneEnd::PointDataInit()
{
	const auto& bonusScores = gm()->scoreMng()->getBonusScore();

	//仮ポイント
	int point = 100;

	int sumpoint = 0;
	float distance = 0.f;

	for (auto& bonusPoint : bonusPoints)
	{
		//表示座標
		bonusPoint.cb.pos.y = 100.f + distance;
		distance += BONUSPOINT_WIDTH;
		
		//ポイント（仮）
		bonusPoint.point = point;
		sumpoint += bonusPoint.point;
		point *= 2;
	}

	// 仮のボーナスポイント詳細
	// 外部ファイル参照などを予定
	bonusPoints[0].bonusInfo = "bonus1";
	bonusPoints[1].bonusInfo = "bonus2";
	bonusPoints[2].bonusInfo = "bonus3";
	bonusPoints[3].bonusInfo = "bonus4";
	bonusPoints[4].bonusInfo = "bonus5";


	//合計のデータ
	resultPoint_.cb.pos(BONUSPOINT_X, 600, 0.f);
	resultPoint_.point = sumpoint;

}

void CSceneEnd::PointDataDraw(){

	for (auto & bonusPoint : bonusPoints)
	{
		font::Draw_FontTextNC(
			(int)BONUSINFO_X, (int)bonusPoint.cb.pos.y,
			0.f, bonusPoint.bonusInfo,
			ARGB(255,200,200,100), 0);

		font::Draw_FontTextNC(
			(int)BONUSPOINT_X, (int)bonusPoint.cb.pos.y,
			0.f, std::to_string(bonusPoint.point),
			ARGB(255, 200, 200, 100), 0);

	}

	font::Draw_FontTextNC(
		(int)resultPoint_.cb.pos.x, (int)resultPoint_.cb.pos.y,
		0.f, std::to_string(resultPoint_.point),
		ARGB(255, 200, 200, 100), 0);

}

void CSceneEnd::ResultDraw()
{
	font::Draw_FontTextNC(100, 100, 0.5f, "右ボタンで終了", -1, 0);

	PointDataDraw();
	
}

void CSceneEnd::NameInputDraw()
{

	font::Draw_FontTextNC(100, 700, 0.5f, "右ボタンで完了", -1, 0);


	// 背景
	//__super::draw();
	static float s = 0.f;
	for (int i = 0; i < 26; ++i)
	{
		graph::Draw_GraphicsNC(
			100 + i * 25,
			300,
			0.1f,
			"img_alphabet",
			i * 20, 0, 20, 40);
		graph::Draw_GraphicsNC(
			100 + i * 25,
			300,
			0.f,
			"img_alphabet",
			i * 20, 0, 20, 40,
			0, 0,
			1.f + std::sinf(s),
			1.f + std::sinf(s),
			static_cast<u_char>(255.f / 2.f + (255.f / 2.f * std::cosf(s) / 2.f)));
	}

	if ((s += math::Calc_DegreeToRad(2.f)) >= math::PI)
		s -= math::PI;
}

void CSceneEnd::HiscoreDispDraw()
{
	font::Draw_FontTextNC(100, 200, 0.5f, "ハイスコア表示", -1, 0);
}

bool CSceneEnd::InputNextKey(){

	return input::CheckPush(input::KEY_MOUSE_RBTN);

}


// 描画
void CSceneEnd::draw()
{

	__super::draw();
	//sceneEndBg_.drawNC(charabase::CharBase::MODE::LeftTop);

	switch (state_)
	{
	case CSceneEnd::State::RESULT:
		ResultDraw();
		break;

	}

}

// 処理
bool CSceneEnd::update()
{
	switch (state_)
	{
	case CSceneEnd::State::RESULT:

		if (InputNextKey())				//かつ、スコアがハイスコアの時
		{
			state_ = State::END_EXIT;
		}

		break;

	case CSceneEnd::State::END_EXIT:

		return true;

		break;
	}


	//auto& sm = std::dynamic_pointer_cast<CScoreMng>(gm()->GetObj(typeid(CScoreMng)));
	//sm->RegisterRanking("MAON");


	/*switch (state_)
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
	}*/

	
	return false;
}

IScene* CSceneEnd::NextScene()
{
	return new CSceneNameInput();
}


void CSceneEnd::DecideName()
{
	/*
	if (input::CheckPush(input::KEY_MOUSE_LBTN))
	{

	}
	if (input::CheckPush(input::KEY_MOUSE_RBTN))
	{
		state_ = CSceneEnd::State::REGISTER_ANIM;
	}*/
}

void CSceneEnd::RegisterAnimation()
{

}
#endif


CSceneEnd::CSceneEnd() :
IScene("SceneEnd", "img_end", "bgm_end")
, clear_(charabase::BaseData("img_stageClear", -1, -1))
, totalScoreObj_(charabase::BaseData("img_totalScore", -1, -1))
, scoreObj_(charabase::BaseData("img_UInumber", 32, 64))
{
	//---------------------------------------
	// ステージクリア画像
	charabase::BaseData bd(clear_.resname, clear_.size);
	charabase::BaseData::ImageSize(bd);
	clear_.SetImgData(bd);

	//---------------------------------------
	// トータルスコア画像
	bd.resname = totalScoreObj_.resname;
	charabase::BaseData::ImageSize(bd);
	totalScoreObj_.SetImgData(bd);
	//---------------------------------------
	// ランクイン画像
	rankinObjs_.resize(std::string("RANKIN").length());
	int i = 0;
	for (auto& obj : rankinObjs_)
	{
		obj.cb.size(48, 48);
		obj.cb.src(0, i++);
		obj.cb.resname = "img_rankin";	
	}
}


CSceneEnd::~CSceneEnd()
{}

void CSceneEnd::start()
{
	__super::start();

	camera::SetScale(1.f);

	phase_ = (CSceneEnd::Phase::CLEAR);
	anim_.set(8, 0.3f);
	animTime_ = (0.f);
	createInterval_ = (0.f);
	dir_ = (1.f);
	totalScore_ = (0);
	dispScore_ = (0);
	clear_.pos.x = static_cast<float>(system::WINW) / 2.f;
	clear_.pos.y = 100.f;
	clear_.pos.z = 0.3f;
	clear_.SetUse(false);

	InitBonus();

	totalScoreObj_.pos.x = (system::WINW - totalScoreObj_.size.x) / 2.f;
	if (bonusObjs_.empty())
		totalScoreObj_.pos.y = TOTAL_Y / 10.f;
	else
		totalScoreObj_.pos.y = bonusObjs_.back().cb.pos.y - BONUS_MOVE_Y / 10.f + bonusObjs_.back().cb.GetSize().y + 10.f;
	totalScoreObj_.pos.z = 0.5f;

	// ランクインオブジェクト初期化
	{
		int i = 0;
		for (auto& obj : rankinObjs_)
		{
			obj.startPos = obj.cb.pos = totalScoreObj_.pos;
			obj.startPos.x = (obj.cb.pos.x += i * obj.cb.size.x + 5.f);
			obj.startPos.y = (obj.cb.pos.y += totalScoreObj_.GetSize().y + RANKIN_MARGIN / 10.f);
			obj.cb.SetUse(false);
			// 一定秒毎に遅く
			obj.time = i * -0.1f;
			++i;
		}
	}
}

bool CSceneEnd::update()
{
	switch (phase_)
	{
	case CSceneEnd::Phase::CLEAR:
		{
			animTime_ += system::FrameTime;
			if (anim_.step())
			{
				anim_.stop();
				animTime_ = 0.f;
			}
			else if (animTime_ > 4.f)
			{
				phase_ = CSceneEnd::Phase::SCORE;
				clear_.SetUse(false);
			}
			else if (animTime_ <= 4.f)
			{
				createInterval_ += system::FrameTime;
				if (createInterval_ > 1.f)
				{
					createInterval_ = 0.f;
					mymath::Vec3f p = camera::GetCameraPos();
					mymath::Vec3f pos = clear_.pos + p;
					pos.x += clear_.HalfWidth() * dir_;
					float ady = math::GetRandom(100.f, 400.f);
					float t = math::GetRandom(0.3f, 1.3f);
					gm()->AddObject(ObjPtr(new CEffectMoveStar(pos, mymath::Vec3f(0.f, ady), t)));
					dir_ = -dir_;
				}

			}
			if (anim_.no == 3)
			{
				clear_.SetUse(true);
			}
		}
		break;
	case CSceneEnd::Phase::SCORE:
		{
			const float moveTime = 1.f;
			for (size_t i = 0; i < bonusObjs_.size(); ++i)
			{
				auto& obj = bonusObjs_[i];
				if (obj.isStarted)
				{
					obj.cb.pos.x = Easing::CubicOut(obj.time, obj.startPos.x, -(system::WINW - BONUS_X / 10.f), moveTime);
					obj.cb.pos.y = Easing::Linear(obj.time, obj.startPos.y, -BONUS_MOVE_Y, moveTime);
					obj.time += system::ONEFRAME_TIME;
					// 揃ったら
					if (obj.time > moveTime)
					{
						// トータルスコア加算
						totalScore_ += obj.bonusInfo.score;
						obj.bonusInfo.score = 0;
						// 次のオブジェクトを整列させる
						if (i + 1 < bonusObjs_.size())
							bonusObjs_[i + 1].isStarted = true;
					}
				}
			}
			if (dispScore_ < totalScore_)
			{
				const int frameAdd = 1000;	// 1フレームあたりの加算量

				dispScore_ += min(frameAdd, totalScore_ - dispScore_);
			}
			if (input::CheckPush(input::KEY_MOUSE_LBTN))
			{
				// 全ボーナスアニメーションが終了している場合
				// スコア加算アニメーションをスキップ
				if (!bonusObjs_.empty() && bonusObjs_.back().time > moveTime)
					dispScore_ = totalScore_;
			}
			// 表示スコアがトータルスコアに追いつき、かつ最終要素が揃ったら
			if (dispScore_ == totalScore_)
			{
				// 次のフェーズへ
				if (bonusObjs_.empty())
				{
					animTime_ += system::ONEFRAME_TIME;
					if (animTime_ > 1.f)
						phase_ = CSceneEnd::Phase::RANKING;					
				}
				else if(bonusObjs_.back().time > moveTime)
				{
					animTime_ += system::ONEFRAME_TIME;
					if (animTime_ > 1.f)
						phase_ = CSceneEnd::Phase::RANKING;
				}
			}
			
		}
		break;
	case CSceneEnd::Phase::RANKING:
		{
			bool rankin = gm()->scoreMng()->isRankin(totalScore_);
			if (rankin)
			{
				const float animTime = 1.25f;
				// ランクイン中のみランキングアニメーション
				for (auto& obj : rankinObjs_)
				{
					float rad = Easing::QuartOut(obj.time, 0.f, mymath::PI, animTime);
					float c = std::cosf(rad);		// 1->0->1
					float s = std::sinf(rad);		// 0->1->0
					obj.cb.pos.y = obj.startPos.y + -s*RANKIN_MOVE_Y / 10.f;
					obj.cb.scale = 1.f + 0.5f * s;
					obj.cb.SetUse(true);
					obj.time += system::ONEFRAME_TIME;
				}
			}
			// ボタン押して次のフェーズへ
			if (input::CheckPush(input::KEY_MOUSE_LBTN))
			{
				// ランクイン状況に応じてフェーズ切り替え
				phase_ = (rankin) ? CSceneEnd::Phase::REGISTER : CSceneEnd::Phase::TITLE;
			}
		}
		break;
	case CSceneEnd::Phase::REGISTER:
		break;
	case CSceneEnd::Phase::TITLE:
		break;
	}
	return (phase_ == CSceneEnd::Phase::TITLE
		|| phase_ == CSceneEnd::Phase::REGISTER);
}

void CSceneEnd::draw()
{
	__super::draw();
	switch (phase_)
	{
	case CSceneEnd::Phase::CLEAR:
		if (!anim_.isStoped())
		{
			int y = anim_.max - anim_.no;
			graph::Draw_GraphicsNC(
				clear_.pos.x,
				clear_.pos.y,
				0.1f,
				"img_effectWind",
				0, 240*y,
				640, 240,0,0,
				system::WINW/640.f);
		}
		if (clear_.CheckUse())
			clear_.drawNC();
		break;
	case CSceneEnd::Phase::RANKING:
		for (auto& obj : rankinObjs_)
		{
			if (obj.cb.CheckUse())
				obj.cb.drawNC();
		}
		// スルー
	case CSceneEnd::Phase::SCORE:
		{
			for (auto& obj : bonusObjs_)
				obj.cb.drawNC(charabase::CharBase::LeftTop);
			totalScoreObj_.drawNC(charabase::CharBase::LeftTop);
			auto numObjs = common::GetNumberImageObjects(
				dispScore_,
				scoreObj_,
				common::LEFT);
			for (auto& nobj : numObjs)
			{
				nobj.pos.x += totalScoreObj_.GetSize().x;
				nobj.pos.y = totalScoreObj_.pos.y + totalScoreObj_.GetSize().y + TOTALSCORE_MARGIN / 10.f;
				nobj.drawNC(charabase::CharBase::MODE::LeftTop);
			}
		}
		break;

	case CSceneEnd::Phase::REGISTER:
		break;
	case CSceneEnd::Phase::TITLE:
		break;
	}
}


void CSceneEnd::InitBonus()
{
	totalScore_ = gm()->scoreMng()->score();
	const auto& bonus = gm()->scoreMng()->getBonusScore();
	
	bonusObjs_.clear();
	bonusObjs_.reserve(bonus.size());
	mymath::Vec3f drawPos(static_cast<float>(system::WINW), BONUS_Y / 10.f + BONUS_MOVE_Y / 10.f);
	bool f = true;		// スタートフラグ　最初のみtrue
	float t = -0.2f;	// 最初のみ少し遅れてスタート
	for (const auto& b : bonus)
	{
		BonusObject tmp;
		tmp.bonusInfo = b;
		tmp.startPos = tmp.cb.pos = drawPos;
		// 画像名をボーナス情報から取得
		charabase::BaseData bd(
			std::string("img_bonus") + common::StrReplace(b.label, " Bonus", ""),
			-1, -1);
		bd.ImageSize(bd);		// 画像サイズ計算
		tmp.cb.SetImgData(bd);	// 画像適用
		tmp.time = t;
		tmp.isStarted = f;
		bonusObjs_.push_back(tmp);

		drawPos.y += bd.size.y + BONUS_MARGIN / 10.f;
		t = 0.f;
		f = false;
	}
}


int CSceneEnd::NextScene() const
{
	if (phase_ == CSceneEnd::Phase::TITLE)
	{
		return CSceneMng::Scene::TITLE;
	}
	else if (phase_ == CSceneEnd::Phase::REGISTER)
	{
		gm()->scoreMng()->score(totalScore_);
		return CSceneMng::Scene::NAMEINPUT;
	}
	return CSceneMng::Scene::END;
}


#pragma endregion
// CSceneEnd methods
