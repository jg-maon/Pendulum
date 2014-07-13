#ifndef DEF_STAGEBASE_H
#define DEF_STAGEBASE_H

#ifndef DFE_OBJ_H
#include "obj.h"
#endif


#ifndef DEF_ACTIONPOINT_H
#include "actionPoint.h"
#endif

#ifndef DEF_MYMATH_HPP
#include "myMath.hpp"
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

	charabase::CharPtr caObj_;					// クリア条件表示用オブジェクト

	IStage::Phase phase_;						// ステージフェーズ
	IStage::ClearType clearType_;				// クリア条件
	mymath::Recti cameraRect_;					// カメラの可動範囲
	mymath::Recti stageRect_;					// ステージの大きさ
	std::vector<std::string> backgroundIMG_;	// 背景画像
	std::vector<ActPtPtr> actionPoints_;		// 

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
	bool LoadEnv(std::ifstream& f);

	/*
		@brief		ステージサイズ,カメラ可動範囲の読み込み
		@attension	fはオープン済み
		@param	[in/out]	f	ステージファイル
		@return	EOFか
		@retval	true	EOF
		@retval	false	EOFでない
	*/
	bool LoadRect(std::ifstream& f);
	/*
		@brief		プレイヤーの読み込み
		@attension	fはオープン済み
		@param	[in/out]	f	ステージファイル
		@return	EOFか
		@retval	true	EOF
		@retval	false	EOFでない
	*/
	bool LoadPlayer(std::ifstream& f);
	/*
		@brief		敵情報の読み込み
		@attension	fはオープン済み
		@param	[in/out]	f	ステージファイル
		@return	EOFか
		@retval	true	EOF
		@retval	false	EOFでない
	*/
	bool LoadEnemies(std::ifstream& f);
	/*
		@brief		ActionCircleの読み込み
		@attension	fはオープン済み
		@param	[in/out]	f	ステージファイル
		@return	EOFか
		@retval	true	EOF
		@retval	false	EOFでない
	*/
	bool LoadActionCircles(std::ifstream& f);
	/*
		@brief		ActionPolygonの読み込み
		@attension	fはオープン済み
		@param	[in/out]	f	ステージファイル
		@return	EOFか
		@retval	true	EOF
		@retval	false	EOFでない
	*/
	bool LoadActionPolygons(std::ifstream& f);

#pragma endregion	// private methods

protected:
#pragma region protected methods
	

	/*
		@brief	各種ファイルロード処理
		@param	[in/out]	f	ステージファイル
		@attension	fはオープン済み
		@return	なし
	*/
	void load(std::ifstream& f);
	
	/*
		@brief	クリア条件表示更新処理
		@return	クリア条件表示終了
		@retval	true	クリア条件表示終了
		@retval	false	クリア条件表示継続
	*/
	virtual bool UpdateClearAnnounce() = 0;
	/*
		@brief	雑魚ステージ更新処理
		@return	ステージ終了
		@retval	true	ステージ終了
		@retval	false	ステージ継続
	*/
	virtual bool UpdateNormal() = 0;
	/*
		@brief	ボスステージ更新処理
		@return	ステージ終了
		@retval	true	ステージ終了
		@retval	false	ステージ継続
	*/
	virtual bool UpdateBoss() = 0;
#pragma endregion	// protected methods
public:
#pragma region public methods
	/*
		@brief	ステージ共通項目初期化
		@param	[in]	name	識別名
	*/
	IStage(const std::string& name);

	/*
		@brief	ステージ共通更新処理
		@return	なし
	*/
	virtual void step() override;

	/*
		@brief	ステージ共通描画処理
		@return	なし
	*/
	virtual void draw() override;

	/*
		@brief	ステージ読み込み(初期化)
		@param	[in/out]	f	ステージファイル
		@attension	fはオープン済み
		@return	なし
	*/
	virtual void init(std::ifstream& f);

	/*
		@brief	ステージが終了したか取得
		@return	ステージ終了フラグ
		@retval	true	ステージ終了
		@retval	false	ステージプレイ継続
	*/
	bool isEndStage() const;

#pragma endregion	// public methods
};

#endif