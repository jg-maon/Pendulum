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
	@brief	ファイル読み込みクラス
			CFileMngより呼び出される
*/
class CFileLoader
{
	/*
		@brief		LoadInfo読み込み
		@attension	fはオープン済み
		@param	[in/out]	f			ファイル
		@param	[in/out]	loadValues	読み込む対象
		@return	読み込み成功したか
		@retval	true	読み込み成功
		@retval	false	読み込み失敗
	*/
#define LoadInfo(f, loadValues)		LoadInfoFunc(__FILE__##" "##__FUNCTION__, f, loadValues);
public:
	typedef std::unordered_map<std::string, int> FontTable;
	// リソースファイル用
	struct ResData
	{
		std::string resname;
		std::string path;
	};
private:
	const std::string iniFile_;		// 各種設定記述ファイルパス

	//------------------------------------------
	// LoadInfo用
	struct LoadValue
	{
		char* tag;				// #タグ
		void* value;			// 格納する値
		std::string type;		// 型情報
	};

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

	//---------------------------------
	/*
		@brief		LoadInfo読み込み
		@attension	fはオープン済み
		@param	[in]		from		呼び出し元
		@param	[in/out]	f			ファイル
		@param	[in/out]	loadValues	読み込む対象
		@return	読み込み成功したか
		@retval	true	読み込み成功
		@retval	false	読み込み失敗
	*/
	bool LoadInfoFunc(const std::string& from, std::ifstream& f, std::vector<LoadValue>& loadValues);
	

	//---------------------------------
#pragma region 敵テーブル読み込み
	/*
		@brief	CBirdロード
		@param	[in]	fileName	敵ファイルパス
		@param	[out]	enemies		ロードした敵情報を格納する配列
		@return	情報を設定できたか
		@retval	true	設定成功
		@retval	false	設定失敗
	*/
	bool LoadBird(const std::string& fileName, std::vector<EnemyPtr>& enemies);

	/*
		@brief	CFairyロード
		@param	[in]	fileName	敵ファイルパス
		@param	[out]	enemies		ロードした敵情報を格納する配列
		@return	情報を設定できたか
		@retval	true	設定成功
		@retval	false	設定失敗
	*/
	bool LoadFairy(const std::string& fileName, std::vector<EnemyPtr>& enemies);
	
	/*
		@brief	CGriffonロード
		@param	[in]	fileName	敵ファイルパス
		@param	[out]	enemies		ロードした敵情報を格納する配列
		@return	情報を設定できたか
		@retval	true	設定成功
		@retval	false	設定失敗
	*/
	bool LoadGriffon(const std::string& fileName, std::vector<EnemyPtr>& enemies);
	
	/*
		@brief	CRaybitロード
		@param	[in]	fileName	敵ファイルパス
		@param	[out]	enemies		ロードした敵情報を格納する配列
		@return	情報を設定できたか
		@retval	true	設定成功
		@retval	false	設定失敗
	*/
	bool LoadRaybit(const std::string& fileName, std::vector<EnemyPtr>& enemies);

	
	/*
		@brief	CRoboticArmロード
		@param	[in]	fileName	敵ファイルパス
		@param	[out]	enemies		ロードした敵情報を格納する配列
		@return	情報を設定できたか
		@retval	true	設定成功
		@retval	false	設定失敗
	*/
	bool LoadRoboticArm(const std::string& fileName, std::vector<EnemyPtr>& enemies);

#pragma endregion	// 敵テーブル読み込み
	//---------------------------------

	//---------------------------------
#pragma region ピックアップアイテムテーブル読み込み
	/*
		@brief	CPickupJewelyロード
		@param	[in]	fileName	ファイルパス
		@param	[out]	pickups		ロードした情報を格納する配列
		@return	情報を設定できたか
		@retval	true	設定成功
		@retval	false	設定失敗
	*/
	bool LoadJewely(const std::string& fileName, std::vector<PickupPtr>& pickups);

#pragma endregion	// ピックアップアイテムテーブル読み込み
	//---------------------------------


public:
	
	/*
		@brief	CharBase読み込み(画像名、サイズ)
				サイズに-1指定すると画像名から取得する
		@attension	fはオープン済み
		@param	[in/out]	f	情報ファイル
		@param	[out]		cb	ロードした情報を格納するCharBase
		@return	情報を設定できたか
		@retval	true	設定成功
		@retval	false	設定失敗
	*/
	static bool LoadCharBase(std::ifstream& f, charabase::CharBase& cb);

	/*
		@brief	各ファイル読み込み
		@param	[in]	iniFile	設定ファイルパス
	*/
	CFileLoader(const std::string& iniFile);

	/*
		@brief	各ファイル読み込み
		@param	[out]	fontTable	フォント管理テーブル
		return	なし
	*/
	void Load(FontTable& fontTable);

	/*
		@brief	プレイヤー読み込み
		@param	[out]	player		プレイヤー
		@return	情報を設定できたか
		@retval	true	設定成功
		@retval	false	設定失敗
	*/
	bool LoadPlayerData(CPlayer& player);

	/*
		@brief	敵情報をファイルからロードする
		@param	[out]	enemies		ロードした敵情報を格納する配列
		@return	情報を設定できたか
		@retval	true	設定成功
		@retval	false	設定失敗
	*/
	bool LoadEnemiesData(std::vector<EnemyPtr>& enemies);
	
	/*
		@brief	ピックアップアイテム読み込み
		@param	[out]	pickups		ロードしたピックアップアイテム情報を格納する配列
		@return	情報を設定できたか
		@retval	true	設定成功
		@retval	false	設定失敗
	*/
	bool LoadPickupData(std::vector<PickupPtr>& pickups);


	/*
		@brief	ファイルパス取得
			GetPath("#File");
		@param	[in]	tag	取得ファイル識別名(#タグ)
		@return ファイルパス
	*/
	std::string GetPath(const std::string& tag) const;
};


#endif