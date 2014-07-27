#ifndef DEF_ATTACKBASE_HPP
#define DEF_ATTACKBASE_HPP

#ifndef DEF_OBJ_H
#include "Obj.h"
#endif

#ifndef DEF_MYMAYH_HPP
#include "MyMath.hpp"
#endif


/*
	@brief		�U�����N���X
	@attention	���O�͕K��"Atk_"�Ŏn�߂�
*/
class IAttack : public IColObject
{
protected:
	int force_;			// �U���́A�񕜗͂Ȃ�

protected:
public:
	IAttack(const std::string& name):
		IColObject(name),force_(0)
	{}
	virtual ~IAttack() = 0{}
	virtual void step() = 0{}
	virtual void draw() = 0{}
	
	/*
		@brief	�З͂̎擾
		@return	�U���́A�񕜗͂Ȃ�
	*/
	virtual int GetForce() const{return force_;}

};
typedef std::shared_ptr<IAttack> AttackPtr;

#endif