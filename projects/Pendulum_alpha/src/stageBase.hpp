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
protected:
	mymath::Recti cameraRect_;					// カメラの可動範囲
	mymath::Recti stageRect_;					// ステージの大きさ
	std::vector<std::string> backgroundIMG_;	// 背景画像
	std::vector<ActPtPtr> actionPoints_;

public:
	const std::vector<ActPtPtr>& actionPoints;
	const mymath::Recti& rect;
	const mymath::Recti& cameraRect;


private:
#pragma region private methods
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
		// ステージ
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
			InsertObject(ObjPtr(new CPlayer(pos[0], pos[1])));
			
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
			auto& ems = gm()->GetObjects("EnemyMng");
			ObjPtr em;
			if (ems.empty())
			{
				// EnemyMngがない場合、新規に追加する
				em = ObjPtr(new CEnemyMng());
				gm()->AddObject(em);
			}
			else
			{
				em = ems[0];
			}
			std::dynamic_pointer_cast<CEnemyMng>(em)->LoadEnemyTable(tableFile);
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
		// 先に登録されているオブジェクトを消してから読み込む
		auto& objs = gm()->GetObjects("Player,Action", ',');
		for (auto& obj : objs)
			obj->kill();


		LoadRect(f);
		LoadPlayer(f);
		LoadEnemies(f);
		LoadActionCircles(f);
		LoadActionPolygons(f);

	}
#pragma endregion	// protected methods
public:
#pragma region public methods
	IStage(const std::string& name) :
		Base(name)
		, actionPoints(actionPoints_)
		, rect(stageRect_)
		, cameraRect(cameraRect_)
	{
		

	}
	virtual void step()override = 0{}
	virtual void draw()override = 0{}

	/*
		@brief	ステージ読み込み(初期化)
		@param	[in/out]	f	ステージファイル
		@attension	fはオープン済み
		@return	なし
	*/
	virtual void init(std::ifstream& f)
	{
		// 先に登録されているオブジェクトを消してから読み込む
		auto& objs = gm()->GetObjects("Player", ',');
		for (auto& obj : objs)
			obj->kill();

		LoadPlayer(f);
		LoadEnemies(f);
	}

#pragma endregion	// public methods
};

#endif