#include "effectAfterImage.h"

#include "define.h"



CEffectAfterImage::CEffectAfterImage(const charabase::CharBase& obj, bool turnFlag, const mymath::Vec3f& sta, const mymath::Vec3f& end, int num, int renderMode, float alpha, float time, float scale) :
IEffect("EffectAfterImage")
{
	obj_ = obj;
	Create(
		std::shared_ptr<IObject>(new CAfterImageObject(obj, turnFlag)),
		sta, end, num, renderMode, alpha, time, scale);
}

CEffectAfterImage::CEffectAfterImage(const std::weak_ptr<IObject>& obj, const mymath::Vec3f& sta, const mymath::Vec3f& end, int num, int renderMode, float alpha, float time, float scale) :
IEffect("EffectAfterImage")
{
	obj_ = obj.lock()->obj();
	Create(obj, sta, end, num, renderMode, alpha, time, scale);
}

void CEffectAfterImage::Create(const std::weak_ptr<IObject>& obj, const mymath::Vec3f& sta, const mymath::Vec3f& end, int num, int renderMode, float alpha, float time, float scale)
{
	bool turnFlag = obj.lock()->isTurn();
	float decrease = alpha * system::ONEFRAME_TIME / time;

	//---------------------------------------------
	// 残像生成
	images_.resize(num);
	mymath::Vec3f dist(end - sta);	// 距離
	for (int i = 0; i < num; ++i)
	{
		// 距離を割合にして位置を計算 staを1番目、endが本体位置
		charabase::CharBase cb = obj.lock()->obj();
		cb.pos = sta + dist * (static_cast<float>(i) / num);
		cb.pos.z = obj_.pos.z;
		// 透明度
		cb.alpha = (alpha*(i + 1)) / num;
		// 拡大率
		float addScale = (scale - 1.f) * system::ONEFRAME_TIME / time;
		
		CAfterImageObject ins(obj, turnFlag, decrease, renderMode, addScale);
		
		ins.obj(cb);

		images_[i] = ins;
	}	
}

void CEffectAfterImage::step()
{
	// ヒットストップ中はスキップ
	if (!isUpdatable())
		return;

	//---------------------------------------
	// 透明度減少
	size_t i = 0;
	for (auto& img : images_)
	{
		img.step();
		if (!img.obj().CheckUse())
			++i;
	}
	
	if (i == images_.size())	// 全残像が消えたら
	{
		kill();		// エフェクターも消す
	}
}

void CEffectAfterImage::draw()
{
	for (auto& img : images_)
	{
		if (img.obj().CheckUse())
			img.draw();
	}
}