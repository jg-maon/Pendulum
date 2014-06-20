#ifndef DEF_STAGEMNG_H
#define DEF_STAGEMNG_H

#ifndef DEF_OBJ_H
#include "obj.h"
#endif

#ifndef DEF_STAGEBASE_HPP
#include "stageBase.hpp"
#endif

#ifndef DEF_MYMATH_HPP
#include "MyMath.hpp"
#endif


class CStageMng : public Base
{
private:
	
	typedef std::shared_ptr<IStage> StagePtr;
	typedef std::unordered_map<std::string, StagePtr> StageMap;
	StageMap stages_;

	std::string nowStage_;		// �X�e�[�W��


public:
	CStageMng();
	virtual void step() override;
	virtual void draw() override;

	/*
		@brief	�X�e�[�W�}�l�[�W���[�I�u�W�F�N�g���擾
		@return	�|�C���^
		@retval	nullptr	�X�e�[�W�}�l�[�W���[�I�u�W�F�N�g������Ă��Ȃ��ꍇ
		@retval	�}�l�[�W���[�̃|�C���^
	*/
	static const std::shared_ptr<CStageMng> GetPtr();

	/*
		@brief	�X�e�[�W�̃��[�h
		@param	[in]	stageName	�X�e�[�W��(�t�@�C����)
		@return	�Ȃ�
	*/
	void LoadStage(const std::string& stageName);


	/*
		@brief	���݂̃X�e�[�W�T�C�Y�̎擾
		@return	�X�e�[�WRect
	*/
	const mymath::Recti& getStageRect() const;

	/*
		@brief	�w�肵���X�e�[�W�̃T�C�Y�̎擾
		@param	[in]	stage	�X�e�[�W��
		@return	�X�e�[�WRect
	*/
	const mymath::Recti& getStageRect(const std::string& stage) const;

	/*
		@brief	���݂̃X�e�[�W�̃J�������͈͂̎擾
		@return	�J�������͈�
	*/
	const mymath::Recti& getCameraRect() const;

	/*
		@brief	�w�肵���X�e�[�W�̃J�������͈͂̎擾
		@param	[in]	stage	�X�e�[�W��
		@return	�J�������͈�
	*/
	const mymath::Recti& getCameraRect(const std::string& stage) const;


	/*
		@brief	�A�N�V�����|�C���g�̎擾
		@return	�A�N�V�����|�C���g
	*/
	const std::vector<ActPtPtr>& getActionPoints() const;
};

#endif