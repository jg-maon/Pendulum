#ifndef DEF_STAGE1_H
#define DEF_STAGE1_H

#ifndef DEF_STAGEBASE_HPP
#include "stageBase.hpp"
#endif

/*
	@brief	ステージ1　森
*/
class Stage1 : public IStage
{
public:
	Stage1();
	virtual void step() override;
	virtual void draw() override;
};


#endif