#ifndef DEF_SCENEDESCRIPTION_H
#define DEF_SCENEDESCRIPTION_H

#ifndef DEF_SCENEBASE_HPP
#include "sceneBase.hpp"
#endif

class CSceneDescription : public IScene
{
	
protected:
	/*
		@brief	メイン更新処理
		@return	メイン終了か
		@retval	true	メイン終了(フェードイン開始)
		@retval	false	メイン処理中
	*/
	bool update() override;

	/*
		@brief	次のシーンに切り替わる瞬間に呼ばれる
		@return	次のシーン
	*/
	IScene* NextScene() override;

public:
	CSceneDescription();
	~CSceneDescription();
	void	draw() override;	// 描画
};

#endif