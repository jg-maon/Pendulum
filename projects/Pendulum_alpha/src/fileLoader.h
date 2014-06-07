#ifndef DEF_FILELOADER_H
#define DEF_FILELOADER_H

#ifndef DEF_ENEMYBASE_HPP
#include "enemyBase.hpp"
#endif

#include <vector>

/*
	@brief	ファイル読み込みクラス
			CFileMngより呼び出される
*/
class CFileLoader
{
	typedef std::unordered_map<std::string, int> FontTable;
	// リソースファイル用
	struct ResData
	{
		std::string resname;
		std::string path;
	};

	const std::string iniFile_;
private:
	//---------------------------------
#pragma region リソースファイル読み込み関連
	/*
		@brief	リソースファイル読み込み処理
		@param	[in/out]	resFile		リソースファイル一覧ファイルパス
		@param	[out]		fontTablle	フォント管理テーブル
		@return	なし
	*/
	void LoadRes(const std::string& resFile, FontTable& fontTable);
	/*
		@brief	画像ファイル読み込み
		@param	[in/out]	resF	リソースファイル一覧ファイル
		@return	なし
	*/
	void LoadImg(std::ifstream& resF);
	/*
		@brief	BGM読み込み
		@param	[in/out]	resF	リソースファイル一覧ファイル
		@return	なし
	*/
	void LoadBGM(std::ifstream& resF);
	/*
		@brief	SE読み込み
		@param	[in/out]	resF	リソースファイル一覧ファイル
		@return	なし
	*/
	void LoadSE(std::ifstream& resF);
	/*
		@brief	フォント読み込み
		@param	[in/out]	resF		リソースファイル一覧ファイル
		@param	[out]		fontTable	フォント管理テーブル
		@return	なし
	*/
	void LoadFont(std::ifstream& resF, FontTable& fontTable);
#pragma endregion	// リソースファイル読み込み関連
	//---------------------------------
public:
	/*
		@brief	各ファイル読み込み
		@param	[in]	iniFile	設定ファイルパス
	*/
	CFileLoader(const std::string& iniFile);

	/*
		@brief	各ファイル読み込み
		@param	[in]	iniFile		設定ファイルパス
		@param	[out]	fontTable	フォント管理テーブル
	*/
	CFileLoader(const std::string& iniFile, FontTable& fontTable);


	/*
		@brief	敵情報をファイルからロードする
		@param	[in]	enemyFile	敵情報一覧ファイル名
		@param	[out]	enemies		ロードした敵情報を格納する配列
		@return	なし
	*/
	void LoadEnemiesData(const std::string& enemyFile, std::vector<EnemyPtr>& enemies);
};


#endif