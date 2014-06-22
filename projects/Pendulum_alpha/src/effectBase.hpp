#ifndef DEF_EFFECTBASE_HPP
#define DEF_EFFECTBASE_HPP

#ifndef DEF_OBJ_H
#include "Obj.h"
#endif

/*
	@brief		�G�t�F�N�g��{�N���X
*/
class IEffect : public IObject
{
public:
	IEffect(const std::string& name) :
		IObject(name)
	{
		status_ = Status::run;
	}
	virtual void step() = 0{}
	virtual void draw() = 0{}
};

#endif