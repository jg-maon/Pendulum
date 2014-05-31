#ifndef DEF_SCENEEND_H
#define DEF_SCENEEND_H

#ifndef DEF_SCENEBASE_HPP
#include "sceneBase.hpp"
#endif

class CSceneEnd : public IScene
{
private:
	enum class State
	{
		NAME,					// 名前選択
		REGISTER_ANIM,			// 登録決定アニメーション中
		ANIM_END,				// アニメーション終了
	};
	State state_;

	int selectChar_;			// 選択文字番号

	float registerAnimTime_;	// 登録アニメーションカウント


private:
	/*
		@brief	名前選択処理
		@return	なし
	*/
	void DecideName();

	/*
		@brief	登録決定アニメーション処理
		@return	なし
	*/
	void RegisterAnimation();

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
	CSceneEnd();
	~CSceneEnd();
	void	draw() override;	// 描画
};


#endif