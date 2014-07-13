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
bool CStageMng::load()
{
	//----------------------------------------------------
	// ステージのロード
	std::string stages = gm()->fm().GetFile("#StageFile");
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



void CStageMng::step()
{
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

void CStageMng::MoveCamera(const mymath::Vec3f& lookAt, float adjust) const
{
	mymath::Vec3f cameraPos = camera::GetLookAt();
	const auto& cameraRect = getCameraRect();

	mymath::Vec3f newPos;
	newPos.x = static_cast<float>(clamp(lookAt.x, (cameraRect.left + system::WINW / 2), (cameraRect.right - system::WINW / 2)));
	newPos.y = static_cast<float>(clamp(lookAt.y, (cameraRect.top + system::WINH / 2), (cameraRect.bottom - system::WINH / 2)));

	
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

void CStageMng::LoadStage(const std::string& stageName)
{

	nowStage_ = stageName;

	std::string stages = gm()->fm().GetFile("#StageFile");
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


const std::string& CStageMng::getStageBGM() const
{
	return stages_.at(nowStage_)->bgm;
}


const mymath::Recti& CStageMng::getStageRect() const
{
	return stages_.at(nowStage_)->rect;
}


const mymath::Recti& CStageMng::getStageRect(const std::string& stage) const
{
	return stages_.at(stage)->rect;
}

const mymath::Recti& CStageMng::getCameraRect() const
{
	return stages_.at(nowStage_)->cameraRect;
}

const mymath::Recti& CStageMng::getCameraRect(const std::string& stage) const
{
	return stages_.at(stage)->cameraRect;
}


const std::vector<ActPtPtr>& CStageMng::getActionPoints() const
{
	return stages_.at(nowStage_)->actionPoints;
}

