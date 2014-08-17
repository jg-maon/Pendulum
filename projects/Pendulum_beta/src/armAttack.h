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
	//charabase::CharBase arm_;	// �A�[�����
	float angle_;				// �A�[���̊p�x
	float rotateSpeed_;			// �A�[���̉�]���x
	float maxAngle_;			// �A�[���̉�]�̍ő�p�x
	mymath::Vec3f center_;		// �A�[���̉�]���S���W
	mymath::Vec3f length_;		// ��]�̒��S����I�u�W�F�N�g�܂ł̒���


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
		@brief	������
		@param	[in]	arm				�A�[���̏��
		@param	[in]	nowAngle		���݂̊p�x
		@param	[in]	length			��]���S���W����A�[���̒��S�܂ł̒���
		@param	[in]	center			��]���S���W
		@param	[in]	rotateSpeed		��]���x
		@param	[in]	maxAngle		��]�ł���ő�l
	*/
	CArmAttack(const CArmAttack& arm, const float nowAngle,
		const mymath::Vec3f& length, const mymath::Vec3f& center, float rotateSpeed, float maxAngle);

	virtual void step() override;
	virtual void draw() override;
	virtual void hit(const ObjPtr& rival) override;
	
	/*
		@
	*/
	void setForce(int force);
};



#endif