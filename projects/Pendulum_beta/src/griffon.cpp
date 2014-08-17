#ifdef _DEBUG
#define DEF_SHAPE_DRAW
#endif

#include "define.h"
#include "griffon.h"
#include "tackle.h"
#include "setting.h"

//#include "player.h"
#include "common.h"

#include "actionPoint.h"

void (CGriffon::*CGriffon::StateStep_[])() =
{
	&CGriffon::WaitStep,
	&CGriffon::ChaseStep,
	//&CGriffon::ReturnStep,
	&CGriffon::DamageStep,
	&CGriffon::AttackStep,
	&CGriffon::DestroyStep,
};

CGriffon::CGriffon() :
IEnemy("E_Boss_Griffon")
{
	std::vector<int> move = { 0, 1, 2, 3, 2, 1 };
	motionTable_.push_back(move);
	attack_ = AttackPtr(new CTackle());
}

CGriffon::CGriffon(const mymath::Vec3f& pos) :
IEnemy("E_Boss_Griffon")
{
	init(pos);
}
CGriffon::CGriffon(float x, float y, float z) :
IEnemy("E_Boss_Griffon")
{
	init(mymath::Vec3f(x, y, z));
}

CGriffon::~CGriffon()
{

}


void CGriffon::init(const mymath::Vec3f& pos)
{

	gm()->GetData(*this);

	obj_.pos = pos;

	startPos_ = obj_.pos;
	endPos_ = obj_.pos;
	backPos_ = obj_.pos;

	elapsedTime_ = 0.f;
	nextActTime_ = 0.f;
	battleState_ = BattleState::WAIT;
	motionType_ = MotionType::MOVE;
	motionAnim_.set(motionTable_[static_cast<int>(motionType_)].size() - 1, 0.3f);
	obj_.src.x = motionTable_[static_cast<int>(motionType_)][motionAnim_.no];

	gravity_ = 0.f;
	gravityF_ = false;

	invincibleTime_ = 0.f;
	invincibleAnim_ = 0.f;

	health_ = loadInfo_.health;
	power_ = loadInfo_.power;

	sway_ = 0.f;
}

void CGriffon::step()
{
	ICharacter::step();

	// ヒットストップ中はスキップ
	if (!isUpdatable()) return;

	elapsedTime_ += system::FrameTime;

	// 無敵
	if (isInvincible())
	{
		// 点滅アニメ
		invincibleAnim_ += system::ONEFRAME_TIME;
		if (invincibleAnim_ >= loadInfo_.invincibleTime / 20.f)
		{
			invincibleAnim_ = 0.f;
			if (obj_.alpha > 200.f)
			{
				obj_.alpha = 150.f;		// 消す
				obj_.g = 20.f;
				obj_.b = 20.f;

			}
			else
			{
				obj_.alpha = 220.f;		// 出す
				obj_.g = 220.f;
				obj_.b = 220.f;
			}
		}
		// 時間減少
		invincibleTime_ -= system::ONEFRAME_TIME;
		if (invincibleTime_ < 0.f)
		{
			// 無敵終了
			obj_.alpha = 255.f;
			obj_.g = 255.f;
			obj_.b = 255.f;
		}
	}

	DecideState();

	// アニメーション処理
	if (motionAnim_.step())
	{
		if (motionType_ != MotionType::MOVE)
		{
			obj_.src.x = 0;
			obj_.src.y = 0;
			motionType_ = MotionType::MOVE;
			motionAnim_.set(motionTable_[static_cast<int>(motionType_)].size() - 1, 0.3f);
		}
	}
	else
	{
		obj_.src.x = motionTable_[static_cast<int>(motionType_)][motionAnim_.no];
	}

	if (attack_ != nullptr)
		attack_->step();

	// プレイヤーを向く
	const mymath::Vec3f& plPos = gm()->GetPlayerPos();

	if (obj_.pos.x < plPos.x && turnFlag_)
		turnFlag_ ^= 1;
	else if (obj_.pos.x > plPos.x && !turnFlag_)
		turnFlag_ ^= 1;

	(this->*StateStep_[static_cast<int>(battleState_)])();

}

void CGriffon::draw()
{
	if (attack_ != nullptr)
		attack_->draw();
	mymath::Rectf rect = camera::GetScreenRect();
	if (rect.Contains(obj_.GetRect()))
	{
		obj_.draw(charabase::CharBase::MODE::Center, turnFlag_);
	}
}


void CGriffon::WaitStep()
{

}

void CGriffon::ChaseStep()
{
	const mymath::Vec3f& plPos = gm()->GetPlayerPos();
	const mymath::Vec3f dist = plPos - obj_.pos;
	float angle = std::atan2f(dist.y, dist.x);
	obj_.add = mymath::Vec3f::Rotate(angle) * loadInfo_.moveSpeed;
	obj_.Move();
}

//void CGriffon::ReturnStep()
//{
//	mymath::Vec3f dist = startPos_ - obj_.pos;
//
//	if (mymath::PYTHA(dist.x, dist.y) > mymath::POW2(loadInfo_.returnRange))
//	{
//		float angle = std::atan2f(dist.y, dist.x);
//		obj_.add = mymath::Vec3f::Rotate(angle) * loadInfo_.moveSpeed;
//	}
//	else
//	{
//		obj_.pos = startPos_;
//		battleState_ = BattleState::WAIT;
//	}
//	obj_.Move();
//}

void CGriffon::DamageStep()
{
	if (nextActTime_ > elapsedTime_ + loadInfo_.damageTime)
	{
		battleState_ = BattleState::WAIT;
		sway_ = 0;
	}
	sway_ = static_cast<float>((static_cast<int>(sway_ + 45) % 360));
	// 横揺れ
	obj_.add.x = std::cosf(math::Calc_DegreeToRad(sway_));
	obj_.Move();
}

void CGriffon::AttackStep()
{
	if (elapsedTime_ > nextActTime_)
	{
		// 攻撃
		CreateAttack();
		//battleState_ = BattleState::WAIT;
		nextActTime_ = elapsedTime_ + loadInfo_.attackInterval;		// 連続間隔
	}

	if (isBack_)
	{
		if (obj_.pos.x < backPos_.x && obj_.pos.y < backPos_.y)
		{
			obj_.add = backPos_.Normalize2() * loadInfo_.moveSpeed;
			obj_.Move();
		}
		else
		{
			isBack_ = false;
			isAttacking_ = true;
		}
	}

	if (isAttacking_)
	{
		if (obj_.pos.x < endPos_.x && obj_.pos.y < endPos_.y)
		{
			obj_.add = endPos_.Normalize2() * loadInfo_.attackSpeed;
			obj_.Move();
		}
		else
		{
			isAttacking_ = false;
			battleState_ = BattleState::WAIT;
		}
	}
}

void CGriffon::DestroyStep()
{
	// 0.5秒
	obj_.alpha -= 255.f / 0.5f * system::FrameTime;
	if (obj_.alpha < 0.f)
	{
		obj_.alpha = 0.f;
		battleState_ = BattleState::WAIT;
		kill();
	}
}

void CGriffon::DecideState()
{
	if (battleState_ == BattleState::DESTROY)
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
	if (plyDist < mymath::POW2(loadInfo_.attackRange)
		|| battleState_ == BattleState::ATTACK)
	{
		// 攻撃中
		battleState_ = BattleState::ATTACK;
	}
	else if (plyDist < mymath::POW2(loadInfo_.searchRange))
	{
		// 攻撃範囲外 索敵範囲内
		if (plyDist < mymath::POW2(loadInfo_.chaseRange))
		{
			// 追跡可能範囲内
			battleState_ = BattleState::CHASE;
		}
		else
		{
			battleState_ = BattleState::ATTACK;
		}
	}
	//else if (staDist > mymath::POW2(loadInfo_.returnRange))
	//{
	//	// 索敵範囲外
	//	battleState_ = BattleState::RETURN;
	//}
	else
	{
		// 保険(各行動の最後にはWAITに戻してるはず)
		battleState_ = BattleState::WAIT;
	}
}

void CGriffon::CreateAttack()
{
	const mymath::Vec3f& mypos = obj_.pos;
	const mymath::Vec3f& plpos = gm()->GetPlayerPos();
	const mymath::Vec3f vec = plpos - mypos;
	const float angle = std::atan2f(-vec.y, vec.x);
	const float c = std::cosf(angle);
	const float s = std::sinf(angle);

	//obj_.add.y = loadInfo_.attackSpeed * -s;
	//obj_.add.z = 0.f;
	//obj_.add.x = loadInfo_.attackSpeed *  c;

	// バック距離
	backPos_.x = -(loadInfo_.backDist * c) + obj_.pos.x;
	backPos_.y = -(loadInfo_.backDist *-s) + obj_.pos.y;
	backPos_.z = 0.f;
	// 移動終点距離
	endPos_.x = (loadInfo_.tackleDist * c) + obj_.pos.x;
	endPos_.y = (loadInfo_.tackleDist *-s) + obj_.pos.y;
	endPos_.z = 0.f;

	isBack_ = true;

}


bool CGriffon::isInvincible() const
{
	return invincibleTime_ > 0.f;
}

void CGriffon::hit(const ObjPtr& rival)
{
	// Polygon
	__super::hit(rival);
}



bool CGriffon::ApplyDamage(int dam)
{
	// 死亡アニメーション中はスキップ
	if (battleState_ == BattleState::DESTROY) return true;

	// 無敵中
	if (isInvincible()) return false;

	health_ -= dam;
	// ひるみ処理
	battleState_ = BattleState::DAMAGE;

	invincibleTime_ = loadInfo_.invincibleTime;

	nextActTime_ = elapsedTime_ + loadInfo_.damageTime;

	if (health_ <= 0)
	{
		battleState_ = BattleState::DESTROY;
		return true;
	}
	//
	//// 爆散エフェクト
	//InsertObject(ObjPtr(new CEffectExplosion(obj_.pos)));
	//// SE
	//DSound_Play(SE::EXPLODE);

	return false;
}

bool CGriffon::isAttacking() const
{
	return isAttacking_;
}

Base::Collisions CGriffon::GetDamageAreas() const
{
	// 被ダメ後無敵中、死亡アニメーション中はスキップ
	if (isInvincible() || battleState_ == BattleState::DESTROY)
	{
		return Base::Collisions();
	}
	return __super::GetDamageAreas();
}

void CGriffon::SetInfo(const LoadInfo& info)
{

	loadInfo_ = info;
}
