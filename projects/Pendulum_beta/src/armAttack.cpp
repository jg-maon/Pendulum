#include "armAttack.h"


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
void CArmAttack::init()
{

}

void CArmAttack::SetArmInfo(const float angle, const float length, const mymath::Vec3f& center)
{
	angle_  = angle;
	length_ = length;
	center_ = center;
	
	obj_.angle = angle_;

	//ç¿ïWÇÃèCê≥
	obj_.pos.x = math::ROUND_X(angle_, length_, center_.x);
	obj_.pos.y = math::ROUND_Y(angle_, length_, center_.y);
}

void CArmAttack::Rotate(float angle)
{
	if (angle_ != angle)
	{
		angle_ = angle;
		obj_.angle = angle_;
		motionType_ = MotionType::ATTACK;
	}
	else
	{
		motionType_ = MotionType::WAIT;
	}


	obj_.src.x = static_cast<int>(motionType_);

	//ç¿ïWÇÃèCê≥
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

