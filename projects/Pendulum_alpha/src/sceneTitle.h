#ifndef DEF_SCENETITLE_H
#define DEF_SCENETITLE_H

#ifndef DEF_SCENEBASE_HPP
#include "sceneBase.hpp"
#endif

#ifndef DEF_MYMATH_HPP
#include "myMath.hpp"
#endif

class CSceneTitle : public IScene
{
	//======================================
	//�萔�錾
	//======================================

private:
	enum
	{
		//TITLELOGO_X = system::WINW / 2,
		CLICK_MIN_ALPHA = 5,
		CLICK_DEG_ALPHA = 50,


	};
	static const float TITLELOGO_X;			// �^�C�g��X�� �W
	static const float TITLELOGO_Y;			// �^�C�g��Y���W
	static const float LOGO_A_X;			// ��P���SX���W
	static const float LOGO_A_Y;			// ��P���SY���W
	static const float LOGO_A_SIZE;			// ��P���S�J�n�T�C�Y
	static const float LOGO_A_STARTTIME;	// ��P���S�J�n����
	static const float LOGO_B_X;			// ��Q���SX���W
	static const float LOGO_B_Y;			// ��Q���SY���W
	static const float LOGO_B_SIZE;			// ��Q���S�J�n�T�C�Y
	static const float LOGO_B_STARTTIME;	// ��Q���S�J�n����
	static const float LOGO_C_X;			// ��R���SX���W
	static const float LOGO_C_Y;			// ��R���SY���W
	static const float LOGO_C_SIZE;			// ��R���S�J�n�T�C�Y
	static const float LOGO_C_STARTTIME;	// ��R���S�J�n����
	static const float LOGO_TIME;			// ���S�ړ�����
	static const float CLICK_ON;			// �N���b�N�\������

	static const float SLASH_A_X;			// ��P�a��X���W
	static const float SLASH_A_Y;			// ��P�a��Y���W
	static const float SLASH_A_DEG;			// ��P�a���p�x
	static const float SLASH_B_X;			// ��Q�a��X���W
	static const float SLASH_B_Y;			// ��Q�a��Y���W
	static const float SLASH_B_DEG;			// ��Q�a���p�x
	static const float SLASH_NEXT_TIME;		// �a���҂�����

	//static const int CLICK_MIN_ALPHA;		// �N���b�N���ŏ��l
	//static const int CLICK_DEG_ALPHA;		// �N���b�N�����Z�n


	static const float TITLETIME;			// �f���v���C�Ɉړ�����܂ł̎���
	static const float DEMOTIME;			// �^�C�g���Ɉړ�����܂ł̎���

private:
	//======================================
	//�t���O�錾
	//======================================

	//�t�F�[�Y
	enum class Phase
	{
		TITLE,			// ���C���^�C�g�����
		DEMO,			// �f���v���C
	}phase_;

	IScene::State fadeState_;

	//�A�j���S�̗̂���
	enum class AnimPhase
	{
		LOGO,
		SLASH,
		FLASH,
		WAIT,
	}animPhase_;

	//���S
	enum class LogoPhase
	{
		RED,			// ��A�ړ�
		GREEN,			// ��B�ړ�
		BLUE,			// ��C�ړ�
	}logoPhase_;

	//�N���b�N�A�j��
	enum class ClickPhase
	{
		HIDDEN,			// ��\��
		SHOW,			// �\��
		FADEOUT,		// �t�F�[�h�A�E�g
		FADEIN			// �t�F�[�h�C��
	}clickPhase_;

	//�a��
	enum class SlashPhase
	{
		HIDDEN1,		// ��\��
		SLASH1,			// �a���P����
		HIDDEN2,		// ���Ԕ�\��
		SLASH2,			// �a���Q����

	}slashPhase_;

	//
	enum class FlashPhase
	{
		FADEOUT,		// �t�F�[�h�A�E�g
		FADEIN			// �t�F�[�h�C��

	}flashPhase_;


	//======================================
	//�ϐ��錾
	//======================================


	//charabase::CharBase titleBack_;		// �w�i
	charabase::CharBase titleLogo_;			// ���S
	charabase::CharBase titleAnim_;			// ���S�A�j��
	charabase::CharBase titleSlash_;		// �a��
	charabase::Anim slashAnim_;				// �a���A�j��


	float phaseTime_;						// �e�t�F�[�Y�\������
	float offTime_;							// �N���b�N�_��
	float nextTime_;						// �a���҂�����
	int clickAlpha_;						// �N���b�N�A�j��Alpha�l
	bool sePlaying_;						// SE�Đ�
	bool flashFlag_;						// true:���������� 

private:
	/*
		@brief	�^�C�g����ʕ\������������
		@return	�Ȃ�
	*/
	void TitleInit();
	/*
		@brief	�^�C�g����ʎ��̍X�V����
		@return	�Ȃ�
	*/
	void TitleStep();
	/*
		@brief	�f���v���C����������
		@return �Ȃ�
	*/
	void DemoInit();
	/*
		@brief	�f���v���C�\�����̍X�V����
		@return	�Ȃ�
	*/
	void DemoStep();



	/*
		@brief	�^�C�g���A�j������
		@return	�Ȃ�
	*/
	void LogoAnime();


	/*
		@brief	�v���[�Y�`�F�b�N�_��
		@return	�Ȃ�
	*/
	void PleaseClick();


	/*
		@brief	�t�F�[�Y�`�F���W
		@param	[in]	nextPhase	�؂�ւ���t�F�[�Y
		@return	�Ȃ�
	*/
	void ChangePhase();

	/*
		@brief	�a���A�j���[�V��������
		@return	�Ȃ�
	*/
	void SlashAnime();

	/*
		@brief	��ʃt���b�V������
		@return	�Ȃ�
	*/
	void TitleFlash();

	/*
		@brief	�ړ����S�g��k������
		@param	[in]	firstsize	�����T�C�Y
		@param	[in]	firstx�@		����X���W
		@return	���݂̃��S�̃T�C�Y
	*/
	float LogoSize(float firstsize, float firsty);


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
	/*
		@brief	BGM�Ǘ����A�t�F�[�h�C�����ԁA�t�F�[�h�A�E�g���Ԃ̏�����
	*/
	CSceneTitle();
	~CSceneTitle();
	void draw() override;	// �`��
};


#endif