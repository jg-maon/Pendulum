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
	elapsedTime_ += system::FrameTime;

	// ���G
	if (isInvincible())
	{
		// �_�ŃA�j��
		invincibleAnim_ += system::ONEFRAME_TIME;
		if (invincibleAnim_ >= loadInfo_.INV_TIME / 20.f)
		{
			invincibleAnim_ = 0.f;
			if (obj_.alpha > 200.f)
			{
				obj_.alpha = 150.f;		// ����
				obj_.g = 20.f;
				obj_.b = 20.f;

			}
			else
			{
				obj_.alpha = 220.f;		// �o��
				obj_.g = 220.f;
				obj_.b = 220.f;
			}
		}
		// ���Ԍ���
		invincibleTime_ -= system::ONEFRAME_TIME;
		if (invincibleTime_ < 0.f)
		{
			// ���G�I��
			obj_.alpha = 255.f;
			obj_.g = 255.f;
			obj_.b = 255.f;
		}
	}

	DecideState();

	// �A�j���[�V��������
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

	// �v���C���[������
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
	obj_.add = mymath::Vec3f::Rotate(angle) * loadInfo_.MOVE_SPEED;
	obj_.Move();
}

//void CGriffon::ReturnStep()
//{
//	mymath::Vec3f dist = startPos_ - obj_.pos;
//
//	if (mymath::PYTHA(dist.x, dist.y) > mymath::POW2(loadInfo_.RETURN_RANGE))
//	{
//		float angle = std::atan2f(dist.y, dist.x);
//		obj_.add = mymath::Vec3f::Rotate(angle) * loadInfo_.MOVE_SPEED;
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
	// ���h��
	obj_.add.x = std::cosf(math::Calc_DegreeToRad(sway_));
	obj_.Move();
}

void CGriffon::AttackStep()
{
	if (elapsedTime_ > nextActTime_)
	{
		// �U��
		CreateAttack();
		//battleState_ = BattleState::WAIT;
		nextActTime_ = elapsedTime_ + loadInfo_.attackInterval;		// �A���Ԋu
	}

	if (obj_.pos.x < endPos_.x && obj_.pos.y < endPos_.y)
	{
		obj_.Move();
	}
	else
	{
		battleState_ = BattleState::WAIT;
	}
}

void CGriffon::DestroyStep()
{
	sway_ = static_cast<float>((static_cast<int>(sway_ + 45) % 360));
	// ���h��
	obj_.add.x = std::cosf(math::Calc_DegreeToRad(sway_));
	obj_.Move();
	// 0.5�b
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
		// ���S�� �����͋�����Ȃ�
		return;
	}

	const mymath::Vec3f& plPos = gm()->GetPlayerPos();
	// �v���C���[�Ƃ̋����x�N�g�� e -> p
	mymath::Vec3f Vdist = plPos - obj_.pos;
	const float plyDist = mymath::PYTHA(Vdist.x, Vdist.y);
	// �����ʒu����̃x�N�g�� start -> now
	Vdist = obj_.pos - startPos_;
	const float staDist = mymath::PYTHA(Vdist.x, Vdist.y);
	if (plyDist < mymath::POW2(loadInfo_.ATTACK_RANGE)
		|| battleState_ == BattleState::ATTACK)
	{
		// �U����
		battleState_ = BattleState::ATTACK;
	}
	else if (plyDist < mymath::POW2(loadInfo_.SEARCH_RANGE))
	{
		// �U���͈͊O ���G�͈͓�
		if (staDist < mymath::POW2(loadInfo_.CHASE_RANGE))
		{
			// �ǐՉ\�͈͓�
			battleState_ = BattleState::CHASE;
		}
		else
		{
			battleState_ = BattleState::ATTACK;
		}
	}
	//else if (staDist > mymath::POW2(loadInfo_.RETURN_RANGE))
	//{
	//	// ���G�͈͊O
	//	battleState_ = BattleState::RETURN;
	//}
	else
	{
		// �ی�(�e�s���̍Ō�ɂ�WAIT�ɖ߂��Ă�͂�)
		battleState_ = BattleState::WAIT;
	}
}

void CGriffon::CreateAttack()
{
	const mymath::Vec3f& mypos = obj_.pos;
	const mymath::Vec3f& plpos = gm()->GetPlayerPos();
	const mymath::Vec3f vec = plpos - mypos;
	float angle = math::Calc_RadToDegree(std::atan2f(-vec.y, vec.x));
	//const float SP = 70.f;			// �����x
	const float ACC = 5.f;			// �����x
	const float rad = math::Calc_DegreeToRad(angle);
	const float c = std::cosf(rad);
	const float s = std::sinf(rad);
	obj_.add.x = loadInfo_.ATTACK_SPEED *  c;
	obj_.add.y = loadInfo_.ATTACK_SPEED * -s;
	obj_.add.z = 0.f;

	std::dynamic_pointer_cast<CTackle>(attack_)->CreateAttack(
		mypos,
		loadInfo_.tackleDist,
		angle,
		loadInfo_.ATTACK_SPEED, ACC);
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
	// ���S�A�j���[�V�������̓X�L�b�v
	if (battleState_ == BattleState::DESTROY) return true;

	// ���G��
	if (isInvincible()) return false;

	health_ -= dam;
	// �Ђ�ݏ���
	battleState_ = BattleState::DAMAGE;

	invincibleTime_ = loadInfo_.INV_TIME;

	nextActTime_ = elapsedTime_ + loadInfo_.damageTime;

	if (health_ <= 0)
	{
		battleState_ = BattleState::DESTROY;
		return true;
	}
	//
	//// ���U�G�t�F�N�g
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
	// ��_���㖳�G���A���S�A�j���[�V�������̓X�L�b�v
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
