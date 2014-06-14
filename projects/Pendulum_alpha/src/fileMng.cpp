#include "fileMng.h"


#include "player.h"
#include "enemyBase.hpp"


#include <fstream>

CFileMng::CFileMng(const std::string& iniFile):
iniFile_(iniFile)
, fileLoader_(iniFile_)
{
	fontTable_.clear();
	dataTable_.clear();
	fileLoader_.Load(fontTable_);
}

void CFileMng::Load()
{
	//fontTable_.clear

	// �v���C���[
	auto player = std::shared_ptr<CPlayer>(new CPlayer());
	fileLoader_.LoadPlayerData(*player);
	
	// �G
	std::vector<EnemyPtr> enemies;
	fileLoader_.LoadEnemiesData(enemies);

	//=====================================
	// �z����
	// push_back�p�ɃT�C�Y���g��(player + size())
	dataTable_.reserve(1 + enemies.size());
	// �v���C���[
	dataTable_.push_back(player);

	// �G
	for (auto& enemy : enemies)
		dataTable_.push_back(enemy);
}


std::string CFileMng::GetFile(const std::string& tag) const
{
	return fileLoader_.GetFile(tag);
}