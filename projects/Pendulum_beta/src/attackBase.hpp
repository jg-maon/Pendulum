#ifndef DEF_ATTACKBASE_HPP
#define DEF_ATTACKBASE_HPP

#ifndef DEF_OBJ_H
#include "Obj.h"
#endif

#ifndef DEF_MYMAYH_HPP
#include "MyMath.hpp"
#endif


/*
	@brief		攻撃基底クラス
	@attention	名前は必ず"Atk_"で始める
*/
class IAttack : public IColObject
{
protected:
	int force_;			// 攻撃力、回復力など

protected:
public:
	IAttack(const std::string& name):
		IColObject(name),force_(0)
	{}
	virtual ~IAttack() = 0{}
	virtual void step() = 0{ __super::step(); }
	virtual void draw() = 0{}
	
	/*
		@brief	威力の取得
		@return	攻撃力、回復力など
	*/
	virtual int GetForce() const{return force_;}

	/*
		@brief	威力の設定(敵側から設定する場合)
		@param	[in]	force	威力(攻撃力、回復力など)
		@return	なし
	*/
	void setForce(int force){ force_ = force; }

};
typedef std::shared_ptr<IAttack> AttackPtr;

#endif