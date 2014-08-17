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
	//charabase::CharBase arm_;	// アーム情報
	float angle_;				// アームの角度
	float rotateSpeed_;			// アームの回転速度
	float maxAngle_;			// アームの回転の最大角度
	mymath::Vec3f center_;		// アームの回転中心座標
	mymath::Vec3f length_;		// 回転の中心からオブジェクトまでの長さ


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
		@brief	初期化
		@param	[in]	arm				アームの情報
		@param	[in]	nowAngle		現在の角度
		@param	[in]	length			回転中心座標からアームの中心までの長さ
		@param	[in]	center			回転中心座標
		@param	[in]	rotateSpeed		回転速度
		@param	[in]	maxAngle		回転できる最大値
	*/
	CArmAttack(const CArmAttack& arm, const float nowAngle,
		const mymath::Vec3f& length, const mymath::Vec3f& center, float rotateSpeed, float maxAngle);

	virtual void step() override;
	virtual void draw() override;
	virtual void hit(const ObjPtr& rival) override;
	
	/*
		@
	*/
	void setForce(int force);
};



#endif