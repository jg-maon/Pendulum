#ifdef _DEBUG
#define DEF_SHAPE_DRAW
#endif

#include "define.h"
#include "dragon.h"
#include "tackle.h"
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
{
	std::vector<int> move = { 0, 1, 2, 3, 2, 1 };
	motionTable_.push_back(move);
	std::vector<int> entry = { 0, 1, 2, 3, 2, 1, 0 };
	motionTable_.push_back(entry);
	std::vector<int> attack = { 0, 1, 2, 3 };
	motionTable_.push_back(attack);
	std::vector<int> fall = { 0, 1, 2, 3 };
	motionTable_.push_back(fall);
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

	obj_.pos = pos;

	startPos_ = obj_.pos;
	endPos_ = obj_.pos;
	backPos_ = obj_.pos;
	actPos_ = obj_.pos;

	obj_.pos.x += loadInfo_.entryWidth;
	obj_.pos.y += loadInfo_.entryHeight;

	elapsedTime_ = 0.f;
	nextActTime_ = 0.f;


	isRoaring_ = false;

	battleState_ = BattleState::ENTRY;
	motionType_ = MotionType::MOVE;
	motionAnim_.set(motionTable_[static_cast<int>(motionType_)].size() - 1, loadInfo_.moveAnimSpeed);
	obj_.src.x = motionTable_[static_cast<int>(motionType_)][motionAnim_.no];


	invincibleTime_ = 0.f;
	invincibleAnim_ = 0.f;

	fallTurnCount_ = 0;

	health_ = loadInfo_.health;
	power_ = loadInfo_.power;

	attack_->setForce(loadInfo_.power);

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
		sm()->setStageState(CStageMng::StageState::BATTLE);
	}
	else{

		DecideState();



		if (isAttacking_)
		{
			if (attack_ != nullptr)
				attack_->step();
		}

		// �v���C���[������
		if (!isBacking_ && !isAttacking_ &&
			battleState_ != BattleState::DESTROY)
		{
			const mymath::Vec3f& plPos = gm()->GetPlayerPos();

			if (obj_.pos.x < plPos.x && turnFlag_)
				turnFlag_ ^= 1;
			else if (obj_.pos.x > plPos.x && !turnFlag_)
				turnFlag_ ^= 1;
		}

		(this->*StateStep_[static_cast<int>(battleState_)])();

		
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
}

void CDragon::draw()
{
	if (attack_ != nullptr)
		attack_->draw();
	mymath::Rectf rect = camera::GetScreenRect();
	if (rect.Contains(obj_.GetRect()))
	{
		obj_.draw(charabase::CharBase::MODE::Center, turnFlag_);
	}

#ifdef _DEBUG
	static const std::string emply_string;
	mymath::Recti rc = obj_.GetRect();

	//rc.offset((int)obj_.pos.x, (int)obj_.pos.y);
	std::stringstream ss;
	ss << "pos("
		<< std::setw(4) << (int)obj_.pos.x
		<< ","
		<< std::setw(4) << (int)obj_.pos.y
		<< ")";
	font::Draw_FontText(
		rc.right, rc.top, 0.f,
		ss.str(), 0xffff0000,
		setting::GetFontID("font_MSG15"));

	ss.str(emply_string);
	const mymath::Vec3f& velocity = obj_.add;
	ss << "add("
		<< std::setw(4) << (int)velocity.x
		<< ","
		<< std::setw(4) << (int)velocity.y
		<< ")";
	font::Draw_FontText(
		rc.right, rc.top + font::Draw_GetCharHeight(setting::GetFontID("font_MSG15")) * 1, 0.f,
		ss.str(), 0xffff0000,
		setting::GetFontID("font_MSG15"));

	ss.str(emply_string);
	ss << "backPos("
		<< std::setw(4) << (int)backPos_.x
		<< ","
		<< std::setw(4) << (int)backPos_.y
		<< ")";
	font::Draw_FontText(
		rc.right, rc.top + font::Draw_GetCharHeight(setting::GetFontID("font_MSG15")) * 2, 0.f,
		ss.str(), 0xffff0000,
		setting::GetFontID("font_MSG15"));

	ss.str(emply_string);
	ss << "endPos("
		<< std::setw(4) << (int)endPos_.x
		<< ","
		<< std::setw(4) << (int)endPos_.y
		<< ")";
	font::Draw_FontText(
		rc.right, rc.top + font::Draw_GetCharHeight(setting::GetFontID("font_MSG15")) * 3, 0.f,
		ss.str(), 0xffff0000,
		setting::GetFontID("font_MSG15"));


#endif
}

void CDragon::EntryStep()
{
	const mymath::Vec3f dist = actPos_ - obj_.pos;
	const float allow = 3.f;	// ���W����̋��e�͈�
	// �ړ���
	if (!isRoaring_)
	{
		if (abs(dist.x) <= allow)
		{
			obj_.pos.x = actPos_.x;
			obj_.add = 0.f;
			obj_.add.y = loadInfo_.moveSpeed;
		}
		else
		{
			obj_.add.x = loadInfo_.moveSpeed;
			if (obj_.pos.x > actPos_.x)
			{
				obj_.add.x = -loadInfo_.moveSpeed;
			}
		}
		if (abs(dist.y) <= allow)
		{
			//���K�n��
			obj_.pos = actPos_;
			obj_.add = 0.f;
			isRoaring_ = true;
			motionType_ = MotionType::ROAR;
			motionAnim_.set(motionTable_[static_cast<int>(motionType_)].size() - 1, loadInfo_.roarAnimSpeed);

		}

		obj_.Move();
	}
	// ���K���͉������Ȃ�
	else
	{
		int max = motionTable_[static_cast<int>(motionType_)].size() - 1;
		// ���K�I��
		if (motionAnim_.no > max - 1)
		{
			battleState_ = BattleState::WAIT;
		}
	}
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
		isBacking_ = false;
		sway_ = 0;

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
		actPos_ = obj_.pos;
		const mymath::Vec3f& plpos = gm()->GetPlayerPos();
		// �O���t�H���ƃv���[���[�Ƃ̊p�x
		float radian = mymath::Vec3f::Angle(obj_.pos, plpos);
		//obj_.angle = angle;

		// �o�b�N����
		obj_.add = mymath::Vec3f::Rotate(radian) * -loadInfo_.moveSpeed;

		nextActTime_ = elapsedTime_ + loadInfo_.attackInterval;		// �A���Ԋu
		isBacking_ = true;

		motionType_ = MotionType::ATTACK;
		motionAnim_.set(motionTable_[static_cast<int>(motionType_)].size() - 1, loadInfo_.backAnimSpeed);
	}
	// ��ޒ��Ȃ�
	if (isBacking_)
	{
		mymath::Vec3f dist = obj_.pos - actPos_;
		float length = mymath::PYTHA(dist.x, dist.y);

		if (motionTable_[static_cast<int>(motionType_)][motionAnim_.no] == 2)
		{
			motionAnim_.stop();
		}

		// ��ނ�����ːi�J�n
		if (length > mymath::POW2(loadInfo_.backDist))
		{
			// �A�j���[�V�����̏C��
			if (motionAnim_.isStoped())
			{
				motionAnim_.no++;
			}
			else
			{
				motionAnim_.no = motionTable_[static_cast<int>(motionType_)].size() - 1;
				motionAnim_.stop();
			}

			isBacking_ = false;
			isAttacking_ = true;
			const mymath::Vec3f& plpos = gm()->GetPlayerPos();
			endPos_ = plpos;
			float radian = mymath::Vec3f::Angle(obj_.pos, endPos_);
			obj_.add = mymath::Vec3f::Rotate(radian) * loadInfo_.attackSpeed;
		}
		else
		{
			obj_.Move();

			if (length > mymath::POW2(loadInfo_.attackDist))
			{
				obj_.add.x = 0.f;
				obj_.add.y = 0.f;
				battleState_ = BattleState::WAIT;
			}
		}
	}
	// �ːi���Ȃ�
	if (isAttacking_)
	{
		mymath::Vec3f dist = obj_.pos - actPos_;
		float length = mymath::PYTHA(dist.x, dist.y);

		if (length > mymath::POW2(loadInfo_.attackDist))
		{
			isAttacking_ = false;
			battleState_ = BattleState::WAIT;
			motionAnim_.start();
			nextActTime_ = elapsedTime_ + loadInfo_.attackInterval;
		}
		else
		{
			obj_.Move();
		}
	}
}

void CDragon::DestroyStep()
{
	if (elapsedTime_ > nextActTime_)
	{
		fallTurnCount_++;
		if (fallTurnCount_ > loadInfo_.fallTurnSpeed)
		{
			fallTurnCount_ = 0;
			turnFlag_ ^= 1;
		}
		obj_.add.x = 0.f;
		obj_.add.y = loadInfo_.fallSpeed;
		obj_.Move();
		motionAnim_.stop();

	}
	// �J�����̊O�ɏo����
	if (obj_.pos.y - obj_.HalfHeight() > camera::GetScreenRect().bottom)
	{
		kill();
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

	// �Ђ�ݏ���
	// �U�����͗h��Ȃ�
	if (!isAttacking_)
	{
		battleState_ = BattleState::DAMAGE;
		nextActTime_ = elapsedTime_ + loadInfo_.damageTime;
		motionType_ = MotionType::FALL;
		motionAnim_.set(motionTable_[static_cast<int>(motionType_)].size() - 1, 0.f);
		motionAnim_.stop();

	}

	invincibleTime_ = loadInfo_.invincibleTime;

	if (health_ <= 0)
	{
		battleState_ = BattleState::DESTROY;
		motionType_ = MotionType::FALL;
		motionAnim_.set(motionTable_[static_cast<int>(motionType_)].size() - 1, loadInfo_.fallTime);
		nextActTime_ = elapsedTime_ + loadInfo_.fallTime;
		return true;
	}

	return false;
}


bool CDragon::isAttacking() const
{
	return isAttacking_;
}
bool CDragon::isBacking() const
{
	return isBacking_;
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
