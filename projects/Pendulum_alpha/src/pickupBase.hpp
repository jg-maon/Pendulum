#ifndef DEF_PICKUPBASE_HPP
#define DEF_PICKUPBASE_HPP

#ifndef DEF_OBJ_H
#include "obj.h"
#endif

/*
	@brief	アイテム系統基底クラス
			食らい判定=取得判定
*/
class IPickup : public IColObject
{
protected:
	enum
	{
		POP_ANIM_TIME = 7,			// 出現アニメーション時間(当たり判定なし)[unit: /10sec ]
		GET_TIME = 10,				// 取得までの時間[unit: /10sec ]
	};
	enum class State
	{
		POP,						// 出現
		CHASE,						// プレイヤーを追いかける
	};
protected:
	float time_;					// 汎用タイマー
	IPickup::State state_;			// アニメーション状態

public:
	IPickup(const std::string& name) :
		IColObject(name)
		, time_(0.f)
		, state_(State::POP)
	{

	}
	virtual void step(){}
	virtual void draw() override
	{
		obj_.draw();
	}
	virtual Base::Collisions GetDamageAreas() const override
	{
		switch (state_)
		{
		case IPickup::State::POP:
			break;
		case IPickup::State::CHASE:
			return __super::GetDamageAreas();
			//break;
		default:
			break;
		}
		return Collisions();
	}


};

typedef std::shared_ptr<IPickup> PickupPtr;

#endif