#ifndef DEF_CHARAOBJBASE_HPP
#define DEF_CHARAOBJBASE_HPP

#ifndef DEF_OBJ_H
#include "Obj.h"
#endif

/*
	@brief	キャラクターオブジェクト基底クラス(プレイヤー、敵、NPC?)
	@attension	step内でICharacter::stepを呼び出す
*/
class ICharacter : public IColObject
{

protected:
	int health_;				// 体力
	int power_;					// 攻撃力

	mymath::Vec3f prePos_;				// 1フレーム前の座標
public:
	const mymath::Vec3f& prePos;		// 1フレーム前座標
public:
	ICharacter(const std::string& name):
		IColObject(name)
		,health_(1)
		,power_(0)
		,prePos_(obj_.pos)
		,prePos(prePos_)
	{}
	~ICharacter() = 0{}
	/*
		@brief		派生後共通処理
					1フレーム前の座標に現在座標を格納
		@attension	派生後のstep内で呼び出すこと
		@return		なし
	*/
	virtual void step() = 0{prePos_ = obj_.pos;}
	virtual void draw() = 0{}
	virtual void hit(const ObjPtr& rival){__super::hit(rival);}
	/*
		@brief	当たり判定領域の取得
		@return	当たり判定領域
	*/
	virtual Collisions GetCollisionAreas(){return __super::GetCollisionAreas();}

	/*
		@brief	攻撃力の取得
		@return	補正後の攻撃力
	*/
	virtual int getPower() const {return power_;}
};

#endif