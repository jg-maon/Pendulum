#ifndef DEF_COLLISION_H
#define DEF_COLLISION_H

#ifndef DEF_OBJ_H
#include "Obj.h"
#endif


/*
	衝突検知クラス
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