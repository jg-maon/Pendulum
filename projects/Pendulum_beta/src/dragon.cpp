#ifdef _DEBUG
#define DEF_SHAPE_DRAW
#endif

#include "define.h"
#include "dragon.h"
#include "fireShot.h"

#include "easing.h"

#include "setting.h"

//#include "player.h"
#include "common.h"

#include "actionPoint.h"

void (CDragon::*CDragon::StateStep_[])() =
{
	&CDragon::EntryStep,
	&CDragon::WaitStep,
	&CDragon::ChaseStep,
	&CDragon::DamageStep,
	&CDragon::AttackStep,
	&CDragon::DestroyStep,
};

CDragon::CDragon() :
IEnemy("E_Boss_Dragon")
, exclamationObj_(mymath::Vec3f(), mymath::Vec3f(), "img_UIexclamation", 32,64)
{
	motionTable_.resize(static_cast<int>(MotionType::MOTION_NUM));
	std::vector<int> move = { 0, 1, 2, 3, 2, 1 };
	motionTable_[static_cast<int>(MotionType::MOVE)] = move;
	std::vector<int> attack = { 0, 1, 2, 3 };
	motionTable_[static_cast<int>(MotionType::ATTACK)] = (attack);
	std::vector<int> roar = { 0, 1, 2, 3, 2, 1, 0 };
	motionTable_[static_cast<int>(MotionType::ROAR)] = (roar);
	std::vector<int> fall = { 0, 1, 2, 3 };
	motionTable_[static_cast<int>(MotionType::FALL)] = (fall);

	exclamationObj_.g = 10.f;
	exclamationObj_.b = 10.f;
}

CDragon::CDragon(const mymath::Vec3f& pos) :
IEnemy("E_Boss_Dragon")
{
	init(pos);
}
CDragon::CDragon(float x, float y, float z) :
IEnemy("E_Boss_Dragon")
{
	init(mymath::Vec3f(x, y, z));
}

CDragon::~CDragon()
{

}


void CDragon::init(const mymath::Vec3f& pos)
{

	gm()->GetData(*this);

	startPos_ = obj_.pos = pos;

	obj_.pos.x += loadInfo_.entryWidth;
	obj_.pos.y += loadInfo_.entryHeight;

	elapsedTime_ = 0.f;
	nextActTime_ = 0.f;


	isSePlay_ = true;

	battleState_ = BattleState::WAIT;
	motionType_ = MotionType::MOVE;
	motionAnim_.set(motionTable_[static_cast<int>(motionType_)].size() - 1, loadInfo_.moveAnimSpeed);
	obj_.src.x = motionTable_[static_cast<int>(motionType_)][motionAnim_.no];

	obj_.SetUse(false);

	invincibleTime_ = 0.f;
	invincibleAnim_ = 0.f;


	health_ = loadInfo_.health;
	power_ = loadInfo_.power;


	sway_ = 0.f;
}

void CDragon::step()
{
	ICharacter::step();
	elapsedTime_ += system::FrameTime;

	// �q�b�g�X�g�b�v���̓X�L�b�v
	if (!isUpdatable()) return;

	// ���G
	if (isInvincible())
	{
		// �_�ŃA�j��
		invincibleAnim_ += system::ONEFRAME_TIME;
		if (invincibleAnim_ >= loadInfo_.invincibleTime / 20.f)
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

	// �A�j���[�V��������
	if (motionAnim_.step())
	{
		if (motionType_ != MotionType::MOVE)
		{
			obj_.src.x = 0;
			obj_.src.y = 0;
			motionType_ = MotionType::MOVE;
			motionAnim_.set(motionTable_[static_cast<int>(motionType_)].size() - 1, loadInfo_.moveAnimSpeed);
		}
	}
	else
	{
		obj_.src.x = motionTable_[static_cast<int>(motionType_)][motionAnim_.no];
		obj_.src.y = static_cast<int>(motionType_);
	}

	// �o��A�j���[�V����
	if (sm()->isBossEnterAnimating())
	{
		if (battleState_ != BattleState::ENTRY)
		{
			battleState_ = BattleState::ENTRY;
			nextActTime_ = elapsedTime_ + 4.f;		// �u�U�[�̒���
			frameAnimTime_ = 0.f;
		}
	}
	else if (sm()->isBossExitAnimating())
	{
		if (battleState_ != BattleState::DESTROY)
		{
			battleState_ = BattleState::DESTROY;
		}
	}
	else
	{

		DecideState();


		if (attack_ != nullptr)
			attack_->step();
		

		// �v���C���[������
		if (battleState_ != BattleState::DESTROY)
		{
			const mymath::Vec3f& plPos = gm()->GetPlayerPos();

			if (obj_.pos.x < plPos.x && turnFlag_)
				turnFlag_ ^= 1;
			else if (obj_.pos.x > plPos.x && !turnFlag_)
				turnFlag_ ^= 1;
		}

		// �X�e�[�W���W����
		{
			auto& col = std::dynamic_pointer_cast<mymath::Rectf>(stageCollisions_[0]);
			mymath::Vec3f size;
			size.x = col->right - col->left;
			size.y = col->bottom - col->top;
			//const mymath::Vec3f size = obj_.size / 2;
			const auto& stageRect = sm()->getStageRect();
			obj_.pos.x = clamp(obj_.pos.x, stageRect.left + size.x, stageRect.right - size.x);
			obj_.pos.y = clamp(obj_.pos.y, stageRect.top + size.y, stageRect.bottom - size.y);
		}
	}

	(this->*StateStep_[static_cast<int>(battleState_)])();



}

void CDragon::draw()
{
	// �ԃt���[��
	graph::Draw_GraphicsLeftTopNC(
		0, 0, 0.0f, "img_UIframeRed",
		0, 0, system::WINW, system::WINH,
		0.f, nullptr,
		1.f, 1.f,
		frameAlpha_);

	// !�}�[�N
	if (exclamationObj_.CheckUse())
		exclamationObj_.draw();

	// �U��
	if (attack_ != nullptr)
		attack_->draw();

	// �g��
	if (obj_.CheckUse())
	{
		mymath::Rectf rect = camera::GetScreenRect();
		if (rect.Contains(obj_.GetRect()))
		{
			obj_.draw(charabase::CharBase::MODE::Center, turnFlag_);
		}
	}
}

void CDragon::EntryStep()
{
	switch (enterPhase_)
	{
	case CDragon::EnterAnimPhase::BUZZER:
		if (isSePlay_)
		{
			se::DSound_Play("se_buzzer");	// ���K
			isSePlay_ = false;
		}
		frameAlpha_ = 50.f + 200.f * (std::sinf(frameAnimTime_));
		frameAnimTime_ += system::FrameTime;
		if (frameAnimTime_ >= mymath::PI)
			frameAnimTime_ -= mymath::PI;

		if (elapsedTime_ >= nextActTime_)
		{
			// �u�U�[�I��
			frameAlpha_ = 0.f;
			//---------------------------------
			// ���K�J�n
			enterPhase_ = EnterAnimPhase::ROAR;
			isSePlay_ = true;
			nextActTime_ = elapsedTime_ + 5.f;
			//---------------------------------
			// �I�}�[�N
			auto& player = gm()->GetPlayer();
			exclamationObj_.pos = player->obj().pos;
			exclamationObj_.pos += mymath::Vec3f(30.f, -50.f, 0.f);
			exclamationObj_.SetUse(true);
			exclamationAnim_.set(2, 0.3f);
			cnt_ = 0;
			//---------------------------------
			// �J�����h��
			sway_ = 0.f;
		}
		break;
	case CDragon::EnterAnimPhase::ROAR:
	{

		if (isSePlay_)
		{
			se::DSound_Play("se_dragonRoar");	// ���K
			isSePlay_ = false;
		}
		// !�}�[�N�A�j���[�V����
		if (exclamationAnim_.step())
		{
			// ���[�v������
			if (++cnt_ >= 2)
			{
				// �w��񐔃��[�v��\������߂�
				exclamationObj_.SetUse(false);
				exclamationAnim_.stop();
			}
		}
		exclamationObj_.src.x = exclamationAnim_.no;

		// �J�����h��
		cameraPos_ = camera::GetLookAt();
		sway_ = static_cast<float>((static_cast<int>(sway_ + 45) % 360));
		cameraPos_.y += (std::cosf(math::Calc_DegreeToRad(sway_)) * 20.f);
		camera::SetLookAt(cameraPos_.x, cameraPos_.y);

		// �o�ߎ��ԏI������
		if (elapsedTime_ >= nextActTime_)
		{
			// ����J�n
			enterPhase_ = EnterAnimPhase::ENTER;
			nextActTime_ = elapsedTime_;
			cameraPos_ = camera::GetLookAt();
			isSePlay_ = true;
		}
	}
		break;
	case CDragon::EnterAnimPhase::ENTER:
		if (elapsedTime_ < nextActTime_ + 1.f)
		{
			// �J�����ړ�
			mymath::Vec3f newCameraPos = cameraPos_;
			newCameraPos.x = Easing::CircOut(elapsedTime_ - nextActTime_, cameraPos_.x, obj_.pos.x - cameraPos_.x, 1.f);
			newCameraPos.y = Easing::CircInOut(elapsedTime_ - nextActTime_, cameraPos_.y, obj_.pos.y - cameraPos_.y, 1.f);
			
			sm()->MoveCamera(newCameraPos);
		}
		else if (elapsedTime_ < nextActTime_ + 2.f)
		{
			if (isSePlay_)
			{
				se::DSound_Play("se_swing");
				isSePlay_ = false;
			}
			obj_.SetUse(true);
			turnFlag_ = true;
			// ������
			mymath::Vec3f sta = startPos_ + mymath::Vec3f(loadInfo_.entryWidth, -loadInfo_.entryHeight);
			mymath::Vec3f end = startPos_ + mymath::Vec3f(-loadInfo_.entryWidth, loadInfo_.entryHeight);
			obj_.pos.x = Easing::Linear(elapsedTime_ - nextActTime_ - 1.f, sta.x, end.x - sta.x, 1.f);
			obj_.pos.y = Easing::Linear(elapsedTime_ - nextActTime_ - 1.f, sta.y, end.y - sta.y, 1.f);
		}
		else if (elapsedTime_ < nextActTime_ + 3.f)
		{
			turnFlag_ = false;
			// �E����
			mymath::Vec3f sta = startPos_ + mymath::Vec3f(-loadInfo_.entryWidth, -loadInfo_.entryHeight);
			mymath::Vec3f end = startPos_ + mymath::Vec3f(loadInfo_.entryWidth, loadInfo_.entryHeight);
			obj_.pos.x = Easing::Linear(elapsedTime_ - nextActTime_ - 2.f, sta.x, end.x - sta.x, 1.f);
			obj_.pos.y = Easing::Linear(elapsedTime_ - nextActTime_ - 2.f, sta.y, end.y - sta.y, 1.f);
		}
		else
		{
			const float animTime = 1.5f;
			turnFlag_ = true;
			// ��ʏォ��
			mymath::Vec3f sta = startPos_ + mymath::Vec3f(0.f, (gm()->winRect()->height()-obj_.HalfHeight()) / 2.f);
			obj_.pos.x = Easing::Linear(elapsedTime_ - nextActTime_ - 3.f, sta.x, startPos_.x - sta.x, animTime);
			obj_.pos.y = Easing::Linear(elapsedTime_ - nextActTime_ - 3.f, sta.y, startPos_.y - sta.y, animTime);

			if ((elapsedTime_ >= nextActTime_ + 3.f + animTime)
				|| obj_.pos.y > startPos_.y)
			{
				// �o��I��
				sm()->setStageState(CStageMng::StageState::BATTLE);
				battleState_ = BattleState::WAIT;
			}
		}
		break;
	default:
		break;
	}
	
	//sm()->setStageState(CStageMng::StageState::BATTLE);
}

void CDragon::WaitStep()
{

}

void CDragon::ChaseStep()
{
	const mymath::Vec3f& plPos = gm()->GetPlayerPos();
	const float radian = mymath::Vec3f::Angle(obj_.pos, plPos);
	obj_.add = mymath::Vec3f::Rotate(radian) * loadInfo_.moveSpeed;
	obj_.Move();
}

void CDragon::DamageStep()
{
	if (elapsedTime_ > nextActTime_ + loadInfo_.damageTime)
	{
		sway_ = 0.f;

		battleState_ = BattleState::WAIT;
		motionType_ = MotionType::MOVE;
		motionAnim_.set(motionTable_[static_cast<int>(motionType_)].size() - 1, loadInfo_.moveAnimSpeed);
	}

	sway_ = static_cast<float>((static_cast<int>(sway_ + 45) % 360));
	// ���h��
	obj_.add.x = std::cosf(math::Calc_DegreeToRad(sway_)) * loadInfo_.swayRange;
	obj_.add.y = 0.f;
	obj_.Move();
}

void CDragon::AttackStep()
{
	// �s�����N�������ԂɂȂ������ފJ�n
	if (elapsedTime_ > nextActTime_)
	{
		// �U��
		mymath::Vec3f pos = obj_.pos + mymath::Vec3f(loadInfo_.attackOffsetX * (turnFlag_ ? -1.f : 1.f), loadInfo_.attackOffsetY);
		std::dynamic_pointer_cast<CFireShot>(attack_)->CreateAttack(
			pos, gm()->GetPlayerPos(), loadInfo_.attackSpeed);

		se::DSound_Play("se_fireBreath");

		nextActTime_ = elapsedTime_ + loadInfo_.attackInterval;		// �A���Ԋu
		
		motionType_ = MotionType::ATTACK;
		motionAnim_.set(motionTable_[static_cast<int>(motionType_)].size() - 1, 0.1f);
	}
	
}

void CDragon::DestroyStep()
{
	if (elapsedTime_ > nextActTime_ + 1.f)
	{
		// �J�����O�֔����鏀��(���ړ�)
		turnFlag_ = false;
		obj_.pos.x = Easing::QuintOut(elapsedTime_ - nextActTime_, startPos_.x, -400.f, 1.f);
	}
	else
	{
		// �J�����O�֔��œ�����
		if (isSePlay_)
		{
			se::DSound_Play("se_swing");
			isSePlay_ = false;
			startPos_ = obj_.pos;
		}
		mymath::Vec3f end = startPos_ + mymath::Vec3f(-loadInfo_.entryWidth, -loadInfo_.entryHeight);
		obj_.pos.x = Easing::Linear(elapsedTime_ - nextActTime_ - 1.f, startPos_.x, end.x - startPos_.x, 2.f);
		obj_.pos.y = Easing::Linear(elapsedTime_ - nextActTime_ - 1.f, startPos_.y, end.y - startPos_.y, 2.f);

		if (!InScreen(-obj_.HalfWidth()))
		{
			// ��ʊO
			kill();
		}
	}
}

void CDragon::DecideState()
{
	if (battleState_ == BattleState::DESTROY)
	{
		// ���S�� �����͋�����Ȃ�
		return;
	}

	if (battleState_ == BattleState::DAMAGE)
	{
		return;
	}

	if (battleState_ == BattleState::ENTRY)
	{
		return;
	}

	const mymath::Vec3f& plPos = gm()->GetPlayerPos();
	// �v���C���[�Ƃ̋����x�N�g�� e -> p
	mymath::Vec3f Vdist = plPos - obj_.pos;
	// �v���C���[�Ƃ̒�������
	const float plyDist = mymath::PYTHA(Vdist.x, Vdist.y);

	// �����ʒu����̃x�N�g�� start -> now
	Vdist = obj_.pos - startPos_;
	const float staDist = mymath::PYTHA(Vdist.x, Vdist.y);

	// �U���͈͓����H
	if (plyDist < mymath::POW2(loadInfo_.attackRange)
		|| battleState_ == BattleState::ATTACK)
	{
		// �U����
		battleState_ = BattleState::ATTACK;
	}
	else if (plyDist < mymath::POW2(loadInfo_.searchRange))
	{
		battleState_ = BattleState::CHASE;
		/*
		// �U���͈͊O ���G�͈͓�
		if (plyDist < mymath::POW2(loadInfo_.chaseRange))
		{
		// �ǐՉ\�͈͓�
		battleState_ = BattleState::CHASE;
		}
		else
		{
		battleState_ = BattleState::CHASE;
		}
		//*/
	}
	else
	{
		// �ی�(�e�s���̍Ō�ɂ�WAIT�ɖ߂��Ă�͂�)
		battleState_ = BattleState::WAIT;
	}
}


bool CDragon::isInvincible() const
{
	return invincibleTime_ > 0.f;
}

void CDragon::hit(const ObjPtr& rival)
{
	// Polygon
	//__super::hit(rival);
}




bool CDragon::ApplyDamage(int dam)
{
	// ���S�A�j���[�V�������̓X�L�b�v
	if (battleState_ == BattleState::DESTROY) return true;

	// ���G��
	if (isInvincible()) return false;

	health_ -= dam;

	

	invincibleTime_ = loadInfo_.invincibleTime;
	
	if (health_ <= 0)
	{
		// ���S

		battleState_ = BattleState::DESTROY;
		isSePlay_ = true;
		motionType_ = MotionType::MOVE;
		motionAnim_.set(motionTable_[static_cast<int>(motionType_)].size() - 1, loadInfo_.moveAnimSpeed);
		nextActTime_ = elapsedTime_;
		startPos_ = obj_.pos;
		return true;
	}
	else //	if (!isAttacking_)	// �U�����͗h��Ȃ�
	{
		// �Ђ�ݏ���

		battleState_ = BattleState::DAMAGE;
		nextActTime_ = elapsedTime_ + loadInfo_.damageTime;
		motionType_ = MotionType::FALL;
		motionAnim_.set(motionTable_[static_cast<int>(motionType_)].size() - 1, 0.f);
		motionAnim_.stop();

	}

	return false;
}



Base::Collisions CDragon::GetDamageAreas() const
{
	// ��_���㖳�G���A���S�A�j���[�V�������̓X�L�b�v
	if (isInvincible() || battleState_ == BattleState::DESTROY)
	{
		return Base::Collisions();
	}
	return __super::GetDamageAreas();
}

void CDragon::SetInfo(const LoadInfo& info)
{

	loadInfo_ = info;
}
