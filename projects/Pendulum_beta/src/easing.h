#ifndef DEF_EASING_H
#define DEF_EASING_H

// #define DEF_HELP_EASING

//
// さまざまなイージング関数
// t = 経過時間 b = 始点 c = 終点 - 始点(=変位) d = 持続時間
//
// x = Easing::Linear(t, sta, end-sta, 2.f);
// t += FrameTime;
class Easing
{
public:
	virtual ~Easing() = 0 {}
	static float Linear(float t, const float b, const float c, const float d);
	static float BackIn(float t, const float b, const float c, const float d);
	static float BackOut(float t, const float b, const float c, const float d);
	static float BackInOut(float t, const float b, const float c, const float d);
	static float BounceOut(float t, const float b, const float c, const float d);
	static float BounceIn(float t, const float b, const float c, const float d);
	static float BounceInOut(float t, const float b, const float c, const float d);
	static float CircIn(float t, const float b, const float c, const float d);
	static float CircOut(float t, const float b, const float c, const float d);
	static float CircInOut(float t, const float b, const float c, const float d);
	static float CubicIn(float t, const float b, const float c, const float d);
	static float CubicOut(float t, const float b, const float c, const float d);
	static float CubicInOut(float t, const float b, const float c, const float d);
	static float ElasticIn(float t, const float b, const float c, const float d);
	static float ElasticOut(float t, const float b, const float c, const float d);
	static float ElasticInOut(float t, const float b, const float c, const float d);
	static float ExpoIn(float t, const float b, const float c, const float d);
	static float ExpoOut(float t, const float b, const float c, const float d);
	static float ExpoInOut(float t, const float b, const float c, const float d);
	static float QuadIn(float t, const float b, const float c, const float d);
	static float QuadOut(float t, const float b, const float c, const float d);
	static float QuadInOut(float t, const float b, const float c, const float d);
	static float QuartIn(float t, const float b, const float c, const float d);
	static float QuartOut(float t, const float b, const float c, const float d);
	static float QuartInOut(float t, const float b, const float c, const float d);
	static float QuintIn(float t, const float b, const float c, const float d);
	static float QuintOut(float t, const float b, const float c, const float d);
	static float QuintInOut(float t, const float b, const float c, const float d);
	static float SineIn(float t, const float b, const float c, const float d);
	static float SineOut(float t, const float b, const float c, const float d);
	static float SineInOut(float t, const float b, const float c, const float d);
};
#ifdef DEF_HELP_EASING
extern float EasingLinear(float t, const float b, const float c, const float d);
extern float EasingBackIn(float t, const float b, const float c, const float d);
extern float EasingBackOut(float t, const float b, const float c, const float d);
extern float EasingBackInOut(float t, const float b, const float c, const float d);
extern float EasingBounceOut(float t, const float b, const float c, const float d);
extern float EasingBounceIn(float t, const float b, const float c, const float d);
extern float EasingBounceInOut(float t, const float b, const float c, const float d);
extern float EasingCircIn(float t, const float b, const float c, const float d);
extern float EasingCircOut(float t, const float b, const float c, const float d);
extern float EasingCircInOut(float t, const float b, const float c, const float d);
extern float EasingCubicIn(float t, const float b, const float c, const float d);
extern float EasingCubicOut(float t, const float b, const float c, const float d);
extern float EasingCubicInOut(float t, const float b, const float c, const float d);
extern float EasingElasticIn(float t, const float b, const float c, const float d);
extern float EasingElasticOut(float t, const float b, const float c, const float d);
extern float EasingElasticInOut(float t, const float b, const float c, const float d);
extern float EasingExpoIn(float t, const float b, const float c, const float d);
extern float EasingExpoOut(float t, const float b, const float c, const float d);
extern float EasingExpoInOut(float t, const float b, const float c, const float d);
extern float EasingQuadIn(float t, const float b, const float c, const float d);
extern float EasingQuadOut(float t, const float b, const float c, const float d);
extern float EasingQuadInOut(float t, const float b, const float c, const float d);
extern float EasingQuartIn(float t, const float b, const float c, const float d);
extern float EasingQuartOut(float t, const float b, const float c, const float d);
extern float EasingQuartInOut(float t, const float b, const float c, const float d);
extern float EasingQuintIn(float t, const float b, const float c, const float d);
extern float EasingQuintOut(float t, const float b, const float c, const float d);
extern float EasingQuintInOut(float t, const float b, const float c, const float d);
extern float EasingSineIn(float t, const float b, const float c, const float d);
extern float EasingSineOut(float t, const float b, const float c, const float d);
extern float EasingSineInOut(float t, const float b, const float c, const float d);
#endif // DEF_HELP_EASING

#endif