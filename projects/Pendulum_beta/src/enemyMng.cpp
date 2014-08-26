
#include "enemyMng.h"
#include "define.h"
#include "common.h"
#include <algorithm>
#include <functional>
#include <fstream>
#include <sstream>



CEnemyMng::CEnemyMng() :
Base("EnemyMng")
{
	status_ = Status::idle;
}




void CEnemyMng::step()
{
	for (auto& enemy : enemies_)
		enemy->step();

	EraseObject(enemies_);
	if (!temp_.empty())
	{
		enemies_.insert(enemies_.end(), temp_.begin(), temp_.end());
		temp_.clear();
	}
}

void CEnemyMng::draw()
{
	for (auto& enemy : enemies_)
		enemy->draw();

#ifdef _DEBUG
	std::stringstream ss;
	ss << enemies_.size() << " / " << enemyNum_;
	font::Draw_FontTextNC(system::WINW - 200, system::WINH - 40, 0.f,
		ss.str(), -1, 3);
#endif
}

void CEnemyMng::kill()
{
	__super::kill();
	for (auto& enemy : enemies_)
		enemy->kill();
	for (auto& enemy : temp_)
		enemy->kill();
}

void CEnemyMng::start()
{
	__super::start();
	for (auto& enemy : enemies_)
		enemy->start();
	for (auto& enemy : temp_)
		enemy->start();
}

void CEnemyMng::stop()
{
	__super::stop();
	for (auto& enemy : enemies_)
		enemy->stop();

}
void CEnemyMng::SetStatusDisp()
{
	__super::SetStatusDisp();
	for (auto& enemy : enemies_)
		enemy->SetStatusDisp();
	for (auto& enemy : temp_)
		enemy->SetStatusDisp();

}

bool CEnemyMng::isDestroyerBonus() const
{
	return enemies_.size() == 0;
}

bool CEnemyMng::isNoKillBonus() const
{
	return enemies_.size() == enemyNum_;
}

ObjPtr CEnemyMng::GetPtr()
{
	extern CGameManager* gm;
	auto& em = gm->GetObjects("EnemyMng");
	if (em.empty()) return nullptr;
	return em[0];
}

void CEnemyMng::LoadEnemyTable(const std::string& fileName)
{
	std::ifstream f(fileName);
	if (f.fail())
	{
		debug::BToMF("file load error path:%s", fileName.c_str());
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
	// グリフォン
	if (common::FindChunk(common::SeekSet(f), "#Griffon"))
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
				enemies_.push_back(EnemyPtr(new CGriffon(pos[0], pos[1])));
			}
		}
	}

	//---------------------------------------
	// レイビット
	if (common::FindChunk(common::SeekSet(f), "#Raybit"))
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
				enemies_.push_back(EnemyPtr(new CRaybit(pos[0], pos[1])));
			}
		}
	}
	
	//---------------------------------------
	// ロボットアーム
	if (common::FindChunk(common::SeekSet(f), "#RoboticArm"))
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
				f >> label;
				int dir = std::atoi(label.c_str());
				enemies_.push_back(EnemyPtr(new CRoboticArm(pos[0], pos[1], dir)));
			}
		}
	}
	//---------------------------------------
	// ドラゴン
	if (common::FindChunk(common::SeekSet(f), "#Dragon"))
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
				enemies_.push_back(EnemyPtr(new CDragon(pos[0], pos[1])));
			}
		}
	}




	// 配置後敵数初期化
	enemyNum_ = enemies_.size();
}

void CEnemyMng::CreateEnemy(const EnemyPtr& enemy)
{
	temp_.push_back(enemy);
}

std::vector<EnemyPtr>& CEnemyMng::getEnemies()
{
	return enemies_;
}

size_t CEnemyMng::GetStartEnemyNum() const
{
	return enemyNum_;
}



void CEnemyMng::ClearEnemies()
{
	enemies_.clear();
	temp_.clear();
}
