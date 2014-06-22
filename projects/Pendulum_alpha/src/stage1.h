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
	mymath::ShapefPtr goalArea_;		// ゴールエリア
	charabase::CharBase goalObj_;		// ゴールオブジェクト
private:
	/*
		@brief		クリア条件のロード
		@attension	fはオープン済み
		@param		[in/out]	f		ステージファイル
		@param		[out]		area	ゴールエリア格納
		@return		なし
	*/
	void LoadClear(std::ifstream& f, mymath::ShapefPtr& area);
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