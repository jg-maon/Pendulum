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

public:
	static std::string enemyTableFile;				// 敵テーブルファイルパス

public:
	CEnemyMng();
	virtual void step() override;
	virtual void draw() override;
	/*
		@brief	敵マネージャオブジェクトを取得
		@return	ポインタ
		@retval	nullptr	敵マネージャオブジェクトが作られていない場合
		@retval	敵マネージャのポインタ
	*/
	static ObjPtr GetPtr();
	
	/*
		@brief	敵情報をファイルから読み込む
		@attention	ファイル書式のエラーチェックが簡易的
		@param	[in]	fileName	ファイル名
		@return	なし
	*/
	void LoadEnemiesInfo(const std::string& fileName);


	/*
		@brief	敵生成
		@param	[in]	enemy	作る敵
		@return	なし
	*/
	void CreateEnemy(const EnemyPtr& enemy);
	/*
		@brief	敵取得
		@return	敵オブジェクト
	*/
	std::vector<EnemyPtr> GetEnemies();

};
#endif