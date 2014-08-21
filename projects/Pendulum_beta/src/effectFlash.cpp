#include "effectFlash.h"

#include "define.h"

CEffectFlash::CEffectFlash(const mymath::Vec3f& pos, const mymath::Vec3f& add, float time, float scale) :
IEffect("EffectFlash")
, animCnt_(0.f)
, animTime_(time)
{
	obj_.pos = pos;
	obj_.add = add;
	obj_.scale = scale;
	obj_.angle = mymath::Vec3f::Angle2(mymath::Vec3f(), add);

	obj_.resname = "img_effectFlash";
	obj_.size(128, 128);

}

void CEffectFlash::step()
{
	// �q�b�g�X�g�b�v���̓X�L�b�v
	if (!isUpdatable())
		return;

	const int animRow = 8;
	const float animMax = 30.f;

	animCnt_ += animMax / animTime_ * system::FrameTime;

	// �ړ�
	obj_.Move();

	// �A�j���[�V����
	if (animCnt_ >= animMax)
	{
		kill();
	}
	else
	{
		// �A�j���[�V�����ԍ��̔��f
		obj_.src.x = static_cast<int>(animCnt_) % animRow;
		obj_.src.y = static_cast<int>(animCnt_) / animRow;
	}
}

void CEffectFlash::draw()
{
	graph::Draw_SetRenderMode(ADD);
	//for (int i = 0; i<3; ++i)
		obj_.draw();
	graph::Draw_EndRenderMode();
}