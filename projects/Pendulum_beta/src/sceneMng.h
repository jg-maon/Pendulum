#ifndef DEF_SCENEMNG_H
#define DEF_SCENEMNG_H

#ifndef DEF_OBJ_H
#include "obj.h"
#endif

#ifndef DEF_SCENEBASE_HPP
#include "sceneBase.hpp"
#endif

class CSceneMng : public Base
{
public:
	enum Scene		// シーンの種類
	{
		JEC,			// JECロゴ
		TITLE,			// タイトル画面
		STAGESELECT,	// ステージセレクト
		MAIN,			// メインステージ
		END,			// リザルト
		NAMEINPUT,		// ランキング登録
		
		
		SCENE_NUM,
	};
private:
	std::vector<ScenePtr> scenes_;
	int nowScene_;					// 現在のシーン
public:
	/*
		@brief シーンのロード
		@return なし
	*/
	CSceneMng();
	/*
		@brief	シーンの破棄
		@return なし
	*/
	~CSceneMng();

	
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

};


#endif