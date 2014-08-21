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
	// ヒットストップ中はスキップ
	if (!isUpdatable())
		return;

	const int animRow = 8;
	const float animMax = 30.f;

	animCnt_ += animMax / animTime_ * system::FrameTime;

	// 移動
	obj_.Move();

	// アニメーション
	if (animCnt_ >= animMax)
	{
		kill();
	}
	else
	{
		// アニメーション番号の反映
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