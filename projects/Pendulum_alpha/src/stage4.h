#ifndef DEF_STAGE4_H
#define DEF_STAGE4_H

#ifndef DEF_STAGEBASE_HPP
#include "stageBase.hpp"
#endif

#include <fstream>

/*
	@brief	ステージ4　
			隠しステージ(遺跡→工場→森)
*/
class CStage4 : public IStage
{
public:
	/*
		@brief	ステージのロード
		@param	[in/out]	f	ステージファイル
		@attension	fはオープン済み
	*/
	CStage4(std::ifstream& f);
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