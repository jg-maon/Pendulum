#ifndef DEF_SCENEMAIN_H
#define DEF_SCENEMAIN_H

#ifndef DEF_SCENEBASE_HPP
#include "sceneBase.hpp"
#endif


class CSceneMain : public IScene
{
protected:
	/*
		@brief	メイン更新処理
		@return	メイン終了か
		@retval	true	メイン終了(フェードアウト開始)
		@retval	false	メイン処理中
	*/	
	bool update() override;


public:
	CSceneMain();
	~CSceneMain();
	/*
		@brief	描画
		@return	なし
	*/
	void draw() override;

	/*
		@brief	初期化
		@return	なし
	*/
	void start() override;
	
	/*
		@brief	次のシーンに切り替わる瞬間に呼ばれる
		@return	次のシーン
	*/
	int NextScene() const override;
};


#endif 