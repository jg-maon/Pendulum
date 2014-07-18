#ifndef DEF_GRIFFON_H
#define DEF_GRIFFON_H

#ifndef DEF_ENEMYBASE_HPP
#include "enemyBase.hpp"
#endif

class CGriffon : public IEnemy
{
public:
	struct LoadInfo
	{
		//float MAX_G;			// 最大重力速度
		//float GRAVITY_ACC;		// 重力加速度
		float SEARCH_RANGE;		// 索敵範囲(現在座標からどれだけ策敵するか)
		float CHASE_RANGE;		// 追跡範囲(初期座標からどれだけ追跡するか)
		float ATTACK_RANGE;		// 攻撃範囲(現在座標からこの範囲にいると攻撃する)
		//float RETURN_RANGE;		// 帰還時最小範囲(この範囲内なら初期位置に戻ったとみなす)
	
		float MOVE_SPEED;		// 移動速度
		float ATTACK_SPEED;		// 攻撃時移動速度

		float INV_TIME;			// 無敵時間

		float damageTime;			// 怯み時間
		float tackleDist;			// 攻撃距離
		float attackInterval;		// 攻撃間隔


		int health;				// 初期HP
		int power;				// 初期攻撃力


	};
private:

	enum class AnimState
	{
		FALL,				// 下降
		ROAR,				// 咆哮
	};

	enum class BattleState
	{
		WAIT,				// 待機
		CHASE,				// 追跡
		//RETURN,				// 帰還
		DAMAGE,				// ダメージ(怯み)
		ATTACK,				// 攻撃
		DESTROY,			// 死亡アニメ
	};

	enum class MotionType
	{
		MOVE,				// 待機・移動
		ATTACK,				// 攻撃
	};
private:

	static void (CGriffon::*StateStep_[])();	// 状態によるstep処理

	LoadInfo loadInfo_;

	AnimState animState_;						// アニメーションの状態
	BattleState battleState_;					// 行動状態

	charabase::Anim motionAnim_;					// アニメーション
	MotionType motionType_;							// モーション
	std::vector<std::vector<int> > motionTable_;	// アニメーションテーブル


	float elapsedTime_;				// 経過時間
	float nextActTime_;				// 次に行動を起こす時間

	float gravity_;						// 重力
	bool gravityF_;						// 重力処理をするか(死んで落ちていくときの処理用)

	bool isBack_;						// 後退中か
	bool isAttacking_;					// 攻撃中か

	float invincibleTime_;				// 無敵時間
	float invincibleAnim_;				// 無敵点滅アニメーション時間

	float sway_;							// 揺れ

	/*const*/ mymath::Vec3f startPos_;			// 初期座標(追跡後元に戻る場所)
	mymath::Vec3f endPos_;				// 攻撃時の移動終点座標
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
		@brief	帰還
		@return	なし
	*/
	//void ReturnStep();
	/*
		@brief	怯み
		@return	なし
	*/
	void DamageStep();
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
		@brief	行動タイプの決定
		@return	なし
	*/
	void DecideState();

	/*
		@brief	攻撃の発生処理
		@return	なし
	*/
	void CreateAttack();


	/*
		@brief	無敵中か取得
		@return	無敵中か
		@retval	true	無敵中
		@retval	false	無敵中でない
	*/
	bool isInvincible() const;

public:
	/*
		@brief	空オブジェクト生成
		DB初期化時に呼ばれる
	*/
	CGriffon();
	/*
		@brief	座標指定したオブジェクト生成
		@param	[in]	pos	座標
	*/
	CGriffon(const mymath::Vec3f& pos);
	/*
		@brief	座標指定したオブジェクト生成
		@param	[in]	x	X座標
		@param	[in]	y	Y座標
		@param	[in]	z	奥行き(デフォルト:0.5f)
	*/
	CGriffon(float x, float y, float z = 0.5f);
	~CGriffon();
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
		@brief	攻撃待機中フラグの取得
		@return	攻撃待機中フラグ
		@retval	true	攻撃待機中
		@retval	false	攻撃待機中でない
	*/
	bool isAttacking() const;


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