#include "Fade.h"

#include "define.h"

#include "common.h"

unsigned char CFade::r_ = 0;
unsigned char CFade::g_ = 0;
unsigned char CFade::b_ = 0;
float CFade::a_ = 0.f;
bool CFade::fadeInOn_;
bool CFade::fadeOutOn_;

#pragma region CFade methods

void CFade::ChangeColor(unsigned char r, unsigned char g, unsigned char b)
{
	r_ = r;
	g_ = g;
	b_ = b;
}

void CFade::ChangeColor(unsigned long argb)
{
	u_char tmp;
	common::color::Color_SeparateARGB(
		argb, tmp, r_, g_, b_);
}

void CFade::StartFadeIn()
{
	a_ = 255.0f;
	fadeInOn_ = true;
	fadeOutOn_ = false;
}

void CFade::StartFadeOut()
{
	a_ = 0.0f;
	fadeOutOn_ = true;
	fadeInOn_ = false;
}

bool CFade::FadeIn(float speed)
{
	if (!fadeInOn_) return false;
	a_ -= speed;
	if (a_ <= 0.0f)
	{
		a_ = 0.0f;
		return true;
	}
	return false;
}

bool CFade::FadeOut(float speed)
{
	if (!fadeOutOn_) return false;
	a_ += speed;
	if (a_ >= 255.0f)
	{
		a_ = 255.0f;
		return true;
	}
	return false;
}

void CFade::draw()
{
	//フェードインアウト描画	
	graph::Draw_BoxNC(
		0, 0, system::WINW, system::WINH, 0.0f,
		ARGB(static_cast<int>(a_), r_, g_, b_),
		ARGB(static_cast<int>(a_), r_, g_, b_),
		0, true);
}

bool CFade::IsFadeInning()
{
	return fadeInOn_ && a_ > 1.f;
}
bool CFade::IsFadeOuting()
{
	return fadeOutOn_ && a_ < 254.f;
}
bool CFade::IsFading()
{
	return IsFadeInning() || IsFadeOuting();
}

#pragma endregion // CFade methods
