#ifndef DEF_STAGEMNG_H
#define DEF_STAGEMNG_H

#ifndef DEF_OBJ_H
#include "obj.h"
#endif

#ifndef DEF_STAGEBASE_H
#include "stageBase.h"
#endif

#ifndef DEF_MYMATH_HPP
#include "MyMath.hpp"
#endif


class CStageMng : public Base
{
public:
	enum class StageState		// ステージのアニメーション状態
	{
		PLAYER_ENTER,			// プレイヤー登場アニメーション中
		CLEAR_ANNOUNCE,			// クリア条件表示中
		BATTLE,					// 戦闘中
		PLAYER_EXIT,			// プレイヤー退場アニメーション中
		BOSS_ENTER,				// ボス登場アニメーション中
		BOSS_EXIT,				// ボス退場アニメーション中
	};
private:
	
	typedef std::shared_ptr<IStage> StagePtr;
	typedef std::unordered_map<std::string, StagePtr> StageMap;
	StageMap stages_;			// ステージクラス

	std::string nowStage_;		// 現在ステージ名
	
	StageState stageState_;		// ステージアニメーション状態

	float time_;				// ステージ内時間


public:
	CStageMng();
	/*
		@brief	更新処理
		@return	なし
	*/
	virtual void step() override;
	/*
		@brief	描画処理
		@return	なし
	*/
	virtual void draw() override;

	/*
		@brief	初期化
		@return	なし
	*/
	void start() override;
	/*
		@brief	終了処理
		@return	なし
	*/
	void stop() override;

	/*
		@brief	ステージマネージャーオブジェクトを取得
		@return	ポインタ
		@retval	nullptr	ステージマネージャーオブジェクトが作られていない場合
		@retval	マネージャーのポインタ
	*/
	static const std::shared_ptr<CStageMng> GetPtr();

	/*
		@brief	クラス情報の初期化(ロード処理)
		@return	ロードに成功したか
		@retval	true	成功
		@retval	false	失敗
	*/
	bool load();

	/*
		@brief	ステージのロード
		@param	[in]	stageName	ステージ名(ファイル名)
		@return	なし
	*/
	void LoadStage(const std::string& stageName);


	/*
		@brief	ステージカメラ可動領域に合わせた[ゲープロ2Dカメラ]移動
		@param	[in]	lookAt	カメラの中心座標
		@param	[in]	adjust	カメラ移動までの制限値(デフォルト:0)
								adjust[px]以上移動していた場合カメラ移動
		@return	なし
	*/
	void MoveCamera(const mymath::Vec3f& lookAt, float adjust = 0.f) const;

	/*
		@brief	ステージが終了したか取得
		@return	ステージ終了フラグ
		@retval	true	ステージ終了
		@retval	false	ステージプレイ継続
	*/
	bool isEndStage() const;
	
	/*
		@brief	ボスステージか
		@return	ボスステージか
		@retval	true	ボスステージ
		@retval	false	ボスステージでない
	*/
	bool isBossStage() const;

	

	/*
		@brief	ステージ状態更新
		@param	[in]	state	状態
		@return	なし
	*/
	void setStageState(StageState state);

	/*
		@brief	クリア条件表示アニメーション中か
		@return	アニメーション中フラグ
		@retval	true	アニメーション中
		@retval	false	アニメーション中ではない
	*/
	bool isClearAnnounceAnimating() const;
	
	/*
		@brief	プレイヤー登場アニメーション中か
		@return	アニメーション中フラグ
		@retval	true	アニメーション中
		@retval	false	アニメーション中ではない
	*/
	bool isPlayerEnterAnimating() const;
		
	/*
		@brief	プレイヤー退場アニメーション中か
		@return	アニメーション中フラグ
		@retval	true	アニメーション中
		@retval	false	アニメーション中ではない
	*/
	bool isPlayerExitAnimating() const;
	
	/*
		@brief	ボス登場アニメーション中か
		@return	アニメーション中フラグ
		@retval	true	アニメーション中
		@retval	false	アニメーション中ではない
	*/
	bool isBossEnterAnimating() const;
	/*
		@brief	ボス退場アニメーション中か
		@return	アニメーション中フラグ
		@retval	true	アニメーション中
		@retval	false	アニメーション中ではない
	*/
	bool isBossExitAnimating() const;


	//=====================================================================

	/*
		@brief	ボーナススコア用ノルマタイム以内にクリアしたか(FastClear)
		@return	FastBonus
		@retval	true	クリアした
		@retval	false	クリアできなかった
	*/
	bool isNormaTimeClear() const;

	//=====================================================================

	/*
		@brief	現在のステージのBGM名を取得
		@return	BGM
	*/
	const std::string& getStageBGM() const;

	/*
		@brief	現在のステージサイズの取得
		@return	ステージRect
	*/
	const mymath::Recti& getStageRect() const;

	/*
		@brief	指定したステージのサイズの取得
		@param	[in]	stage	ステージ名
		@return	ステージRect
	*/
	const mymath::Recti& getStageRect(const std::string& stage) const;

	/*
		@brief	現在のステージのカメラ可動範囲の取得
		@return	カメラ可動範囲
	*/
	const mymath::Recti& getCameraRect() const;

	/*
		@brief	指定したステージのカメラ可動範囲の取得
		@param	[in]	stage	ステージ名
		@return	カメラ可動範囲
	*/
	const mymath::Recti& getCameraRect(const std::string& stage) const;


	/*
		@brief	アクションポイントの取得
		@return	アクションポイント
	*/
	const std::vector<ActPtPtr>& getActionPoints() const;



};

#endif