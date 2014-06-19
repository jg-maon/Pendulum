#ifndef DEF_STAGEMNG_H
#define DEF_STAGEMNG_H

#ifndef DEF_OBJ_H
#include "obj.h"
#endif

#ifndef DEF_STAGEBASE_HPP
#include "stageBase.hpp"
#endif

#ifndef DEF_MYMATH_HPP
#include "MyMath.hpp"
#endif


class CStageMng : public Base
{
private:
	
	typedef std::shared_ptr<IStage> StagePtr;
	std::vector<StagePtr> stages_;

	std::string stageName_;		// ステージ名



public:
	CStageMng();
	virtual void step() override;
	virtual void draw() override;

	/*
		@brief	ステージのロード
		@param	[in]	stageName	ステージ名(ファイル名)
		@return	なし
	*/
	void LoadStage(const std::string& stageName);

	/*
		@brief	現在のステージサイズの取得
		@return	ステージRect
	*/
	const mymath::Recti& getStageRect() const;

	/*
		@brief	指定したステージのサイズの取得
		@param	[in]	stage	ステージ番号
		@return	ステージRect
	*/
	const mymath::Recti& getStageRect(int stage) const;

	static const std::shared_ptr<CStageMng> GetPtr();
};

#endif