#ifndef DEF_SCENEMAIN_H
#define DEF_SCENEMAIN_H

#ifndef DEF_SCENEBASE_HPP
#include "sceneBase.hpp"
#endif


class CSceneMain : public IScene
{
protected:
	/*
		@brief	���C���X�V����
		@return	���C���I����
		@retval	true	���C���I��(�t�F�[�h�A�E�g�J�n)
		@retval	false	���C��������
	*/	
	bool update() override;


public:
	CSceneMain();
	~CSceneMain();
	/*
		@brief	�`��
		@return	�Ȃ�
	*/
	void draw() override;

	/*
		@brief	������
		@return	�Ȃ�
	*/
	void start() override;
	
	/*
		@brief	���̃V�[���ɐ؂�ւ��u�ԂɌĂ΂��
		@return	���̃V�[��
	*/
	int NextScene() const override;
};


#endif 