#ifndef DEF_STAGE3_H
#define DEF_STAGE3_H

#ifndef DEF_STAGEBASE_H
#include "stageBase.h"
#endif

#include <fstream>

/*
	@brief	ステージ3　遺跡
*/
class CStage3 : public IStage
{
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
	CStage3(std::ifstream& f);
	virtual void step() override;
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