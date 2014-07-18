#ifndef DEF_TACKLE_H
#define DEF_TACKLE_H

#ifndef DEF_ATTACKBASE_HPP
#include "attackBase.hpp"
#endif

#ifndef DEF_MYMATH_HPP
#include "MyMath.hpp"
#endif

/*
	@brief	�^�b�N��
*/
class CTackle : public IAttack
{
private:
	//int cnt_;
	mymath::Vec3f	acceleration_;	// �����x
	mymath::Vec3f	endPos_;		// �ړ��I�_���W

private:
	void init();
public:
	/*
		@brief	������
		@param	[in]	tackle			�^�b�N���̏��
		@param	[in]	acceleration	�����x
	*/
	CTackle(const mymath::Vec3f& acceleration = mymath::Vec3f(), const mymath::Vec3f& endPos = mymath::Vec3f());

	/*
		@brief	������
		@param	[in]	tackle			�^�b�N���̏��
	*/
	CTackle(const charabase::CharBase& tackle, const mymath::Vec3f& acceleration, const mymath::Vec3f& endPos);
	
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
		@brief	�^�b�N��
				�ǂ̈ʒu���� �ǂꂭ�炢�̋����� �ǂ̊p�x�� �ǂꂭ�炢�̑����� �����x�͂ǂꂭ�炢��
		@param	[in]	pos			�������W
		@param	[in]	dist		����
		@param	[in]	angle		���ˊp�x(�P��:Degree)
		@param	[in]	speed		����(�����x)
		@param	[in]	acc			�����x
		@return	�Ȃ�
	*/
	void CreateAttack(const mymath::Vec3f& pos, float dist, float angle, float speed, float acc);

};

#endif