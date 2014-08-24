#ifndef DEF_ARMATTACK_H
#define DEF_ARMATTACK_H

#ifndef DEF_ATTACKBASE_HPP
#include "attackBase.hpp"
#endif

#ifndef DEF_MYMATH_HPP
#include "MyMath.hpp"
#endif

/*
	@brief	アーム攻撃
*/
class CArmAttack : public IAttack
{
private:
	enum class MotionType
	{
		WAIT,			// 待機
		ATTACK,			// 攻撃
	};

private:
	float angle_;				// アームの角度
	mymath::Vec3f center_;		// アームの回転中心座標
	float length_;				// 回転の中心からオブジェクトまでの長さ
	MotionType motionType_;		// アニメーションの状態


private:
	void init();

public:
	/*
		@brief	初期化
		@param	[in]	cb		アームのオブジェクト
	*/
	CArmAttack(const charabase::CharBase& cb);

	/*
	@brief	初期化
	@param	[in]	force		アームの攻撃力
	*/
	CArmAttack(const int force);

	/*
		@btief	可動アームの角度などの設定
		@param	[in]	angle			アームの角度
		@param	[in]	length			回転中心座標からアームの中心までの長さ
		@param	[in]	center			回転中心座標
	*/
	void SetArmInfo(const float angle, const float length, const mymath::Vec3f& center);

	/*
		@brief	処理
		@param	[in]	angle	現在の角度
	*/
	void step(float angle);

	virtual void step() override;
	virtual void draw() override;
	virtual void hit(const ObjPtr& rival) override;
};



#endif