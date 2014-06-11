#include "fileMng.h"

#include "fileLoader.h"

#include "player.h"
#include "enemyBase.hpp"


#include <fstream>

CFileMng::CFileMng()
{
	fontTable_.clear();
	dataTable_.clear();
}

void CFileMng::Load(const std::string& iniFile)
{
	//fontTable_.clear
	CFileLoader fl(iniFile, fontTable_);

	// プレイヤー
	std::shared_ptr<CPlayer> player;
	fl.LoadPlayerData(*player);
	
	// 敵
	std::vector<EnemyPtr> enemies;
	fl.LoadEnemiesData(enemies);

	//=====================================
	// 配列代入
	// push_back用にサイズを拡張(player + size())
	dataTable_.reserve(1 + enemies.size());
	// プレイヤー
	dataTable_.push_back(player);

	// 敵
	for (auto& enemy : enemies)
		dataTable_.push_back(enemy);
}