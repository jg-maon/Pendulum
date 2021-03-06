#ifndef DEF_FILEMNG_H
#define DEF_FILEMNG_H

#ifndef DEF_OBJ_H
#include "obj.h"
#endif

#ifndef DEF_FILELOADER_H
#include "fileLoader.h"
#endif


#include <vector>

/*
	@brief	ファイル管理クラス(データベース)
			ゲームマネージャに保持される
*/
class CFileMng
{
private:

	const std::string iniFile_;			// path.iniファイルパス

	CFileLoader fileLoader_;			// ファイルローダー

	// フォント用
	CFileLoader::FontTable fontTable_;

	// データ一覧
	std::vector<ObjPtr> dataTable_;


public:
	const std::string& iniFile;		// iniファイルパス


private:


public:

	/*
		@brief	情報読み込み
		@param	[in]	iniFile		設定ファイルパス
	*/
	CFileMng(const std::string& iniFile);

	/*
		@brief	情報読み込み
		@return	なし
	*/
	void Load();

	/*
		@brief	ファイルパス取得
			GetFile("#File")
		@param	[in]	tag	取得ファイル識別名(#タグ)
		@return ファイルパス
	*/
	std::string GetFile(const std::string& tag) const;


	/*
		@brief	GPライブラリ用フォントID取得
			Draw_FontText(x,y,z,srt,color,size, GetFontID("font_MSG15"));
		@param	[in]	resname	フォント管理名
		@return フォント番号
	*/
	int GetFontID(const std::string& resname) const;

	/*
		@brief	iniファイルのパスを取得
		@return	iniファイルパス
	*/
	//const std::string& getIniFile() const;


	/*
		@brief	情報の取得
			GetData(this);
		@param	[in/out]	obj	格納するクラスオブジェクト
								型情報でキャストを行う
		@return	情報を設定できたか
		@retval	true	設定成功
		@retval	false	設定失敗
	*/
	template<class OBJ>	bool GetData(OBJ* obj) const
	{
		bool successed = false;
		const std::string& name = obj->getName();
		for (auto& data : dataTable_)
		{
			if (name == data->getName())
			{
				*obj = *(std::dynamic_pointer_cast<OBJ>(data));
				//return true;
				successed = true;
				break;
			}
		}
		return successed;
	}
	/*
		@brief	情報の取得
			GetData(*this);
		@param	[in/out]	obj	格納するクラスオブジェクト
							型情報でキャストを行う
		@return	情報を設定できたか
		@retval	true	設定成功
		@retval	false	設定失敗
	*/
	template<class OBJ>	bool GetData(OBJ& obj) const
	{
		bool successed = false;
		const std::string& name = obj.getName();
		for (auto& data : dataTable_)
		{
			if (name == data->getName())
			{
				obj = *(std::dynamic_pointer_cast<OBJ>(data));
				//return true;
				successed = true;
				break;
			}
		}
		return successed;
	}

};

#endif