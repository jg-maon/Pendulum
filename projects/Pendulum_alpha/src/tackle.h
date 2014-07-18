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
private:
	//int cnt_;
	mymath::Vec3f	acceleration_;	// 加速度
	mymath::Vec3f	endPos_;		// 移動終点座標

private:
	void init();
public:
	/*
		@brief	初期化
		@param	[in]	tackle			タックルの情報
		@param	[in]	acceleration	加速度
	*/
	CTackle(const mymath::Vec3f& acceleration = mymath::Vec3f(), const mymath::Vec3f& endPos = mymath::Vec3f());

	/*
		@brief	初期化
		@param	[in]	tackle			タックルの情報
	*/
	CTackle(const charabase::CharBase& tackle, const mymath::Vec3f& acceleration, const mymath::Vec3f& endPos);
	
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
		@brief	タックル
				どの位置から どれくらいの距離を どの角度に どれくらいの速さで 加速度はどれくらいで
		@param	[in]	pos			初期座標
		@param	[in]	dist		距離
		@param	[in]	angle		発射角度(単位:Degree)
		@param	[in]	speed		速さ(初速度)
		@param	[in]	acc			加速度
		@return	なし
	*/
	void CreateAttack(const mymath::Vec3f& pos, float dist, float angle, float speed, float acc);

};

#endif