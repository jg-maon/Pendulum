
#include "stageBase.h"

#include "common.h"

#include "fade.h"

//=============================================================================
#pragma region public methods

IStage::IStage(const std::string& name) :
Base(name)
, phase_(IStage::Phase::CLEAR_ANNOUNCE)
, bgm(bgm_)
{


}
void IStage::step()
{
	//*
	switch (phase_)
	{
	case IStage::Phase::CLEAR_ANNOUNCE:
		if (UpdateClearAnnounce())
		{
			phase_ = IStage::Phase::NORMAL;
		}
		/*
		switch (clearType_)
		{
		case IStage::ClearType::GOAL:
		break;
		case IStage::ClearType::ANNIHILATION:
		break;
		default:
		break;
		}
		//*/
		break;
	case IStage::Phase::NORMAL:
		for (auto& ap : stage_[0].actionPoints)
			ap->step();
		if (UpdateNormal())
		{
			nextPhase_ = IStage::Phase::BOSS;
			phase_ = IStage::Phase::FADE_OUT;
			fadeOutTime_ = 0.3f;
			fadeInTime_ = 0.3f;
			CFade::ChangeColor(0xff000000);
			CFade::StartFadeOut();
		}
		break;
	case IStage::Phase::FADE_OUT:
		if (CFade::FadeOut(255.f / fadeOutTime_*system::FrameTime))
		{
			phase_ = IStage::Phase::FADE_IN;
			CFade::StartFadeIn();
		}
		break;
	case IStage::Phase::FADE_IN:
		if (CFade::FadeIn(255.f / fadeInTime_*system::FrameTime))
		{
			phase_ = nextPhase_;
		}
		break;
	case IStage::Phase::BOSS:
		for (auto& ap : stage_[1].actionPoints)
			ap->step();
		if (UpdateBoss())
		{
			nextPhase_ = IStage::Phase::RESULT;
			phase_ = IStage::Phase::FADE_OUT;
			fadeOutTime_ = 0.7f;
			fadeInTime_ = 0.3f;
			CFade::ChangeColor(0xffffffff);
			CFade::StartFadeOut();
		}
		break;
	}
	if (input::CheckPush(input::KEY_BTN0))
	{
		phase_ = IStage::Phase::RESULT;
	}
}

void IStage::draw()
{
	if (phase_ == Phase::BOSS || phase_ == Phase::RESULT)
		for (auto& ap : stage_[1].actionPoints)
			ap->draw();
	else
		for (auto& ap : stage_[0].actionPoints)
			ap->draw();

}

void IStage::init(std::ifstream& f)
{
	/*
	// 先に登録されているオブジェクトを消してから読み込む
	auto& objs = gm()->GetObjects("Player");
	for (auto& obj : objs)
	obj->kill();
	//*/
	LoadPlayer(f);
	LoadEnemies(f);



}

bool IStage::isEndStage() const
{
	return phase_ == IStage::Phase::RESULT;
}

const std::vector<ActPtPtr>& IStage::getActionPoints() const
{
	return (phase_ == Phase::BOSS) ? stage_[1].actionPoints : stage_[0].actionPoints;
}

const mymath::Recti& IStage::getStageRect() const
{
	return (phase_ == Phase::BOSS) ? stage_[1].stageRect : stage_[0].stageRect;
}

const mymath::Recti& IStage::getCameraRect() const
{
	return (phase_ == Phase::BOSS) ? stage_[1].cameraRect : stage_[0].cameraRect;
}

#pragma endregion	// public methods
//=============================================================================

//=============================================================================
#pragma region private methods

bool IStage::LoadEnv(std::ifstream& f)
{
	//--------------------------------------
	// ステージ
	if (common::FindChunk(common::SeekSet(f), "#Bgm"))
	{
		f >> bgm_;
	}
	//--------------------------------------
	// クリア条件
	if (common::FindChunk(common::SeekSet(f), "#ClearAnnounce"))
	{
		charabase::BaseData bd;
		f >> bd;
		caObj_ = charabase::CharPtr(new charabase::CharBase(bd));
	}

	return f.eof();
}

bool IStage::LoadRect(std::ifstream& f, int stage)
{
	//--------------------------------------
	// ステージ
	if (common::FindChunk(common::SeekSet(f), "#Left"))
	{
		f >> stage_[stage].stageRect.left;
	}
	if (common::FindChunk(common::SeekSet(f), "#Top"))
	{
		f >> stage_[stage].stageRect.top;
	}
	if (common::FindChunk(common::SeekSet(f), "#Right"))
	{
		f >> stage_[stage].stageRect.right;
	}
	if (common::FindChunk(common::SeekSet(f), "#Bottom"))
	{
		f >> stage_[stage].stageRect.bottom;
	}
	//--------------------------------------
	// カメラ
	if (common::FindChunk(common::SeekSet(f), "#CameraLeft"))
	{
		f >> stage_[stage].cameraRect.left;
	}
	if (common::FindChunk(common::SeekSet(f), "#CameraTop"))
	{
		f >> stage_[stage].cameraRect.top;
	}
	if (common::FindChunk(common::SeekSet(f), "#CameraRight"))
	{
		f >> stage_[stage].cameraRect.right;
	}
	if (common::FindChunk(common::SeekSet(f), "#CameraBottom"))
	{
		f >> stage_[stage].cameraRect.bottom;
	}
	return f.eof();
}


bool IStage::LoadActionCircles(std::ifstream& f, int stage)
{
	if (common::FindChunk(common::SeekSet(f), "#ActionCircle"))
	{
		std::string label;
		f >> label;
		if (label != "{" || f.eof()) return f.eof();
		while (!f.eof())
		{
			std::vector<float> info;
			for (int i = 0; i<3; ++i)
			{
				f >> label;
				if (label == "}" || f.eof())return f.eof();
				info.push_back(static_cast<float>(std::atof(label.c_str())));
			}
			if (info.size() == 3)
			{
				stage_[stage].actionPoints.push_back(ActPtPtr(
					new CActionCircle(info[0], info[1], info[2])
					));
			}
		}
	}
	return f.eof();
}

bool IStage::LoadActionPolygons(std::ifstream& f, int stage)
{
	if (common::FindChunk(common::SeekSet(f), "#ActionPolygon"))
	{
		std::string label;
		f >> label;
		if (label != "{" || f.eof()) return f.eof();
		while (!f.eof())
		{
			f >> label;
			if (label == "}")break;
			int num = std::atoi(label.c_str());
			std::vector<mymath::Vec3f> info(num);
			for (int i = 0; i<num; ++i)
			{
				mymath::Vec3f pos;
				f >> label;
				if (label == "}" || f.eof()) return f.eof();
				pos.x = static_cast<float>(std::atof(label.c_str()));
				f >> label;
				if (label == "}" || f.eof()) return f.eof();
				pos.y = static_cast<float>(std::atof(label.c_str()));
				pos.z = 0.5f;
				info[i] = pos;
			}
			//info.push_back(info[0]);	// 閉路にする
			stage_[stage].actionPoints.push_back(ActPtPtr(
				new CActionPolygon(info)));

		}
	}
	return f.eof();
}

#pragma endregion	// private methods

//=============================================================================
#pragma region protected methods

void IStage::load(std::ifstream& f, int stage)
{
	/*
	// 先に登録されているオブジェクトを消してから読み込む
	auto& objs = gm()->GetObjects("Player,Action", ',');
	for (auto& obj : objs)
		obj->kill();
	LoadPlayer(f);
	LoadEnemies(f);
	//*/

	LoadRect(f, stage);
	stage_[stage].actionPoints.clear();
	LoadActionCircles(f, stage);
	LoadActionPolygons(f, stage);
}

bool IStage::LoadPlayer(std::ifstream& f)
{
	if (common::FindChunk(common::SeekSet(f), "#Player"))
	{
		// プレイヤー座標を取得
		float pos[2];	// [0]:x [1]:y
		for (auto& p : pos)
		{
			std::string label;
			f >> label;
			p = static_cast<float>(std::atof(label.c_str()));
		}
		// プレイヤーオブジェクトを追加
		auto& pl = gm()->GetObj(typeid(CPlayer));
		auto& sm = gm()->GetObj(typeid(CStageMng));
		std::shared_ptr<CPlayer> player;
		if (!pl)
		{
			player = std::shared_ptr<CPlayer>(new CPlayer(pos[0], pos[1]));
			gm()->AddObject(player);
		}
		else
		{
			// 多重登録防止
			player = std::dynamic_pointer_cast<CPlayer>(pl);
			player->init(pos[0], pos[1], player->obj().pos.z);
		}
#ifdef DEF_GM_PTR
		gm()->SetPlayerPtr(player);
#endif

	}

	return f.eof();
}

bool IStage::LoadEnemies(std::ifstream& f)
{
	if (common::FindChunk(common::SeekSet(f), "#Enemy"))
	{
		std::string tableFile;
		f >> tableFile;
		// プレイヤーオブジェクトを追加
		auto& pEm = gm()->GetObj(typeid(CEnemyMng));
		std::shared_ptr<CEnemyMng> em;
		if (!pEm)
		{
			// EnemyMngがない場合、新規に追加する
			em = std::shared_ptr<CEnemyMng>(new CEnemyMng());
			gm()->AddObject(em);
		}
		else
		{
			// 多重登録防止
			em = std::dynamic_pointer_cast<CEnemyMng>(pEm);
		}
		em->LoadEnemyTable(tableFile);
#ifdef DEF_GM_PTR
		gm()->SetEnemyMngPtr(em);
#endif
	}
	return f.eof();
}

#pragma endregion	// protected methods

//=============================================================================
