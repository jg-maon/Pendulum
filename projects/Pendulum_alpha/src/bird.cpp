#ifdef _DEBUG
//#define USE_CIRCLE_EXT
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
	std::ifstream f("res/txt/enemy/bird.txt");
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
	// �U��
	if(elapsedTime_ > nextActTime_)
	{
		CreateAttack();
		state_ = State::WAIT;
		nextActTime_ = elapsedTime_ + 2.f;		// �A���Ԋu
	}
}

void CBird::DestroyStep()
{
	// 0.5�b
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
	if (plyDist < mymath::POW2(ATTACK_RANGE) || state_ == State::ATTACK)
	{
		// �U���͈͓� or �U����
		state_ = State::ATTACK;
	}
	else if (plyDist < mymath::POW2(SEARCH_RANGE))
	{
		// �U���͈͊O ���G�͈͓�
		if (staDist < mymath::POW2(CHASE_RANGE))
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
	else if (staDist > mymath::POW2(RETURN_RANGE))
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

void CBird::CreateAttack()
{
	const mymath::Vec3f& mypos = obj_.pos;
	const mymath::Vec3f& plpos = gm()->GetPlayerPos();
	const mymath::Vec3f vec = plpos - mypos;
	float angle = math::Calc_RadToDegree(std::atan2f(-vec.y, vec.x));
	const float INTERVAL = 20.f;	// ���Ԋu
	const float SP = 70.f;			// �����x
	const float ACC = 5.f;			// �����x
	std::dynamic_pointer_cast<CNWayShot>(attack_)->CreateAttack(
				mypos,
				5,
				angle,INTERVAL,
				SP,ACC);
}

void CBird::step()
{
	ICharacter::step();
	elapsedTime_ += system::FrameTime;
	
	DecideState();
	
	if(attack_ != nullptr)
		attack_->step();

	// �v���C���[������
	const mymath::Vec3f& plPos = gm()->GetPlayerPos();
	
	if (obj_.pos.x < plPos.x)
		obj_.scale.x = 1.f;
	else
		obj_.scale.x = -1.f;
	
	(this->*StateStep_[static_cast<int>(state_)])();

}

void CBird::draw()
{
	if(attack_ != nullptr)
		attack_->draw();
	mymath::Rectf rect = camera::GetScreenRect();
	if(rect.Contains(obj_.GetRect()))
	{
		obj_.draw();
	}
}


void CBird::hit(const ObjPtr& rival)
{
	if(rival->FindName("ActionPolygon"))
	{
		// �߂荞�ݕ␳,�ʉߕ␳
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
	// ���S�A�j���[�V�������̓X�L�b�v
	if(state_ == State::DESTROY) return true;
	// �_���[�W���������瑦��
	state_ = State::DESTROY;
	collisions_.clear();	// ���S�A�j���[�V�������̓����蔻�������

	//
	//// ���U�G�t�F�N�g
	//InsertObject(ObjPtr(new CEffectExplosion(obj_.pos)));
	//// SE
	//DSound_Play(SE::EXPLODE);
	
	return true;
}

Base::Collisions CBird::GetCollisionAreas()
{
	Base::Collisions cols;	// return�p
	// ���S�A�j���[�V�������̓X�L�b�v
	if(state_ != State::DESTROY)
	{
		return super::GetCollisionAreas();
	}
	return cols;
}
