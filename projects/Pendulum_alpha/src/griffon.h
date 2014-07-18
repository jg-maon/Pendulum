#ifndef DEF_GRIFFON_H
#define DEF_GRIFFON_H

#ifndef DEF_ENEMYBASE_HPP
#include "enemyBase.hpp"
#endif

class CGriffon : public IEnemy
{
public:
	struct LoadInfo
	{
		//float MAX_G;			// �ő�d�͑��x
		//float GRAVITY_ACC;		// �d�͉����x
		float SEARCH_RANGE;		// ���G�͈�(���ݍ��W����ǂꂾ�����G���邩)
		float CHASE_RANGE;		// �ǐՔ͈�(�������W����ǂꂾ���ǐՂ��邩)
		float ATTACK_RANGE;		// �U���͈�(���ݍ��W���炱�͈̔͂ɂ���ƍU������)
		//float RETURN_RANGE;		// �A�Ҏ��ŏ��͈�(���͈͓̔��Ȃ珉���ʒu�ɖ߂����Ƃ݂Ȃ�)
	
		float MOVE_SPEED;		// �ړ����x
		float ATTACK_SPEED;		// �U�����ړ����x

		float INV_TIME;			// ���G����

		float damageTime;			// ���ݎ���
		float tackleDist;			// �U������
		float attackInterval;		// �U���Ԋu


		int health;				// ����HP
		int power;				// �����U����


	};
private:

	enum class AnimState
	{
		FALL,				// ���~
		ROAR,				// ���K
	};

	enum class BattleState
	{
		WAIT,				// �ҋ@
		CHASE,				// �ǐ�
		//RETURN,				// �A��
		DAMAGE,				// �_���[�W(����)
		ATTACK,				// �U��
		DESTROY,			// ���S�A�j��
	};

	enum class MotionType
	{
		MOVE,				// �ҋ@�E�ړ�
		ATTACK,				// �U��
	};
private:

	static void (CGriffon::*StateStep_[])();	// ��Ԃɂ��step����

	LoadInfo loadInfo_;

	AnimState animState_;						// �A�j���[�V�����̏��
	BattleState battleState_;					// �s�����

	charabase::Anim motionAnim_;					// �A�j���[�V����
	MotionType motionType_;							// ���[�V����
	std::vector<std::vector<int> > motionTable_;	// �A�j���[�V�����e�[�u��


	float elapsedTime_;				// �o�ߎ���
	float nextActTime_;				// ���ɍs�����N��������

	float gravity_;						// �d��
	bool gravityF_;						// �d�͏��������邩(����ŗ����Ă����Ƃ��̏����p)

	bool isBack_;						// ��ޒ���
	bool isAttacking_;					// �U������

	float invincibleTime_;				// ���G����
	float invincibleAnim_;				// ���G�_�ŃA�j���[�V��������

	float sway_;							// �h��

	/*const*/ mymath::Vec3f startPos_;			// �������W(�ǐՌ㌳�ɖ߂�ꏊ)
	mymath::Vec3f endPos_;				// �U�����̈ړ��I�_���W
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
		@brief	�A��
		@return	�Ȃ�
	*/
	//void ReturnStep();
	/*
		@brief	����
		@return	�Ȃ�
	*/
	void DamageStep();
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


	/*
		@brief	���G�����擾
		@return	���G����
		@retval	true	���G��
		@retval	false	���G���łȂ�
	*/
	bool isInvincible() const;

public:
	/*
		@brief	��I�u�W�F�N�g����
		DB���������ɌĂ΂��
	*/
	CGriffon();
	/*
		@brief	���W�w�肵���I�u�W�F�N�g����
		@param	[in]	pos	���W
	*/
	CGriffon(const mymath::Vec3f& pos);
	/*
		@brief	���W�w�肵���I�u�W�F�N�g����
		@param	[in]	x	X���W
		@param	[in]	y	Y���W
		@param	[in]	z	���s��(�f�t�H���g:0.5f)
	*/
	CGriffon(float x, float y, float z = 0.5f);
	~CGriffon();
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
		@brief	�U���ҋ@���t���O�̎擾
		@return	�U���ҋ@���t���O
		@retval	true	�U���ҋ@��
		@retval	false	�U���ҋ@���łȂ�
	*/
	bool isAttacking() const;


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