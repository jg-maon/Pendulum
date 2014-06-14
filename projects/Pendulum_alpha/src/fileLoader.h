#ifndef DEF_FILELOADER_H
#define DEF_FILELOADER_H

#ifndef DEF_ENEMYBASE_HPP
#include "enemyBase.hpp"
#endif

#ifndef DEF_PLAYER_H
class CPlayer;
#endif

#include <vector>

/*
	@brief	�t�@�C���ǂݍ��݃N���X
			CFileMng���Ăяo�����
*/
class CFileLoader
{
public:
	typedef std::unordered_map<std::string, int> FontTable;
	// ���\�[�X�t�@�C���p
	struct ResData
	{
		std::string resname;
		std::string path;
	};
private:
	const std::string iniFile_;		// �e��ݒ�L�q�t�@�C���p�X

private:
	//---------------------------------
#pragma region ���\�[�X�t�@�C���ǂݍ��݊֘A
	/*
		@brief	���\�[�X�t�@�C���ǂݍ��ݏ���
		@param	[in/out]	resFile		���\�[�X�t�@�C���ꗗ�t�@�C���p�X
		@param	[out]		fontTablle	�t�H���g�Ǘ��e�[�u��
		@return	�Ȃ�
	*/
	void LoadRes(const std::string& resFile, FontTable& fontTable);
	/*
		@brief	�摜�t�@�C���ǂݍ���
		@param	[in/out]	resF	���\�[�X�t�@�C���ꗗ�t�@�C��
		@return	�Ȃ�
	*/
	void LoadImg(std::ifstream& resF);
	/*
		@brief	BGM�ǂݍ���
		@param	[in/out]	resF	���\�[�X�t�@�C���ꗗ�t�@�C��
		@return	�Ȃ�
	*/
	void LoadBGM(std::ifstream& resF);
	/*
		@brief	SE�ǂݍ���
		@param	[in/out]	resF	���\�[�X�t�@�C���ꗗ�t�@�C��
		@return	�Ȃ�
	*/
	void LoadSE(std::ifstream& resF);
	/*
		@brief	�t�H���g�ǂݍ���
		@param	[in/out]	resF		���\�[�X�t�@�C���ꗗ�t�@�C��
		@param	[out]		fontTable	�t�H���g�Ǘ��e�[�u��
		@return	�Ȃ�
	*/
	void LoadFont(std::ifstream& resF, FontTable& fontTable);
#pragma endregion	// ���\�[�X�t�@�C���ǂݍ��݊֘A
	//---------------------------------

	//---------------------------------
#pragma region �G�e�[�u���ǂݍ���
	/*
		@brief	CBird���[�h
		@param	[in]	fileName	�G�t�@�C���p�X
		@param	[out]	enemies		���[�h�����G�����i�[����z��
		@return	����ݒ�ł�����
		@retval	true	�ݒ萬��
		@retval	false	�ݒ莸�s
	*/
	bool LoadBird(const std::string& fileName, std::vector<EnemyPtr>& enemies);

	/*
		@brief	CFairy���[�h
		@param	[in]	fileName	�G�t�@�C���p�X
		@param	[out]	enemies		���[�h�����G�����i�[����z��
		@return	����ݒ�ł�����
		@retval	true	�ݒ萬��
		@retval	false	�ݒ莸�s
	*/
	bool LoadFairy(const std::string& fileName, std::vector<EnemyPtr>& enemies);

#pragma endregion	// �G�e�[�u���ǂݍ���
	//---------------------------------


public:

	/*
		@brief	�e�t�@�C���ǂݍ���
		@param	[in]	iniFile	�ݒ�t�@�C���p�X
	*/
	CFileLoader(const std::string& iniFile);

	/*
		@brief	�e�t�@�C���ǂݍ���
		@param	[out]	fontTable	�t�H���g�Ǘ��e�[�u��
		return	�Ȃ�
	*/
	void Load(FontTable& fontTable);

	/*
		@brief	�v���C���[�ǂݍ���
		@param	[out]	player		�v���C���[
		@return	����ݒ�ł�����
		@retval	true	�ݒ萬��
		@retval	false	�ݒ莸�s
	*/
	bool LoadPlayerData(CPlayer& player);

	/*
		@brief	�G�����t�@�C�����烍�[�h����
		@param	[out]	enemies		���[�h�����G�����i�[����z��
		@return	�Ȃ�
	*/
	void LoadEnemiesData(std::vector<EnemyPtr>& enemies);


	/*
		@brief	�t�@�C���p�X�擾
			GetFile("#File");
		@param	[in]	tag	�擾�t�@�C�����ʖ�(#�^�O)
		@return �t�@�C���p�X
	*/
	std::string GetFile(const std::string& tag) const;
};


#endif