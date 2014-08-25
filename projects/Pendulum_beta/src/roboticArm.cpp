#ifdef _DEBUG

#endif

#include "define.h"
#include "roboticArm.h"
#include "armAttack.h"
#include "setting.h"

//#include "player.h"
#include "common.h"

#include "actionPoint.h"

//-------------------------------------------------------------------

void (CRoboticArm::*CRoboticArm::StateStep_[])() =
{
	&CRoboticArm::WaitStep,
	&CRoboticArm::AttackStep,
	&CRoboticArm::DestroyStep,
};

CRoboticArm::CRoboticArm() :
IEnemy("E_RoboticArm")
{
	std::vector<int> move = { 0, 0 };
	motionTable_.push_back(move);
	//std::vector<int> attack = { 1, 0, 1, 3, 4 };
	std::vector<int> attack = { 0 };
	motionTable_.push_back(attack);
	std::vector<int> destroy = { 0 };
	motionTable_.push_back(destroy);
}

CRoboticArm::CRoboticArm(const mymath::Vec3f& pos, int dir) :
IEnemy("E_RoboticArm")
{
	init(pos, dir);
}
CRoboticArm::CRoboticArm(float x, float y, int dir, float z) :
IEnemy("E_RoboticArm")
{
	init(mymath::Vec3f(x, y, z), dir);
}

CRoboticArm::~CRoboticArm()
{

}


void CRoboticArm::init(const mymath::Vec3f& pos, int dir)
{
	using common::FindChunk;

	gm()->GetData(*this);

	charabase::CharBase childObj;
	childObj.resname = loadInfo_.childResname;
	childObj.src = loadInfo_.childSrcPos;
	childObj.size = loadInfo_.childSrcSize;

	childAngle_ = 0.f;
	rotateDir_ = 1.f;

	mymath::Vec3f center;  // �x�_�̊i�[(��]�̒��S���W)
	mymath::Vec3f length;  // �x�_���璆�S���W�܂ł̒���
	
	// �����̏C��
	SetArmDirection(dir);
	SwichArmDir();
	// �Œ�A�[���̏C��
	obj_.pos = pos;
	length = loadInfo_.supportParent - obj_.HalfSize();
	center = obj_.pos + length;
	float len = length.Length2();
	// ���A�[��
	length = childObj.HalfSize() - loadInfo_.supportChild;

	if (attack_ != nullptr)
	{
		attack_->obj(childObj);
		dynamic_pointer_cast<CArmAttack>(attack_)->SetArmInfo(childAngle_, len, center);
	}
	// �C�������܂�

	elapsedTime_ = 0.f;
	nextActTime_ = 0.f;
	state_ = State::WAIT;
	motionType_ = MotionType::MOVE;
	motionAnim_.set(motionTable_[static_cast<int>(motionType_)].size() - 1, 0.3f);
	obj_.src.x = motionTable_[static_cast<int>(motionType_)][motionAnim_.no];

}

void CRoboticArm::SwichArmDir()
{
	float angle = 0.f;
	mymath::Vec3f support = loadInfo_.supportParent;
	switch (armDir_)
	{
	case ArmDirection::RIGHT_UP:
		break;
	case ArmDirection::RIGHT_DOWN:
		support.y = obj_.size.y - loadInfo_.supportParent.y;
		break;
	case ArmDirection::LEFT_UP:
		angle += 180.f;
		support.x = obj_.size.x - loadInfo_.supportParent.x;
		turnParentFlag_ = true;
		break;
	case ArmDirection::LEFT_DOWN:
		angle += 180.f;
		support.x = obj_.size.x - loadInfo_.supportParent.x;
		support.y = obj_.size.y - loadInfo_.supportParent.y;
		turnParentFlag_ = true;
		break;
	default:
		break;
	}
	loadInfo_.supportParent = support;
	loadInfo_.maxAngle += angle;
	loadInfo_.minAngle += angle;
	childAngle_ = angle;
}

void CRoboticArm::step()
{
	ICharacter::step();

	DecideState();

	// �q�b�g�X�g�b�v���̓X�L�b�v
	if (!isUpdatable())
		return;

	(this->*StateStep_[static_cast<int>(state_)])();

	if (attack_ != nullptr)
	{
		// ���A�[���̉�]
		childAngle_ += loadInfo_.rotateSpeed * rotateDir_;

		if (childAngle_ > loadInfo_.maxAngle)
		{
			childAngle_ = loadInfo_.maxAngle;
			elapsedTime_ += system::FrameTime;
		}
		else if (childAngle_ < loadInfo_.minAngle)
		{
			childAngle_ = loadInfo_.minAngle;
			elapsedTime_ += system::FrameTime;
		}

		dynamic_pointer_cast<CArmAttack>(attack_)->Rotate(childAngle_);


		// ���A�[���̏C��
		mymath::Vec3f center;  // �x�_�̊i�[(��]�̒��S���W)
		mymath::Vec3f length;  // �x�_���璆�S���W�܂ł̒���
		// �Œ�A�[��
		length = loadInfo_.supportParent - obj_.HalfSize();
		center = obj_.pos + length;
		// ���A�[��
		//length = loadInfo_.supportChild - obj_.HalfSize();
		length = obj_.HalfSize() - loadInfo_.supportChild;
		//obj_.pos.x = math::ROUND_X(childAngle_, length.x, center.x);
		//obj_.pos.y = math::ROUND_Y(childAngle_, length.x, center.y);
		//�摜�̊p�x
		//obj_.angle = childAngle_;
		// �C�������܂�
	}
}

void CRoboticArm::draw()
{

	mymath::Rectf rect = camera::GetScreenRect();
	if (rect.Contains(obj_.GetRect()))
	{
		// �Œ�A�[���̏㉺���]
		if (armDir_ == ArmDirection::LEFT_DOWN || armDir_ == ArmDirection::RIGHT_DOWN)
		{
			obj_.scale.y = -obj_.scale.y;
		}
		//�Œ�A�[��
		obj_.draw(charabase::CharBase::MODE::Center, turnParentFlag_);

		// �Œ�A�[���̏㉺���]�̏C��
		if (armDir_ == ArmDirection::LEFT_DOWN || armDir_ == ArmDirection::RIGHT_DOWN)
		{
			obj_.scale.y = -obj_.scale.y;
		}

	}
	if (attack_)
		if (rect.Contains(attack_->obj().GetRect()))
			attack_->draw();
}


void CRoboticArm::WaitStep()
{
}

void CRoboticArm::AttackStep()
{
	// �U��
	if (elapsedTime_ > nextActTime_)
	{
		state_ = State::WAIT;
		elapsedTime_ = 0;
		rotateDir_ = -rotateDir_;
		CreateAttack();
	}
}

void CRoboticArm::DestroyStep()
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

void CRoboticArm::DecideState()
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
	if (plyDist < mymath::POW2(loadInfo_.attackRange) || state_ == State::ATTACK)
	{
		// �U���͈͓� or �U����
		if (state_ != State::ATTACK)
			nextActTime_ = elapsedTime_ + loadInfo_.attackInterval;
		state_ = State::ATTACK;
		motionType_ = MotionType::ATTACK;
	}
	else
	{
		// �ی�(�e�s���̍Ō�ɂ�WAIT�ɖ߂��Ă�͂�)
		state_ = State::WAIT;
		motionType_ = MotionType::MOVE;
	}
}

void CRoboticArm::CreateAttack()
{
	mymath::Vec3f center;
	mymath::Vec3f length;
	length = loadInfo_.supportParent - obj_.HalfSize();
	center = obj_.pos + length;
	length = obj_.HalfSize() - loadInfo_.supportChild;

	/*
	CArmAttack armAtk(attack_->obj());// �A�[���U���̏��
	armAtk.obj(obj_);
	armAtk.SetCollisionAreas(collisions_);
	// ��]�ł���ő�l
	float max = 0;
	// ��]���x
	float speed = loadInfo_.rotateSpeed * rotateDir_;
	if (rotateDir_ > 0)
	{
		max = loadInfo_.maxAngle;
	}
	else if (rotateDir_ < 0)
	{
		max = loadInfo_.minAngle;
	}
	//*/
}


void CRoboticArm::hit(const ObjPtr& rival)
{
	// Polygon
	__super::hit(rival);
}

bool CRoboticArm::ApplyDamage(int dam)
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

Base::Collisions CRoboticArm::GetDamageAreas() const
{
	// ���S�A�j���[�V�������̓X�L�b�v
	if (state_ != State::DESTROY)
	{
		return __super::GetDamageAreas();
	}
	return Base::Collisions();
}

void CRoboticArm::SetInfo(const CRoboticArm::LoadInfo& info)
{
	loadInfo_ = info;
}

void CRoboticArm::SetArmDirection(CRoboticArm::ArmDirection dir)
{
	armDir_ = dir;
}

void CRoboticArm::SetArmDirection(const int dir)
{
	armDir_ = static_cast<ArmDirection>(dir);
}
