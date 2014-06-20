
#include "enemyMng.h"
#include "define.h"
#include "common.h"
#include <algorithm>
#include <functional>
#include <fstream>
#include <sstream>



CEnemyMng::CEnemyMng():
	Base("EnemyMng")
{
	status_ = Status::idle;
}




void CEnemyMng::step()
{
	for(auto& enemy : enemies_)
		enemy->step();

	EraseObject(enemies_);
	if(!temp_.empty())
	{
		enemies_.insert(enemies_.end(), temp_.begin(), temp_.end());
		temp_.clear();
	}
}

void CEnemyMng::draw()
{
	for(auto& enemy : enemies_)
		enemy->draw();
}

ObjPtr CEnemyMng::GetPtr()
{
	extern CGameManager* gm;
	auto& em = gm->GetObjects("EnemyMng");
	if(em.empty()) return nullptr;
	return em[0];
}

void CEnemyMng::LoadEnemyTable(const std::string& fileName)
{
	std::ifstream f(fileName);
	if (f.fail())
	{
		debug::BToM("CEnemyMng::LoadEnemyTable path:%s", fileName.c_str());
		return;
	}
	enemies_.clear();
	temp_.clear();

	//---------------------------------------
	// 鳥
	if (common::FindChunk(common::SeekSet(f), "#Bird"))
	{
		std::string label;
		f >> label;
		if (label == "{")
		{
			while (!f.eof())
			{
				float pos[2];	// [0]:x [1]:y
				for (auto& p : pos)
				{
					f >> label;
					// エラーチェック
					if (label == "}" || f.eof()) break;
					p = static_cast<float>(std::atof(label.c_str()));
				}
				if (label == "}") break;
				enemies_.push_back(EnemyPtr(new CBird(pos[0], pos[1])));
			}
		}
	}

	//---------------------------------------
	// 妖精
	if (common::FindChunk(common::SeekSet(f), "#Fairy"))
	{
		std::string label;
		f >> label;
		if (label == "{")
		{
			while (!f.eof())
			{
				float pos[2];	// [0]:x [1]:y
				for (auto& p : pos)
				{
					f >> label;
					// エラーチェック
					if (label == "}" || f.eof()) break;
					p = static_cast<float>(std::atof(label.c_str()));
				}
				if (label == "}") break;
				enemies_.push_back(EnemyPtr(new CFairy(pos[0], pos[1])));
			}
		}
	}

	//---------------------------------------
	// 別敵
	/*
	if (common::FindChunk(common::SeekSet(f), "#enemyName"))
	{
		std::string label;
		f >> label;
		if (label == "{")
		{
			while (!f.eof())
			{
				float pos[2];	// [0]:x [1]:y
				for (auto& p : pos)
				{
					f >> label;
					// エラーチェック
					if (label == "}" || f.eof()) break;
					p = static_cast<float>(std::atof(label.c_str()));
				}
				if (label == "}") break;
				enemies_.push_back(EnemyPtr(new CFairy(pos[0], pos[1])));
			}
		}
	}
	//*/
}

void CEnemyMng::CreateEnemy(const EnemyPtr& enemy)
{
	temp_.push_back(enemy);
}

std::vector<EnemyPtr>& CEnemyMng::getEnemies()
{
	return enemies_;
}

