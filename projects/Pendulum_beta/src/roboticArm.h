#ifndef DEF_RoboticArm_H
#define DEF_RoboticArm_H

#ifndef DEF_ENEMYBASE_HPP
#include "enemyBase.hpp"
#endif

#ifndef DEF_ARMATTACK_H
#include "armAttack.h"
#endif

/*
	@brief	ロボットアームクラス
			obj_は固定アーム
*/
class CRoboticArm : public IEnemy
{
public:
	enum class ArmDirection
	{
		RIGHT_UP,		// 右向き
		RIGHT_DOWN,		// 右向き、上下反転
		LEFT_UP,		// 左向き
		LEFT_DOWN,		// 左向き、上下反転
	};

	struct LoadInfo
	{
		float attackRange;				// 攻撃範囲(現在座標からこの範囲にいると攻撃する)

		std::string childResname;		//固定アームの画像管理名

		mymath::Vec3f supportParent;	// 固定アームの支点(親側)(回転中心座標)
		mymath::Vec3f supportChild;		// 可動アームの支点(子側)(回転中心座標)

		mymath::Vec3i childSrcPos;		// 可動アームの画像抽出位置
		mymath::Vec3i childSrcSize;		// 可動アームの画像抽出サイズ

		float rotateSpeed;				// アームの回転速度
		float maxAngle;					// アームの回転の最大角度
		float minAngle;					// アームの回転の最小角度

		float attackInterval;			// 攻撃間隔
	};
	

private:
	enum class State
	{
		WAIT,				// 待機
		ATTACK,				// 攻撃
		DESTROY,			// 死亡アニメ

	};

	enum class MotionType
	{
		MOVE,				// 待機・移動
		ATTACK,				// 攻撃
		DESTROY,			// 死亡
	};

	struct InvincibleInfo
	{
		float time;			// 点滅アニメーション用
		bool isOn;			// on / off
		int cnt;			// 点滅オンオフカウント
	};


private:
	static void (CRoboticArm::*StateStep_[])();	// 状態によるstep処理

	LoadInfo loadInfo_;

	charabase::Anim motionAnim_;					// アニメーション用
	MotionType motionType_;							// モーション
	std::vector<std::vector<int> > motionTable_;	// アニメーションテーブル

	State state_;							// 行動状態

	InvincibleInfo invincible_;				// 点滅

	float elapsedTime_;						// 経過時間
	float nextActTime_;						// 次に行動を起こす時間

	bool  turnParentFlag_;					// 可動アームの反転
	float childAngle_;						// 可動アームの角度
	float rotateDir_;						// 回転する方向
	ArmDirection armDir_;					// アームの向き

	charabase::CharBase armAtkObj_;			// アーム攻撃の情報

	mymath::Vec3f startPos_;				// 初期座標(追跡後元に戻る場所)

private:

	/*
		@brief	情報の初期化
		@param	[in]	pos		初期座標
		@param	[in]	dir		アームの向き
		@return	なし
	*/
	void init(const mymath::Vec3f& pos, int dir);

	/*
		@brief	アームの向きの切り替え
		@return	なし
	*/
	void SwichArmDir();
	/*
		@brief	待機
		@return	なし
	*/
	void WaitStep();
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
	CRoboticArm();

	/*
		@brief	座標指定したオブジェクト生成
		@param	[in]	pos	座標
		@param	[in]	dir	アームの向き(デフォルト:RIGHT_UP)
	*/
	CRoboticArm(const mymath::Vec3f& pos, int dir = static_cast<int>(ArmDirection::RIGHT_UP));
	
	/*
		@brief	座標指定したオブジェクト生成
		@param	[in]	x	X座標
		@param	[in]	y	Y座標
		@param	[in]	dir	アームの向き(デフォルト:RIGHT_UP)
		@param	[in]	z	奥行き(デフォルト:0.5f)
	*/
	CRoboticArm(float x, float y, int dir = static_cast<int>(ArmDirection::RIGHT_UP), float z = 0.5f);
	~CRoboticArm();
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

	/*
		@brief	アーム攻撃情報の保持
		@param	[in]	atk		アーム攻撃の情報
		@param	[in]	force	アームの攻撃力
		@return	なし
	*/
	void SetArmAtkInfo(const charabase::CharBase& atk, const int force);

	/*
		@brief	アームの向きの変更
		@param	[in]	dir		アームの向き
		@return	なし
	*/
	void SetArmDirection(ArmDirection dir);

	/*
		@brief	アームの向きの変更
		@param	[in]	dir		アームの向き
		@return	なし
	*/
	void SetArmDirection(const int dir);
};

#endif