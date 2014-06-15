#ifndef DEF_FAIRY_H
#define DEF_FAIRY_H

#ifndef DEF_ENEMYBASE_HPP
#include "enemyBase.hpp"
#endif

class CFairy : public IEnemy
{
public:
	struct LoadInfo
	{
		float SEARCH_RANGE;		// ���G�͈�(���ݍ��W����ǂꂾ�����G���邩)
		float CHASE_RANGE;		// �ǐՔ͈�(�������W����ǂꂾ���ǐՂ��邩)
		float ATTACK_RANGE;		// �U���͈�(���ݍ��W���炱�͈̔͂ɂ���ƍU������)
		float RETURN_RANGE;		// �A�Ҏ��ŏ��͈�(���͈͓̔��Ȃ珉���ʒu�ɖ߂����Ƃ݂Ȃ�)
		float MOVE_SPEED;		// �ړ����x
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

private:
	/*static const float SEARCH_RANGE;		// ���G�͈�(���ݍ��W����ǂꂾ�����G���邩)
	static const float CHASE_RANGE;			// �ǐՔ͈�(�������W����ǂꂾ���ǐՂ��邩)
	static const float ATTACK_RANGE;		// �U���͈�(���ݍ��W���炱�͈̔͂ɂ���ƍU������)
	static const float RETURN_RANGE;		// �A�Ҏ��ŏ��͈�(���͈͓̔��Ȃ珉���ʒu�ɖ߂����Ƃ݂Ȃ�)
	static const float MOVE_SPEED;			// �ړ����x
	//*/
	static void (CFairy::*StateStep_[])();	// ��Ԃɂ��step����

	LoadInfo loadInfo_;

	charabase::Anim anim_;			// �A�j���[�V����

	State state_;					// �s�����

	float elapsedTime_;				// �o�ߎ���
	float nextActTime_;				// ���ɍs�����N��������

	/*const*/ mymath::Vec3f startPos_;			// �������W(�ǐՌ㌳�ɖ߂�ꏊ)
public:
	/*CFairy& operator=(const CFairy& obj)
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
	@return	�Ȃ�
	*/
	void init();

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
	CFairy();
	/*
	@brief	���W�w�肵���I�u�W�F�N�g����
	@param	[in]	pos	���W
	*/
	CFairy(const mymath::Vec3f& pos);
	/*
	@brief	���W�w�肵���I�u�W�F�N�g����
	@param	[in]	x	X���W
	@param	[in]	y	Y���W
	@param	[in]	z	���s��(�f�t�H���g:0.5f)
	*/
	CFairy(float x, float y, float z = 0.5f);
	~CFairy();
	virtual void step() override;
	virtual void draw() override;

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
	virtual Collisions GetCollisionAreas() override;


	/*
	@brief	��񏉊���
	@param	[in]	info	���[�h���Ă������ꗗ
	@return	�Ȃ�
	*/
	void SetInfo(const LoadInfo& info);
};

#endif