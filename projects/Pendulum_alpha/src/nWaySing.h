#ifndef DEF_NWAYSING_H
#define DEF_NWAYSING_H

#ifndef DEF_ATTACKBASE_HPP
#include "attackBase.hpp"
#endif

#ifndef DEF_MYMATH_HPP
#include "MyMath.hpp"
#endif

#include <vector>

/*
	@brief	音符
*/
class CSing : public IAttack
{
private:
	int cnt_;
	float acceleration_;		// 加速度
	float	addAngle_;	// 角速度

private:
	void init()
	{
		start();
		force_ = 2;
		cnt_ = 0;
		
		obj_.src(0, 0);
		//obj_.src(0, math::GetRandom<int>(0, 2));

		//collisions_.push_back(mymath::ShapefPtr(new mymath::Circlef(0.f, 0.f, obj_.pos.z, obj_.HalfHeight())));
		collisions_.push_back(mymath::ShapefPtr(new mymath::Rectf(obj_.GetRect())));
	}
public:
	/*
		@brief	初期化
		@param	[in]	sing		弾の情報
		@param	[in]	acc			加速度[unit:px/sec](デフォルト:100)
		@param	[in]	addAngle	角速度[unit:Degree](デフォルト:360) 
	*/
	CSing(const CSing& sing, float acc = 100.f, float addAngle = 360.f) :
		IAttack("Atk_Sing")
		, acceleration_(acc)
		, addAngle_(addAngle)
	{
		obj_ = sing.obj();
		init();
	}

	/*
		@brief	初期化
		@param	[in]	sing		弾の情報
	*/
	CSing(const charabase::CharBase& sing) :
		IAttack("Atk_Sing")
	{
		obj_ = sing;
		init();
	}

	virtual void step() override
	{
		//obj_.add += acceleration_;

		obj_.angle += addAngle_ * system::FrameTime;
		if (obj_.angle >= 360.f)
			obj_.angle -= 360.f;
		else if (obj_.angle < 0.f)
			obj_.angle += 360.f;

		obj_.add.x += math::ROUND_X(obj_.angle, (float)cnt_);
		obj_.add.y += math::ROUND_Y(obj_.angle, (float)cnt_);

		obj_.Move();
		if (++cnt_ > 60)
		{
			kill();
		}
	}

	virtual void draw() override
	{
		obj_.draw();
	}

	virtual void hit(const ObjPtr& rival) override
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
};


/*
	@brief	NWay音符
*/
class CNWaySing : public IAttack
{
private:
	const CSing sing_;			// 音符の情報
public:
	/*
		@brief	NWay音符の生成
		@param	[in]	base		音符の情報(画像情報,当たり判定)
	*/
	CNWaySing(const CSing& base);

	virtual void step() override;
	virtual void draw() override;

	/*
		@brief	NWay音符の発射
				どの位置から 何個 どの角度に どれくらいの間隔で どれくらいの角速度で 加速度はどれくらいで
		@param	[in]	pos			音符の初期座標
		@param	[in]	n			音符数
		@param	[in]	angle		音符の発射角度(単位:Degree)
		@param	[in]	acc			音符の加速度
		@param	[in]	addAngle	音符の角速度
		@return	なし
	*/
	void CreateAttack(const mymath::Vec3f& pos, int n, float angle, float acc, float addAngle);


};

#endif