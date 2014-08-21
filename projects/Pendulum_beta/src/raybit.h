#ifndef DEF_RAYBIT_H
#define DEF_RAYBIT_H

#ifndef DEF_ENEMYBASE_HPP
#include "enemyBase.hpp"
#endif

class CRaybit : public IEnemy
{
public:
	struct LoadInfo
	{
		mymath::Rectf searchRect;		// 索敵範囲(現在座標からどれだけ策敵するか)
		mymath::Rectf chaseRect;		// 追跡範囲(初期座標からどれだけ追跡するか)
		mymath::Rectf attackRect;		// 攻撃範囲(現在座標からこの範囲にいると攻撃する)
		float moveSpeed;				// 移動速度
		float moveInterval;				// 移動間隔
		float moveTime;					// 移動時間

		float attackInterval;			// 攻撃間隔
		int attackNum;					// 攻撃回数
		float oneAttackInterval;		// 1発あたりの間隔

		mymath::Vec3f attackPos[2];		// 攻撃発射箇所
	};
private:
	enum class State
	{
		WAIT,				// 待機
		CHASE,				// 追跡
		LOST,				// 見失う
		ATTACK,				// 攻撃
		DESTROY,			// 死亡アニメ
	};

	enum class MoveState
	{
		STOP,				// 停止
		MOVE,				// 移動
	};

	enum class MotionType
	{
		MOVE,				// 待機・移動
	//	ATTACK,				// 攻撃
	};


private:

	static void (CRaybit::*StateStep_[])();	// 状態によるstep処理

	LoadInfo loadInfo_;

	charabase::Anim motionAnim_;					// アニメーション用
	MotionType motionType_;							// モーション
	std::vector<std::vector<int> > motionTable_;	// アニメーションテーブル

	charabase::CharBase eye_;				// 目画像

	State state_;							// 行動状態

	MoveState moveState_;					// 移動状態

	float elapsedTime_;						// 経過時間
	float nextActTime_;						// 次に行動を起こす時間
		
	mymath::Vec3f startPos_;				// 初期座標(追跡範囲用)

	int attackNum_;							// 攻撃回数(連続攻撃)
	bool attackPreliminary_;				// 攻撃予備行動を行うか true:行う

	charabase::CharBase targetCursor_;		// 攻撃照準アニメーション用

public:
	
private:

	/*
		@brief	情報の初期化
		@param	[in]	pos	初期座標
		@return	なし
	*/
	void init(const mymath::Vec3f& pos);

	/*
		@brief	待機
		@return	なし
	*/
	void WaitStep();
	/*
		@brief	追跡
		@return	なし
	*/
	void ChaseStep();
	/*
		@brief	プレイヤーを見失っている時の動き
		@return	なし
	*/
	void LostStep();
	/*
		@brief	攻撃
		@return	なし
	*/
	void AttackStep();
	/*
		@brief	死亡
		@return	なし
	*/
	void DestroyStep();


	/*
		@brief	残光を出しながら移動
		@return	なし
	*/
	void Move();

	/*
		@brief	行動タイプの決定
		@return	なし
	*/
	void DecideState();

	/*
		@brief	攻撃の発生処理
		@return	連続攻撃中か
		@retval	true	攻撃中
		@retval	false	攻撃終了
	*/
	bool CreateAttack();
	
public:
	/*
		@brief	空オブジェクト生成
				DB初期化時に呼ばれる
	*/
	CRaybit();
	/*
		@brief	座標指定したオブジェクト生成
		@param	[in]	pos	座標
	*/
	CRaybit(const mymath::Vec3f& pos);
	/*
		@brief	座標指定したオブジェクト生成
		@param	[in]	x	X座標
		@param	[in]	y	Y座標
		@param	[in]	z	奥行き(デフォルト:0.5f)
	*/
	CRaybit(float x, float y, float z = 0.5f);
	~CRaybit();
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
		@brief	オブジェクト同士の重なり処理
		@param	[in]	rival	重なっている相手オブジェクト
		@return	なし
	*/
	virtual void hit(const ObjPtr& rival) override;

	/*
		@brief	ダメージ加算
		@param	[in]	dam	ダメージ量
		@return 死亡したか
		@retval	true	死亡
		@retval	false	残存
	*/
	virtual bool ApplyDamage(int dam) override;
	
	/*
		@brief	当たり判定領域の取得
		@return	当たり判定領域
	*/
	virtual Collisions GetDamageAreas() const override;
	

	/*
		@brief	情報初期化
		@param	[in]	info	ロードしてきた情報一覧
		@return	なし
	*/
	void SetInfo(const LoadInfo& info);
};

#endif