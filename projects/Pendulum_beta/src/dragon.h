#ifndef DEF_DRAGON_H
#define DEF_DRAGON_H

#ifndef DEF_ENEMYBASE_HPP
#include "enemyBase.hpp"
#endif

class CDragon : public IEnemy
{
public:
	struct LoadInfo
	{
		float searchRange;			// 索敵範囲(現在座標からどれだけ策敵するか)
		float chaseRange;			// 追跡範囲(初期座標からどれだけ追跡するか)
		float attackRange;			// 攻撃範囲(現在座標からこの範囲にいると攻撃する)

		float moveSpeed;			// 移動速度
		float attackSpeed;			// 攻撃時移動速度

		float invincibleTime;		// 無敵時間

		float backDist;				// バック距離
		float backTime;				// バック距離に到達するまでの時間
		float damageTime;			// 怯み時間
		float attackDist;			// 攻撃距離
		float attackInterval;		// 攻撃間隔
		float swayRange;			// 揺れる範囲
		float fallTime;				// 落ちだす時間
		float fallSpeed;			// 落ちる速度
		int fallTurnSpeed;			// 落下中に回転させるためのスピード

		float entryWidth;			// init の pos を終着点としてそこから離れる 幅
		float entryHeight;			// init の pos を終着点としてそこから離れる 高さ

		int health;					// 初期HP
		int power;					// 初期攻撃力

		float roarAnimSpeed;		// 咆哮アニメーションのスピード
		float moveAnimSpeed;		// 飛行アニメーションのスピード
		float backAnimSpeed;		// 後退アニメーションのスピード
	};
private:


	enum class BattleState
	{
		ENTRY,				// 登場
		WAIT,				// 待機
		CHASE,				// 追跡
		DAMAGE,				// ダメージ(怯み)
		ATTACK,				// 攻撃
		DESTROY,			// 死亡アニメ
	};

	enum class MotionType
	{
		MOVE,				// 待機・移動
		ATTACK,				// 攻撃
		ROAR,				// 咆哮
		FALL,				// 落下
	};
private:

	static void (CDragon::*StateStep_[])();	// 状態によるstep処理

	LoadInfo loadInfo_;

	BattleState battleState_;					// 行動状態

	charabase::Anim motionAnim_;					// アニメーション
	MotionType motionType_;							// モーション
	std::vector<std::vector<int> > motionTable_;	// アニメーションテーブル


	float elapsedTime_;					// 経過時間
	float nextActTime_;					// 次に行動を起こす時間

	bool isBacking_;						// 後退中か
	bool isAttacking_;					// 攻撃中か

	float invincibleTime_;				// 無敵時間
	float invincibleAnim_;				// 無敵点滅アニメーション時間

	float sway_;						// 揺れ

	bool isRoaring_;						// 咆哮中 (true)

	int fallTurnCount_;				// 落下中に回転させるためのカウンタ

	mymath::Vec3f startPos_;			// 初期座標(追跡後元に戻る場所)

	mymath::Vec3f endPos_;				// 攻撃時の移動終点座標
	mymath::Vec3f backPos_;				// 攻撃時のバック終点座標
	mymath::Vec3f actPos_;				// 攻撃始動位置
public:

private:

	/*
		@brief	情報の初期化
		@param	[in]	pos	初期座標
		@return	なし
	*/
	void init(const mymath::Vec3f& pos);

	/*
		@brief	登場
		@return	なし
	*/
	void EntryStep();
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
	CDragon();
	/*
		@brief	座標指定したオブジェクト生成
		@param	[in]	pos	座標
	*/
	CDragon(const mymath::Vec3f& pos);
	/*
		@brief	座標指定したオブジェクト生成
		@param	[in]	x	X座標
		@param	[in]	y	Y座標
		@param	[in]	z	奥行き(デフォルト:0.7f)
	*/
	CDragon(float x, float y, float z = 0.7f);
	~CDragon();
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
		@brief	攻撃中フラグの取得
		@return	攻撃中フラグ
		@retval	true	攻撃中
		@retval	false	攻撃中でない
	*/
	bool isAttacking() const;

	/*
		@brief	攻撃準備中フラグの取得
		@return	攻撃準備中フラグ
		@retval	true	攻撃準備中
		@retval	false	攻撃準備中でない
	*/
	bool isBacking() const;

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