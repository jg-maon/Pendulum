#ifndef DEF_RoboticArm_H
#define DEF_RoboticArm_H

#ifndef DEF_ENEMYBASE_HPP
#include "enemyBase.hpp"
#endif

#ifndef DEF_ARMATTACK_H
#include "armAttack.h"
#endif

class CRoboticArm : public IEnemy
{
public:
	struct LoadInfo
	{
		float attackRange;				// �U���͈�(���ݍ��W���炱�͈̔͂ɂ���ƍU������)

		std::string parentResname;		//�Œ�A�[���̉摜�Ǘ���

		mymath::Vec3f supportParent;	// �Œ�A�[���̎x�_(�e��)(��]���S���W)
		mymath::Vec3f supportChild;		// ���A�[���̎x�_(�q��)(��]���S���W)

		mymath::Vec3i parentSrcPos;		// ���A�[���̉摜���o�ʒu
		mymath::Vec3i parentSrcSize;	// ���A�[���̉摜���o�T�C�Y

		float rotateSpeed;				// �A�[���̉�]���x
		float maxAngle;					// �A�[���̉�]�̍ő�p�x
		float minAngle;					// �A�[���̉�]�̍ŏ��p�x

		float attackInterval;		// �U���Ԋu
	};
	
	enum class ArmDirectin
	{
		RIGHT_UP,		// �E����
		RIGHT_DOWN,		// �E�����A�㉺���]
		LEFT_UP,		// ������
		LEFT_DOWN,		// �������A�㉺���]
	};

private:
	enum class State
	{
		WAIT,				// �ҋ@
		CHASE,				// �ǐ�
		RETURN,				// �A��
		ATTACK,				// �U��
		DESTROY,			// ���S�A�j��

	};

	enum class MotionType
	{
		MOVE,				// �ҋ@�E�ړ�
		ATTACK,				// �U��
		DESTROY,			// ���S
	};

	struct InvincibleInfo
	{
		float time;		// �_�ŃA�j���[�V�����p
		bool isOn;		// on / off
		int cnt;		// �_�ŃI���I�t�J�E���g
	};


private:
	static void (CRoboticArm::*StateStep_[])();	// ��Ԃɂ��step����

	LoadInfo loadInfo_;

	charabase::Anim motionAnim_;					// �A�j���[�V�����p
	MotionType motionType_;							// ���[�V����
	std::vector<std::vector<int> > motionTable_;	// �A�j���[�V�����e�[�u��

	State state_;					// �s�����

	InvincibleInfo invincible_;		// �_��

	float elapsedTime_;				// �o�ߎ���
	float nextActTime_;				// ���ɍs�����N��������

	charabase::CharBase parentObj_;	// �Œ�A�[��
	bool  turnParentFlag_;			// �ғ��A�[���̔��]
	float childAngle_;				// �ғ��A�[���̊p�x
	float rotateDir_;				// ��]�������
	ArmDirectin armDir_;			// �A�[���̌���

	charabase::CharBase armAtkObj_;	// �A�[���U���̏��

	mymath::Vec3f startPos_;			// �������W(�ǐՌ㌳�ɖ߂�ꏊ)


public:
	/*CRoboticArm& operator=(const CRoboticArm& obj)
	{
	if (this != &obj) return *this;
	loadInfo_ = obj.loadInfo_;
	state_ = obj.state_;
	elapsedTime_ = obj.elapsedTime_;
	nextActTime_ = obj.nextActTime_;
	}
	//*/
private:

	/*
	@brief	���̏�����
	@param	[in]	pos		�������W
	@param	[in]	dir		�A�[���̌���
	@return	�Ȃ�
	*/
	void init(const mymath::Vec3f& pos);

	/*
	@brief	�A�[���̌����̐؂�ւ�
	@return	�Ȃ�
	*/
	void SwichArmDir();
	/*
	@brief	�ҋ@
	@return	�Ȃ�
	*/
	void WaitStep();
	/*
	@brief	�ǐ�
	@return	�Ȃ�
	*/
	void ChaseStep();
	/*
	@brief	�A��
	@return	�Ȃ�
	*/
	void ReturnStep();
	/*
	@brief	�U��
	@return	�Ȃ�
	*/
	void AttackStep();
	/*
	@brief	���S
	@return	�Ȃ�
	*/
	void DestroyStep();




	/*
	@brief	�s���^�C�v�̌���
	@return	�Ȃ�
	*/
	void DecideState();

	/*
	@brief	�U���̔�������
	@return	�Ȃ�
	*/
	void CreateAttack();

public:
	/*
	@brief	��I�u�W�F�N�g����
	DB���������ɌĂ΂��
	*/
	CRoboticArm();
	/*
	@brief	���W�w�肵���I�u�W�F�N�g����
	@param	[in]	pos	���W
	*/
	CRoboticArm(const mymath::Vec3f& pos);
	/*
	@brief	���W�w�肵���I�u�W�F�N�g����
	@param	[in]	x	X���W
	@param	[in]	y	Y���W
	@param	[in]	z	���s��(�f�t�H���g:0.5f)
	*/
	CRoboticArm(float x, float y, float z = 0.5f);
	~CRoboticArm();
	/*
	@brief	�X�V����
	@return	�Ȃ�
	*/
	virtual void step() override;

	/*
	@brief	�`�揈��
	@return	�Ȃ�
	*/
	virtual void draw() override;

	/*
	@brief	�I�u�W�F�N�g���m�̏d�Ȃ菈��
	@param	[in]	rival	�d�Ȃ��Ă��鑊��I�u�W�F�N�g
	@return	�Ȃ�
	*/
	virtual void hit(const ObjPtr& rival) override;

	/*
	@brief	�_���[�W���Z
	@param	[in]	dam	�_���[�W��
	@return ���S������
	@retval	true	���S
	@retval	false	�c��
	*/
	virtual bool ApplyDamage(int dam) override;

	/*
	@brief	�����蔻��̈�̎擾
	@return	�����蔻��̈�
	*/
	virtual Collisions GetDamageAreas() const override;


	/*
	@brief	��񏉊���
	@param	[in]	info	���[�h���Ă������ꗗ
	@return	�Ȃ�
	*/
	void SetInfo(const LoadInfo& info);

	/*
	@brief	�A�[���U�����̕ێ�
	@param	[in]	atk		�A�[���U���̏��
	@param	[in]	force	�A�[���̍U����
	@return	�Ȃ�
	*/
	void SetArmAtkInfo(const charabase::CharBase atk, const int force);

	/*
	@brief	�A�[���̌����̕ύX
	@param	[in]	dir		�A�[���̌���
	@return	�Ȃ�
	*/
	void SetArmDirection(ArmDirectin dir);
};

#endif