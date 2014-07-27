#ifndef DEF_COLLISION_H
#define DEF_COLLISION_H

#ifndef DEF_OBJ_H
#include "Obj.h"
#endif


/*
	Õ“ËŒŸ’mƒNƒ‰ƒX
*/
class CCollision : public IObject
{
private:
public:
	CCollision();
	virtual void step() override;
	virtual void draw() override;

};
#endif