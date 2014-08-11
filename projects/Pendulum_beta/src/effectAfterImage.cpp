#include "effectAfterImage.h"
#include "setting.h"

#include "common.h"

#include "define.h"



CEffectAfterImage::CEffectAfterImage(const charabase::CharBase& obj, bool turnFlag, const mymath::Vec3f& sta, const mymath::Vec3f& end, int num, float alpha, float dec) :
IEffect("EffectAfterImage")
{
	obj_ = obj;
	Create(
		std::shared_ptr<IObject>(new CAfterImageObject(obj, turnFlag)),
		sta, end, num, alpha, dec);
}

CEffectAfterImage::CEffectAfterImage(const std::weak_ptr<IObject>& obj, const mymath::Vec3f& sta, const mymath::Vec3f& end, int num, float alpha, float dec) :
IEffect("EffectAfterImage")
{
	obj_ = obj.lock()->obj();
	Create(obj, sta, end, num, alpha, dec);
}

void CEffectAfterImage::Create(const std::weak_ptr<IObject>& obj, const mymath::Vec3f& sta, const mymath::Vec3f& end, int num, float alpha, float dec)
{
	bool turnFlag = obj.lock()->isTurn();
	float decrease = dec;
	if (dec < 0.f)
		decrease = alpha * system::ONEFRAME_TIME;

	//---------------------------------------------
	// �c������
	images_.resize(num);
	mymath::Vec3f dist(end - sta);	// ����
	for (int i = 0; i < num; ++i)
	{
		// �����������ɂ��Ĉʒu���v�Z sta��1�ԖځAend���{�̈ʒu
		charabase::CharBase cb = obj.lock()->obj();
		cb.pos = sta + (dist * i) / num;
		cb.pos.z = obj_.pos.z;
		// �����x
		cb.alpha = (alpha*(i + 1)) / num;
		
		CAfterImageObject ins(obj, turnFlag, decrease);
		
		ins.obj(cb);

		images_[i] = ins;
	}	
}

void CEffectAfterImage::step()
{
	// �q�b�g�X�g�b�v���̓X�L�b�v
	if (!isUpdatable())
		return;

	//---------------------------------------
	// �����x����
	size_t i = 0;
	for (auto& img : images_)
	{
		img.step();
		if (!img.obj().CheckUse())
			++i;
	}
	
	if (i == images_.size())	// �S�c������������
	{
		kill();		// �G�t�F�N�^�[������
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