#ifndef DEF_EFFECTEXPLOSION_H
#define DEF_EFFECTEXPLOSION_H

#ifndef DEF_EFFECTBASE_HPP
#include "effectBase.hpp"
#endif

#ifndef DEF_MYMATH_HPP
#include "MyMath.hpp"
#endif

/*
	@brief	爆発エフェクト
*/
class CEffectExplosion : public IEffect
{
	float animCnt_;			// アニメーションカウンタ
	float addAngle_;		// 回転描画
public:
	/*
		@brief	爆発発生
		@param	[in]	pos		中心座標
		@param	[in]	scale	拡大率(デフォルト：1.0f倍)
	*/
	CEffectExplosion(const mymath::Vec3f& pos, float scale = 1.f);
	
	void step() override;
	void draw() override;

};


#endif