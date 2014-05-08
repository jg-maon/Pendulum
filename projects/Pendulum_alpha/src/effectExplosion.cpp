#include "effectExplosion.h"
#include "setting.h"

#include "lib\gplib.h"

#include "define.h"

CEffectExplosion::CEffectExplosion(const Vec3f& pos, float scale):
	IEffect("EffectExplosion")
	,animCnt_(0.f)
{
	obj_.pos = pos;
	obj_.scale = scale;

	obj_.img = expIMG;
	obj_.src.x = 500;
	obj_.src.y = 500;

	addAngle_ = math::GetRandom(-3600.f, 3600.f) / 10.f;
	obj_.angle = math::GetRandom(0.f, 3600.f) / 10.f;

	obj_.alpha = 200.f;
	obj_.scale = math::GetRandom(0.9f,1.2f);
	
}

void CEffectExplosion::step()
{
	const float animTime = 0.5f;
	// 0.5•b
	animCnt_ += 12.f / animTime * FrameTime;
	
	// ‰ñ“]
	obj_.angle += addAngle_ / animTime * FrameTime;
	if(obj_.angle >= 360.f)	obj_.angle -= 360.f;
	else if(obj_.angle < 0.f)	obj_.angle += 360.f;

	if(animCnt_ > 14.f)
	{
		kill();
	}
	else
	{
		// ƒAƒjƒ[ƒVƒ‡ƒ“”Ô†‚Ì”½‰f
		obj_.anim = static_cast<int>(animCnt_) % 3;
		obj_.dir = static_cast<int>(animCnt_) / 3;
	}
}

void CEffectExplosion::draw()
{
	Draw_SetRenderMode(ADD);
	for(int i=0; i<2; ++i)
		obj_.draw();
	Draw_EndRenderMode();
}