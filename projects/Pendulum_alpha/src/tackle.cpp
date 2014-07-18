#include "tackle.h"

#include "define.h"

#include "MyMath.hpp"


CTackle::CTackle(const mymath::Vec3f& acceleration, const mymath::Vec3f& endPos) :
IAttack("Atk_Tackle")
, acceleration_(acceleration)
{
	endPos_ = endPos;
	init();
}

CTackle::CTackle(const charabase::CharBase& tackle, const mymath::Vec3f& acceleration, const mymath::Vec3f& endPos) :
IAttack("Atk_Tackle")
, acceleration_(acceleration)
{
	obj_ = tackle;
	endPos_ = endPos;
	init();
}

void CTackle::init()
{
	force_ = 5;
}

void CTackle::step()
{
	/*
	// �ړ��I�_�łȂ����
	if (obj_.pos.x < endPos_.x && obj_.pos.y < endPos_.y)
	{
		obj_.Move();
		obj_.add += acceleration_;
	}
	else
	{
		kill();
	}
	//*/
}
void CTackle::draw()
{
}

void CTackle::hit(const ObjPtr& rival)
{
	/*
	if (rival->FindName("Player"))
	{
		kill();
	}
	//*/
}

void CTackle::CreateAttack(const mymath::Vec3f& pos, float dist, float angle, float speed, float acc)
{
	/*
	charabase::CharBase tackle(obj_);	// ���W�A�p�x�A�摜�A�����x
	// ���W
	tackle.pos = pos;
	// ���ˊp
	//shot.angle = angle * (((centerFlag) ? static_cast<float>(i - n / 2) : static_cast<float>(i)));
	//if (shot.angle >= 360.f)
	//	shot.angle -= 360.f;
	//else if (shot.angle < 0.f)
	//	shot.angle += 360.f;
	// ���ˊp�ɍ��킹�ď����x������x�̌�����ς���
	const float rad = math::Calc_DegreeToRad(angle);
	const float c = std::cosf(rad);
	const float s = std::sinf(rad);
	tackle.add.x = speed *  c;
	tackle.add.y = speed * -s;
	tackle.add.z = 0.f;
	mymath::Vec3f a(acc *  c,
		acc * -s);	// �����x
	mymath::Vec3f end((dist * c) + obj_.pos.x, (dist * -s) + obj_.pos.y);	// �ړ��I�_���W
	CTackle tackle_info(tackle, a, end);	// �ǉ��p
	// �����蔻��̈�̃R�s�[
	//tackle_info.SetCollisionAreas(tackle);

	// �o�^
	gm()->AddObject(ObjPtr(new CTackle(tackle_info)));
	//*/
}