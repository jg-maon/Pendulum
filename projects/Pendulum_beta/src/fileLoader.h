#ifndef DEF_FILELOADER_H
#define DEF_FILELOADER_H

#ifndef DEF_ENEMYBASE_HPP
#include "enemyBase.hpp"
#endif

#ifndef DEF_PLAYER_H
class CPlayer;
#endif

#include <unordered_map>
#include <string>
#include <vector>

/*
	@brief	�t�@�C���ǂݍ��݃N���X
			CFileMng���Ăяo�����
*/
class CFileLoader
{
	/*
		@brief		LoadInfo�ǂݍ���
		@attension	f�̓I�[�v���ς�
		@param	[in/out]	f			�t�@�C��
		@param	[in/out]	loadValues	�ǂݍ��ޑΏ�
		@return	�ǂݍ��ݐ���������
		@retval	true	�ǂݍ��ݐ���
		@retval	false	�ǂݍ��ݎ��s
	*/
#define LoadInfo(f, loadValues)		LoadInfoFunc(__FILE__##" "##__FUNCTION__, f, loadValues);
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

	//------------------------------------------
	// LoadInfo�p
	struct LoadValue
	{
		char* tag;				// #�^�O
		void* value;			// �i�[����l
		std::string type;		// �^���
	};

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
	/*
		@brief		LoadInfo�ǂݍ���
		@attension	f�̓I�[�v���ς�
		@param	[in]		from		�Ăяo����
		@param	[in/out]	f			�t�@�C��
		@param	[in/out]	loadValues	�ǂݍ��ޑΏ�
		@return	�ǂݍ��ݐ���������
		@retval	true	�ǂݍ��ݐ���
		@retval	false	�ǂݍ��ݎ��s
	*/
	bool LoadInfoFunc(const std::string& from, std::ifstream& f, std::vector<LoadValue>& loadValues);
	

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
	
	/*
		@brief	CGriffon���[�h
		@param	[in]	fileName	�G�t�@�C���p�X
		@param	[out]	enemies		���[�h�����G�����i�[����z��
		@return	����ݒ�ł�����
		@retval	true	�ݒ萬��
		@retval	false	�ݒ莸�s
	*/
	bool LoadGriffon(const std::string& fileName, std::vector<EnemyPtr>& enemies);
	
	/*
		@brief	CRaybit���[�h
		@param	[in]	fileName	�G�t�@�C���p�X
		@param	[out]	enemies		���[�h�����G�����i�[����z��
		@return	����ݒ�ł�����
		@retval	true	�ݒ萬��
		@retval	false	�ݒ莸�s
	*/
	bool LoadRaybit(const std::string& fileName, std::vector<EnemyPtr>& enemies);

	
	/*
		@brief	CRoboticArm���[�h
		@param	[in]	fileName	�G�t�@�C���p�X
		@param	[out]	enemies		���[�h�����G�����i�[����z��
		@return	����ݒ�ł�����
		@retval	true	�ݒ萬��
		@retval	false	�ݒ莸�s
	*/
	bool LoadRoboticArm(const std::string& fileName, std::vector<EnemyPtr>& enemies);

#pragma endregion	// �G�e�[�u���ǂݍ���
	//---------------------------------

	//---------------------------------
#pragma region �s�b�N�A�b�v�A�C�e���e�[�u���ǂݍ���
	/*
		@brief	CPickupJewely���[�h
		@param	[in]	fileName	�t�@�C���p�X
		@param	[out]	pickups		���[�h���������i�[����z��
		@return	����ݒ�ł�����
		@retval	true	�ݒ萬��
		@retval	false	�ݒ莸�s
	*/
	bool LoadJewely(const std::string& fileName, std::vector<PickupPtr>& pickups);

#pragma endregion	// �s�b�N�A�b�v�A�C�e���e�[�u���ǂݍ���
	//---------------------------------


public:
	
	/*
		@brief	CharBase�ǂݍ���(�摜���A�T�C�Y)
				�T�C�Y��-1�w�肷��Ɖ摜������擾����
		@attension	f�̓I�[�v���ς�
		@param	[in/out]	f	���t�@�C��
		@param	[out]		cb	���[�h���������i�[����CharBase
		@return	����ݒ�ł�����
		@retval	true	�ݒ萬��
		@retval	false	�ݒ莸�s
	*/
	static bool LoadCharBase(std::ifstream& f, charabase::CharBase& cb);

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
		@return	����ݒ�ł�����
		@retval	true	�ݒ萬��
		@retval	false	�ݒ莸�s
	*/
	bool LoadEnemiesData(std::vector<EnemyPtr>& enemies);
	
	/*
		@brief	�s�b�N�A�b�v�A�C�e���ǂݍ���
		@param	[out]	pickups		���[�h�����s�b�N�A�b�v�A�C�e�������i�[����z��
		@return	����ݒ�ł�����
		@retval	true	�ݒ萬��
		@retval	false	�ݒ莸�s
	*/
	bool LoadPickupData(std::vector<PickupPtr>& pickups);


	/*
		@brief	�t�@�C���p�X�擾
			GetPath("#File");
		@param	[in]	tag	�擾�t�@�C�����ʖ�(#�^�O)
		@return �t�@�C���p�X
	*/
	std::string GetPath(const std::string& tag) const;
};


#endif