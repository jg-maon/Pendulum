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
public:
	/*
		@brief	������
	*/
	CTackle();

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
	
};

#endif