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
	// 弾数に合わせて等間隔に配置する
	int interval = 360 / n;

	for (int i = 0; i<n; ++i)
	{
		charabase::CharBase sing(sing_.obj());	// 座標、角度、画像、初速度
		// 座標
		sing.pos = pos;
		// 発射角
		sing.angle = angle + (interval * ((centerFlag) ? static_cast<float>(i - n / 2) : static_cast<float>(i)));
		if (sing.angle >= 360.f)
			sing.angle -= 360.f;
		else if (sing.angle < 0.f)
			sing.angle += 360.f;
		// 発射角に合わせて初速度や加速度の向きを変える
		const float rad = math::Calc_DegreeToRad(sing.angle);
		const float c = std::cosf(rad);
		const float s = std::sinf(rad);
		sing.add.x = speed *  c;
		sing.add.y = speed * -s;
		sing.add.z = 0.f;

		mymath::Vec3f a(acc *  c,
			acc * -s);	// 加速度
		CSing sing_info(sing, a);	// 追加用
		// 当たり判定領域のコピー
		sing_info.SetCollisionAreas(sing_);

		// 登録
		InsertObject(ObjPtr(new CSing(sing_info)));
	}

}

