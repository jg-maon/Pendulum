#ifndef DEF_SCENETITLE_H
#define DEF_SCENETITLE_H

#ifndef DEF_SCENEBASE_HPP
#include "sceneBase.hpp"
#endif

class CSceneTitle : public IScene
{
public:
	CSceneTitle();
	~CSceneTitle();
	IScene* step() override;	// çXêV
	void	draw() override;	// ï`âÊ
};


#endif