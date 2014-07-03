#include "fileMng.h"

#include "player.h"
#include "enemyBase.hpp"


#include <fstream>

CFileMng::CFileMng(const std::string& iniFile):
iniFile_(iniFile)
, fileLoader_(iniFile_)
, iniFile(iniFile_)
{
	fontTable_.clear();
	dataTable_.clear();
	fileLoader_.Load(fontTable_);
}

void CFileMng::Load()
{
	//fontTable_.clear

	dataTable_.clear();

	// �v���C���[
	auto player = std::make_shared<CPlayer>(new CPlayer());
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

int CFileMng::GetFontID(const std::string& resname) const
{
	return fontTable_.at(resname);
}


/*
const std::string& CFileMng::getIniFile() const
{
	return iniFile_;
}
//*/