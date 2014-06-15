#include "nWaySing.h"
#include "define.h"

#include "MyMath.hpp"


CNWaySing::CNWaySing(const CSing& base) :
IAttack("Atk_NWaySing")
, sing_(base)
{
	force_ = 3;
}

void CNWaySing::step()
{
}
void CNWaySing::draw()
{
}


void CNWaySing::CreateAttack(const mymath::Vec3f& pos, int n, float angle, float speed, float acc, bool centerFlag)
{
	// �e���ɍ��킹�ē��Ԋu�ɔz�u����
	int interval = 360 / n;

	for (int i = 0; i<n; ++i)
	{
		charabase::CharBase sing(sing_.obj());	// ���W�A�p�x�A�摜�A�����x
		// ���W
		sing.pos = pos;
		// ���ˊp
		sing.angle = angle + (interval * ((centerFlag) ? static_cast<float>(i - n / 2) : static_cast<float>(i)));
		if (sing.angle >= 360.f)
			sing.angle -= 360.f;
		else if (sing.angle < 0.f)
			sing.angle += 360.f;
		// ���ˊp�ɍ��킹�ď����x������x�̌�����ς���
		const float rad = math::Calc_DegreeToRad(sing.angle);
		const float c = std::cosf(rad);
		const float s = std::sinf(rad);
		sing.add.x = speed *  c;
		sing.add.y = speed * -s;
		sing.add.z = 0.f;

		mymath::Vec3f a(acc *  c,
			acc * -s);	// �����x
		CSing sing_info(sing, a);	// �ǉ��p
		// �����蔻��̈�̃R�s�[
		sing_info.SetCollisionAreas(sing_);

		// �o�^
		InsertObject(ObjPtr(new CSing(sing_info)));
	}

}

