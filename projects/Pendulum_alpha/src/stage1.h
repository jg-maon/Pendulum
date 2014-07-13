#ifndef DEF_STAGE1_H
#define DEF_STAGE1_H

#ifndef DEF_STAGEBASE_H
#include "stageBase.h"
#endif

#ifndef DEF_STAGEMNG_H
#include "stageMng.h"
#endif

#include <memory>
#include <fstream>

/*
	@brief	ステージ1　森
			ステージクリア条件はゴール形式
*/
class CStage1 : public IStage
{
private:
	std::weak_ptr<CStageMng> sm_;		// ステージマネージャへの参照

	enum class ClearAnnouncePhase
	{
		TO_GOAL,			// ゴール表示
		WAIT,				// ウェイト
		TO_PLAYER,			// プレイヤー表示
		MSGIN,				// 条件表示
		MSGOUT,				// 条件非表示

	}caPhase_;
	mymath::Vec3f caStartPos_;			// クリア条件表示オブジェクト初期座標

	mymath::ShapefPtr goalArea_;		// ゴールエリア
	charabase::CharPtr goalObj_;		// ゴールオブジェクト


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