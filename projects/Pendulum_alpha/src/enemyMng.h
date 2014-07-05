#ifndef DEF_ENEMYMNG_H
#define DEF_ENEMYMNG_H

#ifndef DEF_OBJ_H
#include "Obj.h"
#endif

#ifndef DEF_ENEMYBASE_HPP
#include "enemyBase.hpp"
#endif

//==================================
// 敵
#include "bird.h"
#include "fairy.h"
//==================================

class CEnemyMng : public Base
{
private:
	std::vector<EnemyPtr> temp_;			// 敵テーブル追加用
	std::vector<EnemyPtr> enemies_;			// 敵テーブル


public:
	CEnemyMng();
	/*
		@brief	更新処理
		@return	なし
	*/
	virtual void step() override;
	/*
		@brief	描画処理
		@return	なし
	*/
	virtual void draw() override;
	
	/*
		@brief	オブジェクトを消す
		@return	なし
	*/
	virtual void kill() override;
	/*
		@brief	オブジェクトを始動させる
		@return	なし
	*/
	virtual void start() override;
	/*
		@brief	オブジェクトを停止させる
		@return	なし
	*/
	virtual void stop() override;

	/*
		@brief	オブジェクトを描画のみに設定する
		@return	なし
	*/
	virtual void SetStatusDisp() override;

	/*
		@brief	敵マネージャオブジェクトを取得
		@return	ポインタ
		@retval	nullptr	敵マネージャオブジェクトが作られていない場合
		@retval	敵マネージャのポインタ
	*/
	static ObjPtr GetPtr();
	
	/*
		@brief	敵配置テーブルをファイルから読み込む
		@attention	ファイル書式のエラーチェックが簡易的
		@param	[in]	fileName	テーブルファイルパス
		@return	なし
	*/
	void LoadEnemyTable(const std::string& fileName);


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
	std::vector<EnemyPtr>& getEnemies();

};
#endif