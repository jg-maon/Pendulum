#ifndef DEF_SCENEJECLOGO_H
#define DEF_SCENEJECLOGO_H

#ifndef DEF_SCENEBASE_HPP
#include "sceneBase.hpp"
#endif

class CSceneJecLogo : public IScene
{
private:
	//float time_;
	enum
	{
		WAIT_TIME = 20,		// シーン遷移待ち時間(unit[/10sec])
	};

protected:
	/*
		@brief	メイン更新処理
		@return	メイン終了か
		@retval	true	メイン終了(フェードアウト開始)
		@retval	false	メイン処理中
	*/
	bool update() override;

public:
	CSceneJecLogo();
	~CSceneJecLogo();

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