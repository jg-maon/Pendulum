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
	&CRoboticArm::ChaseStep,
	&CRoboticArm::ReturnStep,
	&CRoboticArm::AttackStep,
	&CRoboticArm::DestroyStep,
};

CRoboticArm::CRoboticArm() :
IEnemy("E_RoboticArm")
{
	//std::vector<int> move = { 1, 2, 4, 3, 2 };
	std::vector<int> move = { 0 };
	motionTable_.push_back(move);
	//std::vector<int> attack = { 1, 0, 1, 3, 4 };
	std::vector<int> attack = { 0 };
	motionTable_.push_back(attack);
	std::vector<int> destroy = { 0 };
	motionTable_.push_back(destroy);
}

CRoboticArm::CRoboticArm(const mymath::Vec3f& pos) :
IEnemy("Atk_E_RoboticArm")
{
	init(pos);
}
CRoboticArm::CRoboticArm(float x, float y, float z) :
IEnemy("E_RoboticArm")
{
	init(mymath::Vec3f(x, y, z));
}

CRoboticArm::~CRoboticArm()
{

}


void CRoboticArm::init(const mymath::Vec3f& pos)
{
	using common::FindChunk;

	gm()->GetData(*this);
	parentObj_ = obj_;
	parentObj_.resname = loadInfo_.parentResname;
	parentObj_.src  = loadInfo_.parentSrcPos;
	parentObj_.size = loadInfo_.parentSrcSize;

	childAngle_ = 0.f;
	rotateDir_ = 1.f;

	// ���A�[���̏C��
	mymath::Vec3f center;  // �x�_�̊i�[(��]�̒��S���W)
	mymath::Vec3f length;  // �x�_���璆�S���W�܂ł̒���
	//�����̏C��
	SwichArmDir();
	// �Œ�A�[��
	parentObj_.pos = pos;
	length = loadInfo_.supportParent - parentObj_.HalfSize();
	center = parentObj_.pos + length;
	float len = length.Length2();
	// ���A�[��
	//length = loadInfo_.supportChild - obj_.HalfSize();
	length = obj_.HalfSize() - loadInfo_.supportChild;
	obj_.pos.x = math::ROUND_X(childAngle_, len, center.x);
	obj_.pos.y = math::ROUND_Y(childAngle_, len, center.y);
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
	case ArmDirectin::RIGHT_UP:
		break;
	case ArmDirectin::RIGHT_DOWN:
		support.y = parentObj_.size.y - loadInfo_.supportParent.y;
		break;
	case ArmDirectin::LEFT_UP:
		angle = 180.f;
		support.x = parentObj_.size.x - loadInfo_.supportParent.x;
		turnParentFlag_ = true;
		break;
	case ArmDirectin::LEFT_DOWN:
		angle += 180.f;
		support.x = parentObj_.size.x - loadInfo_.supportParent.x;
		support.y = parentObj_.size.y - loadInfo_.supportParent.y;
		turnParentFlag_ = true;
		break;
	default:
		break;
	}
	loadInfo_.supportParent = support;
	loadInfo_.maxAngle += angle;
	loadInfo_.minAngle += -angle;
	childAngle_ = angle;
}

void CRoboticArm::step()
{
	ICharacter::step();

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

	(this->*StateStep_[static_cast<int>(state_)])();

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

	// ���A�[���̏C��
	mymath::Vec3f center;  // �x�_�̊i�[(��]�̒��S���W)
	mymath::Vec3f length;  // �x�_���璆�S���W�܂ł̒���
	// �Œ�A�[��
	length = loadInfo_.supportParent - parentObj_.HalfSize();
	center = parentObj_.pos + length;
	// ���A�[��
	//length = loadInfo_.supportChild - obj_.HalfSize();
	length = obj_.HalfSize() - loadInfo_.supportChild;
	obj_.pos.x = math::ROUND_X(childAngle_, length.x, center.x);
	obj_.pos.y = math::ROUND_Y(childAngle_, length.x, center.y);
	//�摜�̊p�x
	obj_.angle = childAngle_;
	// �C�������܂�
}

void CRoboticArm::draw()
{
	if (attack_)
		attack_->draw();
	mymath::Rectf rect = camera::GetScreenRect();
	if (rect.Contains(obj_.GetRect()))
	{
		if (armDir_ == ArmDirectin::LEFT_DOWN || armDir_ == ArmDirectin::RIGHT_DOWN)
		{
			parentObj_.scale.y = -parentObj_.scale.y;
		}
		//�Œ�A�[��
		parentObj_.draw(charabase::CharBase::MODE::Center, turnParentFlag_);

		if (armDir_ == ArmDirectin::LEFT_DOWN || armDir_ == ArmDirectin::RIGHT_DOWN)
		{
			parentObj_.scale.y = -parentObj_.scale.y;
		}

		//���A�[��
		obj_.draw();
	}
}


void CRoboticArm::WaitStep()
{
}

void CRoboticArm::ChaseStep()
{
}

void CRoboticArm::ReturnStep()
{

}

void CRoboticArm::AttackStep()
{
	// �U��
	if (elapsedTime_ > nextActTime_)
	{
		state_ = State::WAIT;
		elapsedTime_ = 0;
		rotateDir_ *= -1;
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
	length = loadInfo_.supportParent - parentObj_.HalfSize();
	center = parentObj_.pos + length;
	length = obj_.HalfSize() - loadInfo_.supportChild;

	CArmAttack armAtk(attack_->obj());// �A�[���U���̏��
	armAtk.obj(obj_);
	armAtk.setForce(attack_->GetForce());
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
	gm()->AddObject(ObjPtr(new CArmAttack(armAtk,
		childAngle_, length, center, speed, max)));
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

void CRoboticArm::SetInfo(const LoadInfo& info)
{

	loadInfo_ = info;
}

void CRoboticArm::SetArmDirection(ArmDirectin dir)
{
	armDir_ = dir;
}
