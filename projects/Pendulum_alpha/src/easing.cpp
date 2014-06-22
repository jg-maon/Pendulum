#include "easing.h"
#include "MyMath.hpp"
#include "define.h"

#include <cmath>
#include <algorithm>

using namespace math;
using namespace mymath;

#pragma region Easing methods

//
// さまざまなイージング関数
// t = 経過時間 b = 始点 c = 終点 - 始点 d = 持続時間
//
float Easing::Linear(float t, const float b, const float c, const float d)
{
	t = clamp(t, 0.0f, d);
	return c*t / d + b;
}


float Easing::BackIn(float t, const float b, const float c, const float d)
{
	t = clamp(t, 0.0f, d);

	float s = 1.70158f;
	t /= d;
	return c*t*t*((s + 1.f)*t - s) + b;
}

float Easing::BackOut(float t, const float b, const float c, const float d)
{
	t = clamp(t, 0.0f, d);

	float s = 1.70158f;
	t = t / d - 1.f;
	return c*(t*t*((s + 1.f)*t + s) + 1.f) + b;
}

float Easing::BackInOut(float t, const float b, const float c, const float d)
{
	t = clamp(t, 0.0f, d);

	float s = 1.70158f * 1.525f;
	if ((t /= d / 2.f) < 1.f) return c / 2.f*(t*t*((s + 1.f)*t - s)) + b;
	t -= 2.f;
	return c / 2.f*(t*t*((s + 1.f)*t + s) + 2.f) + b;
}


float Easing::BounceOut(float t, const float b, const float c, const float d)
{
	t = clamp(t, 0.0f, d);

	if ((t /= d) < (1.f / 2.75f))
	{
		return c*(7.5625f*t*t) + b;
	}
	else if (t < (2.f / 2.75f))
	{
		t -= (1.5f / 2.75f);
		return c*(7.5625f*t*t + .75f) + b;
	}
	else if (t < (2.5f / 2.75f))
	{
		t -= (2.25f / 2.75f);
		return c*(7.5625f*t*t + .9375f) + b;
	}
	else {
		t -= (2.625f / 2.75f);
		return c*(7.5625f*t*t + .984375f) + b;
	}
}

float Easing::BounceIn(float t, const float b, const float c, const float d)
{
	t = clamp(t, 0.0f, d);
	return c - Easing::BounceOut(d - t, 0.0f, c, d) + b;
}

float Easing::BounceInOut(float t, const float b, const float c, const float d)
{
	t = clamp(t, 0.0f, d);

	if (t < d / 2.f) return Easing::BounceIn(t*2.f, 0.0f, c, d) * .5f + b;
	else         return Easing::BounceOut(t*2.f - d, 0.0f, c, d) * .5f + c*.5f + b;
}


float Easing::CircIn(float t, const float b, const float c, const float d)
{
	t = clamp(t, 0.0f, d);

	t /= d;
	return -c * (std::sqrtf(1.f - t*t) - 1.f) + b;
}

float Easing::CircOut(float t, const float b, const float c, const float d)
{
	t = clamp(t, 0.0f, d);

	t = t / d - 1.f;
	return c * std::sqrtf(1.f - t*t) + b;
}

float Easing::CircInOut(float t, const float b, const float c, const float d)
{
	t = clamp(t, 0.0f, d);

	if ((t /= d / 2.f) < 1.f) return -c / 2.f * (std::sqrtf(1.f - t*t) - 1.f) + b;
	t -= 2.f;
	return c / 2.f * (std::sqrtf(1.f - t*t) + 1.f) + b;
}


float Easing::CubicIn(float t, const float b, const float c, const float d)
{
	t = clamp(t, 0.0f, d);

	t /= d;
	return c*t*t*t + b;
}

float Easing::CubicOut(float t, const float b, const float c, const float d)
{
	t = clamp(t, 0.0f, d);

	t = t / d - 1.f;
	return c*(t*t*t + 1.f) + b;
}

float Easing::CubicInOut(float t, const float b, const float c, const float d)
{
	t = clamp(t, 0.0f, d);

	if ((t /= d / 2.f) < 1.f) return c / 2.f*t*t*t + b;
	t -= 2.f;
	return c / 2.f*(t*t*t + 2.f) + b;
}


float Easing::ElasticIn(float t, const float b, const float c, const float d)
{
	t = clamp(t, 0.0f, d);

	if (t == 0.f)      return b;
	if ((t /= d) == 1.f) return b + c;
	float p = d*0.3f;

	float a = c;
	float s = p / 4.f;
	t -= 1.f;
	return -(a * std::powf(2.0f, 10.0f*t) * std::sinf((t*d - s)*(2.0f * M_PI) / p)) + b;
}

float Easing::ElasticOut(float t, const float b, const float c, const float d)
{
	t = clamp(t, 0.0f, d);

	if (t == 0.f)      return b;
	if ((t /= d) == 1.f) return b + c;
	float p = d*0.3f;

	float a = c;
	float s = p / 4.f;
	return (a* std::powf(2.0f, -10.0f*t) * std::sinf((t*d - s)*(2.0f * M_PI) / p) + c + b);
}

float Easing::ElasticInOut(float t, const float b, const float c, const float d)
{
	t = clamp(t, 0.0f, d);

	if (t == 0.f) return b;
	if ((t /= d / 2.f) == 2.f) return b + c;
	float p = d*(0.3f*1.5f);

	float a = c;
	float s = p / 4.f;
	if (t < 1.f)
	{
		t -= 1.f;
		return -0.5f*(a * std::powf(2.0f, 10.0f*t) * std::sinf((t*d - s)*(2.0f * M_PI) / p)) + b;
	}
	t -= 1.f;
	return a * std::powf(2.0f, -10.0f*t) * std::sinf((t*d - s)*(2.0f * M_PI) / p)*0.5f + c + b;
}


float Easing::ExpoIn(float t, const float b, const float c, const float d)
{
	t = clamp(t, 0.0f, d);
	return (t == 0.f) ? b : c * std::powf(2.0f, 10.0f * (t / d - 1.0f)) + b;
}

float Easing::ExpoOut(float t, const float b, const float c, const float d)
{
	return (t == d) ? b + c : c * (-std::powf(2.0f, -10.0f * t / d) + 1.0f) + b;
}

float Easing::ExpoInOut(float t, const float b, const float c, const float d)
{
	t = clamp(t, 0.0f, d);

	if (t == 0.f)         return b;
	if (t == d)         return b + c;
	if ((t /= d / 2.f) < 1.f) return c / 2.f * std::powf(2.0f, 10.0f * (t - 1.0f)) + b;
	return c / 2.f * (-std::powf(2.0f, -10.0f * --t) + 2.0f) + b;
}


float Easing::QuadIn(float t, const float b, const float c, const float d)
{
	t = clamp(t, 0.0f, d);

	t /= d;
	return c*t*t + b;
}

float Easing::QuadOut(float t, const float b, const float c, const float d)
{
	t = clamp(t, 0.0f, d);

	t /= d;
	return -c *t*(t - 2.f) + b;
}

float Easing::QuadInOut(float t, const float b, const float c, const float d)
{
	t = clamp(t, 0.0f, d);

	if ((t /= d / 2.f) < 1.f) return c / 2.f*t*t + b;
	--t;
	return -c / 2.f * (t*(t - 2.f) - 1.f) + b;
}


float Easing::QuartIn(float t, const float b, const float c, const float d)
{
	t = clamp(t, 0.0f, d);

	t /= d;
	return c*t*t*t*t + b;
}

float Easing::QuartOut(float t, const float b, const float c, const float d)
{
	t = clamp(t, 0.0f, d);

	t = t / d - 1.f;
	return -c * (t*t*t*t - 1.f) + b;
}

float Easing::QuartInOut(float t, const float b, const float c, const float d)
{
	t = clamp(t, 0.0f, d);

	if ((t /= d / 2.f) < 1.f) return c / 2.f*t*t*t*t + b;
	t -= 2.f;
	return -c / 2.f * (t*t*t*t - 2.f) + b;
}


float Easing::QuintIn(float t, const float b, const float c, const float d)
{
	t = clamp(t, 0.0f, d);

	t /= d;
	return c*t*t*t*t*t + b;
}

float Easing::QuintOut(float t, const float b, const float c, const float d)
{
	t = clamp(t, 0.0f, d);

	t = t / d - 1.f;
	return c*(t*t*t*t*t + 1.f) + b;
}

float Easing::QuintInOut(float t, const float b, const float c, const float d)
{
	t = clamp(t, 0.0f, d);

	if ((t /= d / 2.f) < 1.f) return c / 2.f*t*t*t*t*t + b;
	t -= 2.f;
	return c / 2.f*(t*t*t*t*t + 2.f) + b;
}


float Easing::SineIn(float t, const float b, const float c, const float d)
{
	t = clamp(t, 0.0f, d);
	return -c * std::cosf(t / d * (M_PI / 2.f)) + c + b;
}

float Easing::SineOut(float t, const float b, const float c, const float d)
{
	t = clamp(t, 0.0f, d);
	return c * std::sinf(t / d * (M_PI / 2.f)) + b;
}

float Easing::SineInOut(float t, const float b, const float c, const float d)
{
	t = clamp(t, 0.0f, d);
	return -c / 2.f * (std::cosf(M_PI*t / d) - 1.f) + b;
}

#pragma endregion	// Easing methods

#ifdef DEF_HELP_EASING
#pragma region easing help methods
float EasingLinear(float t, const float b, const float c, const float d)
{
	return Easing::Linear(t, d, c, d);
}
float EasingBackIn(float t, const float b, const float c, const float d)
{
	return Easing::BackIn(t, b, c, d);
}
float EasingBackOut(float t, const float b, const float c, const float d)
{
	return Easing::BackOut(t, b, c, d);
}
float EasingBackInOut(float t, const float b, const float c, const float d)
{
	return Easing::BackInOut(t, b, c, d);
}
float EasingBounceOut(float t, const float b, const float c, const float d)
{
	return Easing::BounceOut(t, b, c, d);
}
float EasingBounceIn(float t, const float b, const float c, const float d)
{
	return Easing::BounceIn(t, b, c, d);
}
float EasingBounceInOut(float t, const float b, const float c, const float d)
{
	return Easing::BounceInOut(t, b, c, d);
}
float EasingCircIn(float t, const float b, const float c, const float d)
{
	return Easing::CircIn(t, b, c, d);
}
float EasingCircOut(float t, const float b, const float c, const float d)
{
	return Easing::CircOut(t, b, c, d);
}
float EasingCircInOut(float t, const float b, const float c, const float d)
{
	return Easing::CircInOut(t, b, c, d);
}
float EasingCubicIn(float t, const float b, const float c, const float d)
{
	return Easing::CubicIn(t, b, c, d);
}
float EasingCubicOut(float t, const float b, const float c, const float d)
{
	return Easing::CubicOut(t, b, c, d);
}
float EasingCubicInOut(float t, const float b, const float c, const float d)
{
	return Easing::CubicInOut(t, b, c, d);
}
float EasingElasticIn(float t, const float b, const float c, const float d)
{
	return Easing::ElasticIn(t, b, c, d);
}
float EasingElasticOut(float t, const float b, const float c, const float d)
{
	return Easing::ElasticOut(t, b, c, d);
}
float EasingElasticInOut(float t, const float b, const float c, const float d)
{
	return Easing::ElasticInOut(t, b, c, d);
}
float EasingExpoIn(float t, const float b, const float c, const float d)
{
	return Easing::ExpoIn(t, b, c, d);
}
float EasingExpoOut(float t, const float b, const float c, const float d)
{
	return Easing::ExpoOut(t, b, c, d);
}
float EasingExpoInOut(float t, const float b, const float c, const float d)
{
	return Easing::ExpoInOut(t, b, c, d);
}
float EasingQuadIn(float t, const float b, const float c, const float d)
{
	return Easing::QuadIn(t, b, c, d);
}
float EasingQuadOut(float t, const float b, const float c, const float d)
{
	return Easing::QuadOut(t, b, c, d);
}
float EasingQuadInOut(float t, const float b, const float c, const float d)
{
	return Easing::QuadInOut(t, b, c, d);
}
float EasingQuartIn(float t, const float b, const float c, const float d)
{
	return Easing::QuartIn(t, b, c, d);
}
float EasingQuartOut(float t, const float b, const float c, const float d)
{
	return Easing::QuartOut(t, b, c, d);
}
float EasingQuartInOut(float t, const float b, const float c, const float d)
{
	return Easing::QuartInOut(t, b, c, d);
}
float EasingQuintIn(float t, const float b, const float c, const float d)
{
	return Easing::QuintIn(t, b, c, d);
}
float EasingQuintOut(float t, const float b, const float c, const float d)
{
	return Easing::QuintOut(t, b, c, d);
}
float EasingQuintInOut(float t, const float b, const float c, const float d)
{
	return Easing::QuintInOut(t, b, c, d);
}
float EasingSineIn(float t, const float b, const float c, const float d)
{
	return Easing::SineIn(t, b, c, d);
}
float EasingSineOut(float t, const float b, const float c, const float d)
{
	return Easing::SineOut(t, b, c, d);
}
float EasingSineInOut(float t, const float b, const float c, const float d)
{
	return Easing::SineInOut(t, b, c, d);
}

#pragma endregion easing help methods
#endif // DEF_HELP_EASING