#ifndef DEF_SCENESTAGESELECT_H
#define DEF_SCENESTAGESELECT_H

#ifndef DEF_SCENEMNG_H
#include "sceneMng.h"
#endif

#ifndef DEF_SCENEBASE_HPP
#include "sceneBase.hpp"
#endif

class CSceneStageSelect : public IScene
{
private:
	mymath::Vec3f startCameraPos_;			// �J���������ʒu

	mymath::Recti fieldRect_;		// �X�e�[�W�I���t�B�[���h�T�C�Y
	mymath::Recti bgRect_;			// �w�i�T�C�Y
	charabase::CharPtr	back_;		// �w�i
	
	// �X�e�[�W�摜�A�����摜
	std::vector<std::pair<charabase::CharPtr, charabase::CharPtr>> stages_;

	float descHeight_;				// �������A�j���[�V����%(0.0f~1.0f)

	std::string stageName_;			// �v���C����X�e�[�W��

	CSceneMng::Scene nextScene_;

	
private:
	/*
		@brief	���ǂݍ���
		@param	[in/out]	f	�t�@�C��
		@return	�Ȃ�
	*/
	void LoadInfo(std::ifstream& f);
	/*
		@brief	�X�e�[�W�ǂݍ���
		@param	[in/out]	f	�t�@�C��
		@return	�Ȃ�
	*/
	void LoadStages(std::ifstream& f);
protected:
	/*
		@brief	���C���X�V����
		@return	���C���I����
		@retval	true	���C���I��(�t�F�[�h�A�E�g�J�n)
		@retval	false	���C��������
	*/
	bool update() override;

	
public:
	CSceneStageSelect();
	~CSceneStageSelect();
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