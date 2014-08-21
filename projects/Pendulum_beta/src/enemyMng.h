#ifndef DEF_ENEMYMNG_H
#define DEF_ENEMYMNG_H

#ifndef DEF_OBJ_H
#include "Obj.h"
#endif

#ifndef DEF_ENEMYBASE_HPP
#include "enemyBase.hpp"
#endif

//==================================
// �G
#include "bird.h"
#include "fairy.h"
#include "griffon.h"
#include "raybit.h"
#include "roboticArm.h"
//==================================

class CEnemyMng : public Base
{
private:
	std::vector<EnemyPtr> temp_;			// �G�e�[�u���ǉ��p
	std::vector<EnemyPtr> enemies_;			// �G�e�[�u��

	size_t enemyNum_;						// �����G��


public:
	CEnemyMng();
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
		@brief	�I�u�W�F�N�g������
		@return	�Ȃ�
	*/
	virtual void kill() override;
	/*
		@brief	�I�u�W�F�N�g���n��������
		@return	�Ȃ�
	*/
	virtual void start() override;
	/*
		@brief	�I�u�W�F�N�g���~������
		@return	�Ȃ�
	*/
	virtual void stop() override;

	/*
		@brief	�I�u�W�F�N�g��`��݂̂ɐݒ肷��
		@return	�Ȃ�
	*/
	virtual void SetStatusDisp() override;


	/*	
		@brief	�{�[�i�X�X�R�A�p�Skill������
		@return	�S�ł�������
		@retval	true	�S�ł�����
		@retval	false	�S�ł��Ă��Ȃ�
	*/
	bool isDestroyerBonus() const;


	/*
		@brief	�{�[�i�X�X�R�A�p�m�[Kill��
		@return	�m�[Kill
		@retval	true	�m�[kill
		@retval	false	1�̂ł�kill����
	*/
	bool isNoKillBonus() const;

	/*
		@brief	�G�}�l�[�W���I�u�W�F�N�g���擾
		@return	�|�C���^
		@retval	nullptr	�G�}�l�[�W���I�u�W�F�N�g������Ă��Ȃ��ꍇ
		@retval	�G�}�l�[�W���̃|�C���^
	*/
	static ObjPtr GetPtr();
	
	/*
		@brief	�G�z�u�e�[�u�����t�@�C������ǂݍ���
		@attention	�t�@�C�������̃G���[�`�F�b�N���ȈՓI
		@param	[in]	fileName	�e�[�u���t�@�C���p�X
		@return	�Ȃ�
	*/
	void LoadEnemyTable(const std::string& fileName);


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
	std::vector<EnemyPtr>& getEnemies();

	/*
		@brief	�G�S����
		@return	�Ȃ�
	*/
	void ClearEnemies();

};
#endif