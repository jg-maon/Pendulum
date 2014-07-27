#ifndef DEF_SCENEDESCRIPTION_H
#define DEF_SCENEDESCRIPTION_H

#ifndef DEF_SCENEBASE_HPP
#include "sceneBase.hpp"
#endif

class CSceneDescription : public IScene
{
private:

protected:
	/*
		@brief	���C���X�V����
		@return	���C���I����
		@retval	true	���C���I��(�t�F�[�h�A�E�g�J�n)
		@retval	false	���C��������
	*/
	bool update() override;

public:
	CSceneDescription();
	~CSceneDescription();
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