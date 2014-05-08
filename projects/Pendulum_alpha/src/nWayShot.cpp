#include "nWayShot.h"
#include "define.h"

#include "MyMath.hpp"


CNWayShot::CNWayShot(const CShot& base):
	IAttack("Atk_NWayShot")
	,shot_(base)
{
	force_ = 3;
}

void CNWayShot::step()
{
}
void CNWayShot::draw()
{
}


void CNWayShot::CreateAttack(const mymath::Vec3f& pos, int n, float angle, float interval, float speed, float acc, bool centerFlag)
{
	for(int i=0; i<n; ++i)
	{
		charabase::CharBase shot(shot_.obj());	// 座標、角度、画像、初速度
		// 座標
		shot.pos = pos;
		// 発射角
		shot.angle = angle + (interval * ((centerFlag) ?  static_cast<float>(i - n/2) : static_cast<float>(i)));
		if(shot.angle >= 360.f)
			shot.angle -= 360.f;
		else if(shot.angle < 0.f)
			shot.angle += 360.f;
		// 発射角に合わせて初速度や加速度の向きを変える
		const float rad = math::Calc_DegreeToRad(shot.angle);
		const float c = std::cosf(rad);
		const float s = std::sinf(rad);
		shot.add.x = speed *  c;
		shot.add.y = speed * -s;
		shot.add.z = 0.f;
		
		mymath::Vec3f a(acc *  c,
				acc * -s);	// 加速度
		CShot shot_info(shot,a);	// 追加用
		// 当たり判定領域のコピー
		shot_info.SetCollisionAreas(shot_.collisions);

		// 登録
		InsertObject(ObjPtr(new CShot(shot_info)));
	}

}

