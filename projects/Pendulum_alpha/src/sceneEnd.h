#ifndef DEF_SCENEEND_H
#define DEF_SCENEEND_H

#ifndef DEF_SCENEBASE_HPP
#include "sceneBase.hpp"
#endif

class CSceneEnd : public IScene
{
private:
	enum class State
	{
		RESULT,					// �X�R�A�\��

		END_EXIT,				// �G���h��ʏI��
	
	};
	State state_;


	struct EndObject
	{
		charabase::CharBase cb;		// �\�����W��
		int point;					// �|�C���g�f�[�^
		char charData;				// �����f�[�^
		std::string bonusInfo;		// �{�[�i�X�ڍ�

	};

	//charabase::CharBase sceneEndBg_;
	EndObject bonusPoints[5];
	EndObject resultPoint_;
	

	int selectChar_;			// �I�𕶎��ԍ�

	float registerAnimTime_;	// �o�^�A�j���[�V�����J�E���g

private:

	static const float BONUSPOINT_X;		//�{�[�i�X�|�C���g�\��x���W
	static const float BONUSINFO_X;			//�{�[�i�X�ڍו\��x���W
	static const float BONUSPOINT_WIDTH;	//�{�[�i�X�|�C���g�\����

private:
	/*
		@brief	���O�I������
		@return	�Ȃ�
	*/
	void DecideName();

	/*
		@brief	�o�^����A�j���[�V��������
		@return	�Ȃ�
	*/
	void RegisterAnimation();

	
	/*
		@brief	���U���g�`��
		@return	�Ȃ�
	*/
	void ResultDraw();

	/*
		@brief	���O���͕`��
		@return	�Ȃ�
	*/
	void NameInputDraw();

	/*
		@brief	�n�C�X�R�A�\���`��
		@return	�Ȃ�
	*/
	void HiscoreDispDraw();


	/*
		@brief	�L�[���͊m�F
		@return	�L�[�����͂��ꂽ���ǂ���
		@retval	true	���͂��ꂽ
		@retval	false	���͂���Ă��Ȃ�
	*/
	bool InputNextKey();

	/*
		@brief	�|�C���g�f�[�^�̏�����
		@return	�Ȃ�
	*/
	void PointDataInit();

	/*
		@brief	�|�C���g�f�[�^�̕`��
		@return	�Ȃ�
	*/
	void PointDataDraw();


	/*
		@brief	�I�u�W�F�N�g������
		@return	�Ȃ�
	*/
	void init();

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
	CSceneEnd();
	~CSceneEnd();
	void	draw() override;	// �`��
};


#endif