#ifndef DEF_SCENEDESCRIPTION_H
#define DEF_SCENEDESCRIPTION_H

#ifndef DEF_SCENEBASE_HPP
#include "sceneBase.hpp"
#endif

class CSceneDescription : public IScene
{
	
protected:
	/*
		@brief	���C���X�V����
		@return	���C���I����
		@retval	true	���C���I��(�t�F�[�h�C���J�n)
		@retval	false	���C��������
	*/
	bool update() override;

	/*
		@brief	���̃V�[���ɐ؂�ւ��u�ԂɌĂ΂��
		@return	���̃V�[��
	*/
	IScene* NextScene() override;

public:
	CSceneDescription();
	~CSceneDescription();
	void	draw() override;	// �`��
};

#endif