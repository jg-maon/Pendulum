#ifndef DEF_SCENETITLE_H
#define DEF_SCENETITLE_H

#ifndef DEF_SCENEBASE_HPP
#include "sceneBase.hpp"
#endif

#ifndef DEF_MYMATH_HPP
#include "myMath.hpp"
#endif

class CSceneTitle : public IScene
{
private:
	enum class Phase
	{
		TITLE,			// メインタイトル画面
		DEMO,			// デモプレイ
	}phase_;			// フェーズ

	float phaseTime_;	// 各フェーズ表示時間

	//-------------------------------------
	// デモプレイで必要な変数等
	class CDemo
	{
	public:
		mymath::Vec3f pos;
	}demo_;
	//-------------------------------------

private:
	/*
		@brief	タイトル画面表示初期化処理
		@return	なし
	*/
	void TitleInit();
	/*
		@brief	タイトル画面時の更新処理
		@return	なし
	*/
	void TitleStep();
	/*
		@brief	デモプレイ初期化処理
		@return なし
	*/
	void DemoInit();
	/*
		@brief	デモプレイ表示中の更新処理
		@return	なし
	*/
	void DemoStep();


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
	CSceneTitle();
	~CSceneTitle();
	void draw() override;	// 描画
};


#endif