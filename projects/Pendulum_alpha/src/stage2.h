#ifndef DEF_STAGE2_H
#define DEF_STAGE2_H

#ifndef DEF_STAGEBASE_HPP
#include "stageBase.hpp"
#endif

#include <fstream>

/*
	@brief	ステージ2　工場
*/
class CStage2 : public IStage
{
public:
	/*
		@brief	ステージのロード
		@param	[in/out]	f	ステージファイル
		@attension	fはオープン済み
	*/
	CStage2(std::ifstream& f);
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