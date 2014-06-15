#ifndef DEF_FAIRY_H
#define DEF_FAIRY_H

#ifndef DEF_ENEMYBASE_HPP
#include "enemyBase.hpp"
#endif

class CFairy : public IEnemy
{
public:
	struct LoadInfo
	{
		float SEARCH_RANGE;		// 索敵範囲(現在座標からどれだけ策敵するか)
		float CHASE_RANGE;		// 追跡範囲(初期座標からどれだけ追跡するか)
		float ATTACK_RANGE;		// 攻撃範囲(現在座標からこの範囲にいると攻撃する)
		float RETURN_RANGE;		// 帰還時最小範囲(この範囲内なら初期位置に戻ったとみなす)
		float MOVE_SPEED;		// 移動速度
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

private:
	/*static const float SEARCH_RANGE;		// 索敵範囲(現在座標からどれだけ策敵するか)
	static const float CHASE_RANGE;			// 追跡範囲(初期座標からどれだけ追跡するか)
	static const float ATTACK_RANGE;		// 攻撃範囲(現在座標からこの範囲にいると攻撃する)
	static const float RETURN_RANGE;		// 帰還時最小範囲(この範囲内なら初期位置に戻ったとみなす)
	static const float MOVE_SPEED;			// 移動速度
	//*/
	static void (CFairy::*StateStep_[])();	// 状態によるstep処理

	LoadInfo loadInfo_;

	charabase::Anim anim_;			// アニメーション

	State state_;					// 行動状態

	float elapsedTime_;				// 経過時間
	float nextActTime_;				// 次に行動を起こす時間

	/*const*/ mymath::Vec3f startPos_;			// 初期座標(追跡後元に戻る場所)
public:
	/*CFairy& operator=(const CFairy& obj)
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
	@return	なし
	*/
	void init();

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
	CFairy();
	/*
	@brief	座標指定したオブジェクト生成
	@param	[in]	pos	座標
	*/
	CFairy(const mymath::Vec3f& pos);
	/*
	@brief	座標指定したオブジェクト生成
	@param	[in]	x	X座標
	@param	[in]	y	Y座標
	@param	[in]	z	奥行き(デフォルト:0.5f)
	*/
	CFairy(float x, float y, float z = 0.5f);
	~CFairy();
	virtual void step() override;
	virtual void draw() override;

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
	virtual Collisions GetCollisionAreas() override;


	/*
	@brief	情報初期化
	@param	[in]	info	ロードしてきた情報一覧
	@return	なし
	*/
	void SetInfo(const LoadInfo& info);
};

#endif