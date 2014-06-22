#ifdef _DEBUG
#define DEF_SHAPE_DRAW
#endif


#include "define.h"
#include "fairy.h"
#include "nWayShot.h"
#include "nWaySing.h"
#include "setting.h"

//#include "player.h"
#include "common.h"

#include "actionPoint.h"

/*
const float CFairy::SEARCH_RANGE = 500.0f;
const float CFairy::CHASE_RANGE = 300.0f;
const float CFairy::ATTACK_RANGE = 100.f;
const float CFairy::RETURN_RANGE = 3.f;
const float CFairy::MOVE_SPEED = 100.f;
//*/

void (CFairy::*CFairy::StateStep_[])() =
{
	&CFairy::WaitStep,
	&CFairy::ChaseStep,
	&CFairy::ReturnStep,
	&CFairy::AttackStep,
	&CFairy::DestroyStep,
};

CFairy::CFairy() :
IEnemy("E_Fairy")
{
}

CFairy::CFairy(const mymath::Vec3f& pos) :
IEnemy("E_Fairy")
{
	init(pos);
}
CFairy::CFairy(float x, float y, float z) :
IEnemy("E_Fairy")
{
	init(mymath::Vec3f(x, y, z));
}

CFairy::~CFairy()
{

}


void CFairy::init(const mymath::Vec3f& pos)
{
	using common::FindChunk;

	gm()->GetData(*this);

	obj_.pos = pos;

	startPos_ = obj_.pos;

	elapsedTime_ = 0.f;
	nextActTime_ = 0.f;
	state_ = State::WAIT;
	//obj_.alpha = 200.f;

	anim_.set(3, 0.3f);

}

void CFairy::step()
{
	ICharacter::step();
	elapsedTime_ += system::FrameTime;

	DecideState();

	if (attack_ != nullptr)
		attack_->step();

	// �v���C���[������
	const mymath::Vec3f& plPos = gm()->GetPlayerPos();

	if (obj_.pos.x < plPos.x && turnFlag_)
		turnFlag_ ^= 1;
	else if (obj_.pos.x > plPos.x && !turnFlag_)
		turnFlag_ ^= 1;


	// �A�j���[�V����
	int animTable[] = { 0, 1, 2, 1, };
	anim_.step();
	obj_.src.x = animTable[anim_.no];

	(this->*StateStep_[static_cast<int>(state_)])();

}

void CFairy::draw()
{
	if (attack_ != nullptr)
		attack_->draw();
	mymath::Rectf rect = camera::GetScreenRect();
	if (rect.Contains(obj_.GetRect()))
	{
		obj_.draw(charabase::CharBase::MODE::Center, turnFlag_);
	}
#ifdef DEF_SHAPE_DRAW
	const auto& cols = GetCollisionAreas();
	for (const auto& col : cols)
	{
		col->Offset(mymath::Vec3f(0.f, 0.f, -0.1f));
		col->draw();
	}
#endif
}


void CFairy::WaitStep()
{

}

void CFairy::ChaseStep()
{
	const mymath::Vec3f& plPos = gm()->GetPlayerPos();
	const mymath::Vec3f dist = plPos - obj_.pos;
	float angle = std::atan2f(dist.y, dist.x);
	obj_.add = mymath::Vec3f::Rotate(angle) * loadInfo_.MOVE_SPEED;
	obj_.Move();
}

void CFairy::ReturnStep()
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

void CFairy::AttackStep()
{
	// �U��
	if (elapsedTime_ > nextActTime_)
	{
		CreateAttack();
		state_ = State::WAIT;
		nextActTime_ = elapsedTime_ + 2.f;		// �A���Ԋu
	}
}

void CFairy::DestroyStep()
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

void CFairy::DecideState()
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
		state_ = State::ATTACK;
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
	}
}

void CFairy::CreateAttack()
{

	const mymath::Vec3f& mypos = obj_.pos;

	int dice = gplib::math::GetRandom<int>(0, 9);
	float angle = gplib::math::GetRandom(0.f, 360.f);
	
	const float INTERVAL = 20.f;	// ���Ԋu
	const float SP = 70.f;			// �����x
	const float ACC = 5.f;			// �����x

	if (dice < 2) {
		// 2/10�̊m���ŉ̍U�����s��
		std::dynamic_pointer_cast<CNWaySing>(attack_)->CreateAttack(
			mypos,
			5,
			angle,
			SP, ACC);
	} else {
	// 8/10�̊m���œG����������
	    // HaleEnemy();
	};

	
}


void CFairy::hit(const ObjPtr& rival)
{
	if (rival->FindName("ActionPolygon"))
	{
		// �߂荞�ݕ␳,�ʉߕ␳
		const auto& ap = std::dynamic_pointer_cast<CActionPolygon>(rival);
		mymath::Vec3f dist = obj_.pos - prePos_;
		mymath::Vec3f intersection;
		intersection = ap->IntersectionPoint2Nearest(prePos_, obj_.pos);
		obj_.pos = intersection;
		obj_.pos -= dist.Normalize();

	}
}



bool CFairy::ApplyDamage(int dam)
{
	// ���S�A�j���[�V�������̓X�L�b�v
	if (state_ == State::DESTROY) return true;
	// �_���[�W���������瑦��
	state_ = State::DESTROY;

	//
	//// ���U�G�t�F�N�g
	//InsertObject(ObjPtr(new CEffectExplosion(obj_.pos)));
	//// SE
	//DSound_Play(SE::EXPLODE);

	return true;
}

Base::Collisions CFairy::GetCollisionAreas() const
{
	// ���S�A�j���[�V�������̓X�L�b�v
	if (state_ != State::DESTROY)
	{
		return __super::GetCollisionAreas();
	}
	return Base::Collisions();
}

void CFairy::SetInfo(const LoadInfo& info)
{

	loadInfo_ = info;
}
