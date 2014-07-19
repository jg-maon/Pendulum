#ifdef _DEBUG

#endif

#include "define.h"
#include "bird.h"
#include "nWayShot.h"
#include "setting.h"

//#include "player.h"
#include "common.h"

#include "actionPoint.h"

/*
const float CBird::SEARCH_RANGE = 500.0f;
const float CBird::CHASE_RANGE = 300.0f;
const float CBird::ATTACK_RANGE = 100.f;
const float CBird::RETURN_RANGE = 3.f;
const float CBird::MOVE_SPEED = 100.f;
//*/

void (CBird::*CBird::StateStep_[])() =
{
	&CBird::WaitStep,
	&CBird::ChaseStep,
	&CBird::ReturnStep,
	&CBird::AttackStep,
	&CBird::DestroyStep,
};

CBird::CBird() :
IEnemy("E_Bird")
{
	std::vector<int> move = { 1, 2, 4, 3, 2 };
	motionTable_.push_back(move);
	std::vector<int> attack = { 1, 0, 1, 3, 4 };
	motionTable_.push_back(attack);
}

CBird::CBird(const mymath::Vec3f& pos) :
IEnemy("E_Bird")
{
	init(pos);
}
CBird::CBird(float x, float y, float z) :
IEnemy("E_Bird")
{
	init(mymath::Vec3f(x, y, z));
}

CBird::~CBird()
{

}


void CBird::init(const mymath::Vec3f& pos)
{
	using common::FindChunk;

	gm()->GetData(*this);

	prePos_ = obj_.pos = pos;

	startPos_ = obj_.pos;

	elapsedTime_ = 0.f;
	nextActTime_ = 0.f;
	invincible_.time = 0.f;
	invincible_.cnt = 0;
	state_ = State::WAIT;
	motionType_ = MotionType::MOVE;
	motionAnim_.set(motionTable_[static_cast<int>(motionType_)].size() - 1, 0.3f);
	obj_.src.x = motionTable_[static_cast<int>(motionType_)][motionAnim_.no];
	//obj_.alpha = 200.f;

}

void CBird::step()
{
	ICharacter::step();
	elapsedTime_ += system::FrameTime;

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

	(this->*StateStep_[static_cast<int>(state_)])();

}

void CBird::draw()
{
	if (attack_.get())
		attack_->draw();
	mymath::Rectf rect = camera::GetScreenRect();
	if (rect.Contains(obj_.GetRect()))
	{
		obj_.draw(charabase::CharBase::MODE::Center, turnFlag_);
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
	obj_.add = mymath::Vec3f::Rotate(angle) * loadInfo_.MOVE_SPEED;
	obj_.Move();

}

void CBird::ReturnStep()
{
	mymath::Vec3f dist = startPos_ - obj_.pos;

	if (mymath::PYTHA(dist.x, dist.y) > mymath::POW2(loadInfo_.RETURN_RANGE))
	{
		float angle = std::atan2f(dist.y, dist.x);
		obj_.add = mymath::Vec3f::Rotate(angle) * loadInfo_.MOVE_SPEED;
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
	// ���ߒ��͉H���Œ�
	if (motionAnim_.no == 1)
		motionAnim_.stop();
	// �U��
	if (elapsedTime_ > nextActTime_)
	{
		motionAnim_.start();		// �A�j���[�V�����ĊJ
		CreateAttack();
		state_ = State::WAIT;
		nextActTime_ = elapsedTime_ + loadInfo_.attackInterval;		// �A���Ԋu
	}
	else
	{
		// �_��
		invincible_.time += system::ONEFRAME_TIME;
		if (invincible_.time >= loadInfo_.attackInterval / 20.f)
		{
			invincible_.time = 0.f;
			if (invincible_.isOn)
			{
				// Off�ɂ���
				invincible_.isOn = false;
				obj_.b = 20.f;
			}
			else
			{
				// On�ɂ���
				invincible_.isOn = true;
				obj_.b = 255.f;
			}
			//invincible_.cnt++;
		}
	}
}

void CBird::DestroyStep()
{
	// 0.5�b
	obj_.alpha -= 255.f / 0.5f * system::FrameTime;
	if (obj_.alpha < 0.f)
	{
		obj_.alpha = 0.f;
		state_ = State::WAIT;
		kill();
	}
}

void CBird::DecideState()
{
	if (state_ == State::DESTROY)
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
	if (plyDist < mymath::POW2(loadInfo_.ATTACK_RANGE) || state_ == State::ATTACK)
	{
		// �U���͈͓� or �U����
		if (state_ != State::ATTACK)
			nextActTime_ = elapsedTime_ + loadInfo_.attackInterval;
		state_ = State::ATTACK;
		motionType_ = MotionType::ATTACK;
	}
	else if (plyDist < mymath::POW2(loadInfo_.SEARCH_RANGE))
	{
		// �U���͈͊O ���G�͈͓�
		if (staDist < mymath::POW2(loadInfo_.CHASE_RANGE))
		{
			// �ǐՉ\�͈͓�
			state_ = State::CHASE;
		}
		else
		{
			// �ǐՉ\�͈͊O
			state_ = State::ATTACK;
		}
	}
	else if (staDist > mymath::POW2(loadInfo_.RETURN_RANGE))
	{
		// ���G�͈͊O
		state_ = State::RETURN;
	}
	else
	{
		// �ی�(�e�s���̍Ō�ɂ�WAIT�ɖ߂��Ă�͂�)
		state_ = State::WAIT;
		motionType_ = MotionType::MOVE;
	}
}

void CBird::CreateAttack()
{
	const mymath::Vec3f& mypos = obj_.pos;
	const mymath::Vec3f& plpos = gm()->GetPlayerPos();
	const mymath::Vec3f vec = plpos - mypos;
	float angle = math::Calc_RadToDegree(std::atan2f(-vec.y, vec.x));
	const float INTERVAL = 20.f;	// ���Ԋu
	const float SP = 70.f;			// �����x
	const float ACC = 5.f;			// �����x
	invincible_.time = 0.f;
	std::dynamic_pointer_cast<CNWayShot>(attack_)->CreateAttack(
		mypos,
		3,
		angle, INTERVAL,
		SP, ACC);
}


void CBird::hit(const ObjPtr& rival)
{
	// Polygon
	__super::hit(rival);
}



bool CBird::ApplyDamage(int dam)
{
	// ���S�A�j���[�V�������̓X�L�b�v
	if (state_ == State::DESTROY) return true;
	// �_���[�W���������瑦��
	state_ = State::DESTROY;

	// ���U�G�t�F�N�g
	for (int i = 0; i < 3; ++i)
		gm()->AddObject(ObjPtr(new CEffectExplosion(obj_.pos)));
	// SE
	se::DSound_Play("se_explosion");

	// �s�b�N�A�b�v�A�C�e��
	gm()->AddObject(ObjPtr(new CPickupJewely(obj_.pos)));

	return true;
}

Base::Collisions CBird::GetDamageAreas() const
{
	// ���S�A�j���[�V�������̓X�L�b�v
	if (state_ != State::DESTROY)
	{
		return __super::GetDamageAreas();
	}
	return Base::Collisions();
}

void CBird::SetInfo(const LoadInfo& info)
{

	loadInfo_ = info;
}
