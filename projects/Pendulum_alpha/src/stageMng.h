#ifndef DEF_STAGEMNG_H
#define DEF_STAGEMNG_H

#ifndef DEF_ACTIONPOINT_H
#include "actionPoint.h"
#endif

#ifndef DEF_MYMATH_HPP
#include "MyMath.hpp"
#endif

class CStageMng
{
private:
	mymath::Recti stageRect_;			// ステージの大きさ
	std::vector<std::string> backgroundIMG_;			// 背景画像
	std::vector<ActPtPtr> actionPoints_;
	std::string stageName_;		// ステージ名
public:
	const std::vector<ActPtPtr>& actionPoints;
	const mymath::Recti& rect;
private:
	/*
		@brief		ステージサイズの読み込み
		@attension	fはオープン済み
		@param	[in/out]	f	ファイル
		@return	EOFか
		@retval	true	EOF
		@retval	false	EOFでない
	*/
	bool LoadSize(std::ifstream& f);
	/*
		@brief		プレイヤーの読み込み
		@attension	fはオープン済み
		@param	[in/out]	f	ファイル
		@return	EOFか
		@retval	true	EOF
		@retval	false	EOFでない
	*/
	bool LoadPlayer(std::ifstream& f);
	/*
		@brief		敵情報の読み込み
		@attension	fはオープン済み
		@param	[in/out]	f	ファイル
		@return	EOFか
		@retval	true	EOF
		@retval	false	EOFでない
	*/
	bool LoadEnemies(std::ifstream& f);

	/*
		@brief		ActionCircleの読み込み
		@attension	fはオープン済み
		@param	[in/out]	f	ファイル
		@return	EOFか
		@retval	true	EOF
		@retval	false	EOFでない
	*/
	bool LoadActionCircles(std::ifstream& f);
	/*
		@brief		ActionPolygonの読み込み
		@attension	fはオープン済み
		@param	[in/out]	f	ファイル
		@return	EOFか
		@retval	true	EOF
		@retval	false	EOFでない
	*/
	bool LoadActionPolygons(std::ifstream& f);

public:
	CStageMng();
	void step();
	void draw();

	/*
		@brief	ステージのロード
		@param	[in]	stageName	ステージ名(ファイル名)
		@return	なし
	*/
	void LoadStage(const std::string& stageName);
};

#endif