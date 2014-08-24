#ifndef DEF_TACKLE_H
#define DEF_TACKLE_H

#ifndef DEF_ATTACKBASE_HPP
#include "attackBase.hpp"
#endif

#ifndef DEF_MYMATH_HPP
#include "MyMath.hpp"
#endif

/*
	@brief	タックル
*/
class CTackle : public IAttack
{
public:
	/*
		@brief	初期化
	*/
	CTackle();

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
	
};

#endif