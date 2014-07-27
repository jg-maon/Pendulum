#ifndef DEF_SCENEEND_H
#define DEF_SCENEEND_H

#ifndef DEF_SCENEBASE_HPP
#include "sceneBase.hpp"
#endif

#ifndef DFE_SCOREMNG_H
#include "scoreMng.h"
#endif

class CSceneEnd : public IScene
{
public:

private:
	enum class Phase
	{
		CLEAR,				// �N���A�[���ĕ\��
		SCORE,				// �{�[�i�X�X�R�A���\��
		RANKING,			// �����L���O����

		//-----------------------------------------------
		// �G���h��ʏI��
		REGISTER,			// �����L���O�o�^
		TITLE,				// �����L���O�o�^�����Ƀ^�C�g����
	
	};
	CSceneEnd::Phase phase_;

	enum
	{
		TOTAL_Y = 4700,						// �g�[�^���X�R�A�摜�\��Y(unit[/10px])
		TOTALSCORE_MARGIN = 100,			// �g�[�^���X�R�A�摜�Ƃ̊Ԋu(unit[/10px])
		BONUS_X = 6000,						// �{�[�i�X����n�_X(unit[/10px])
		BONUS_Y = 2000,						// �{�[�i�X����n�_Y(unit[/10px])
		BONUS_MOVE_Y = 150,					// �{�[�i�X�ړ���Y(unit[/10px])
		BONUS_MARGIN = 300,					// �{�[�i�X�Ԋu(unit[/10px])

		RANKIN_Y = 6000,					// �����N�C���摜�\��Y(unit[/10px])
		RANKIN_MARGIN = BONUS_MARGIN + 100,	// �����N�C���摜�Ԋu(unit[/10px])
		RANKIN_MOVE_Y = 300,				// �����N�C���摜�A�j���[�V����Y�ړ���(unit[/10px])
	};

	charabase::Anim anim_;					// �A�j���[�V�����p
	charabase::CharBase clear_;				// �X�e�[�W�N���A�摜
	float animTime_;						// �A�j���[�V�����p�J�E���^
	float createInterval_;					// �����G�t�F�N�g�����Ԋu
	float dir_;								// �����G�t�F�N�g��������

	struct BonusObject
	{
		charabase::CharBase cb;				// �\�����W��
		float time;							// �\������
		CScoreMng::BonusScore bonusInfo;	// �{�[�i�X�ڍ�
		mymath::Vec3f startPos;				// �C�[�W���O�p�������W
		bool isStarted;						// �C�[�W���O�p�X�^�[�g�����t���O

	};
	std::vector<BonusObject> bonusObjs_;	// �{�[�i�X
	charabase::CharBase totalScoreObj_;		// �g�[�^���X�R�A�摜
	charabase::CharBase scoreObj_;			// �����I�u�W�F�N�g
	int dispScore_;							// �`��X�R�A
	int totalScore_;						// ���ʃX�R�A

	struct RankinObj
	{
		charabase::CharBase cb;				// �摜
		float time;							// �A�j���[�V�����p
		mymath::Vec3f startPos;				// �������W
	};
	std::vector<RankinObj> rankinObjs_;		// �����N�C���I���Ă����

#if 0
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
#endif

private:
	/*
		@brief	�{�[�i�X�I�u�W�F�N�g������
		@return	�Ȃ�
	*/
	void InitBonus();

protected:
	/*
		@brief	���C���X�V����
		@return	���C���I����
		@retval	true	���C���I��(�t�F�[�h�A�E�g�J�n)
		@retval	false	���C��������
	*/
	bool update() override;


public:
	/*
		@brief	������
		@return	�Ȃ�
	*/
	CSceneEnd();
	~CSceneEnd();
	
	/*
		@brief	�`��
		@return	�Ȃ�
	*/
	void	draw() override;

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