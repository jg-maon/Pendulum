#ifndef DEF_FILEMNG_H
#define DEF_FILEMNG_H

#ifndef DEF_OBJ_H
#include "obj.h"
#endif

#include <vector>

/*
	@brief	ファイル管理クラス(データベース)
			ゲームマネージャに保持される
*/
class CFileMng
{
private:
	// フォント用
	std::unordered_map<std::string, int> fontTable_;

	// データ一覧
	std::vector<ObjPtr> dataTable_;

private:

public:

	CFileMng();

	/*
		@brief	情報読み込み
		@param	[in]	iniFile		設定ファイルパス
		@return	なし
	*/
	void Load(const std::string& iniFile);

	/*
		@brief	情報の取得
		@param	[in/out]	obj	格納するクラスオブジェクト
								型情報でキャストを行う
		@return	情報を設定できたか
		@retval	true	設定成功
		@retval	false	設定失敗
	*/
	template<class OBJ>	bool GetData(OBJ& obj) const
	{
		bool ret = false;
		const std::string& name = obj.getName();
		for (auto& data : dataTable_)
		{
			if (name == data->getName())
			{
				obj = *(std::dynamic_pointer_cast<OBJ>(data));
				//return true;
				ret = true;
				break;
			}
		}
		return ret;
	}

};


#endif