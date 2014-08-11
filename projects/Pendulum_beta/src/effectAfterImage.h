#ifndef DEF_EFFECTAFTERIMAGE_H
#define DEF_EFFECTAFTERIMAGE_H

#ifndef DEF_EFFECTBASE_HPP
#include "effectBase.hpp"
#endif

#ifndef DEF_MYMATH_HPP
#include "MyMath.hpp"
#endif


/*
	@brief	残像
*/
class CEffectAfterImage : public IEffect
{
private:
	/*
		@brief	一個あたりのオブジェクト
	*/
	class CAfterImageObject : public IObject
	{
	private:
		float decrease_;	// 減少量
	public:
		CAfterImageObject() :
			IObject("AfterImageObject")
		{}
		CAfterImageObject(const charabase::CharBase& obj, bool turnFlag) :
			IObject("AfterImageObject")
		{
			obj_ = obj;
			turnFlag_ = turnFlag;
		}
		CAfterImageObject(const std::weak_ptr<IObject>& obj, bool turnFlag, float dec) :
			IObject("AfterImageObject")
			, decrease_(dec)
		{
			obj_ = obj.lock()->obj();
			turnFlag_ = turnFlag;
		}

		void step() override
		{
			__super::step();
			obj_.alpha -= decrease_;
			if (obj_.alpha < 0.f)
				obj_.SetUse(false);
		}
	};
private:
	std::vector<CAfterImageObject> images_;	// 残像

	/*
		@brief	エフェクト発生
		@param	[in]	obj			残像用元オブジェクト
		@param	[in]	sta			始点中心座標
		@param	[in]	end			終点中心座標
		@param	[in]	num			補間数
		@param	[in]	alpha		最大不透明度(デフォルト:220.f)
		@param	[in]	dec			減少量(デフォルト:1秒で消滅)
	*/
	void Create(const std::weak_ptr<IObject>& obj, const mymath::Vec3f& sta, const mymath::Vec3f& end, int num, float alpha = 220.f, float dec = -1.f);


public:
	/*
		@brief	エフェクト発生
		@param	[in]	obj			残像用元オブジェクト
		@param	[in]	turnFlag	オブジェクト反転フラグ
		@param	[in]	sta			始点中心座標
		@param	[in]	end			終点中心座標
		@param	[in]	num			補間数
		@param	[in]	alpha		最大不透明度(デフォルト:220.f)
		@param	[in]	dec			減少量(デフォルト:1秒で消滅)
	*/
	CEffectAfterImage(const charabase::CharBase& obj, bool turnFlag, const mymath::Vec3f& sta, const mymath::Vec3f& end, int num, float alpha = 220.f, float dec = -1.f);
	/*
		@brief	エフェクト発生
		@param	[in]	obj			残像用元オブジェクト
		@param	[in]	sta			始点中心座標
		@param	[in]	end			終点中心座標
		@param	[in]	num			補間数
		@param	[in]	alpha		最大不透明度(デフォルト:220.f)
		@param	[in]	dec			減少量(デフォルト:1秒で消滅)
	*/
	CEffectAfterImage(const std::weak_ptr<IObject>& obj, const mymath::Vec3f& sta, const mymath::Vec3f& end, int num, float alpha = 220.f, float dec = -1.f);

	void step() override;
	void draw() override;

};


#endif