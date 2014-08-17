#ifndef DEF_GAMESTATUS_H
#define DEF_GAMESTATUS_H

#ifndef DEF_OBJ_H
#include "obj.h"
#endif

#ifndef DEF_COMMON_H
#include "common.h"
#endif

/*
	@brief	メインゲームの状態を管理
*/
class CGameStatus : public Base
{
public:
	/*
		@brief	環境、システム系
	*/
	struct Environment
	{
		float gravityAcc;			// 重力加速度
	};
private:

	/*
		@brief	ヒットストップ系
	*/
	class HitStop
	{
	private:
		float hitStopTime_;			// ヒットストップ時間
		float maxHitStopTime_;		// ヒットストップの最長時間

		float backAlpha_;			// ヒットストップ中の背景暗さ
		float backAnimTime_;		// 背景表示アニメーション用
		common::DispState state_;	// 背景表示状態
	public:
		/*
			@brief	初期化
		*/
		HitStop();
		/*
			@brief	初期化
			@param	[in]	stopTime	ヒットストップ時間
			@return	なし
		*/
		void init(float hitStopTime);
		/*
			@brief	ヒットストップ時間更新処理
			@return	なし
		*/
		void step();
		/*
			@brief	ヒットストップ中描画処理
			@return	なし
		*/
		void draw() const;


		/*
			@brief	ヒットストップ開始
			@param	[in]	extendTime	延長時間
			@return	なし
		*/
		void Begin(float extendTime);

		/*
			@brief	ヒットストップの強制終了
			@return	なし
		*/
		void End();

		/*
			@brief	ヒットストップ中か
			@return	ヒットストップ中か
			@retval	true	ヒットストップ中
			@retval	false	ヒットストップ中でない
		*/
		bool isStopping() const;
	}hitStop_;


	int slowInterval_;			// ヒットストップ中更新フレーム
	int updateCnt_;				// 更新計算フレーム数(0->MAX)

	Environment environment_;	// システム

public:
	/*
		@brief	初期化
	*/
	CGameStatus();
	~CGameStatus();
	
	/*
		@brief	初期化
	*/
	virtual void start() override;

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
		@brief	環境情報取得
		@return	環境情報
	*/
	const Environment& getEnv() const;

	/*
		@brief	ヒットストップ開始
		@param	[in]	extendTime	延長時間(デフォルト:0.f=通常通り)
		@return	なし
	*/
	void BeginHitStop(float extendTime = 0.f);

	/*
		@brief	ヒットストップの強制終了
		@return	なし
	*/
	void EndHitStop();

	/*
		@brief	ヒットストップ中か
		@return	ヒットストップ中か
		@retval	true	ヒットストップ中
		@retval	false	ヒットストップ中でない
	*/
	bool isHitStopping() const;

	/*
		@brief	更新をかけられるか
		@return	更新をかけられるか
		@retval	true	更新をかけることができる
		@retval	false	更新しない
	*/
	bool isUpdatable() const;


};


#endif