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
	typedef std::unordered_map<std::string, StagePtr> StageMap;
	StageMap stages_;

	std::string nowStage_;		// ステージ名


public:
	CStageMng();
	virtual void step() override;
	virtual void draw() override;

	/*
		@brief	ステージマネージャーオブジェクトを取得
		@return	ポインタ
		@retval	nullptr	ステージマネージャーオブジェクトが作られていない場合
		@retval	マネージャーのポインタ
	*/
	static const std::shared_ptr<CStageMng> GetPtr();

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
		@param	[in]	stage	ステージ名
		@return	ステージRect
	*/
	const mymath::Recti& getStageRect(const std::string& stage) const;

	/*
		@brief	現在のステージのカメラ可動範囲の取得
		@return	カメラ可動範囲
	*/
	const mymath::Recti& getCameraRect() const;

	/*
		@brief	指定したステージのカメラ可動範囲の取得
		@param	[in]	stage	ステージ名
		@return	カメラ可動範囲
	*/
	const mymath::Recti& getCameraRect(const std::string& stage) const;


	/*
		@brief	アクションポイントの取得
		@return	アクションポイント
	*/
	const std::vector<ActPtPtr>& getActionPoints() const;
};

#endif