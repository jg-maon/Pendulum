#ifndef DEF_SCENEMAIN_H
#define DEF_SCENEMAIN_H

#ifndef DEF_SCENEBASE_HPP
#include "sceneBase.hpp"
#endif


class CSceneMain : public IScene
{
public:
	CSceneMain();
	~CSceneMain();
	IScene* step() override;	// �X�V
	void	draw() override;	// �`��
};


#endif 