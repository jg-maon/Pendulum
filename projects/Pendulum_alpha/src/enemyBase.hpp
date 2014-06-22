#ifndef DEF_ENEMYBASE_HPP
#define DEF_ENEMYBASE_HPP

#ifndef DEF_CHARAOBJBASE_HPP
#include "charaObjBase.hpp"
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
	
public:
	/*
		@brief	�R���X�g���N�^
		@param	[in]	name	���O("E_"����n�߂�)
		@param	[in]	attack	�U���p�N���X�|�C���^
		@attention	name��"E_"����n�߂邱��
	*/
	IEnemy(const std::string& name):
		ICharacter(name)
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
	virtual void draw() = 0{}
	/*
		@brief	�_���[�W���Z
		@param	[in]	dam	�_���[�W��
		@return ���S������
		@retval	true	���S
		@retval	false	�c��
	*/
	virtual bool ApplyDamage(int dam) = 0{ return true; }

	/*
		@brief	���[���h���W�̓����蔻��̈�̎擾
		@return	���[���h���W�̓����蔻��̈�
	*/
	virtual Collisions GetCollisionAreas() const { return __super::GetCollisionAreas(); }

	/*
		@brief	�U���̓����蔻��̈�̎擾
		@return	�U���̓����蔻��̈�
	*/
	virtual Collisions GetAttackCollisions() const {return attack_->GetCollisionAreas();}

	/*
		@brief	�U����i���̊i�[
		@attension	ifstream��currentPosition�ɒ���
					if (FindChunk(f,"#Attack"))
						LoadAttack(f);
		@param	[in/out]	f	�I�[�v���ς݃t�@�C��
		@return		EOF��
		@retval		true		EOF
		@retval		false		EOF�łȂ�
	*/
	bool LoadAttack(std::ifstream& f)
	{
		std::string buf;
		f >> buf;
		if (buf == "NWayShot")
		{
			f >> buf;
			if (buf != "{") return f.eof();
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
				attack_ = AttackPtr(new CNWayShot(shot));
			}
		}
		else if (buf == "NWaySing")
		{
			f >> buf;
			if (buf != "{") return f.eof();
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
		
		return f.eof();
	}

	/*
		@brief	�U����i�̎擾
		@return	�U����i
	*/
	const AttackPtr& getAttack() const{ return attack_; }

};
typedef std::shared_ptr<IEnemy> EnemyPtr;

#endif