#include "armAttack.h"

void CArmAttack::init()
{

}

CArmAttack::CArmAttack(const charabase::CharBase& cb) :
	IAttack("Atk_ArmAttack")
{
	obj_ = cb;
	motionType_ = MotionType::WAIT;
}

CArmAttack::CArmAttack(const int force) :
	IAttack("Atk_ArmAttack")
{
	force_ = force;
}

void CArmAttack::SetArmInfo(const float angle, const float length, const mymath::Vec3f& center)
{
	angle_  = angle;
	length_ = length;
	center_ = center;
	
	obj_.angle = angle_;

	//À•W‚ÌC³
	obj_.pos.x = math::ROUND_X(angle_, length_, center_.x);
	obj_.pos.y = math::ROUND_Y(angle_, length_, center_.y);
}

void CArmAttack::step(float angle)
{
	if (angle_ != angle)
	{
		angle_ = angle;
		obj_.angle = angle_;
		motionType_ = MotionType::ATTACK;
		for (auto col : collisions_)
		{
			col->Rotate(obj_.angle);
		}
	}
	else
	{
		motionType_ = MotionType::WAIT;
	}


	obj_.src.x = static_cast<int>(motionType_);

	//À•W‚ÌC³
	obj_.pos.x = math::ROUND_X(angle_, length_, center_.x);
	obj_.pos.y = math::ROUND_Y(angle_, length_, center_.y);
}

void CArmAttack::step()
{

}

void CArmAttack::draw()
{
	mymath::Rectf rect = camera::GetScreenRect();
	if (rect.Contains(obj_.GetRect()))
	{
		obj_.draw();
	}
}

void CArmAttack::hit(const ObjPtr& rival)
{
	if (rival->FindName("Player"))
	{
		kill();
	}
	else if (rival->FindName("ActionPolygon"))
	{
		kill();
	}
}

