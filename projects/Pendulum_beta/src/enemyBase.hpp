#ifndef DEF_ENEMYBASE_HPP
#define DEF_ENEMYBASE_HPP

#ifndef DE_CHARACTERBASE_H
#include "characterBase.h"
#endif

#ifndef DEF_ATTACKBASE_HPP
#include "attackBase.hpp"
#endif

#ifndef DEF_MYMATH_HPP
#include "MyMath.hpp
#endif

#ifndef DEF_NWAYSHOT_H
#include "nWayShot.h"
#endif

#ifndef DEF_NWAYSING_H
#include "nWaySing.h"
#endif

#ifndef DEF_TACKLE_H
#include "tackle.h"
#endif

#ifndef DEF_ARMATTACK_H
#include "armAttack.h"
#endif

#ifndef DEF_EFFECTEXPLOSION_H
#include "effectExplosion.h"
#endif

#ifndef DEF_PICKUPJEWELY_H
#include "pickupJewely.h"
#endif

#include <memory>

/*
	@brief		�G��{�N���X
	@attention	���O�͕K��"E_"����n�߂邱��
*/
class IEnemy : public ICharacter
{
protected:
	AttackPtr attack_;							// �U����i
protected:

	IEnemy& operator = (const IEnemy& copy) 
	{
		*((ICharacter*)this) = copy;
		auto& attack = copy.attack_;
		std::string name = attack->getName();
		if (name == "Atk_NWayShot")
			attack_.reset(new CNWayShot(*std::dynamic_pointer_cast<CNWayShot>(attack)));
		else if (name == "Atk_NWaySing")
			attack_.reset(new CNWaySing(*std::dynamic_pointer_cast<CNWaySing>(attack)));
		else if (name == "Atk_Tackle")
			attack_.reset(new CTackle(*std::dynamic_pointer_cast<CTackle>(attack)));
		else if (name == "Atk_ArmAttack")
			attack_.reset(new CArmAttack(*std::dynamic_pointer_cast<CArmAttack>(attack)));
		else if (name == "Atk_FireShot")
			attack_.reset(new CFireShot(*std::dynamic_pointer_cast<CFireShot>(attack)));
		else
			debug::BToMF("attack %d proccess no defined", name.c_str());
		return *this;
	}


public:
	/*	
		@brief	�I�u�W�F�N�g����
		@param	[in]	name	���O("E_"����n�߂�)
		@attention	name��"E_"����n�߂邱��
	*/
	IEnemy(const std::string& name) :
		ICharacter(name)
		, attack_(nullptr)
	{}

	/*
		@brief	�C���^�[�t�F�[�X�����邽�߂̏������z�֐�
	*/
	virtual ~IEnemy() = 0{}
	/*
		@brief	�X�V����
		@return	�Ȃ�
	*/
	virtual void step() = 0{ __super::step(); }
	/*
		@brief	�`�揈��
		@return	�Ȃ�
	*/
	virtual void draw() = 0{ __super::draw(); }
	/*
		@brief	�_���[�W���Z
		@param	[in]	dam	�_���[�W��
		@return ���S������
		@retval	true	���S
		@retval	false	�c��
	*/
	virtual bool ApplyDamage(int dam) = 0{ return true; }

	/*
		@brief	���[���h���W�̐H�炢����̈�̎擾
		@return	���[���h���W�̓����蔻��̈�
	*/
	virtual Collisions GetDamageAreas() const { return __super::GetDamageAreas(); }

	/*
		@brief	�U���̓����蔻��̈�̎擾
		@return	�U���̓����蔻��̈�
	*/
	virtual Collisions GetAttackAreas() const override { return attack_->GetDamageAreas(); }

	/*
		@brief	�U����i���̊i�[
		@attension	ifstream��currentPosition�ɒ���
					if (FindChunk(f,"#Attack"))
						LoadAttack(f);
		@param	[in/out]	f	�I�[�v���ς݃t�@�C��
		@return	�ǂݍ��ݐ���������
		@retval	true	�ǂݍ��ݐ���
		@retval	false	�ǂݍ��ݎ��s
	*/
	bool LoadAttack(std::ifstream& f)
	{
		std::string buf;
		f >> buf;
		if (buf == "NWayShot")
		{
			f >> buf;
			if (buf != "{") return false;
			else
			{
				charabase::CharBase shot_info;
				f >> buf;	// Img���x���ǂݔ�΂�
				f >> shot_info.resname;

				f >> buf;	// Width���x���ǂݔ�΂�
				LoadValue(f, obj_, shot_info.size.x);
				f >> buf;	// Height���x���ǂݔ�΂�
				LoadValue(f, obj_, shot_info.size.y);

				CShot shot(shot_info);
				f >> buf;	// �����蔻�胉�x���ǂݔ�΂�
				shot.LoadCollisions(f);
				f >> buf;	// �X�e�[�W�����蔻�胉�x���ǂݔ�΂�
				shot.LoadStageCollisions(f);
				attack_ = AttackPtr(new CNWayShot(shot));
			}
		}
		else if (buf == "NWaySing")
		{
			f >> buf;
			if (buf != "{") return false;
			else
			{
				charabase::CharBase sing_info;
				f >> buf;	// Img���x���ǂݔ�΂�
				f >> sing_info.resname;

				f >> buf;	// Width���x���ǂݔ�΂�
				LoadValue(f, obj_, sing_info.size.x);
				f >> buf;	// Height���x���ǂݔ�΂�
				LoadValue(f, obj_, sing_info.size.y);

				CSing sing(sing_info);
				f >> buf;	// �����蔻�胉�x���ǂݔ�΂�
				sing.LoadCollisions(f);
				attack_ = AttackPtr(new CNWaySing(sing));
			}

		}
		else if (buf == "Tackle")
		{
			f >> buf;
			if (buf != "{") return false;
			else
			{
				attack_ = AttackPtr(new CTackle());
				attack_->LoadCollisions(f);
			}
		}
		else if (buf == "ArmAttack")
		{
			f >> buf;
			if (buf != "{") return false;
			else
			{
				charabase::CharBase arm_info;
				int force;
				f >> buf;	// �U���̓��x���ǂݔ�΂�
				LoadValue(f, obj_, force);
				CArmAttack arm(force);
				f >> buf;	// �����蔻�胉�x���ǂݔ�΂�
				arm.LoadCollisions(f);
				attack_ = AttackPtr(new CArmAttack(arm));
			}
		}
		else
		{
			debug::BToMF("attack load failed : %s", buf.c_str());
			return false;
		}
		
		return true;
	}

	/*
		@brief	�U����i�̎擾
		@return	�U����i
	*/
	const AttackPtr& getAttack() const{ return attack_; }

};
typedef std::shared_ptr<IEnemy> EnemyPtr;

#endif