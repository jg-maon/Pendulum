#ifndef DEF_DRAGON_H
#define DEF_DRAGON_H

#ifndef DEF_ENEMYBASE_HPP
#include "enemyBase.hpp"
#endif

class CDragon : public IEnemy
{
public:
	struct LoadInfo
	{
		float searchRange;			// ���G�͈�(���ݍ��W����ǂꂾ�����G���邩)
		float chaseRange;			// �ǐՔ͈�(�������W����ǂꂾ���ǐՂ��邩)
		float attackRange;			// �U���͈�(���ݍ��W���炱�͈̔͂ɂ���ƍU������)

		float moveSpeed;			// �ړ����x
		float attackSpeed;			// �U�����ړ����x

		float invincibleTime;		// ���G����

		float backDist;				// �o�b�N����
		float backTime;				// �o�b�N�����ɓ��B����܂ł̎���
		float damageTime;			// ���ݎ���
		float attackDist;			// �U������
		float attackInterval;		// �U���Ԋu
		float swayRange;			// �h���͈�
		float fallTime;				// ������������
		float fallSpeed;			// �����鑬�x
		int fallTurnSpeed;			// �������ɉ�]�����邽�߂̃X�s�[�h

		float entryWidth;			// init �� pos ���I���_�Ƃ��Ă������痣��� ��
		float entryHeight;			// init �� pos ���I���_�Ƃ��Ă������痣��� ����

		int health;					// ����HP
		int power;					// �����U����

		float roarAnimSpeed;		// ���K�A�j���[�V�����̃X�s�[�h
		float moveAnimSpeed;		// ��s�A�j���[�V�����̃X�s�[�h
		float backAnimSpeed;		// ��ރA�j���[�V�����̃X�s�[�h
	};
private:


	enum class BattleState
	{
		ENTRY,				// �o��
		WAIT,				// �ҋ@
		CHASE,				// �ǐ�
		DAMAGE,				// �_���[�W(����)
		ATTACK,				// �U��
		DESTROY,			// ���S�A�j��
	};

	enum class MotionType
	{
		MOVE,				// �ҋ@�E�ړ�
		ATTACK,				// �U��
		ROAR,				// ���K
		FALL,				// ����
	};
private:

	static void (CDragon::*StateStep_[])();	// ��Ԃɂ��step����

	LoadInfo loadInfo_;

	BattleState battleState_;					// �s�����

	charabase::Anim motionAnim_;					// �A�j���[�V����
	MotionType motionType_;							// ���[�V����
	std::vector<std::vector<int> > motionTable_;	// �A�j���[�V�����e�[�u��


	float elapsedTime_;					// �o�ߎ���
	float nextActTime_;					// ���ɍs�����N��������

	bool isBacking_;						// ��ޒ���
	bool isAttacking_;					// �U������

	float invincibleTime_;				// ���G����
	float invincibleAnim_;				// ���G�_�ŃA�j���[�V��������

	float sway_;						// �h��

	bool isRoaring_;						// ���K�� (true)

	int fallTurnCount_;				// �������ɉ�]�����邽�߂̃J�E���^

	mymath::Vec3f startPos_;			// �������W(�ǐՌ㌳�ɖ߂�ꏊ)

	mymath::Vec3f endPos_;				// �U�����̈ړ��I�_���W
	mymath::Vec3f backPos_;				// �U�����̃o�b�N�I�_���W
	mymath::Vec3f actPos_;				// �U���n���ʒu
public:

private:

	/*
		@brief	���̏�����
		@param	[in]	pos	�������W
		@return	�Ȃ�
	*/
	void init(const mymath::Vec3f& pos);

	/*
		@brief	�o��
		@return	�Ȃ�
	*/
	void EntryStep();
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
	CDragon();
	/*
		@brief	���W�w�肵���I�u�W�F�N�g����
		@param	[in]	pos	���W
	*/
	CDragon(const mymath::Vec3f& pos);
	/*
		@brief	���W�w�肵���I�u�W�F�N�g����
		@param	[in]	x	X���W
		@param	[in]	y	Y���W
		@param	[in]	z	���s��(�f�t�H���g:0.7f)
	*/
	CDragon(float x, float y, float z = 0.7f);
	~CDragon();
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
		@brief	�U�����t���O�̎擾
		@return	�U�����t���O
		@retval	true	�U����
		@retval	false	�U�����łȂ�
	*/
	bool isAttacking() const;

	/*
		@brief	�U���������t���O�̎擾
		@return	�U���������t���O
		@retval	true	�U��������
		@retval	false	�U���������łȂ�
	*/
	bool isBacking() const;

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