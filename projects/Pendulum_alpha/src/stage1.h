#ifndef DEF_STAGE1_H
#define DEF_STAGE1_H

#ifndef DEF_STAGEBASE_HPP
#include "stageBase.hpp"
#endif

#include <fstream>

/*
	@brief	ステージ1　森
			ステージクリア条件はゴール形式
*/
class CStage1 : public IStage
{
private:
	enum class ClearAnnouncePhase
	{
		TO_GOAL,			// ゴール表示
		WAIT,				// ウェイト
		TO_PLAYER,			// プレイヤー表示

	}caPhase_;
	mymath::ShapefPtr goalArea_;		// ゴールエリア
	charabase::CharBase goalObj_;		// ゴールオブジェクト

	float announceTime_;				// クリア条件表示中タイマー
	//mymath::Vec3f playerPos_;			// プレイヤースタート座標
private:
	/*
		@brief		クリア条件のロード
		@attension	fはオープン済み
		@param		[in/out]	f		ステージファイル
		@param		[out]		area	ゴールエリア格納
		@return		なし
	*/
	void LoadClear(std::ifstream& f, mymath::ShapefPtr& area);

protected:
	
	/*
		@brief	クリア条件表示更新処理
		@return	クリア条件表示終了
		@retval	true	クリア条件表示終了
		@retval	false	クリア条件表示継続
	*/
	virtual bool UpdateClearAnnounce() override;

	/*
		@brief	雑魚ステージ更新処理
		@return	ステージ終了
		@retval	true	ステージ終了
		@retval	false	ステージ継続
	*/
	virtual bool UpdateNormal() override;

	/*
		@brief	ボスステージ更新処理
		@return	ステージ終了
		@retval	true	ステージ終了
		@retval	false	ステージ継続
	*/
	virtual bool UpdateBoss() override;

public:
	/*
		@brief		ステージのロード
		@attension	fはオープン済み
		@param		[in/out]	f	ステージファイル
	*/
	CStage1(std::ifstream& f);
	/*
		@brief	ActionPoint、多重スクロール更新処理
		@return	なし
	*/
	virtual void step() override;
	/*
		@brief	描画処理
		@return	なし
	*/
	virtual void draw() override;

	/*
		@brief	ステージ読み込み(初期化)
		@param	[in/out]	f	ステージファイル
		@attension	fはオープン済み
		@return	なし
	*/
	virtual void init(std::ifstream& f) override;
};


#endif