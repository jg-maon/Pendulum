#include "fileLoader.h"

#include "../../../lib/gplib.h"
using namespace gplib;
#include "common.h"

#include "bird.h"

#include <fstream>

CFileLoader::CFileLoader(const std::string& iniFile) :
iniFile_(iniFile)
{
	std::ifstream iniF(iniFile);
	if (iniF.fail())
	{
		gplib::debug::Dbg_BoxToMessage("CFileLoader::CFileLoader iniFpath:%s", iniFile.c_str());
	}
	else
	{
		if (common::FindChunk(iniF, "#ResourceFile"))
		{
			std::string resFile;
			iniF >> resFile;
			LoadRes(resFile, FontTable());
		}
	}
}
CFileLoader::CFileLoader(const std::string& iniFile, FontTable& fontTable) :
iniFile_(iniFile)
{
	std::ifstream iniF(iniFile);
	if (iniF.fail())
	{
		gplib::debug::Dbg_BoxToMessage("CFileLoader::CFileLoader iniFpath:%s", iniFile.c_str());
	}
	else
	{
		if (common::FindChunk(iniF, "#ResourceFile"))
		{
			std::string resFile;
			iniF >> resFile;
			LoadRes(resFile, fontTable);
		}
	}
}

//=======================================================
#pragma region リソースファイル読み込み
void CFileLoader::LoadRes(const std::string& resFile, FontTable& fontTable)
{
	//====================================
	// リソースファイル読み込み
	ifstream resF(resFile);
	if (resF.fail())
	{
		gplib::debug::Dbg_BoxToMessage("CFileLoader::LoadRes path:%s", resFile.c_str());
		return;
	}

	//--------------------------
	if (common::FindChunk(resF, "#Img"))
	{
		LoadImg(resF);
	}
	common::SeekSet(resF);
	if (common::FindChunk(resF, "#Bgm"))
	{
		LoadBGM(resF);
	}
	common::SeekSet(resF);
	if (common::FindChunk(resF, "#Se"))
	{
		LoadSE(resF);
	}
	common::SeekSet(resF);
	if (common::FindChunk(resF, "#Font"))
	{
		LoadFont(resF, fontTable);
	}
	common::SeekSet(resF);
}

// 画像ロード
void CFileLoader::LoadImg(ifstream& resF)
{
	std::string buf;
	resF >> buf;
	if (buf != "{") return;
	while (!resF.eof())
	{
		resF >> buf;
		if (buf == "}") return;
		ResData data;
		D3DCOLOR transparent;
		data.resname = buf;
		resF >> data.path >> std::hex >> transparent;

		graph::Draw_LoadObject(data.resname, data.path, transparent);
	}
}

// BGMロード
void CFileLoader::LoadBGM(ifstream& resF)
{
	std::string buf;
	resF >> buf;
	if (buf != "{") return;
	while (!resF.eof())
	{
		resF >> buf;
		if (buf == "}") return;
		ResData data;
		data.resname = buf;
		resF >> data.path;
		bgm::DShow_LoadFile(data.resname, data.path);
	}
}

// SEロード
void CFileLoader::LoadSE(ifstream& resF)
{
	std::string buf;
	resF >> buf;
	if (buf != "{") return;
	while (!resF.eof())
	{
		resF >> buf;
		if (buf == "}") return;
		ResData data;
		data.resname = buf;
		resF >> data.path;
		se::DSound_LoadFile(data.resname, data.path);
	}
}

// フォントロード
void CFileLoader::LoadFont(ifstream& resF, FontTable& fontTable)
{
	fontTable.clear();
	std::string buf;
	resF >> buf;
	if (buf != "{") return;
	int i = 0;
	while (!resF.eof())
	{
		resF >> buf;
		if (buf == "}") return;
		ResData data;
		int size;
		data.resname = buf;
		resF >> data.path >> std::dec >> size;
		data.path = common::StrReplace(data.path, "＿", "");
		font::Draw_CreateFont(i, size, data.path.c_str());
		fontTable.insert(std::unordered_map<std::string, int>::value_type(data.resname, i));
		i++;
	}
}

#pragma endregion // リソースファイル読み込み
//=======================================================





void CFileLoader::LoadEnemiesData(const std::string& enemyFile, std::vector<EnemyPtr>& enemies)
{
	std::ifstream f(enemyFile);
	if (f.fail())
	{
		debug::Dbg_BoxToMessage("CFileLoader::LoadEnemiesData path:%s", enemyFile.c_str());
		return;
	}
	struct TempEnemy : public IEnemy
	{
		TempEnemy() :IEnemy("TempEnemy"){}
	};
	//==================================================
	// 画像系情報
	

	//==================================================

	std::string buf;
	if (buf == "Bird")
	{
		CBird tmp;
		//tmp.a
		enemies.push_back(EnemyPtr(new CBird(tmp)));
	}

}