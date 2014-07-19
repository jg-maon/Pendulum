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
		RESULT,					// スコア表示

		END_EXIT,				// エンド画面終了
	
	};
	State state_;


	struct EndObject
	{
		charabase::CharBase cb;		// 表示座標等
		int point;					// ポイントデータ
		char charData;				// 文字データ
		std::string bonusInfo;		// ボーナス詳細

	};

	//charabase::CharBase sceneEndBg_;
	EndObject bonusPoints[5];
	EndObject resultPoint_;
	

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