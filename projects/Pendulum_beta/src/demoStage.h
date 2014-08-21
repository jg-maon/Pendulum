#ifndef DEF_DEMOSTAGE_H
#define DEF_DEMOSTAGE_H

#ifndef DEF_STAGEBASE_H
#include "stageBase.h"
#endif

#ifndef DEF_STAGEMNG_H
#include "stageMng.h"
#endif

#include <memory>
#include <fstream>

/*
	@brief	�f���X�e�[�W
*/
class CDemoStage : public IStage
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

	mymath::ShapefPtr goalArea_;		// �S�[���G���A
	charabase::CharPtr goalObj_;		// �S�[���I�u�W�F�N�g

	std::string bossFile_;

	float announceTime_;				// �N���A�����\�����^�C�}�[
	//mymath::Vec3f playerPos_;			// �v���C���[�X�^�[�g���W
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
		@brief		�v���C���[�̓ǂݍ���
		@attension	f�̓I�[�v���ς�
		@param	[in/out]	f	�X�e�[�W�t�@�C��
		@return	EOF��
		@retval	true	EOF
		@retval	false	EOF�łȂ�
	*/
	virtual bool LoadPlayer(std::ifstream& f) override;



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
		@brief		�X�e�[�W�̃��[�h
		@attension	f�̓I�[�v���ς�
		@param		[in/out]	f	�X�e�[�W�t�@�C��
	*/
	CDemoStage(std::ifstream& f);
	/*
		@brief	ActionPoint�A���d�X�N���[���X�V����
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