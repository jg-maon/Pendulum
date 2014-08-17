#ifndef DEF_BIRD_H
#define DEF_BIRD_H

#ifndef DEF_ENEMYBASE_HPP
#include "enemyBase.hpp"
#endif

class CBird : public IEnemy
{
public:
	struct LoadInfo
	{
		float searchRange;		// ���G�͈�(���ݍ��W����ǂꂾ�����G���邩)
		float chaseRange;		// �ǐՔ͈�(�������W����ǂꂾ���ǐՂ��邩)
		float attackRange;		// �U���͈�(���ݍ��W���炱�͈̔͂ɂ���ƍU������)
		float returnRange;		// �A�Ҏ��ŏ��͈�(���͈͓̔��Ȃ珉���ʒu�ɖ߂����Ƃ݂Ȃ�)
		float moveSpeed;		// �ړ����x

		float attackInterval;		// �U���Ԋu
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
	};

	struct InvincibleInfo
	{
		float time;		// �_�ŃA�j���[�V�����p
		bool isOn;		// on / off
		int cnt;		// �_�ŃI���I�t�J�E���g
	};

private:
	/*static const float searchRange;		// ���G�͈�(���ݍ��W����ǂꂾ�����G���邩)
	static const float chaseRange;			// �ǐՔ͈�(�������W����ǂꂾ���ǐՂ��邩)
	static const float attackRange;		// �U���͈�(���ݍ��W���炱�͈̔͂ɂ���ƍU������)
	static const float returnRange;		// �A�Ҏ��ŏ��͈�(���͈͓̔��Ȃ珉���ʒu�ɖ߂����Ƃ݂Ȃ�)
	static const float moveSpeed;			// �ړ����x
	//*/
	static void (CBird::*StateStep_[])();	// ��Ԃɂ��step����

	LoadInfo loadInfo_;

	charabase::Anim motionAnim_;					// �A�j���[�V�����p
	MotionType motionType_;							// ���[�V����
	std::vector<std::vector<int> > motionTable_;	// �A�j���[�V�����e�[�u��

	State state_;					// �s�����

	InvincibleInfo invincible_;		// �_��

	float elapsedTime_;				// �o�ߎ���
	float nextActTime_;				// ���ɍs�����N��������
		
	/*const*/ mymath::Vec3f startPos_;			// �������W(�ǐՌ㌳�ɖ߂�ꏊ)
public:
	/*CBird& operator=(const CBird& obj)
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
	CBird();
	/*
		@brief	���W�w�肵���I�u�W�F�N�g����
		@param	[in]	pos	���W
	*/
	CBird(const mymath::Vec3f& pos);
	/*
		@brief	���W�w�肵���I�u�W�F�N�g����
		@param	[in]	x	X���W
		@param	[in]	y	Y���W
		@param	[in]	z	���s��(�f�t�H���g:0.5f)
	*/
	CBird(float x, float y, float z = 0.5f);
	~CBird();
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