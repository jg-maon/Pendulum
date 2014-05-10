#ifndef DEF_ENEMYMNG_H
#define DEF_ENEMYMNG_H

#ifndef DEF_OBJ_H
#include "Obj.h"
#endif

#include "enemyBase.hpp"

class CEnemyMng : public Base
{
private:
	std::vector<EnemyPtr> temp_;
	std::vector<EnemyPtr> enemies_;

private:
public:
	CEnemyMng();
	virtual void step() override;
	virtual void draw() override;
	/*
		@brief	�G�}�l�[�W���I�u�W�F�N�g���擾
		@return	�|�C���^
		@retval	nullptr	�G�}�l�[�W���I�u�W�F�N�g������Ă��Ȃ��ꍇ
		@retval	�G�}�l�[�W���̃|�C���^
	*/
	static ObjPtr GetPtr();
	
	/*
		@brief	�G�����t�@�C������ǂݍ���
		@attention	�t�@�C�������̃G���[�`�F�b�N���ȈՓI
		@param	[in]	fileName	�t�@�C����
		@return	�Ȃ�
	*/
	void LoadEnemiesInfo(const std::string& fileName);


	/*
		@brief	�G����
		@param	[in]	enemy	���G
		@return	�Ȃ�
	*/
	void CreateEnemy(const EnemyPtr& enemy);
	/*
		@brief	�G�擾
		@return	�G�I�u�W�F�N�g
	*/
	std::vector<EnemyPtr> GetEnemies();

};
#endif