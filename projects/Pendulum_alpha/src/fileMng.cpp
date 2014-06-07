#include "fileMng.h"

#include "fileLoader.h"

#include <fstream>

CFileMng::CFileMng()
{
	fontTable_.clear();
	enemiesData_.clear();
}

void CFileMng::Load(const std::string& iniFile)
{
	//fontTable_.clear
	CFileLoader fl(iniFile, fontTable_);
	fl.LoadEnemiesData(enemiesData_);
}