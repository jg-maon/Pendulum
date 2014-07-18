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
		ENTER,			// 登場アニメーション中
		BATTLE,			// 戦闘中
		EXIT,			// 退場アニメーション中
	};
private:
	
	typedef std::shared_ptr<IStage> StagePtr;
	typedef std::unordered_map<std::string, StagePtr> StageMap;
	StageMap stages_;			// ステージクラス

	std::string nowStage_;		// 現在ステージ名
	
	StageState stageState_;		// ステージアニメーション状態




public:
	CStageMng();
	virtual void step() override;
	virtual void draw() override;

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
		@brief	ステージ状態更新
		@param	[in]	state	状態
		@return	なし
	*/
	void setStageState(StageState state);

	/*
		@brief	登場アニメーション中か
		@return	アニメーション中フラグ
		@retval	true	アニメーション中
		@retval	false	アニメーション中ではない
	*/
	bool isEnterAnimating() const;
	
	/*
		@brief	退場アニメーション中か
		@return	アニメーション中フラグ
		@retval	true	アニメーション中
		@retval	false	アニメーション中ではない
	*/
	bool isExitAnimating() const;


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