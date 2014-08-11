#include "stageMng.h"

#include "define.h"

#include "stage1.h"
#include "stage2.h"
#include "stage3.h"
#include "stage4.h"

#include "setting.h"
#include "common.h"

#include "player.h"
#include "enemyMng.h"

#include "gameManager.h"

#include <sstream>
#include <fstream>


using std::ifstream;
using common::FindChunk;
using common::SeekSet;

CStageMng::CStageMng() :
Base("StageMng")
{
	// メインゲームになるまで待機
	status_ = Status::idle;
}


void CStageMng::start()
{
	__super::start();
	time_ = 0.f;
}

void CStageMng::step()
{
	time_ += system::FrameTime;
	stages_[nowStage_]->step();
}

void CStageMng::draw()
{
	stages_[nowStage_]->draw();
}


const std::shared_ptr<CStageMng> CStageMng::GetPtr()
{
	extern CGameManager *gm;
	const auto& sm = gm->GetObj(typeid(CStageMng));
	return std::dynamic_pointer_cast<CStageMng>(sm);
}

bool CStageMng::load()
{
	//----------------------------------------------------
	// ステージのロード
	std::string stages = gm()->fm()->GetPath("#StageFile");
	ifstream stageF(stages);
	if (stageF.fail())
	{
		debug::BToM("CStageMng::CStageMng [stageF] path:%s", stages.c_str());
		return false;
	}

	std::string path;		// ステージファイルパス
	if (!FindChunk(stageF, "#StagePath"))
	{
		debug::BToM("CStageMng::Loadstage #StagePath not found");
		return false;
	}
	stageF >> path;

	for (int i = 1; i <= 4; ++i)
	{
		std::stringstream tag;
		tag << "#Stage" << std::setw(2) << std::setfill('0') << i;
		if (FindChunk(SeekSet(stageF), tag.str()))
		{
			std::string stage;
			stageF >> stage;
			stage = path + stage;		// ステージファイル名にパスを結合
			ifstream f(stage);			// ステージファイル
			if (f.fail())
			{
				debug::BToM("CStageMng::CStageMng [%s] path:%s", tag.str().c_str(), stage.c_str());
				return false;
			}
			switch (i)
			{
			case 1:
				stages_.insert(StageMap::value_type("Stage01", StagePtr(new CStage1(f))));
				break;

			case 2:
				stages_.insert(StageMap::value_type("Stage02", StagePtr(new CStage2(f))));
				break;

			case 3:
				stages_.insert(StageMap::value_type("Stage03", StagePtr(new CStage3(f))));
				break;

			case 4:
				stages_.insert(StageMap::value_type("Stage04", StagePtr(new CStage4(f))));
				break;

			}
		}
		else
		{
			debug::BToM("CStageMng::CStageMng %s tag not found", tag.str().c_str());
			return false;
		}
	}

	return true;
}


void CStageMng::LoadStage(const std::string& stageName)
{
	stageState_ = StageState::ENTER;
	nowStage_ = stageName;

	std::string stages = gm()->fm()->GetPath("#StageFile");
	ifstream stageF(stages);
	if (stageF.fail())
	{
		debug::BToM("CStageMng::Loadstage [stageF] path:%s", stages.c_str());
		return;
	}


	std::string path;		// ステージファイルパス
	if (!FindChunk(stageF, "#StagePath"))
	{
		debug::BToM("CStageMng::Loadstage #StagePath not found");
		return;
	}
	stageF >> path;

	// 現在のステージファイルからタグの生成
	std::stringstream tag;
	tag << "#" << nowStage_;

	if (FindChunk(SeekSet(stageF), tag.str()))
	{
		std::string stage;
		stageF >> stage;
		stage = path + stage;		// ステージファイル名にパスを結合
		ifstream f(stage);			// ステージファイル
		if (f.fail())
		{
			debug::BToM("CStageMng::LoadStage [%s] path:%s", tag.str().c_str(), stage.c_str());
			return;
		}
		// ステージ初期化
		stages_[nowStage_]->init(f);
	}
	else
	{
		debug::BToM("CStageMng::Loadstage %s not found", tag.str().c_str());
		return;
	}

}


void CStageMng::MoveCamera(const mymath::Vec3f& lookAt, float adjust) const
{
	mymath::Vec3f cameraPos = camera::GetLookAt();
	auto cameraRect = getCameraRect();

	//cameraRect.Scale(1.f / camera::GetScale());

	float s = camera::GetScale();

	mymath::Vec3f newPos;
	//newPos.x = static_cast<float>(clamp(lookAt.x, (cameraRect.left + system::WINW / 2), (cameraRect.right - system::WINW / 2)));
	//newPos.y = static_cast<float>(clamp(lookAt.y, (cameraRect.top + system::WINH / 2), (cameraRect.bottom - system::WINH / 2)));
	newPos.x = static_cast<float>(clamp(lookAt.x, (cameraRect.left + system::WINW / 2 / s), (cameraRect.right - system::WINW / 2 / s)));
	newPos.y = static_cast<float>(clamp(lookAt.y, (cameraRect.top + system::WINH / 2 / s), (cameraRect.bottom - system::WINH / 2 / s)));

	
	if (mymath::POW2(cameraPos.x - newPos.x) > mymath::POW2(adjust))
		cameraPos.x = newPos.x;
	if (mymath::POW2(cameraPos.y - newPos.y) > mymath::POW2(adjust))
		cameraPos.y = newPos.y;
	

	camera::SetLookAt(cameraPos.x, cameraPos.y);
}

bool CStageMng::isEndStage() const
{
	return stages_.at(nowStage_)->isEndStage();
}


void CStageMng::setStageState(CStageMng::StageState state)
{
	stageState_ = state;
}

bool CStageMng::isEnterAnimating() const
{
	return stageState_ == StageState::ENTER;
}

bool CStageMng::isExitAnimating() const
{
	return stageState_ == StageState::EXIT;
}

bool CStageMng::isNormaTimeClear() const
{
	return stages_.at(nowStage_)->isNormaTimeClear(time_);
}


const std::string& CStageMng::getStageBGM() const
{
	return stages_.at(nowStage_)->bgm;
}


const mymath::Recti& CStageMng::getStageRect() const
{
	return stages_.at(nowStage_)->getStageRect();
}


const mymath::Recti& CStageMng::getStageRect(const std::string& stage) const
{
	return stages_.at(stage)->getStageRect();
}

const mymath::Recti& CStageMng::getCameraRect() const
{
	return stages_.at(nowStage_)->getCameraRect();
}

const mymath::Recti& CStageMng::getCameraRect(const std::string& stage) const
{
	return stages_.at(stage)->getCameraRect();
}


const std::vector<ActPtPtr>& CStageMng::getActionPoints() const
{
	return stages_.at(nowStage_)->getActionPoints();
}

