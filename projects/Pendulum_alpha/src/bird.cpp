#ifdef _DEBUG
#define DEF_SHAPE_DRAW
#endif

#include "define.h"
#include "bird.h"
#include "nWayShot.h"
#include "setting.h"

//#include "player.h"
#include "common.h"

#include "actionPoint.h"


const float CBird::SEARCH_RANGE = 500.0f;
const float CBird::CHASE_RANGE = 300.0f;
const float CBird::ATTACK_RANGE = 100.f;
const float CBird::RETURN_RANGE = 3.f;
const float CBird::MOVE_SPEED = 100.f;

void (CBird::*CBird::StateStep_[])() =
{
	&CBird::WaitStep,
	&CBird::ChaseStep,
	&CBird::ReturnStep,
	&CBird::AttackStep,
	&CBird::DestroyStep,
};


CBird::CBird(const mymath::Vec3f& pos):
	IEnemy("E_Bird") 
	,startPos_(pos)
{
	obj_.pos = pos;
	init();
}
CBird::CBird(float x, float y, float z):
	IEnemy("E_Bird")
	,startPos_(mymath::Vec3f(x,y,z))
{
	obj_.pos.x = x;
	obj_.pos.y = y;
	obj_.pos.z = z;
	init();
}

CBird::~CBird()
{
	
}


void CBird::init()
{
	using common::FindChunk;
	std::ifstream f("res/dat/enemy/bird.txt");
	if (f.fail())
	{
		debug::Dbg_BoxToMessage("CBird::init");
		return;
	}
	std::string buf;
	if(FindChunk(f, "#Img"))
	{
		f >> obj_.resname;
	}
	if(FindChunk(f, "#Size"))
	{
		LoadValue(f, obj_, obj_.size.x);
		LoadValue(f, obj_, obj_.size.y);
	}
	if(FindChunk(f, "#Collision")) 
		LoadCollisions(f);
	if(FindChunk(f,"#Attack"))
		LoadAttack(f);
	//obj_.img = birdIMG;
	elapsedTime_ = 0.f;
	nextActTime_ = 0.f;
	state_ = State::WAIT;
	//obj_.alpha = 200.f;


}

void CBird::step()
{
	ICharacter::step();
	elapsedTime_ += system::FrameTime;

	DecideState();

	if (attack_ != nullptr)
		attack_->step();

	// プレイヤーを向く
	const mymath::Vec3f& plPos = gm()->GetPlayerPos();

	if (obj_.pos.x < plPos.x)
		obj_.scale.x = 1.f;
	else
		obj_.scale.x = -1.f;

	(this->*StateStep_[static_cast<int>(state_)])();

}

void CBird::draw()
{
	const auto& cols = GetCollisionAreas();
	for (const auto& col : cols)
		col->draw();
	if (attack_ != nullptr)
		attack_->draw();
	mymath::Rectf rect = camera::GetScreenRect();
	if (rect.Contains(obj_.GetRect()))
	{
		obj_.draw();
	}
}


void CBird::WaitStep()
{

}

void CBird::ChaseStep()
{
	const mymath::Vec3f& plPos = gm()->GetPlayerPos();
	const mymath::Vec3f dist = plPos - obj_.pos;
	float angle = std::atan2f(dist.y, dist.x);
	obj_.add = mymath::Vec3f::Rotate(angle) * MOVE_SPEED;
	obj_.Move();
}

void CBird::ReturnStep()
{
	mymath::Vec3f dist = startPos_ - obj_.pos;
	
	if(mymath::PYTHA(dist.x,dist.y) > mymath::POW2(RETURN_RANGE))
	{
		float angle = std::atan2f(dist.y,dist.x);
		obj_.add = mymath::Vec3f::Rotate(angle) * MOVE_SPEED;
	}
	else
	{
		obj_.pos = startPos_;
		state_ = State::WAIT;
	}
	obj_.Move();
}

void CBird::AttackStep()
{
	// 攻撃
	if(elapsedTime_ > nextActTime_)
	{
		CreateAttack();
		state_ = State::WAIT;
		nextActTime_ = elapsedTime_ + 2.f;		// 連続間隔
	}
}

void CBird::DestroyStep()
{
	// 0.5秒
	obj_.alpha -= 255.f / 0.5f * system::FrameTime;
	if(obj_.alpha < 0.f)
	{
		obj_.alpha = 0.f;
		state_ = State::WAIT;
		kill();
	}
	return;
}

void CBird::DecideState()
{
	if (state_ == State::DESTROY)
	{
		// 死亡中 復活は許されない
		return;
	}
	const mymath::Vec3f& plPos = gm()->GetPlayerPos();
	// プレイヤーとの距離ベクトル e -> p
	mymath::Vec3f Vdist = plPos - obj_.pos;
	const float plyDist = mymath::PYTHA(Vdist.x, Vdist.y);
	// 初期位置からのベクトル start -> now
	Vdist = obj_.pos - startPos_;
	const float staDist = mymath::PYTHA(Vdist.x, Vdist.y);
	if (plyDist < mymath::POW2(ATTACK_RANGE) || state_ == State::ATTACK)
	{
		// 攻撃範囲内 or 攻撃中
		state_ = State::ATTACK;
	}
	else if (plyDist < mymath::POW2(SEARCH_RANGE))
	{
		// 攻撃範囲外 索敵範囲内
		if (staDist < mymath::POW2(CHASE_RANGE))
		{
			// 追跡可能範囲内
			state_ = State::CHASE;
		}
		else
		{
			// 追跡可能範囲外
			state_ = State::ATTACK;
		}
	}
	else if (staDist > mymath::POW2(RETURN_RANGE))
	{
		// 索敵範囲外
		state_ = State::RETURN;
	}
	else
	{
		// 保険(各行動の最後にはWAITに戻してるはず)
		state_ = State::WAIT;
	}
}

void CBird::CreateAttack()
{
	const mymath::Vec3f& mypos = obj_.pos;
	const mymath::Vec3f& plpos = gm()->GetPlayerPos();
	const mymath::Vec3f vec = plpos - mypos;
	float angle = math::Calc_RadToDegree(std::atan2f(-vec.y, vec.x));
	const float INTERVAL = 20.f;	// 横間隔
	const float SP = 70.f;			// 初速度
	const float ACC = 5.f;			// 加速度
	std::dynamic_pointer_cast<CNWayShot>(attack_)->CreateAttack(
				mypos,
				5,
				angle,INTERVAL,
				SP,ACC);
}


void CBird::hit(const ObjPtr& rival)
{
	if(rival->FindName("ActionPolygon"))
	{
		// めり込み補正,通過補正
		const auto& ap = std::dynamic_pointer_cast<CActionPolygon>(rival);
		mymath::Vec3f dist = obj_.pos - prePos;
		mymath::Vec3f intersection;
		intersection = ap->IntersectionPoint2Nearest(prePos_, obj_.pos);
		obj_.pos = intersection;
		obj_.pos -= dist.Normalize();
		
	}
}



bool CBird::ApplyDamage(int dam)
{
	// 死亡アニメーション中はスキップ
	if(state_ == State::DESTROY) return true;
	// ダメージが入ったら即死
	state_ = State::DESTROY;

	//
	//// 爆散エフェクト
	//InsertObject(ObjPtr(new CEffectExplosion(obj_.pos)));
	//// SE
	//DSound_Play(SE::EXPLODE);
	
	return true;
}

Base::Collisions CBird::GetCollisionAreas()
{
	// 死亡アニメーション中はスキップ
	if(state_ != State::DESTROY)
	{
		return __super::GetCollisionAreas();
	}
	return Base::Collisions();
}

