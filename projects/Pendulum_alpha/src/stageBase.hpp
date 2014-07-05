#ifndef DEF_STAGEBASE_HPP
#define DEF_STAGEBASE_HPP

#ifndef DFE_OBJ_H
#include "obj.h"
#endif

#ifndef DEF_ACTIONPOINT_H
#include "actionPoint.h"
#endif

#ifndef DEF_MYMATH_HPP
#include "myMath.hpp"
#endif

#ifndef DEF_COMMON_H
#include "common.h"
#endif

#ifndef DEF_PLAYER_H
#include "player.h"
#endif

#ifndef DEF_ENEMYMNG_H
#include "enemyMng.h"
#endif




#include <vector>
#include <string>
#include <sstream>
#include <fstream>




/*
	@brief	ステージ基底クラス
*/
class IStage : public Base
{
public:
	enum class Phase	// ステージフェーズ
	{
		CLEAR_ANNOUNCE,	// クリア条件表示
		NORMAL,			// 雑魚ステージ
		BOSS,			// ボスステージ
		RESULT,			// リザルト画面へ
	};
	enum class ClearType	// クリア条件
	{
		GOAL,			// ゴール
		ANNIHILATION,	// 殲滅
	};
protected:
	std::string bgm_;							// BGM

	IStage::Phase phase_;						// ステージフェーズ
	IStage::ClearType clearType_;				// クリア条件
	mymath::Recti cameraRect_;					// カメラの可動範囲
	mymath::Recti stageRect_;					// ステージの大きさ
	std::vector<std::string> backgroundIMG_;	// 背景画像
	std::vector<ActPtPtr> actionPoints_;

public:
	const std::string& bgm;

	const std::vector<ActPtPtr>& actionPoints;
	const mymath::Recti& rect;
	const mymath::Recti& cameraRect;


private:
#pragma region private methods

	/*
		@brief		ステージシステム環境の読み込み
		@attension	fはオープン済み
		@param	[in/out]	f	ステージファイル
		@return	EOFか
		@retval	true	EOF
		@retval	false	EOFでない
	*/
	bool LoadEnv(std::ifstream& f)
	{
		//--------------------------------------
		// ステージ
		if (common::FindChunk(common::SeekSet(f), "#Bgm"))
		{
			f >> bgm_;
		}
		return f.eof();
	}

	/*
		@brief		ステージサイズ,カメラ可動範囲の読み込み
		@attension	fはオープン済み
		@param	[in/out]	f	ステージファイル
		@return	EOFか
		@retval	true	EOF
		@retval	false	EOFでない
	*/
	bool LoadRect(std::ifstream& f)
	{
		//--------------------------------------
		// ステージ
		if (common::FindChunk(common::SeekSet(f), "#Left"))
		{
			f >> stageRect_.left;
		}
		if (common::FindChunk(common::SeekSet(f), "#Top"))
		{
			f >> stageRect_.top;
		}
		if (common::FindChunk(common::SeekSet(f), "#Right"))
		{
			f >> stageRect_.right;
		}
		if (common::FindChunk(common::SeekSet(f), "#Bottom"))
		{
			f >> stageRect_.bottom;
		}
		//--------------------------------------
		// カメラ
		if (common::FindChunk(common::SeekSet(f), "#CameraLeft"))
		{
			f >> cameraRect_.left;
		}
		if (common::FindChunk(common::SeekSet(f), "#CameraTop"))
		{
			f >> cameraRect_.top;
		}
		if (common::FindChunk(common::SeekSet(f), "#CameraRight"))
		{
			f >> cameraRect_.right;
		}
		if (common::FindChunk(common::SeekSet(f), "#CameraBottom"))
		{
			f >> cameraRect_.bottom;
		}
		return f.eof();
	}
	/*
		@brief		プレイヤーの読み込み
		@attension	fはオープン済み
		@param	[in/out]	f	ステージファイル
		@return	EOFか
		@retval	true	EOF
		@retval	false	EOFでない
	*/
	bool LoadPlayer(std::ifstream& f)
	{
		if (common::FindChunk(common::SeekSet(f), "#Player"))
		{
			// プレイヤー座標を取得
			float pos[2];	// [0]:x [1]:y
			for (auto& p : pos)
			{
				std::string label;
				f >> label;
				p = static_cast<float>(std::atof(label.c_str()));
			}
			// プレイヤーオブジェクトを追加
			auto& pl = gm()->GetObj(typeid(CPlayer));
			std::shared_ptr<CPlayer> player;
			if (!pl)
			{
				player = std::shared_ptr<CPlayer>(new CPlayer(pos[0], pos[1]));
				gm()->AddObject(player);
			}
			else
			{
				// 多重登録防止
				player = std::dynamic_pointer_cast<CPlayer>(pl);
				player->init(pos[0], pos[1], player->obj().pos.z);
			}
#ifdef DEF_GM_PTR
			gm()->SetPlayerPtr(player);
#endif
			
		}

		return f.eof();
	}
	/*
		@brief		敵情報の読み込み
		@attension	fはオープン済み
		@param	[in/out]	f	ステージファイル
		@return	EOFか
		@retval	true	EOF
		@retval	false	EOFでない
	*/
	bool LoadEnemies(std::ifstream& f)
	{
		if (common::FindChunk(common::SeekSet(f), "#Enemy"))
		{
			std::string tableFile;
			f >> tableFile;
			// プレイヤーオブジェクトを追加
			auto& pEm = gm()->GetObj(typeid(CEnemyMng));
			std::shared_ptr<CEnemyMng> em;
			if (!pEm)
			{
				// EnemyMngがない場合、新規に追加する
				em = std::shared_ptr<CEnemyMng>(new CEnemyMng());
				gm()->AddObject(em);
			}
			else
			{
				// 多重登録防止
				em = std::dynamic_pointer_cast<CEnemyMng>(pEm);
			}
			em->LoadEnemyTable(tableFile);
#ifdef DEF_GM_PTR
			gm()->SetEnemyMngPtr(em);
#endif
		}
		return f.eof();
	}

	/*
		@brief		ActionCircleの読み込み
		@attension	fはオープン済み
		@param	[in/out]	f	ステージファイル
		@return	EOFか
		@retval	true	EOF
		@retval	false	EOFでない
	*/
	bool LoadActionCircles(std::ifstream& f)
	{
		if (common::FindChunk(common::SeekSet(f), "#ActionCircle"))
		{
			std::string label;
			f >> label;
			if (label != "{" || f.eof()) return f.eof();
			while (!f.eof())
			{
				std::vector<float> info;
				for (int i = 0; i<3; ++i)
				{
					f >> label;
					if (label == "}" || f.eof())return f.eof();
					info.push_back(static_cast<float>(std::atof(label.c_str())));
				}
				if (info.size() == 3)
				{
					actionPoints_.push_back(ActPtPtr(
						new CActionCircle(info[0], info[1], info[2])
						));
				}
			}
		}
		return f.eof();
	}
	/*
		@brief		ActionPolygonの読み込み
		@attension	fはオープン済み
		@param	[in/out]	f	ステージファイル
		@return	EOFか
		@retval	true	EOF
		@retval	false	EOFでない
	*/
	bool LoadActionPolygons(std::ifstream& f)
	{
		if (common::FindChunk(common::SeekSet(f), "#ActionPolygon"))
		{
			std::string label;
			f >> label;
			if (label != "{" || f.eof()) return f.eof();
			while (!f.eof())
			{
				f >> label;
				if (label == "}")break;
				int num = std::atoi(label.c_str());
				std::vector<mymath::Vec3f> info(num);
				for (int i = 0; i<num; ++i)
				{
					mymath::Vec3f pos;
					f >> label;
					if (label == "}" || f.eof()) return f.eof();
					pos.x = static_cast<float>(std::atof(label.c_str()));
					f >> label;
					if (label == "}" || f.eof()) return f.eof();
					pos.y = static_cast<float>(std::atof(label.c_str()));
					pos.z = 0.5f;
					info[i] = pos;
				}
				//info.push_back(info[0]);	// 閉路にする
				actionPoints_.push_back(ActPtPtr(
					new CActionPolygon(info)));

			}
		}
		return f.eof();
	}

#pragma endregion	// private methods

protected:
#pragma region protected methods
	/*
		@brief	各種ファイルロード処理
		@param	[in/out]	f	ステージファイル
		@attension	fはオープン済み
		@return	なし
	*/
	void load(std::ifstream& f)
	{
		/*
		// 先に登録されているオブジェクトを消してから読み込む
		auto& objs = gm()->GetObjects("Player,Action", ',');
		for (auto& obj : objs)
			obj->kill();
		LoadPlayer(f);
		LoadEnemies(f);
		//*/

		LoadEnv(f);
		LoadRect(f);
		LoadActionCircles(f);
		LoadActionPolygons(f);
	}

	/*
		@brief	クリア条件表示更新処理
		@return	クリア条件表示終了
		@retval	true	クリア条件表示終了
		@retval	false	クリア条件表示継続
	*/
	virtual bool UpdateClearAnnounce() = 0{ return true; };

	/*
		@brief	雑魚ステージ更新処理
		@return	ステージ終了
		@retval	true	ステージ終了
		@retval	false	ステージ継続
	*/
	virtual bool UpdateNormal() = 0{ return true; }

	/*
		@brief	ボスステージ更新処理
		@return	ステージ終了
		@retval	true	ステージ終了
		@retval	false	ステージ継続
	*/
	virtual bool UpdateBoss() = 0{ return true; }

#pragma endregion	// protected methods
public:
#pragma region public methods
	IStage(const std::string& name) :
		Base(name)
		, phase_(IStage::Phase::CLEAR_ANNOUNCE)
		, bgm(bgm_)
		, actionPoints(actionPoints_)
		, rect(stageRect_)
		, cameraRect(cameraRect_)
	{
		

	}
	/*
		@brief	ステージ共通更新処理
		@return	なし
	*/
	virtual void step() override
	{
		//*
		switch (phase_)
		{
		case IStage::Phase::CLEAR_ANNOUNCE:
			if (UpdateClearAnnounce())
			{
				phase_ = IStage::Phase::NORMAL;
			}
			/*
			switch (clearType_)
			{
			case IStage::ClearType::GOAL:
				break;
			case IStage::ClearType::ANNIHILATION:
				break;
			default:
				break;
			}
			//*/
			break;
		case IStage::Phase::NORMAL:
			for (auto& ap : actionPoints_)
				ap->step();
			if (UpdateNormal())
			{
				phase_ = IStage::Phase::BOSS;
			}
			break;
		case IStage::Phase::BOSS:
			for (auto& ap : actionPoints_)
				ap->step();
			if (UpdateBoss())
			{
				phase_ = IStage::Phase::RESULT;
			}
			break;
		}
		if (input::CheckPush(input::KEY_BTN0))
		{
			phase_ = IStage::Phase::RESULT;
		}
	}
	/*
		@brief	ステージ共通描画処理
		@return	なし
	*/
	virtual void draw() override
	{
		for (auto& ap : actionPoints_)
			ap->draw();
	}

	/*
		@brief	ステージ読み込み(初期化)
		@param	[in/out]	f	ステージファイル
		@attension	fはオープン済み
		@return	なし
	*/
	virtual void init(std::ifstream& f)
	{
		/*
		// 先に登録されているオブジェクトを消してから読み込む
		auto& objs = gm()->GetObjects("Player");
		for (auto& obj : objs)
			obj->kill();
		//*/
		phase_ = IStage::Phase::CLEAR_ANNOUNCE;
		LoadPlayer(f);
		LoadEnemies(f);



	}

	/*
		@brief	ステージが終了したか取得
		@return	ステージ終了フラグ
		@retval	true	ステージ終了
		@retval	false	ステージプレイ継続
	*/
	bool isEndStage() const
	{
		return phase_ == IStage::Phase::RESULT;
	}

#pragma endregion	// public methods
};

#endif