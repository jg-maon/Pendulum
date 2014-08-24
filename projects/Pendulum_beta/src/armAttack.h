#ifndef DEF_ARMATTACK_H
#define DEF_ARMATTACK_H

#ifndef DEF_ATTACKBASE_HPP
#include "attackBase.hpp"
#endif

#ifndef DEF_MYMATH_HPP
#include "MyMath.hpp"
#endif

/*
	@brief	�A�[���U��
*/
class CArmAttack : public IAttack
{
private:
	enum class MotionType
	{
		WAIT,			// �ҋ@
		ATTACK,			// �U��
	};

private:
	float angle_;				// �A�[���̊p�x
	mymath::Vec3f center_;		// �A�[���̉�]���S���W
	float length_;				// ��]�̒��S����I�u�W�F�N�g�܂ł̒���
	MotionType motionType_;		// �A�j���[�V�����̏��


private:
	void init();

public:
	/*
		@brief	������
		@param	[in]	cb		�A�[���̃I�u�W�F�N�g
	*/
	CArmAttack(const charabase::CharBase& cb);

	/*
	@brief	������
	@param	[in]	force		�A�[���̍U����
	*/
	CArmAttack(const int force);

	/*
		@btief	���A�[���̊p�x�Ȃǂ̐ݒ�
		@param	[in]	angle			�A�[���̊p�x
		@param	[in]	length			��]���S���W����A�[���̒��S�܂ł̒���
		@param	[in]	center			��]���S���W
	*/
	void SetArmInfo(const float angle, const float length, const mymath::Vec3f& center);

	/*
		@brief	����
		@param	[in]	angle	���݂̊p�x
	*/
	void step(float angle);

	virtual void step() override;
	virtual void draw() override;
	virtual void hit(const ObjPtr& rival) override;
};



#endif