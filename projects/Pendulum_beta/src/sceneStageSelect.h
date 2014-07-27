#ifndef DEF_SCENESTAGESELECT_H
#define DEF_SCENESTAGESELECT_H

#ifndef DEF_SCENEBASE_HPP
#include "sceneBase.hpp"
#endif

class CSceneStageSelect : public IScene
{
private:
	mymath::Vec3f startCameraPos_;			// カメラ初期位置

	mymath::Recti fieldRect_;		// ステージ選択フィールドサイズ
	mymath::Recti bgRect_;			// 背景サイズ
	charabase::CharPtr	back_;		// 背景
	
	// ステージ画像、説明画像
	std::vector<std::pair<charabase::CharPtr, charabase::CharPtr>> stages_;

	float descHeight_;				// 説明文アニメーション%(0.0f~1.0f)

	std::string stageName_;			// プレイするステージ名

	enum class NextSceneType
	{
		TITLE,		// タイトル画面へ
		MAIN,		// メインステージ画面へ
	}nextScene_;
	enum
	{
		TO_TITLE_TIME = 30,		// タイトル画面へ移行する時間[unit:/10sec]
	};
	float toTitleTime_;				// タイトル画面へ移行する時間

private:
	/*
		@brief	情報読み込み
		@param	[in/out]	f	ファイル
		@return	なし
	*/
	void LoadInfo(std::ifstream& f);
	/*
		@brief	ステージ読み込み
		@param	[in/out]	f	ファイル
		@return	なし
	*/
	void LoadStages(std::ifstream& f);
protected:
	/*
		@brief	メイン更新処理
		@return	メイン終了か
		@retval	true	メイン終了(フェードアウト開始)
		@retval	false	メイン処理中
	*/
	bool update() override;

	
public:
	CSceneStageSelect();
	~CSceneStageSelect();
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