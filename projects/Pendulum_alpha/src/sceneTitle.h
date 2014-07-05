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
	//======================================
	//定数宣言
	//======================================

private:
	enum
	{
		//TITLELOGO_X = system::WINW / 2,
		CLICK_MIN_ALPHA = 5,
		CLICK_DEG_ALPHA = 50,


	};
	static const float TITLELOGO_X;			// タイトルX座 標
	static const float TITLELOGO_Y;			// タイトルY座標
	static const float LOGO_A_X;			// 第１ロゴX座標
	static const float LOGO_A_Y;			// 第１ロゴY座標
	static const float LOGO_A_SIZE;			// 第１ロゴ開始サイズ
	static const float LOGO_A_STARTTIME;	// 第１ロゴ開始時間
	static const float LOGO_B_X;			// 第２ロゴX座標
	static const float LOGO_B_Y;			// 第２ロゴY座標
	static const float LOGO_B_SIZE;			// 第２ロゴ開始サイズ
	static const float LOGO_B_STARTTIME;	// 第２ロゴ開始時間
	static const float LOGO_C_X;			// 第３ロゴX座標
	static const float LOGO_C_Y;			// 第３ロゴY座標
	static const float LOGO_C_SIZE;			// 第３ロゴ開始サイズ
	static const float LOGO_C_STARTTIME;	// 第３ロゴ開始時間
	static const float LOGO_TIME;			// ロゴ移動時間
	static const float CLICK_ON;			// クリック表示時間

	static const float SLASH_A_X;			// 第１斬撃X座標
	static const float SLASH_A_Y;			// 第１斬撃Y座標
	static const float SLASH_A_DEG;			// 第１斬撃角度
	static const float SLASH_B_X;			// 第２斬撃X座標
	static const float SLASH_B_Y;			// 第２斬撃Y座標
	static const float SLASH_B_DEG;			// 第２斬撃角度
	static const float SLASH_NEXT_TIME;		// 斬撃待ち時間

	//static const int CLICK_MIN_ALPHA;		// クリックα最小値
	//static const int CLICK_DEG_ALPHA;		// クリックα減算地


	static const float TITLETIME;			// デモプレイに移動するまでの時間
	static const float DEMOTIME;			// タイトルに移動するまでの時間

private:
	//======================================
	//フラグ宣言
	//======================================

	//フェーズ
	enum class Phase
	{
		TITLE,			// メインタイトル画面
		DEMO,			// デモプレイ
	}phase_;

	IScene::State fadeState_;

	//アニメ全体の流れ
	enum class AnimPhase
	{
		LOGO,
		SLASH,
		FLASH,
		WAIT,
	}animPhase_;

	//ロゴ
	enum class LogoPhase
	{
		RED,			// 玉A移動
		GREEN,			// 玉B移動
		BLUE,			// 玉C移動
	}logoPhase_;

	//クリックアニメ
	enum class ClickPhase
	{
		HIDDEN,			// 非表示
		SHOW,			// 表示
		FADEOUT,		// フェードアウト
		FADEIN			// フェードイン
	}clickPhase_;

	//斬撃
	enum class SlashPhase
	{
		HIDDEN1,		// 非表示
		SLASH1,			// 斬撃１処理
		HIDDEN2,		// 中間非表示
		SLASH2,			// 斬撃２処理

	}slashPhase_;

	//
	enum class FlashPhase
	{
		FADEOUT,		// フェードアウト
		FADEIN			// フェードイン

	}flashPhase_;


	//======================================
	//変数宣言
	//======================================


	//charabase::CharBase titleBack_;		// 背景
	charabase::CharBase titleLogo_;			// ロゴ
	charabase::CharBase titleAnim_;			// ロゴアニメ
	charabase::CharBase titleSlash_;		// 斬撃
	charabase::Anim slashAnim_;				// 斬撃アニメ


	float phaseTime_;						// 各フェーズ表示時間
	float offTime_;							// クリック点滅
	float nextTime_;						// 斬撃待ち時間
	int clickAlpha_;						// クリックアニメAlpha値
	bool sePlaying_;						// SE再生
	bool flashFlag_;						// true:初期化処理 

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



	/*
		@brief	タイトルアニメ処理
		@return	なし
	*/
	void LogoAnime();


	/*
		@brief	プリーズチェック点滅
		@return	なし
	*/
	void PleaseClick();


	/*
		@brief	フェーズチェンジ
		@param	[in]	nextPhase	切り替えるフェーズ
		@return	なし
	*/
	void ChangePhase();

	/*
		@brief	斬撃アニメーション処理
		@return	なし
	*/
	void SlashAnime();

	/*
		@brief	画面フラッシュ処理
		@return	なし
	*/
	void TitleFlash();

	/*
		@brief	移動ロゴ拡大縮小処理
		@param	[in]	firstsize	初期サイズ
		@param	[in]	firstx　		初期X座標
		@return	現在のロゴのサイズ
	*/
	float LogoSize(float firstsize, float firsty);


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
	/*
		@brief	BGM管理名、フェードイン時間、フェードアウト時間の初期化
	*/
	CSceneTitle();
	~CSceneTitle();
	void draw() override;	// 描画
};


#endif