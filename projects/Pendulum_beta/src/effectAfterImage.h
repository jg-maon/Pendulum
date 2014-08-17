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
		float addScale_;	// 拡縮率
		float decrease_;	// 減少量
		int renderMode_;	// 描画方法
	public:
		CAfterImageObject() :
			IObject("AfterImageObject")
		{}
		/*
			@brief	CharBase一時変換用
		*/
		CAfterImageObject(const charabase::CharBase& obj, bool turnFlag) :
			IObject("AfterImageObject")
		{
			obj_ = obj;
			turnFlag_ = turnFlag;
		}

		/*
			@brief	vector登録用
		*/
		CAfterImageObject(const std::weak_ptr<IObject>& obj, bool turnFlag, float dec, int renderMode, float scale) :
			IObject("AfterImageObject")
			, decrease_(dec)
			, renderMode_(renderMode)
			, addScale_(scale)
		{
			obj_ = obj.lock()->obj();
			turnFlag_ = turnFlag;
		}

		void step() override
		{
			__super::step();
			obj_.scale += addScale_;
			obj_.alpha -= decrease_;
			if (obj_.alpha < 0.f)
				obj_.SetUse(false);
		}
		void draw() override
		{
			graph::Draw_SetRenderMode(renderMode_);
			__super::draw();
			graph::Draw_EndRenderMode();
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
		@param	[in]	renderMode	描画方法
		@param	[in]	alpha		最大不透明度
		@param	[in]	time		最大不透明度が0になるまでの時間
		@param	[in]	scale		最終拡大率
	*/
	void Create(const std::weak_ptr<IObject>& obj, const mymath::Vec3f& sta, const mymath::Vec3f& end, int num, int renderMode, float alpha, float time, float scale);


public:
	/*
		@brief	エフェクト発生
		@param	[in]	obj			残像用元オブジェクト
		@param	[in]	turnFlag	オブジェクト反転フラグ
		@param	[in]	sta			始点中心座標
		@param	[in]	end			終点中心座標
		@param	[in]	num			補間数
		@param	[in]	renderMode	描画方法(デフォルト:半透明描画)
		@param	[in]	alpha		最大不透明度(デフォルト:220.f)
		@param	[in]	time		最大不透明度が0になるまでの時間(デフォルト:1秒で消滅)
		@param	[in]	scale		最終拡大率(デフォルト1.f)		
	*/
	CEffectAfterImage(const charabase::CharBase& obj, bool turnFlag, const mymath::Vec3f& sta, const mymath::Vec3f& end, int num, int renderMode = gplib::RENDER_MODE::NORMAL, float alpha = 220.f, float time = 1.f, float scale = 1.f);
	/*
		@brief	エフェクト発生
		@param	[in]	obj			残像用元オブジェクト
		@param	[in]	sta			始点中心座標
		@param	[in]	end			終点中心座標
		@param	[in]	num			補間数
		@param	[in]	renderMode	描画方法(デフォルト:半透明描画)
		@param	[in]	alpha		最大不透明度(デフォルト:220.f)
		@param	[in]	time		最大不透明度が0になるまでの時間(デフォルト:1秒で消滅)
		@param	[in]	scale		最終拡大率(デフォルト1.f)
	*/
	CEffectAfterImage(const std::weak_ptr<IObject>& obj, const mymath::Vec3f& sta, const mymath::Vec3f& end, int num, int renderMode = gplib::RENDER_MODE::NORMAL, float alpha = 220.f, float time = 1.f, float scale = 1.f);

	void step() override;
	void draw() override;

};


#endif