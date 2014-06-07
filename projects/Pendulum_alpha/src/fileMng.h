#ifndef DEF_FILEMNG_H
#define DEF_FILEMNG_H

#ifndef DEF_ENEMYBASE_HPP
#include "enemyBase.hpp"
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

	// 敵データ
	std::vector<EnemyPtr> enemiesData_;

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
		@brief	敵情報の設定
		@param	[in/out]	enemy	格納する敵クラスオブジェクト
		@return	敵情報を設定できたか
		@retval	true	設定成功
		@retval	false	設定失敗
	*/
	template<class ENEMY>	void GetEnemyData(ENEMY& enemy) const
	{
		if (enemiesData_.empty())	LoadEnemiesData();
		bool ret = false;
		const std::string& name = enemy.getName();
		for (const auto& data : enemiesData_)
		{
			if (name == data->getName())
			{
				enemy = *data;
				//return true;
				ret = true;
				break;
			}
		}
		return ret;
	}

};


#endif