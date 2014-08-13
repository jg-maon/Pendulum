#ifndef DEF_SCENEJECLOGO_H
#define DEF_SCENEJECLOGO_H

#ifndef DEF_SCENEBASE_HPP
#include "sceneBase.hpp"
#endif

class CSceneJecLogo : public IScene
{
private:
	//float time_;
	enum
	{
		WAIT_TIME = 20,		// �V�[���J�ڑ҂�����(unit[/10sec])
	};

protected:
	/*
		@brief	���C���X�V����
		@return	���C���I����
		@retval	true	���C���I��(�t�F�[�h�A�E�g�J�n)
		@retval	false	���C��������
	*/
	bool update() override;

public:
	CSceneJecLogo();
	~CSceneJecLogo();

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