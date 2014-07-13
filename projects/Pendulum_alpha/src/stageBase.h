#ifndef DEF_STAGEBASE_H
#define DEF_STAGEBASE_H

#ifndef DFE_OBJ_H
#include "obj.h"
#endif


#ifndef DEF_ACTIONPOINT_H
#include "actionPoint.h"
#endif

#ifndef DEF_MYMATH_HPP
#include "myMath.hpp"
#endif


#include <vector>
#include <string>
#include <sstream>
#include <fstream>




/*
	@brief	�X�e�[�W���N���X
*/
class IStage : public Base
{
public:
	enum class Phase	// �X�e�[�W�t�F�[�Y
	{
		CLEAR_ANNOUNCE,	// �N���A�����\��
		NORMAL,			// �G���X�e�[�W
		BOSS,			// �{�X�X�e�[�W
		RESULT,			// ���U���g��ʂ�
	};
	enum class ClearType	// �N���A����
	{
		GOAL,			// �S�[��
		ANNIHILATION,	// �r��
	};
protected:
	std::string bgm_;							// BGM

	charabase::CharPtr caObj_;					// �N���A�����\���p�I�u�W�F�N�g

	IStage::Phase phase_;						// �X�e�[�W�t�F�[�Y
	IStage::ClearType clearType_;				// �N���A����
	mymath::Recti cameraRect_;					// �J�����̉��͈�
	mymath::Recti stageRect_;					// �X�e�[�W�̑傫��
	std::vector<std::string> backgroundIMG_;	// �w�i�摜
	std::vector<ActPtPtr> actionPoints_;		// 

public:
	const std::string& bgm;

	const std::vector<ActPtPtr>& actionPoints;
	const mymath::Recti& rect;
	const mymath::Recti& cameraRect;


private:
#pragma region private methods

	/*
		@brief		�X�e�[�W�V�X�e�����̓ǂݍ���
		@attension	f�̓I�[�v���ς�
		@param	[in/out]	f	�X�e�[�W�t�@�C��
		@return	EOF��
		@retval	true	EOF
		@retval	false	EOF�łȂ�
	*/
	bool LoadEnv(std::ifstream& f);

	/*
		@brief		�X�e�[�W�T�C�Y,�J�������͈͂̓ǂݍ���
		@attension	f�̓I�[�v���ς�
		@param	[in/out]	f	�X�e�[�W�t�@�C��
		@return	EOF��
		@retval	true	EOF
		@retval	false	EOF�łȂ�
	*/
	bool LoadRect(std::ifstream& f);
	/*
		@brief		�v���C���[�̓ǂݍ���
		@attension	f�̓I�[�v���ς�
		@param	[in/out]	f	�X�e�[�W�t�@�C��
		@return	EOF��
		@retval	true	EOF
		@retval	false	EOF�łȂ�
	*/
	bool LoadPlayer(std::ifstream& f);
	/*
		@brief		�G���̓ǂݍ���
		@attension	f�̓I�[�v���ς�
		@param	[in/out]	f	�X�e�[�W�t�@�C��
		@return	EOF��
		@retval	true	EOF
		@retval	false	EOF�łȂ�
	*/
	bool LoadEnemies(std::ifstream& f);
	/*
		@brief		ActionCircle�̓ǂݍ���
		@attension	f�̓I�[�v���ς�
		@param	[in/out]	f	�X�e�[�W�t�@�C��
		@return	EOF��
		@retval	true	EOF
		@retval	false	EOF�łȂ�
	*/
	bool LoadActionCircles(std::ifstream& f);
	/*
		@brief		ActionPolygon�̓ǂݍ���
		@attension	f�̓I�[�v���ς�
		@param	[in/out]	f	�X�e�[�W�t�@�C��
		@return	EOF��
		@retval	true	EOF
		@retval	false	EOF�łȂ�
	*/
	bool LoadActionPolygons(std::ifstream& f);

#pragma endregion	// private methods

protected:
#pragma region protected methods
	

	/*
		@brief	�e��t�@�C�����[�h����
		@param	[in/out]	f	�X�e�[�W�t�@�C��
		@attension	f�̓I�[�v���ς�
		@return	�Ȃ�
	*/
	void load(std::ifstream& f);
	
	/*
		@brief	�N���A�����\���X�V����
		@return	�N���A�����\���I��
		@retval	true	�N���A�����\���I��
		@retval	false	�N���A�����\���p��
	*/
	virtual bool UpdateClearAnnounce() = 0;
	/*
		@brief	�G���X�e�[�W�X�V����
		@return	�X�e�[�W�I��
		@retval	true	�X�e�[�W�I��
		@retval	false	�X�e�[�W�p��
	*/
	virtual bool UpdateNormal() = 0;
	/*
		@brief	�{�X�X�e�[�W�X�V����
		@return	�X�e�[�W�I��
		@retval	true	�X�e�[�W�I��
		@retval	false	�X�e�[�W�p��
	*/
	virtual bool UpdateBoss() = 0;
#pragma endregion	// protected methods
public:
#pragma region public methods
	/*
		@brief	�X�e�[�W���ʍ��ڏ�����
		@param	[in]	name	���ʖ�
	*/
	IStage(const std::string& name);

	/*
		@brief	�X�e�[�W���ʍX�V����
		@return	�Ȃ�
	*/
	virtual void step() override;

	/*
		@brief	�X�e�[�W���ʕ`�揈��
		@return	�Ȃ�
	*/
	virtual void draw() override;

	/*
		@brief	�X�e�[�W�ǂݍ���(������)
		@param	[in/out]	f	�X�e�[�W�t�@�C��
		@attension	f�̓I�[�v���ς�
		@return	�Ȃ�
	*/
	virtual void init(std::ifstream& f);

	/*
		@brief	�X�e�[�W���I���������擾
		@return	�X�e�[�W�I���t���O
		@retval	true	�X�e�[�W�I��
		@retval	false	�X�e�[�W�v���C�p��
	*/
	bool isEndStage() const;

#pragma endregion	// public methods
};

#endif