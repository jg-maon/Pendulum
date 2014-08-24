#include "fireShot.h"
#include "define.h"

#include "MyMath.hpp"

#ifndef DEF_EFFECTBASE_HPP
#include "effectBase.hpp"
#endif

/*
	@brief	火炎弾オブジェクト
*/
class CFire : public IAttack
{
	enum class State
	{
		move, explode,
	}state_;
private:
	charabase::Anim anim_;

	
public:

	/*
		@brief	初期化
		@param	[in]	force	威力
		@param	[in]	obj		オブジェクト情報
	*/
	CFire(int force, const charabase::CharBase& obj) :
		IAttack("Atk_Fire")
		, anim_(21, 21.f / 1.5f * system::FrameTime)
		, state_(State::move)
	{
		force_ = force;
		obj_ = obj;
		start();
	}

	/*
		@brief	更新処理
		@return	なし
	*/
	virtual void step() override
	{
		// ヒットストップ中はスキップ
		if (!isUpdatable())
			return;

		bool animEnd = anim_.step();
		const int animRow = 16;
		obj_.src.x = anim_.no % animRow;
		obj_.src.y = anim_.no / animRow;
		switch (state_)
		{
		case CFire::State::move:
			break;
		case CFire::State::explode:
			if (animEnd) 
				kill();
			break;
		default:
			break;
		}

		// 移動
		obj_.Move();

	}

	/*
		@brief	描画処理
	*/
	virtual void draw() override
	{
		graph::Draw_SetRenderMode(RENDER_MODE::HALFADD);
		obj_.draw();
		graph::Draw_EndRenderMode();
	}

	virtual void hit(const ObjPtr& rival) override
	{
		if (rival->FindName("Player"))
		{
			anim_.max = 64;
			anim_.no = 22;
			state_ = State::explode;
		}
	}

};


CFireShot::CFireShot() :
IAttack("Atk_FireShot")
{
	force_ = 3;
}

void CFireShot::step()
{
}
void CFireShot::draw()
{
}


void CFireShot::CreateAttack(const mymath::Vec3f& pos, const mymath::Vec3f& vec, float speed) const
{
	// 弾
	charabase::CharBase shot(charabase::BaseData("img_fireShot", 256, 256));
	// 大きさ
	shot.scale = 0.5f;
	// 座標
	shot.pos = pos;
	// 発射角
	float rad = vec.Angle2(pos, vec);
	shot.angle = math::Calc_RadToDegree(rad);
	if (shot.angle >= 360.f)
		shot.angle -= 360.f;
	else if (shot.angle < 0.f)
		shot.angle += 360.f;
	// 発射角に合わせて初速度や加速度の向きを変える
	const float c = std::cosf(rad);
	const float s = std::sinf(rad);
	shot.add.x = speed *  c;
	shot.add.y = speed * -s;
	shot.add.z = 0.f;

	// 登録
	gm()->AddObject(ObjPtr(new CFire(force_, shot)));
}



