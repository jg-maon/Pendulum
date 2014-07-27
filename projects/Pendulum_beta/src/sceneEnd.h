#ifndef DEF_SCENEEND_H
#define DEF_SCENEEND_H

#ifndef DEF_SCENEBASE_HPP
#include "sceneBase.hpp"
#endif

#ifndef DFE_SCOREMNG_H
#include "scoreMng.h"
#endif

class CSceneEnd : public IScene
{
public:

private:
	enum class Phase
	{
		CLEAR,				// クリアーって表示
		SCORE,				// ボーナススコア等表示
		RANKING,			// ランキング検索

		//-----------------------------------------------
		// エンド画面終了
		REGISTER,			// ランキング登録
		TITLE,				// ランキング登録せずにタイトルへ
	
	};
	CSceneEnd::Phase phase_;

	enum
	{
		TOTAL_Y = 4700,						// トータルスコア画像表示Y(unit[/10px])
		TOTALSCORE_MARGIN = 100,			// トータルスコア画像との間隔(unit[/10px])
		BONUS_X = 6000,						// ボーナス整列始点X(unit[/10px])
		BONUS_Y = 2000,						// ボーナス整列始点Y(unit[/10px])
		BONUS_MOVE_Y = 150,					// ボーナス移動量Y(unit[/10px])
		BONUS_MARGIN = 300,					// ボーナス間隔(unit[/10px])

		RANKIN_Y = 6000,					// ランクイン画像表示Y(unit[/10px])
		RANKIN_MARGIN = BONUS_MARGIN + 100,	// ランクイン画像間隔(unit[/10px])
		RANKIN_MOVE_Y = 300,				// ランクイン画像アニメーションY移動量(unit[/10px])
	};

	charabase::Anim anim_;					// アニメーション用
	charabase::CharBase clear_;				// ステージクリア画像
	float animTime_;						// アニメーション用カウンタ
	float createInterval_;					// 煌きエフェクト生成間隔
	float dir_;								// 煌きエフェクト生成方向

	struct BonusObject
	{
		charabase::CharBase cb;				// 表示座標等
		float time;							// 表示時間
		CScoreMng::BonusScore bonusInfo;	// ボーナス詳細
		mymath::Vec3f startPos;				// イージング用初期座標
		bool isStarted;						// イージング用スタートしたフラグ

	};
	std::vector<BonusObject> bonusObjs_;	// ボーナス
	charabase::CharBase totalScoreObj_;		// トータルスコア画像
	charabase::CharBase scoreObj_;			// 数字オブジェクト
	int dispScore_;							// 描画スコア
	int totalScore_;						// 結果スコア

	struct RankinObj
	{
		charabase::CharBase cb;				// 画像
		float time;							// アニメーション用
		mymath::Vec3f startPos;				// 初期座標
	};
	std::vector<RankinObj> rankinObjs_;		// ランクイン！ってやるやつ

#if 0
	int selectChar_;			// 選択文字番号

	float registerAnimTime_;	// 登録アニメーションカウント

private:

	static const float BONUSPOINT_X;		//ボーナスポイント表示x座標
	static const float BONUSINFO_X;			//ボーナス詳細表示x座標
	static const float BONUSPOINT_WIDTH;	//ボーナスポイント表示幅

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

	
	/*
		@brief	リザルト描画
		@return	なし
	*/
	void ResultDraw();

	/*
		@brief	名前入力描画
		@return	なし
	*/
	void NameInputDraw();

	/*
		@brief	ハイスコア表示描画
		@return	なし
	*/
	void HiscoreDispDraw();


	/*
		@brief	キー入力確認
		@return	キーが入力されたかどうか
		@retval	true	入力された
		@retval	false	入力されていない
	*/
	bool InputNextKey();

	/*
		@brief	ポイントデータの初期化
		@return	なし
	*/
	void PointDataInit();

	/*
		@brief	ポイントデータの描画
		@return	なし
	*/
	void PointDataDraw();


	/*
		@brief	オブジェクト初期化
		@return	なし
	*/
	void init();
#endif

private:
	/*
		@brief	ボーナスオブジェクト初期化
		@return	なし
	*/
	void InitBonus();

protected:
	/*
		@brief	メイン更新処理
		@return	メイン終了か
		@retval	true	メイン終了(フェードアウト開始)
		@retval	false	メイン処理中
	*/
	bool update() override;


public:
	/*
		@brief	初期化
		@return	なし
	*/
	CSceneEnd();
	~CSceneEnd();
	
	/*
		@brief	描画
		@return	なし
	*/
	void	draw() override;

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