#include "effectMoveStar.h"

#include "define.h"

CEffectMoveStar::CEffectMoveStar(const mymath::Vec3f& pos, const mymath::Vec3f& add, float time, float scale) :
IEffect("EffectMoveStar")
, animCnt_(0.f)
, animTime_(time)
{
	obj_.pos = pos;
	obj_.add = add;
	obj_.scale = scale;

	obj_.resname = "img_effectStar";
	obj_.size(240, 240);

}

void CEffectMoveStar::step()
{
	const int animRow = 8;
	const float animMax = 12.f;

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

void CEffectMoveStar::draw()
{
	graph::Draw_SetRenderMode(ADD);
	for (int i = 0; i<3; ++i)
		obj_.draw();
	graph::Draw_EndRenderMode();
}