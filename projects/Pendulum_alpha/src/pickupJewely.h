#ifndef DEF_PICKUPJEWELY_H
#define DEF_PICKUPJEWELY_H

#ifndef DFE_PICKUPBASE_HPP
#include "pickupBase.hpp"
#endif

class CPickupJewely : public IPickup
{
private:
	int score_;					// 一個あたりのスコア

	mymath::Vec3f startPos_;	// 出現初期座標
	
private:
	/*
		@brief	初期化
		@param	[in]	pos		座標
		@param	[in]	score	スコア
		@param	[in]	type	種類
		return	なし
	*/
	void init(const mymath::Vec3f& pos, int score, int type);
public:
	/*
		@brief	DB追加用
	*/
	CPickupJewely();
	/*
		@brief	指定した座標に生成
		@param	[in]	pos		座標
		@param	[in]	score	スコア(デフォルト:100)
		@param	[in]	type	種類
	*/
	CPickupJewely(const mymath::Vec3f& pos, int score = 100, int type = -1);

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
		@brief	オブジェクト同士の重なり処理
		@param	[in]	rival	重なっている相手オブジェクト
		@return	なし
	*/
	virtual void hit(const ObjPtr& rival) override;

	/*
		@brief	スコア取得
		@return	スコア
	*/
	int getScore() const;
};

#endif