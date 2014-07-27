#ifndef DEF_SCENEDESCRIPTION_H
#define DEF_SCENEDESCRIPTION_H

#ifndef DEF_SCENEBASE_HPP
#include "sceneBase.hpp"
#endif

class CSceneDescription : public IScene
{
private:

protected:
	/*
		@brief	メイン更新処理
		@return	メイン終了か
		@retval	true	メイン終了(フェードアウト開始)
		@retval	false	メイン処理中
	*/
	bool update() override;

public:
	CSceneDescription();
	~CSceneDescription();
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