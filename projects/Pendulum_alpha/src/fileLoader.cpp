#include "fileLoader.h"

#include "../../../lib/gplib.h"
using namespace gplib;
#include "common.h"

#include "bird.h"

#include "player.h"


#include "scoreMng.h"
#include "enemyMng.h"
#include "stageMng.h"
#include "sceneStageSelect.h"

#include <string>
#include <fstream>

#include "fileLoader.h"


CFileLoader::CFileLoader(const std::string& iniFile) :
iniFile_(iniFile)
{
	
}
void CFileLoader::Load(FontTable& fontTable)
{
	std::ifstream iniF(iniFile_);
	if (iniF.fail())
	{
		gplib::debug::BToM("CFileLoader::CFileLoader iniFpath:%s", iniFile_.c_str());
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
		gplib::debug::BToM("CFileLoader::LoadRes path:%s", resFile.c_str());
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
		data.path = common::StrReplace(data.path, "＿", "　");
		data.path = common::StrReplace(data.path, "_", " ");
		font::Draw_CreateFont(i, size, data.path.c_str());
		fontTable.insert(std::unordered_map<std::string, int>::value_type(data.resname, i));
		i++;
	}
}

#pragma endregion // リソースファイル読み込み
//=======================================================

//=======================================================
#pragma region 敵テーブル読み込み
bool CFileLoader::LoadBird(const std::string& fileName, std::vector<EnemyPtr>& enemies)
{
	using common::FindChunk;
	using common::SeekSet;
	std::ifstream eneF(fileName);
	if (eneF.fail())
	{
		debug::BToM("CFileLoader::LoadBird path:%s", fileName.c_str());
		return false;
	}
	// 情報ロード用
	CBird tmp;
	CBird::LoadInfo lf;
	charabase::CharBase cb;
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
	// 画像情報
	//-----------------------------------------------
	// 画像管理名
	if (success = FindChunk(SeekSet(eneF), "#Img"))
	{
		eneF >> cb.resname;
	}
	// 画像サイズ
	if (success && (success = FindChunk(SeekSet(eneF), "#Size")))
	{
		eneF >> cb.size.x;
		eneF >> cb.size.y;
	}
	//-----------------------------------------------
	if (success)
	{
		// 画像情報設定
		tmp.obj(cb);
	}
	//-----------------------------------------------
	// クラス情報
	//-----------------------------------------------
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
	//-------------------------------------
	// 範囲系
	if (success && (success = FindChunk(SeekSet(eneF), "#SearchRange")))
	{
		eneF >> lf.SEARCH_RANGE;
	}
	if (success && (success = FindChunk(SeekSet(eneF), "#ChaseRange")))
	{
		eneF >> lf.CHASE_RANGE;
	}
	if (success && (success = FindChunk(SeekSet(eneF), "#AttackRange")))
	{
		eneF >> lf.ATTACK_RANGE;
	}
	if (success && (success = FindChunk(SeekSet(eneF), "#ReturnRange")))
	{
		eneF >> lf.RETURN_RANGE;
	}
	if (success && (success = FindChunk(SeekSet(eneF), "#MoveSpeed")))
	{
		eneF >> lf.MOVE_SPEED;
	}
	//-------------------------------------

	if (success)
	{
		//-----------------------------------------------
		// 全ての情報が正しく読み込めた際のみここに入る
		// パラメータ
		tmp.SetInfo(lf);

		// コピーコンストラクタを用いオリジナル作成
		enemies.push_back(EnemyPtr(new CBird(tmp)));
	}

	return success;
}

#pragma endregion	// 敵テーブル読み込み
//=======================================================

//=======================================================
bool CFileLoader::LoadPlayerData(CPlayer& player)
{
	using common::FindChunk;
	using common::SeekSet;
	//========================================================
	// パスファイルから敵DBファイルパスの読み込み
	std::ifstream iniF(iniFile_);
	std::string playerFile;
	if (iniF.fail() || (!FindChunk(iniF, "#PlayerFile")))
	{
		debug::BToM("CFileLoader::LoadPlayerData [iniF]Error path:%s", iniFile_.c_str());
		return false;
	}
	iniF >> playerFile;
	//========================================================
	// DBファイルを開く
	std::ifstream f(playerFile);
	if (f.fail())
	{
		debug::BToM("CFileLoader::LoadPlayerData [f]Error path:%s", playerFile.c_str());
		return false;
	}

	// 情報ロード用
	CPlayer tmp;
	charabase::CharBase cb;
	CPlayer::LoadInfo lf;
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
	//-----------------------------------------------
	// 画像情報
	//-----------------------------------------------
	// 画像管理名
	if (success = FindChunk(SeekSet(f), "#Img"))
	{
		f >> cb.resname;
	}
	// 画像サイズ
	if (success && (success = FindChunk(SeekSet(f), "#Size")))
	{
		f >> cb.size.x;
		f >> cb.size.y;
	}

	if (success)
	{
		// 画像情報設定
		tmp.obj(cb);
	}
	//-----------------------------------------------
	// クラス情報
	//-----------------------------------------------
	// 当たり判定
	if (success && (success = FindChunk(SeekSet(f), "#Collision")))
	{
		tmp.LoadCollisions(f);
	}

	//------------------------------------------
	// LoadInfo
	if (success && (success = FindChunk(SeekSet(f), "#GravityAcc")))
	{
		f >> lf.GRAVITY_ACC;
	}
	if (success && (success = FindChunk(SeekSet(f), "#MaxGravity")))
	{
		f >> lf.MAX_G;
	}
	if (success && (success = FindChunk(SeekSet(f), "#Tension")))
	{
		f >> lf.TENSION;
	}
	if (success && (success = FindChunk(SeekSet(f), "#DownTension")))
	{
		f >> lf.DOWN_TENSION;
	}
	if (success && (success = FindChunk(SeekSet(f), "#DownSpeed")))
	{
		f >> lf.DOWN_SPEED;
	}
	if (success && (success = FindChunk(SeekSet(f), "#MaxVelocityX")))
	{
		f >> lf.MAX_VX;
	}
	if (success && (success = FindChunk(SeekSet(f), "#MaxVelocityY")))
	{
		f >> lf.MAX_VY;
	}
	if (success && (success = FindChunk(SeekSet(f), "#ChainTime")))
	{
		f >> lf.CHAIN_TIME[0] >> lf.CHAIN_TIME[1];
	}
	if (success && (success = FindChunk(SeekSet(f), "#InvincibleTime")))
	{
		f >> lf.INV_TIME;
	}

	if (success)
	{
		//-----------------------------------------------
		// 全ての情報が正しく読み込めた際のみここに入る
		// パラメータ
		tmp.SetInfo(lf);
		// 代入を用いオリジナル複製
		player = tmp;
	}
	return success;
}
//=======================================================

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
		debug::BToM("CFileLoader::LoadEnemiesData [iniF]Error path:%s", iniFile_.c_str());
		return;
	}
	iniF >> enemyFile;
	//========================================================
	// DBファイルを開く
	std::ifstream f(enemyFile);
	if (f.fail())
	{
		debug::BToM("CFileLoader::LoadEnemiesData [f]Error path:%s", enemyFile.c_str());
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

//=======================================================

std::string CFileLoader::GetFile(const std::string& tag) const
{
	std::ifstream iniF(iniFile_);
	if (iniF.fail())
	{
		gplib::debug::BToM("CFileLoader::GetFile iniFpath:%s", iniFile_.c_str());
	}
	else
	{
		if (common::FindChunk(iniF, tag))
		{
			std::string fileName;
			iniF >> fileName;
			return fileName;
		}
	}
	return "";
}