#ifndef DEF_STAGE4_H
#define DEF_STAGE4_H

#ifndef DEF_STAGEBASE_HPP
#include "stageBase.hpp"
#endif

#include <fstream>

/*
	@brief	�X�e�[�W4�@
			�B���X�e�[�W(��Ձ��H�ꁨ�X)
*/
class CStage4 : public IStage
{
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