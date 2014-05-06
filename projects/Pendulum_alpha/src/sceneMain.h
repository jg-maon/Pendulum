#ifndef DEF_SCENEMAIN_H
#define DEF_SCENEMAIN_H

#ifndef DEF_SCENEBASE_H
#include "sceneBase.h"
#endif


class CSceneMain : public IScene
{
public:
	CSceneMain();
	~CSceneMain();
	IScene* step() override;	// çXêV
	void	draw() override;	// ï`âÊ
};


#endif 