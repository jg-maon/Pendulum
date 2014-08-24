#ifndef DEF_FIRESHOT_H
#define DEF_FIRESHOT_H

#ifdef _DEBUG
#define DEF_SHAPE_DRAW
#endif


#ifndef DEF_ATTACKBASE_HPP
#include "attackBase.hpp"
#endif

#ifndef DEF_MYMATH_HPP
#include "MyMath.hpp"
#endif

#include <vector>

/*
	@brief	�Ή��e
*/
class CFireShot : public IAttack
{
private:

public:
	/*
		@brief	�Ή��e
	*/
	CFireShot();

	virtual void step() override;
	virtual void draw() override;

	/*
		@brief	�Ή��e�̔���
		@param	[in]	pos			�e�̏������W
		@param	[in]	vec			�e�̔��˕���(�x�N�g��)
		@param	[in]	speed		�e�̑���
		@return	�Ȃ�
	*/
	void CreateAttack(const mymath::Vec3f& pos, const mymath::Vec3f& vec, float speed) const;
	

};

#endif