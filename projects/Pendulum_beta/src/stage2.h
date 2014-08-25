#ifndef DEF_STAGE2_H
#define DEF_STAGE2_H

#ifndef DEF_STAGEBASE_H
#include "stageBase.h"
#endif

#ifndef DEF_STAGEMNG_H
#include "stageMng.h"
#endif

#include <fstream>

/*
	@brief	�X�e�[�W2�@�H��
			�X�e�[�W�N���A�����͟r��
*/
class CStage2 : public IStage
{
private:
	std::weak_ptr<CStageMng> sm_;		// �X�e�[�W�}�l�[�W���ւ̎Q��

	enum class ClearAnnouncePhase
	{
		WAIT,				// �E�F�C�g
		LEFTTOP,			// ����
		RIGHTTOP,			// ������
		RIGHTBOTTOM,		// ����
		LEFTBOTTOM,			// ����
		TO_PLAYER,			// �v���C���[��
		MSGIN,				// �����\��
		MSGOUT,				// ������\��

	}caPhase_;
	mymath::Vec3f caStartPos_;			// �N���A�����\���I�u�W�F�N�g�������W

	int clearNorma_;					// �N���A�m���}�G��

	mymath::ShapefPtr goalArea_;		// �S�[���G���A
	charabase::CharPtr goalObj_;		// �S�[���I�u�W�F�N�g

	std::string bossFile_;

	float announceTime_;				// �N���A�����\�����^�C�}�[

private:
	/*
		@brief		�N���A�����̃��[�h
		@attension	f�̓I�[�v���ς�
		@param		[in/out]	f		�X�e�[�W�t�@�C��
		@param		[out]		area	�S�[���G���A�i�[
		@return		�Ȃ�
	*/
	void LoadClear(std::ifstream& f, mymath::ShapefPtr& area);

protected:
	
	/*
		@brief	�N���A�����\���X�V����
		@return	�N���A�����\���I��
		@retval	true	�N���A�����\���I��
		@retval	false	�N���A�����\���p��
	*/
	virtual bool UpdateClearAnnounce() override;

	/*
		@brief	�G���X�e�[�W�X�V����
		@return	�X�e�[�W�I��
		@retval	true	�X�e�[�W�I��
		@retval	false	�X�e�[�W�p��
	*/
	virtual bool UpdateNormal() override;

	/*
		@brief	�{�X�X�e�[�W�X�V����
		@return	�X�e�[�W�I��
		@retval	true	�X�e�[�W�I��
		@retval	false	�X�e�[�W�p��
	*/
	virtual bool UpdateBoss() override;
public:
	/*
		@brief	�X�e�[�W�̃��[�h
		@param	[in/out]	f	�X�e�[�W�t�@�C��
		@attension	f�̓I�[�v���ς�
	*/
	CStage2(std::ifstream& f);
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

	/*
		@brief	�X�e�[�W�ǂݍ���(������)
		@param	[in/out]	f	�X�e�[�W�t�@�C��
		@attension	f�̓I�[�v���ς�
		@return	�Ȃ�
	*/
	virtual void init(std::ifstream& f) override;

};


#endif