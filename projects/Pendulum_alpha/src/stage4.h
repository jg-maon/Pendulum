#ifndef DEF_STAGE4_H
#define DEF_STAGE4_H

#ifndef DEF_STAGEBASE_H
#include "stageBase.h"
#endif

#include <fstream>

/*
	@brief	�X�e�[�W4�@
			�B���X�e�[�W(��Ձ��H�ꁨ�X)
*/
class CStage4 : public IStage
{
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
	CStage4(std::ifstream& f);
	virtual void step() override;
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