#ifndef DEF_SCENEDEMOPLAY_H
#define DEF_SCENEDEMOPLAY_H

#ifndef DEF_PLAYER_H
#include "player.h"
#endif

#ifndef DEF_EASING_H
#include "easing.h"
#endif

#ifndef DEF_GAMEMANAGER_H
#include "gameManager.h"
#endif
/*
	@brief		デモプレイ用クラス
				タイトルシーンから呼ばれる
	@attension	通常のシーケンス処理とは勝手が違うので注意
*/
class CSceneDemoPlay  : public Base
{
public:
	/*
		@brief	デモプレイ用プレイヤー
	*/
	class CDemoPlayer : public CPlayer
	{
		float elapsedTime_;						// 経過時間
		
		enum class TimeActType		// タイムテーブル用行動一覧
		{
			HANG,		// ぶら下がり
			UNHANG,		// ぶら下がり解除
			ATTACK,		// 攻撃
		};
		/*
			@brief	タイムテーブル用
		*/
		struct TableContent
		{
			float time;			// 時間
			mymath::Vec3f pos;	// カーソル座標
			CDemoPlayer::TimeActType act;	// 行動
		};

		float preActTime_;						// 移動計算用
		std::vector<TableContent> timeTable_;	// 行動一覧
		int timeIndex_;							// タイムテーブル用
	protected:
		/*
			@brief	オブジェクト移動
			@return	なし
		*/
		virtual void MoveObject() override;

		/*
			@brief	キー入力
			@return	なし
		*/
		virtual void key() override;

	public:

		/*
			@param	[in]	x		初期座標
			@param	[in]	y		初期座標
			@param	[in]	z		初期座標
		*/
		CDemoPlayer(float x, float y, float z = 0.2f);

		/*
			@brief	更新処理
			@retrn	なし
		*/
		void step() override;


		/*
			@brief	デモプレイ終了か
			@return	デモプレイ終了か
			@retval	true	デモプレイ終了
			@retval	false	デモプレイ継続
		*/
		bool isEndDemoPlay() const;
	};

private:
	std::weak_ptr<CDemoPlayer> player_;

	/*
		@brief	スタッフクレジット用
	*/
	struct CreditObject
	{
		float time;					// 時間
		mymath::Linef pos;			// 計算用座標
		charabase::CharBase obj;	// 画像
		common::DispState state;	// 表示状態
	};
	std::vector<CreditObject> creditObjs_;		// スタッフクレジット

public:
	/*
		@brief	準備
	*/
	CSceneDemoPlay();
	/*
		@brief	終了処理
	*/
	~CSceneDemoPlay();

	
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

	void step() override{}

	/*	
		@brief	更新
		@return	デモプレイ終了か
		@retval	true	デモプレイ終了
		@retval	false	デモプレイ継続中
	*/
	bool update();

	/*
		@brief	描画
		@return	なし
	*/
	void draw();
};


#endif
