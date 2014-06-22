#include "effectSlash.h"
#include "setting.h"

#include "common.h"

#include "define.h"

CEffectSlash::CEffectSlash(const mymath::Vec3f& pos, float scale):
	IEffect("EffectSlash")
	,animCnt_(0.f)
{
	obj_.pos = pos;
	obj_.scale = scale;

	obj_.resname = "img_slash";
	obj_.size(500, 500);
	

	/*
	addAngle_ = math::GetRandom(-3600.f, 3600.f) / 10.f;
	obj_.angle = math::GetRandom(0.f, 3600.f) / 10.f;

	obj_.alpha = 200.f;
	obj_.scale = math::GetRandom(0.9f,1.2f);
	*/
}

void CEffectSlash::step()
{
	const int animRow = 8;
	const float animMax = 8.f;
	const float animTime = 0.5f;	// 0.5•b

	animCnt_ += animMax / animTime * system::FrameTime;
	
	/*
	// ‰ñ“]
	obj_.angle += addAngle_ / animTime * system::FrameTime;
	if (obj_.angle >= 360.f)	obj_.angle -= 360.f;
	else if (obj_.angle < 0.f)	obj_.angle += 360.f;
	//*/

	if (animCnt_ >= animMax)
	{
		kill();
	}
	else
	{
		// ƒAƒjƒ[ƒVƒ‡ƒ“”Ô†‚Ì”½‰f
		obj_.src.x = static_cast<int>(animCnt_) % animRow;
		obj_.src.y = static_cast<int>(animCnt_) / animRow;
	}
}

void CEffectSlash::draw()
{
	graph:: Draw_SetRenderMode(ADD);
	//for(int i=0; i<1; ++i)
		obj_.draw();
	graph::Draw_EndRenderMode();
}