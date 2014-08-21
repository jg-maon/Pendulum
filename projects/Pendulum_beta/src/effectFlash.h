#ifndef DEF_EFFECTFLASH_H
#define DEF_EFFECTFLASH_H

#ifndef DEF_EFFECTBASE_HPP
#include "effectBase.hpp"
#endif

#ifndef DEF_MYMATH_HPP
#include "MyMath.hpp"
#endif

/*
	@brief	光拡散エフェクト
*/
class CEffectFlash : public IEffect
{
	float animCnt_;			// アニメーションカウンタ
	float animTime_;		// アニメーション時間
public:
	/*
		@brief	爆発発生
		@param	[in]	pos		中心座標
		@param	[in]	add		移動量
		@param	[in]	time	時間
		@param	[in]	scale	拡大率(デフォルト：1.0f倍)
	*/
	CEffectFlash(const mymath::Vec3f& pos, const mymath::Vec3f& add, float time, float scale = 1.f);

	void step() override;
	void draw() override;

};


#endif