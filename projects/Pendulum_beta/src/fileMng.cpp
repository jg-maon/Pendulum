#include "fileMng.h"

#include "player.h"
#include "enemyBase.hpp"
#include "pickupBase.hpp"

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
	auto player = std::shared_ptr<CPlayer>(new CPlayer());
	fileLoader_.LoadPlayerData(*player);

	// �G
	std::vector<EnemyPtr> enemies;
	fileLoader_.LoadEnemiesData(enemies);

	// �s�b�N�A�b�v�A�C�e��
	std::vector<PickupPtr> pickups;
	fileLoader_.LoadPickupData(pickups);
	
	//=====================================
	// �z����
	
	// push_back�p�ɃT�C�Y���g��(player + size())
	dataTable_.reserve(1 + enemies.size() + pickups.size());
	// �v���C���[
	dataTable_.push_back(player);
	// �G
	for (auto& enemy : enemies)
		dataTable_.push_back(enemy);
	// �s�b�N�A�b�v�A�C�e��
	for (auto& pickup : pickups)
		dataTable_.push_back(pickup);
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