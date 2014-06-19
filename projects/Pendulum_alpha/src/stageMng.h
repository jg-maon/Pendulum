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
	std::vector<StagePtr> stages_;

	std::string stageName_;		// �X�e�[�W��



public:
	CStageMng();
	virtual void step() override;
	virtual void draw() override;

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
		@param	[in]	stage	�X�e�[�W�ԍ�
		@return	�X�e�[�WRect
	*/
	const mymath::Recti& getStageRect(int stage) const;

	static const std::shared_ptr<CStageMng> GetPtr();
};

#endif