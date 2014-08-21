#include "fileLoader.h"

#include "../../../lib/gplib.h"
using namespace gplib;
#include "common.h"

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
		gplib::debug::BToMF("iniFpath:%s", iniFile_.c_str());
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

bool CFileLoader::LoadInfoFunc(const std::string& from, std::ifstream& f, std::vector<CFileLoader::LoadValue>& loadValues)
{
	for (auto& lv : loadValues)
	{
		if (common::FindChunk(common::SeekSet(f), lv.tag))
		{
			if (lv.type == "int")
				f >> *((int*)(lv).value);
			else if (lv.type == "float")
				f >> *((float*)(lv).value);
			else if (lv.type == "string")
				f >> *((std::string*)(lv).value);
			else
			{
				debug::BToM("%s\n%s type error %s", from, lv.tag, lv.type);
				return false;
			}
		}
		else
		{
			debug::BToM("%s\n%s load error", from, lv.tag);
			return false;
		}
	}
	return true;
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
		gplib::debug::BToMF("CFileLoader::LoadRes path:%s", resFile.c_str());
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
		debug::BToMF("CFileLoader::LoadBird path:%s", fileName.c_str());
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
	std::vector<LoadValue> loadValues = {
		{ "#SearchRange", &(lf.searchRange), "float" },
		{ "#ChaseRange", &(lf.chaseRange), "float" },
		{ "#AttackRange", &(lf.attackRange), "float" },
		{ "#ReturnRange", &(lf.returnRange), "float" },
		{ "#MoveSpeed", &(lf.moveSpeed), "float" },
		{ "#AttackInterval", &(lf.attackInterval), "float" },
	};
	success = LoadInfo(eneF, loadValues);
	
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
		debug::BToMF("CFileLoader::LoadBird load failed");
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
		debug::BToMF("CFileLoader::LoadFairy path:%s", fileName.c_str());
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
	std::vector<LoadValue> loadValues = {
		{ "#SearchRange", &(lf.searchRange), "float" },
		{ "#ChaseRange", &(lf.chaseRange), "float" },
		{ "#AttackRange", &(lf.attackRange), "float" },
		{ "#ReturnRange", &(lf.returnRange), "float" },
		{ "#MoveSpeed", &(lf.moveSpeed), "float" },
		{ "#AttackInterval", &(lf.attackInterval), "float" },
		{ "#AttackRatio", &(lf.attackRatio), "float" },
	};
	success = LoadInfo(eneF, loadValues);
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
		debug::BToMF("CFileLoader::LoadFairy load failed");
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
		debug::BToMF("CFileLoader::LoadGriffon path:%s", fileName.c_str());
		return false;
	}
	// 情報ロード用
	CGriffon tmp;
	CGriffon::LoadInfo lf;
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
	std::vector<LoadValue> loadValues = {
		{ "#SearchRange", &(lf.searchRange), "float" },
		{ "#ChaseRange", &(lf.chaseRange), "float" },
		{ "#AttackRange", &(lf.attackRange), "float" },
		{ "#MoveSpeed", &(lf.moveSpeed), "float" },
		{ "#AttackInterval", &(lf.attackInterval), "float" },
		{ "#AttackSpeed", &(lf.attackSpeed), "float" },
		{ "#InvincibleTime", &(lf.invincibleTime), "float" },
		{ "#DamageTime", &(lf.damageTime), "float" },
		{ "#AttackDist", &(lf.tackleDist), "float" },
		{ "#AttackInterval", &(lf.attackInterval), "float" },
		{ "#Health", &(lf.health), "int" },
		{ "#Power", &(lf.power), "int" },
	};
	success = LoadInfo(eneF, loadValues);
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

	return success;
}
//=====================================================================================
//=====================================================================================
//=====================================================================================
//=====================================================================================
bool CFileLoader::LoadRaybit(const std::string& fileName, std::vector<EnemyPtr>& enemies)
{
	using common::FindChunk;
	using common::SeekSet;
	std::ifstream eneF(fileName);
	if (eneF.fail())
	{
		debug::BToMF("eneF open error path:%s", fileName.c_str());
		return false;
	}
	// 情報ロード用
	CRaybit tmp;
	CRaybit::LoadInfo lf;
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
	if (success && (success = FindChunk(SeekSet(eneF), "#SearchRect")))
	{
		auto& rc = lf.searchRect;
		eneF >> rc.left >> rc.top >> rc.right >> rc.bottom;
	}
	if (success && (success = FindChunk(SeekSet(eneF), "#ChaseRect")))
	{
		auto& rc = lf.chaseRect;
		eneF >> rc.left >> rc.top >> rc.right >> rc.bottom;
	}
	if (success && (success = FindChunk(SeekSet(eneF), "#AttackRect")))
	{
		auto& rc = lf.attackRect;
		eneF >> rc.left >> rc.top >> rc.right >> rc.bottom;
	}
	//-------------------------------------
	// その他
	std::vector<LoadValue> loadValues = {
		{ "#MoveSpeed", &(lf.moveSpeed), "float" },
		{ "#MoveInterval", &(lf.moveInterval), "float" },
		{ "#MoveTime", &(lf.moveTime), "float" },
		{ "#AttackInterval", &(lf.attackInterval), "float" },
		{ "#AttackNum", &(lf.attackNum), "int" },
		{ "#OneAttackInterval", &(lf.oneAttackInterval), "float" },
	};
	success = LoadInfo(eneF, loadValues);

	if (success && (success = FindChunk(SeekSet(eneF), "#AttackPos1")))
	{
		auto& pos = lf.attackPos[0];
		eneF >> pos.x >> pos.y;
	}
	if (success && (success = FindChunk(SeekSet(eneF), "#AttackPos2")))
	{
		auto& pos = lf.attackPos[1];
		eneF >> pos.x >> pos.y;
	}

	//-------------------------------------

	if (success)
	{
		//-----------------------------------------------
		// 全ての情報が正しく読み込めた際のみここに入る
		// パラメータ
		tmp.SetInfo(lf);

		// コピーコンストラクタを用いオリジナル作成
		enemies.push_back(EnemyPtr(new CRaybit(tmp)));
	}
	else
	{
		debug::BToMF("load failed");
	}

	return success;
}
//=====================================================================================
//=====================================================================================
//=====================================================================================
//=====================================================================================
bool CFileLoader::LoadRoboticArm(const std::string& fileName, std::vector<EnemyPtr>& enemies)
{
	using common::FindChunk;
	using common::SeekSet;
	std::ifstream eneF(fileName);
	if (eneF.fail())
	{
		debug::BToMF("eneF open error path:%s", fileName.c_str());
		return false;
	}
	// 情報ロード用
	CRoboticArm tmp;
	CRoboticArm::LoadInfo lf;
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
		//bd.resname = cb.resname;
		//bd.size = cb.size;
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
	if (success && (success = FindChunk(SeekSet(eneF), "#ParentSrc")))
	{
		eneF >> lf.parentSrcPos.x >> lf.parentSrcPos.y >> lf.parentSrcSize.x >> lf.parentSrcSize.y;
	}
	if (success && (success = FindChunk(SeekSet(eneF), "#SupportParent")))
	{
		eneF >> lf.supportParent.x >> lf.supportParent.y;
	}
	if (success && (success = FindChunk(SeekSet(eneF), "#SupportChild")))
	{
		eneF >> lf.supportChild.x >> lf.supportChild.y;
	}
	//-------------------------------------
	std::vector<LoadValue> loadValues = {
		{ "#AttackRange", &(lf.attackRange), "float" },
		{ "#AttackInterval", &(lf.attackInterval), "float" },
		{ "#ParentImg", &(lf.parentResname), "string" },
		{ "#RotateSpeed", &(lf.rotateSpeed), "float" },
		{ "#MaxAngle", &(lf.maxAngle), "float" },
		{ "#MinAngle", &(lf.minAngle), "float" },
	};
	success = LoadInfo(eneF, loadValues);
	

	//-------------------------------------

	if (success)
	{
		//-----------------------------------------------
		// 全ての情報が正しく読み込めた際のみここに入る
		// パラメータ
		tmp.SetInfo(lf);
		tmp.SetArmDirection(CRoboticArm::ArmDirectin::RIGHT_UP);

		// コピーコンストラクタを用いオリジナル作成
		enemies.push_back(EnemyPtr(new CRoboticArm(tmp)));
	}
	else
	{
		debug::BToMF("load failed");
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
		debug::BToMF("CFileLoader::LoadJewely path:%s", fileName.c_str());
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
		debug::BToMF("CFileLoader::LoadJewelyload failed");
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
	if (iniF.fail())
	{
		debug::BToMF("CFileLoader::LoadPlayerData [iniF]Error path:%s", iniFile_.c_str());
		return false;
	}
	if (!FindChunk(iniF, "#PlayerFile"))
	{
		debug::BToMF("#PlayerFile tag not found");
		return false;
	}
	std::string playerFile;
	iniF >> playerFile;
	//======================================================================================
	// DBファイルを開く
	std::ifstream f(playerFile);
	if (f.fail())
	{
		debug::BToMF("CFileLoader::LoadPlayerData [f]Error path:%s", playerFile.c_str());
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


	//*
	std::vector<LoadValue> loadValues = {
		{ "#ArmImg", &(lf.armImg), "string" },
		{ "#ArmRotateX", &(lf.armRotateX), "float" },
		{ "#ArmRotateY", &(lf.armRotateY), "float" },
		{ "#ArmOffsetX", &(lf.armX), "float" },
		{ "#ArmOffsetY", &(lf.armY), "float" },
		{ "#ChainImg", &(lf.chainImg), "string" },
		{ "#MaxGravity", &(lf.MAX_G), "float" },
		{ "#Tension", &(lf.TENSION), "float" },
		{ "#DownTension", &(lf.DOWN_TENSION), "float" },
		{ "#DownSpeed", &(lf.DOWN_SPEED), "float" },
		{ "#MaxVelocityX", &(lf.MAX_VX), "float" },
		{ "#MaxVelocityY", &(lf.MAX_VY), "float" },
		{ "#ChainTime1", &(lf.CHAIN_TIME[0]), "float" },
		{ "#ChainTime2", &(lf.CHAIN_TIME[1]), "float" },
		{ "#InvincibleTime", &(lf.invincibleTime), "float" },
		{ "#Health", &(lf.health), "int" },
		{ "#Power", &(lf.power), "int" },
		{ "#AttackRadius", &(lf.maxAttackRadius), "float" },
		{ "#AddAttackRadius", &(lf.addRadius), "float" },
	};
	success = LoadInfo(f, loadValues);
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
		debug::BToMF("CFileLoader::LoadPlayer load failed");
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
	if (iniF.fail())
	{
		debug::BToMF("CFileLoader::LoadEnemiesData [iniF]Error path:%s", iniFile_.c_str());
		return false;
	}
	if (!FindChunk(iniF, "#EnemyFile"))
	{
		debug::BToMF("#EnemyFile tag not found");
		return false;
	}
	std::string enemyFile;
	iniF >> enemyFile;
	//======================================================================================
	// DBファイルを開く
	std::ifstream f(enemyFile);
	if (f.fail())
	{
		debug::BToMF("CFileLoader::LoadEnemiesData [f]Error path:%s", enemyFile.c_str());
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
	if (FindChunk(SeekSet(f), "#Raybit"))
	{
		std::string buf;
		f >> buf;
		if (!LoadRaybit(buf, enemies))
		{
			return false;
		}
	}
	if (FindChunk(SeekSet(f), "#RoboticArm"))
	{
		std::string buf;
		f >> buf;
		if (!LoadRoboticArm(buf, enemies))
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
	if (iniF.fail())
	{
		debug::BToMF("CFileLoader::LoadPickupData [iniF]Error path:%s", iniFile_.c_str());
		return false;
	}
	if (!FindChunk(iniF, "#PickupFile"))
	{
		debug::BToMF("#PickupFile tag not found");
		return false;
	}
	std::string pickupFile;
	iniF >> pickupFile;
	//======================================================================================
	// DBファイルを開く
	std::ifstream f(pickupFile);
	if (f.fail())
	{
		debug::BToMF("CFileLoader::LoadPickupData [f]Error path:%s", pickupFile.c_str());
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


std::string CFileLoader::GetPath(const std::string& tag) const
{
	std::ifstream iniF(iniFile_);
	if (iniF.fail())
	{
		gplib::debug::BToMF("CFileLoader::GetFile iniFpath:%s", iniFile_.c_str());
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