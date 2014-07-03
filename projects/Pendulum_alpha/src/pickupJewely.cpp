#include "pickupJewelry.h"
#include "easing.h"
#include "gameManager.h"

CPickupJewely::CPickupJewely(const mymath::Vec3f& pos) :
IPickup("PickupJewely")
{
	obj_.pos = pos;
}

void CPickupJewely::step()
{
	switch (state_)
	{
	case IPickup::State::POP:
		time_ += system::FrameTime;
		obj_.add.y = Easing::CubicOut(time_, 0.3f, 10.f, POP_ANIM_TIME / 10.f);
		if (time_ >= POP_ANIM_TIME / 10.f)
		{
			state_ = IPickup::State::CHASE;
		}
		break;
	case IPickup::State::CHASE:
		//auto plpos = gm()->GetPlayerPos();

		break;
	default:
		break;
	}
	obj_.Move();
}

void CPickupJewely::hit(const ObjPtr& rival)
{
	if (rival->FindName("Player"))
	{
		kill();
	}
}