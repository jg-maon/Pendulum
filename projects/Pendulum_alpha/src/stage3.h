#ifndef DEF_STAGE3_H
#define DEF_STAGE3_H

#ifndef DEF_STAGEBASE_HPP
#include "stageBase.hpp"
#endif

#include <fstream>

/*
	@brief	ステージ3　遺跡
*/
class CStage3 : public IStage
{
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