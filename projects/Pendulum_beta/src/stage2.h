#ifndef DEF_STAGE2_H
#define DEF_STAGE2_H

#ifndef DEF_STAGEBASE_H
#include "stageBase.h"
#endif

#ifndef DEF_STAGEMNG_H
#include "stageMng.h"
#endif

#include <fstream>

/*
	@brief	ステージ2　工場
			ステージクリア条件は殲滅
*/
class CStage2 : public IStage
{
private:
	std::weak_ptr<CStageMng> sm_;		// ステージマネージャへの参照

	enum class ClearAnnouncePhase
	{
		WAIT,				// ウェイト
		LEFTTOP,			// ↑へ
		RIGHTTOP,			// →↑へ
		RIGHTBOTTOM,		// ↓へ
		LEFTBOTTOM,			// ←へ
		TO_PLAYER,			// プレイヤーへ
		MSGIN,				// 条件表示
		MSGOUT,				// 条件非表示

	}caPhase_;
	mymath::Vec3f caStartPos_;			// クリア条件表示オブジェクト初期座標

	int clearNorma_;					// クリアノルマ敵数

	mymath::ShapefPtr goalArea_;		// ゴールエリア
	charabase::CharPtr goalObj_;		// ゴールオブジェクト

	std::string bossFile_;

	float announceTime_;				// クリア条件表示中タイマー

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
		@brief	ステージのロード
		@param	[in/out]	f	ステージファイル
		@attension	fはオープン済み
	*/
	CStage2(std::ifstream& f);
	/*
		@brief	更新処理
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