#ifndef DEF_STAGE1_H
#define DEF_STAGE1_H

#ifndef DEF_STAGEBASE_HPP
#include "stageBase.hpp"
#endif

#include <fstream>

/*
	@brief	�X�e�[�W1�@�X
			�X�e�[�W�N���A�����̓S�[���`��
*/
class CStage1 : public IStage
{
private:
	mymath::ShapefPtr goalArea_;		// �S�[���G���A
	charabase::CharBase goalObj_;		// �S�[���I�u�W�F�N�g
private:
	/*
		@brief		�N���A�����̃��[�h
		@attension	f�̓I�[�v���ς�
		@param		[in/out]	f		�X�e�[�W�t�@�C��
		@param		[out]		area	�S�[���G���A�i�[
		@return		�Ȃ�
	*/
	void LoadClear(std::ifstream& f, mymath::ShapefPtr& area);
public:
	/*
		@brief		�X�e�[�W�̃��[�h
		@attension	f�̓I�[�v���ς�
		@param		[in/out]	f	�X�e�[�W�t�@�C��
	*/
	CStage1(std::ifstream& f);
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