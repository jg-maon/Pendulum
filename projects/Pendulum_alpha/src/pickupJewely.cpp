#ifdef _DEBUG
//#define D_INFO_TEST		// 情報表示
#endif

#include "pickupJewely.h"
#include "easing.h"
#include "gameManager.h"

CPickupJewely::CPickupJewely() :
IPickup("PickupJewely")
{
	status_ = Status::run;
}

CPickupJewely::CPickupJewely(const mymath::Vec3f& pos, int score, int type) :
IPickup("PickupJewely")
{
	gm()->GetData(*this);

	if (type == -1)
		init(pos, score, math::GetRandom<int>(0, 8));
	else
		init(pos, score, type);
	
}

void CPickupJewely::init(const mymath::Vec3f& pos, int score, int type)
{
	score_ = score;
	obj_.pos = startPos_ = pos;
	obj_.src(type, 0);

	obj_.pos.z += -0.1f;


}


void CPickupJewely::step()
{
	switch (state_)
	{
	case IPickup::State::POP:
		time_ += system::FrameTime;
		obj_.pos.y = Easing::CubicOut(time_, startPos_.y, -60.f, POP_ANIM_TIME / 10.f);
		if (time_ >= POP_ANIM_TIME / 10.f)
		{
			time_ = 0.f;
			state_ = IPickup::State::CHASE;
		}
		//if ((obj_.angle += 2.f) > 360.f)
		//	obj_.angle -= 360.f;
		break;
	case IPickup::State::CHASE:
		mymath::Vec3f plpos = gm()->GetPlayerPos();
		//mymath::Vec3f plpos = camera::GetCameraPos();
		if (plpos.z != -1.f)
		{
			// プレイヤーへのベクトル
			mymath::Vec3f vec = plpos - obj_.pos;
			float angle = atan2f(vec.y, vec.x);

			float chase = GET_TIME / 10.f;

			obj_.add = vec * (time_ / chase / system::FrameTime);
			//obj_.add *= -vec;

			obj_.Move();

			time_ += system::FrameTime;
			// 一応
			if (time_ >= chase)
				time_ = chase;
		}
		break;
	}
}


void CPickupJewely::draw()
{
	__super::draw();
#ifdef D_INFO_TEST
	mymath::Vec3f plpos = camera::GetCameraPos();
	float chase = GET_TIME / 10.f;
	mymath::Vec3f vec = plpos - obj_.pos;
	float timeRatio = time_ / chase;
	auto add = vec * timeRatio;
	std::stringstream ss;
	ss << "time_:" << time_ << " timeRatio:" << timeRatio;
	font::Draw_TextXY((int)obj_.pos.x, (int)obj_.pos.y,
		ss.str(), -1,1);
	ss.str("");
	ss << "vec(" << vec.x << ", " << vec.y << ")";
	font::Draw_TextXY((int)obj_.pos.x, (int)obj_.pos.y + font::Draw_GetCharHeight(1),
		ss.str(), -1, 1);
	ss.str("");
	ss << "add(" << add.x << ", " << add.y << ")";
	font::Draw_TextXY((int)obj_.pos.x, (int)obj_.pos.y + font::Draw_GetCharHeight(1)*2,
		ss.str(), -1, 1);

#endif
}

void CPickupJewely::hit(const ObjPtr& rival)
{
	if (rival->FindName("Player"))
	{
		kill();
	}
}


int CPickupJewely::getScore() const
{
	return score_;
}