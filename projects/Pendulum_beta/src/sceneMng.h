#ifndef DEF_SCENEMNG_H
#define DEF_SCENEMNG_H

#ifndef DEF_OBJ_H
#include "obj.h"
#endif

#ifndef DEF_SCENEBASE_HPP
#include "sceneBase.hpp"
#endif

class CSceneMng : public Base
{
public:
	enum Scene		// �V�[���̎��
	{
		JEC,			// JEC���S
		TITLE,			// �^�C�g�����
		STAGESELECT,	// �X�e�[�W�Z���N�g
		MAIN,			// ���C���X�e�[�W
		END,			// ���U���g
		NAMEINPUT,		// �����L���O�o�^
		
		
		SCENE_NUM,
	};
private:
	std::vector<ScenePtr> scenes_;
	int nowScene_;					// ���݂̃V�[��
public:
	/*
		@brief �V�[���̃��[�h
		@return �Ȃ�
	*/
	CSceneMng();
	/*
		@brief	�V�[���̔j��
		@return �Ȃ�
	*/
	~CSceneMng();

	
	/*
		@brief	�X�V����
		@return	�Ȃ�
	*/
	virtual void step() override;
	/*
		@brief	�`�揈��
		@return	�Ȃ�
	*/
	virtual void draw() override;

};


#endif