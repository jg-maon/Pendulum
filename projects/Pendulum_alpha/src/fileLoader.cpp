#include "fileLoader.h"

#include "../../../lib/gplib.h"
using namespace gplib;
#include "common.h"

#include "bird.h"

#include <string>
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

//=======================================================
#pragma region 敵テーブル読み込み
void CFileLoader::LoadBird(const std::string& fileName, std::vector<EnemyPtr>& enemies)
{
	using common::FindChunk;
	using common::SeekSet;
	std::ifstream eneF(fileName);
	if (eneF.fail())
	{
		debug::Dbg_BoxToMessage("CFileLoader::LoadBird path:%s", fileName.c_str());
		return;
	}
	// 情報ロード用
	CBird tmp;
	// ファイルから情報を抜き取る際のタグ検索用
	bool success;
	// success が一度でもfalseになったら他処理スキップ
	// if (success){
	// 	ロード
	// }
	// else {
	// 	return;
	// }みたいな感じ

	//-----------------------------------------------
	// 画像管理名
	if (success = FindChunk(SeekSet(eneF), "#Img"))
	{
		eneF >> tmp.obj().resname;
	}
	// 画像サイズ
	if (success && (success = FindChunk(SeekSet(eneF), "#Size")))
	{
		eneF >> tmp.obj().size.x;
		eneF >> tmp.obj().size.y;
	}
	// 当たり判定
	if (success && (success = FindChunk(SeekSet(eneF), "#Collision")))
	{
		tmp.LoadCollisions(eneF);
	}
	// 攻撃
	if (success && (success = FindChunk(SeekSet(eneF), "#Attack")))
	{
		tmp.LoadAttack(eneF);
	}

	if (success)
	{
		//-----------------------------------------------
		// 全ての情報が正しく読み込めた際のみここに入る
		// コピーコンストラクタを用いオリジナル作成
		enemies.push_back(EnemyPtr(new CBird(tmp)));
	}
}

#pragma endregion	// 敵テーブル読み込み
//=======================================================



void CFileLoader::LoadEnemiesData(std::vector<EnemyPtr>& enemies)
{
	using common::FindChunk;
	using common::SeekSet;
	//========================================================
	// パスファイルから敵DBファイルパスの読み込み
	std::ifstream iniF(iniFile_);
	std::string enemyFile;
	if (iniF.fail() || (!FindChunk(iniF, "#EnemyFile")))
	{
		debug::Dbg_BoxToMessage("CFileLoader::LoadEnemiesData [iniF]Error path:%s", iniFile_.c_str());
		return;
	}
	//========================================================
	// DBファイルを開く
	std::ifstream f(enemyFile);
	if (f.fail())
	{
		debug::Dbg_BoxToMessage("CFileLoader::LoadEnemiesData path:%s", enemyFile.c_str());
		return;
	}

	// 敵テーブル初期化
	enemies.clear();

	//========================================================
	// 敵情報読み込み開始
	if (FindChunk(SeekSet(f),"#Bird"))
	{
		std::string buf;
		f >> buf;
		LoadBird(buf, enemies);
	}
	//========================================================

}