#include "armAttack.h"

CArmAttack::CArmAttack(const charabase::CharBase& cb) :
	IAttack("Atk_ArmAttack")
{
	obj_ = cb;
}

CArmAttack::CArmAttack(const int force) :
	IAttack("Atk_ArmAttack")
{
	force_ = force;
}

CArmAttack::CArmAttack(const CArmAttack& arm, const float nowAngle,
	const mymath::Vec3f& length, const mymath::Vec3f& center, float rotateSpeed, float maxAngle) :
	IAttack("Atk_ArmAttack")
	, angle_(nowAngle)
	, length_(length)
	, center_(center)
	, rotateSpeed_(rotateSpeed)
	, maxAngle_(maxAngle)
{
	obj_ = arm.obj_;
	SetCollisionAreas(arm);
	force_ = arm.force_;
	start();
}

void CArmAttack::init()
{

}
void CArmAttack::step()
{
	angle_ += rotateSpeed_;
	if ((rotateSpeed_ > 0 && maxAngle_ < angle_) ||
		(rotateSpeed_ < 0 && maxAngle_ > angle_)){
		kill();
	}
	obj_.angle = angle_;
	for (auto col : collisions_)
	{
		col->Rotate(obj_.angle);
	}
	obj_.pos.x = math::ROUND_X(angle_, length_.x, center_.x);
	obj_.pos.y = math::ROUND_Y(angle_, length_.x, center_.y);
}

void CArmAttack::draw()
{

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

void CArmAttack::setForce(int force)
{
	force_ = force;
}
