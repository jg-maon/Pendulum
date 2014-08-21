#ifndef DEF_RAYBIT_H
#define DEF_RAYBIT_H

#ifndef DEF_ENEMYBASE_HPP
#include "enemyBase.hpp"
#endif

class CRaybit : public IEnemy
{
public:
	struct LoadInfo
	{
		mymath::Rectf searchRect;		// ���G�͈�(���ݍ��W����ǂꂾ�����G���邩)
		mymath::Rectf chaseRect;		// �ǐՔ͈�(�������W����ǂꂾ���ǐՂ��邩)
		mymath::Rectf attackRect;		// �U���͈�(���ݍ��W���炱�͈̔͂ɂ���ƍU������)
		float moveSpeed;				// �ړ����x
		float moveInterval;				// �ړ��Ԋu
		float moveTime;					// �ړ�����

		float attackInterval;			// �U���Ԋu
		int attackNum;					// �U����
		float oneAttackInterval;		// 1��������̊Ԋu

		mymath::Vec3f attackPos[2];		// �U�����ˉӏ�
	};
private:
	enum class State
	{
		WAIT,				// �ҋ@
		CHASE,				// �ǐ�
		LOST,				// ������
		ATTACK,				// �U��
		DESTROY,			// ���S�A�j��
	};

	enum class MoveState
	{
		STOP,				// ��~
		MOVE,				// �ړ�
	};

	enum class MotionType
	{
		MOVE,				// �ҋ@�E�ړ�
	//	ATTACK,				// �U��
	};


private:

	static void (CRaybit::*StateStep_[])();	// ��Ԃɂ��step����

	LoadInfo loadInfo_;

	charabase::Anim motionAnim_;					// �A�j���[�V�����p
	MotionType motionType_;							// ���[�V����
	std::vector<std::vector<int> > motionTable_;	// �A�j���[�V�����e�[�u��

	charabase::CharBase eye_;				// �ډ摜

	State state_;							// �s�����

	MoveState moveState_;					// �ړ����

	float elapsedTime_;						// �o�ߎ���
	float nextActTime_;						// ���ɍs�����N��������
		
	mymath::Vec3f startPos_;				// �������W(�ǐՔ͈͗p)

	int attackNum_;							// �U����(�A���U��)
	bool attackPreliminary_;				// �U���\���s�����s���� true:�s��

	charabase::CharBase targetCursor_;		// �U���Ə��A�j���[�V�����p

public:
	
private:

	/*
		@brief	���̏�����
		@param	[in]	pos	�������W
		@return	�Ȃ�
	*/
	void init(const mymath::Vec3f& pos);

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
		@brief	�v���C���[���������Ă��鎞�̓���
		@return	�Ȃ�
	*/
	void LostStep();
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
		@brief	�c�����o���Ȃ���ړ�
		@return	�Ȃ�
	*/
	void Move();

	/*
		@brief	�s���^�C�v�̌���
		@return	�Ȃ�
	*/
	void DecideState();

	/*
		@brief	�U���̔�������
		@return	�A���U������
		@retval	true	�U����
		@retval	false	�U���I��
	*/
	bool CreateAttack();
	
public:
	/*
		@brief	��I�u�W�F�N�g����
				DB���������ɌĂ΂��
	*/
	CRaybit();
	/*
		@brief	���W�w�肵���I�u�W�F�N�g����
		@param	[in]	pos	���W
	*/
	CRaybit(const mymath::Vec3f& pos);
	/*
		@brief	���W�w�肵���I�u�W�F�N�g����
		@param	[in]	x	X���W
		@param	[in]	y	Y���W
		@param	[in]	z	���s��(�f�t�H���g:0.5f)
	*/
	CRaybit(float x, float y, float z = 0.5f);
	~CRaybit();
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
};

#endif