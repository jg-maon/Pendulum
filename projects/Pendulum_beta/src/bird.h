#ifndef DEF_BIRD_H
#define DEF_BIRD_H

#ifndef DEF_ENEMYBASE_HPP
#include "enemyBase.hpp"
#endif

class CBird : public IEnemy
{
public:
	struct LoadInfo
	{
		float searchRange;		// 索敵範囲(現在座標からどれだけ策敵するか)
		float chaseRange;		// 追跡範囲(初期座標からどれだけ追跡するか)
		float attackRange;		// 攻撃範囲(現在座標からこの範囲にいると攻撃する)
		float returnRange;		// 帰還時最小範囲(この範囲内なら初期位置に戻ったとみなす)
		float moveSpeed;		// 移動速度

		float attackInterval;		// 攻撃間隔
	};
private:
	enum class State
	{
		WAIT,				// 待機
		CHASE,				// 追跡
		RETURN,				// 帰還
		ATTACK,				// 攻撃
		DESTROY,			// 死亡アニメ

	};

	enum class MotionType
	{
		MOVE,				// 待機・移動
		ATTACK,				// 攻撃
	};

	struct InvincibleInfo
	{
		float time;		// 点滅アニメーション用
		bool isOn;		// on / off
		int cnt;		// 点滅オンオフカウント
	};

private:
	/*static const float searchRange;		// 索敵範囲(現在座標からどれだけ策敵するか)
	static const float chaseRange;			// 追跡範囲(初期座標からどれだけ追跡するか)
	static const float attackRange;		// 攻撃範囲(現在座標からこの範囲にいると攻撃する)
	static const float returnRange;		// 帰還時最小範囲(この範囲内なら初期位置に戻ったとみなす)
	static const float moveSpeed;			// 移動速度
	//*/
	static void (CBird::*StateStep_[])();	// 状態によるstep処理

	LoadInfo loadInfo_;

	charabase::Anim motionAnim_;					// アニメーション用
	MotionType motionType_;							// モーション
	std::vector<std::vector<int> > motionTable_;	// アニメーションテーブル

	State state_;					// 行動状態

	InvincibleInfo invincible_;		// 点滅

	float elapsedTime_;				// 経過時間
	float nextActTime_;				// 次に行動を起こす時間
		
	/*const*/ mymath::Vec3f startPos_;			// 初期座標(追跡後元に戻る場所)
public:
	/*CBird& operator=(const CBird& obj)
	{
		if (this != &obj) return *this;
		loadInfo_ = obj.loadInfo_;
		state_ = obj.state_;
		elapsedTime_ = obj.elapsedTime_;
		nextActTime_ = obj.nextActTime_;
	}
	//*/
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
	void ReturnStep();
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
	
public:
	/*
		@brief	空オブジェクト生成
				DB初期化時に呼ばれる
	*/
	CBird();
	/*
		@brief	座標指定したオブジェクト生成
		@param	[in]	pos	座標
	*/
	CBird(const mymath::Vec3f& pos);
	/*
		@brief	座標指定したオブジェクト生成
		@param	[in]	x	X座標
		@param	[in]	y	Y座標
		@param	[in]	z	奥行き(デフォルト:0.5f)
	*/
	CBird(float x, float y, float z = 0.5f);
	~CBird();
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