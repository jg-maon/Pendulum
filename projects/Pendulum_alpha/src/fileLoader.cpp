#include "fileLoader.h"

#include "../../../lib/gplib.h"
using namespace gplib;
#include "common.h"

#include "bird.h"
#include "fairy.h"
#include "griffon.h"
#include "player.h"
#include "pickupJewely.h"

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

//=====================================================================================
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
		fontTable.insert(FontTable::value_type(data.resname, i));
		i++;
	}
}

#pragma endregion // リソースファイル読み込み
//=====================================================================================

//=====================================================================================
//=====================================================================================

bool CFileLoader::LoadCharBase(std::ifstream& f, charabase::CharBase& cb)
{
	// ファイルから情報を抜き取る際のタグ検索用
	bool success;

	//-----------------------------------------------
	// 画像情報
	//-----------------------------------------------
	// 画像管理名
	if (success = common::FindChunk(common::SeekSet(f), "#Img"))
	{
		f >> cb.resname;
	}
	// 画像サイズ
	if (success && (success = common::FindChunk(common::SeekSet(f), "#Size")))
	{
		f >> cb.size.x;
		if (cb.size.x == -1)
			cb.size.x = graph::Draw_GetImageWidth(cb.resname);
		f >> cb.size.y;
		if (cb.size.y == -1)
			cb.size.y = graph::Draw_GetImageHeight(cb.resname);
	}
	//-----------------------------------------------

	return success;
}

//=====================================================================================
#pragma region 敵テーブル読み込み
//=====================================================================================
//=====================================================================================
//=====================================================================================
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
	
	//-----------------------------------------------
	if (success = LoadCharBase(eneF, cb))
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
	// 当たり判定
	if (success && (success = FindChunk(SeekSet(eneF), "#StageCollision")))
	{
		tmp.LoadStageCollisions(eneF);
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
	if (success && (success = FindChunk(SeekSet(eneF), "#AttackInterval")))
	{
		eneF >> lf.attackInterval;
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
	else
	{
		debug::BToM("CFileLoader::LoadBird load failed");
	}

	return success;
}


//=====================================================================================
//=====================================================================================
//=====================================================================================
//=====================================================================================
bool CFileLoader::LoadFairy(const std::string& fileName, std::vector<EnemyPtr>& enemies)
{
	using common::FindChunk;
	using common::SeekSet;
	std::ifstream eneF(fileName);
	if (eneF.fail())
	{
		debug::BToM("CFileLoader::LoadFairy path:%s", fileName.c_str());
		return false;
	}
	// 情報ロード用
	CFairy tmp;
	CFairy::LoadInfo lf;
	charabase::CharBase cb;
	// ファイルから情報を抜き取る際のタグ検索用
	bool success;

	//-----------------------------------------------
	// 画像情報
	//-----------------------------------------------
	if (success = LoadCharBase(eneF, cb))
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
	// 当たり判定
	if (success && (success = FindChunk(SeekSet(eneF), "#StageCollision")))
	{
		tmp.LoadStageCollisions(eneF);
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
	if (success && (success = FindChunk(SeekSet(eneF), "#AttackInterval")))
	{
		eneF >> lf.attackInterval;
	}
	if (success && (success = FindChunk(SeekSet(eneF), "#AttackRatio")))
	{
		eneF >> lf.attackRatio;
	}
	//-------------------------------------

	if (success)
	{
		//-----------------------------------------------
		// 全ての情報が正しく読み込めた際のみここに入る
		// パラメータ
		tmp.SetInfo(lf);

		// コピーコンストラクタを用いオリジナル作成
		enemies.push_back(EnemyPtr(new CFairy(tmp)));
	}
	else
	{
		debug::BToM("CFileLoader::LoadFairy load failed");
	}

	return success;
}
//=====================================================================================
//=====================================================================================
//=====================================================================================
//=====================================================================================
bool CFileLoader::LoadGriffon(const std::string& fileName, std::vector<EnemyPtr>& enemies)
{
	using common::FindChunk;
	using common::SeekSet;
	std::ifstream eneF(fileName);
	if (eneF.fail())
	{
		debug::BToM("CFileLoader::LoadGriffon path:%s", fileName.c_str());
		return false;
	}
	// 情報ロード用
	CGriffon tmp;
	CGriffon::LoadInfo lf;
	charabase::CharBase cb;
	// ファイルから情報を抜き取る際のタグ検索用
	bool success;

	std::string successProc = "none";

	//-----------------------------------------------
	// 画像情報
	//-----------------------------------------------
	if (success = LoadCharBase(eneF, cb))
	{
		// 画像情報設定
		tmp.obj(cb);
		successProc = "CharBase";
	}
	//-----------------------------------------------
	// クラス情報
	//-----------------------------------------------
	// 当たり判定
	if (success && (success = FindChunk(SeekSet(eneF), "#Collision")))
	{
		tmp.LoadCollisions(eneF);
		successProc = "Collision";
	}
	// 当たり判定
	if (success && (success = FindChunk(SeekSet(eneF), "#StageCollision")))
	{
		tmp.LoadStageCollisions(eneF);
		successProc = "StageCollision";
	}
	// 攻撃
	if (success && (success = FindChunk(SeekSet(eneF), "#Attack")))
	{
		tmp.LoadAttack(eneF);
		successProc = "Attack";
	}
	//-------------------------------------
	// 範囲系
	if (success && (success = FindChunk(SeekSet(eneF), "#SearchRange")))
	{
		eneF >> lf.SEARCH_RANGE;
		successProc = "SearchRange";
	}
	if (success && (success = FindChunk(SeekSet(eneF), "#ChaseRange")))
	{
		eneF >> lf.CHASE_RANGE;
		successProc = "ChaseRange";
	}
	if (success && (success = FindChunk(SeekSet(eneF), "#AttackRange")))
	{
		eneF >> lf.ATTACK_RANGE;
		successProc = "AttackRange";
	}
	if (success && (success = FindChunk(SeekSet(eneF), "#MoveSpeed")))
	{
		eneF >> lf.MOVE_SPEED;
		successProc = "MoveSpeed";
	}
	if (success && (success = FindChunk(SeekSet(eneF), "#AttackSpeed")))
	{
		eneF >> lf.ATTACK_SPEED;
		successProc = "AttackSpeed";
	}
	if (success && (success = FindChunk(SeekSet(eneF), "#InvincibleTime")))
	{
		eneF >> lf.INV_TIME;
		successProc = "InvincibleTime";
	}
	if (success && (success = FindChunk(SeekSet(eneF), "#DamageTime")))
	{
		eneF >> lf.damageTime;
		successProc = "DamageTime";
	}
	if (success && (success = FindChunk(SeekSet(eneF), "#AttackDist")))
	{
		eneF >> lf.tackleDist;
		successProc = "AttackDist";
	}
	if (success && (success = FindChunk(SeekSet(eneF), "#AttackInterval")))
	{
		eneF >> lf.attackInterval;
		successProc = "AttackInterval";
	}

	if (success && (success = FindChunk(SeekSet(eneF), "#Health")))
	{
		eneF >> lf.health;
		successProc = "Health";
	}
	if (success && (success = FindChunk(SeekSet(eneF), "#Power")))
	{
		eneF >> lf.power;
		successProc = "Power";
	}
	//-------------------------------------

	if (success)
	{
		//-----------------------------------------------
		// 全ての情報が正しく読み込めた際のみここに入る
		// パラメータ
		tmp.SetInfo(lf);

		// コピーコンストラクタを用いオリジナル作成
		enemies.push_back(EnemyPtr(new CGriffon(tmp)));
	}
	else
	{
		debug::BToM("CFileLoader::LoadGriffon load failed\n last successProc = %s", successProc.c_str());
	}

	return success;
}

#pragma endregion	// 敵テーブル読み込み
//=====================================================================================
//=====================================================================================
//=====================================================================================


#pragma region ピックアップアイテムテーブル読み込み
//=====================================================================================
//=====================================================================================
//=====================================================================================
bool CFileLoader::LoadJewely(const std::string& fileName, std::vector<PickupPtr>& pickups)
{
	using common::FindChunk;
	using common::SeekSet;
	std::ifstream f(fileName);
	if (f.fail())
	{
		debug::BToM("CFileLoader::LoadJewely path:%s", fileName.c_str());
		return false;
	}
	// 情報ロード用
	CPickupJewely tmp;
	charabase::CharBase cb;
	// ファイルから情報を抜き取る際のタグ検索用
	bool success;
	
	//-----------------------------------------------
	// 画像情報
	//-----------------------------------------------
	if (success = LoadCharBase(f, cb))
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
	//-----------------------------------------------
	if (success)
	{
		//-----------------------------------------------
		// 全ての情報が正しく読み込めた際のみここに入る

		// コピーコンストラクタを用いオリジナル作成
		pickups.push_back(PickupPtr(new CPickupJewely(tmp)));
	}
	else
	{
		debug::BToM("CFileLoader::LoadJewelyload failed");
	}

	return success;
}

#pragma endregion	// ピックアップアイテムテーブル読み込み
//=====================================================================================



//=====================================================================================
//=====================================================================================
//=====================================================================================
//=====================================================================================
bool CFileLoader::LoadPlayerData(CPlayer& player)
{
	using common::FindChunk;
	using common::SeekSet;
	//======================================================================================
	// パスファイルからプレイヤー情報ファイルパスの読み込み
	std::ifstream iniF(iniFile_);
	if (iniF.fail() || (!FindChunk(iniF, "#PlayerFile")))
	{
		debug::BToM("CFileLoader::LoadPlayerData [iniF]Error path:%s", iniFile_.c_str());
		return false;
	}
	std::string playerFile;
	iniF >> playerFile;
	//======================================================================================
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
	if (success = LoadCharBase(f, cb))
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
	// ステージ当たり判定
	if (success && (success = FindChunk(SeekSet(f), "#StageCollision")))
	{
		tmp.LoadStageCollisions(f);
	}

	//------------------------------------------
	// LoadInfo
	struct LoadValue
	{
		char* tag;				// #タグ
		void* value;			// 格納する値
		const type_info& inf;	// 型情報
	};

	//*
	LoadValue loadValues[] = {
		{ "#ArmImg", &(lf.armImg), typeid(std::string) },
		{ "#ArmRotateX", &(lf.armRotateX), typeid(float) },
		{ "#ArmRotateY", &(lf.armRotateY), typeid(float) },
		{ "#ArmOffsetX", &(lf.armX), typeid(float) },
		{ "#ArmOffsetY", &(lf.armY), typeid(float) },
		{ "#ChainImg", &(lf.chainImg), typeid(std::string) },
		{ "#GravityAcc", &(lf.GRAVITY_ACC), typeid(float) },
		{ "#MaxGravity", &(lf.MAX_G), typeid(float) },
		{ "#Tension", &(lf.TENSION), typeid(float) },
		{ "#DownTension", &(lf.DOWN_TENSION), typeid(float) },
		{ "#DownSpeed", &(lf.DOWN_SPEED), typeid(float) },
		{ "#MaxVelocityX", &(lf.MAX_VX), typeid(float) },
		{ "#MaxVelocityY", &(lf.MAX_VY), typeid(float) },
		{ "#ChainTime1", &(lf.CHAIN_TIME[0]), typeid(float) },
		{ "#ChainTime2", &(lf.CHAIN_TIME[1]), typeid(float) },
		{ "#InvincibleTime", &(lf.INV_TIME), typeid(float) },
		{ "#Health", &(lf.health), typeid(int) },
		{ "#Power", &(lf.power), typeid(int) },
		{ "#AttackRadius", &(lf.attackRadius), typeid(float) },
		//{ "#MaxVelocityX", &(lf.MAX_VX) , typeid(float)},
	};
	for (auto& lv : loadValues)
	{
		success = FindChunk(SeekSet(f), lv.tag);
		if (success)
		{
			if (lv.inf == typeid(int))
				f >> *((int*)(lv).value);
			else if (lv.inf == typeid(float))
				f >> *((float*)(lv).value);
			else if (lv.inf == typeid(std::string))
				f >> *((std::string*)(lv).value);
			//else if (lv.inf == typeid(ttt))
			//	f >> *((ttt*)(lv).value);
		}
		else break;
	}
	//*/
	/*
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
	if (success && (success = FindChunk(SeekSet(f), "#Health")))
	{
		f >> lf.health;
	}
	if (success && (success = FindChunk(SeekSet(f), "#Power")))
	{
		f >> lf.power;
	}
	//*/

	if (success)
	{
		//-----------------------------------------------
		// 全ての情報が正しく読み込めた際のみここに入る
		// パラメータ
		tmp.SetInfo(lf);
		// 代入を用いオリジナル複製
		player = tmp;
	}
	else
	{
		debug::BToM("CFileLoader::LoadPlayer load failed");
	}
	return success;
}
//=====================================================================================

//=====================================================================================
bool CFileLoader::LoadEnemiesData(std::vector<EnemyPtr>& enemies)
{
	using common::FindChunk;
	using common::SeekSet;
	//======================================================================================
	// パスファイルから敵DBファイルパスの読み込み
	std::ifstream iniF(iniFile_);
	if (iniF.fail() || (!FindChunk(iniF, "#EnemyFile")))
	{
		debug::BToM("CFileLoader::LoadEnemiesData [iniF]Error path:%s", iniFile_.c_str());
		return false;
	}
	std::string enemyFile;
	iniF >> enemyFile;
	//======================================================================================
	// DBファイルを開く
	std::ifstream f(enemyFile);
	if (f.fail())
	{
		debug::BToM("CFileLoader::LoadEnemiesData [f]Error path:%s", enemyFile.c_str());
		return false;
	}

	// 敵テーブル初期化
	enemies.clear();

	//======================================================================================
	// 敵情報読み込み開始
	if (FindChunk(SeekSet(f), "#Bird"))
	{
		std::string buf;
		f >> buf;
		if (!LoadBird(buf, enemies))
		{
			return false;
		}
	}
	if (FindChunk(SeekSet(f), "#Fairy"))
	{
		std::string buf;
		f >> buf;
		if (!LoadFairy(buf, enemies))
		{
			return false;
		}
	}
	if (FindChunk(SeekSet(f), "#Griffon"))
	{
		std::string buf;
		f >> buf;
		if (!LoadGriffon(buf, enemies))
		{
			return false;
		}
	}
	//======================================================================================
	return true;
}

//=====================================================================================

//=====================================================================================
bool CFileLoader::LoadPickupData(std::vector<PickupPtr>& pickups)
{
	using common::FindChunk;
	using common::SeekSet;
	//======================================================================================
	// パスファイルからpickupDBファイルパスの読み込み
	std::ifstream iniF(iniFile_);
	if (iniF.fail() || (!FindChunk(iniF, "#PickupFile")))
	{
		debug::BToM("CFileLoader::LoadPickupData [iniF]Error path:%s", iniFile_.c_str());
		return false;
	}
	std::string pickupFile;
	iniF >> pickupFile;
	//======================================================================================
	// DBファイルを開く
	std::ifstream f(pickupFile);
	if (f.fail())
	{
		debug::BToM("CFileLoader::LoadPickupData [f]Error path:%s", pickupFile.c_str());
		return false;
	}

	// テーブル初期化
	pickups.clear();

	//======================================================================================
	// 情報読み込み開始
	if (FindChunk(SeekSet(f), "#PickupJewely"))
	{
		std::string buf;
		f >> buf;
		if (!LoadJewely(buf, pickups))
		{
			return false;
		}
		
	}
	/*
	if (FindChunk(SeekSet(f), "#Fairy"))
	{
		std::string buf;
		f >> buf;
		if (!LoadFairy(buf, enemies))
		{
			return;
		}
	}
	//*/
	//======================================================================================
	return true;
}
//=====================================================================================


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