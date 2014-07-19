#ifndef DEF_ACTIONPOINT_H
#define DEF_ACTIONPOINT_H

#ifndef DEF_OBJ_H
#include "Obj.h"
#endif

#ifndef DEF_MYMATH_HPP
#include "MyMath.hpp"
#endif

#include <vector>	// Polygon用

#pragma region class IActionPoint
class IActionPoint : public IObject
{
public:
	IActionPoint(const std::string& name, float x=0.f, float y=0.f);
	virtual ~IActionPoint() = 0;
	virtual void step() = 0;
	virtual void draw() = 0;
	/*
		@brief	アクションポイントの取得
		@return	アクションポイント
	*/
	virtual mymath::ShapefPtr GetAP() = 0;
	//================================================================================
#pragma region Contains
	/*
		@brief	点の内包判定
		@param	[in]	point	判定する点
		@return	内包しているか
		@retval true	している
		@retval false	していない
	*/
	virtual bool Contains(const mymath::Vec3f& point) const;
	/*
		@brief			線分の交差、内包判定
		@attension		線分の両端が境界線の中にある場合は第3引数により変わる
						完全内包と境界線交差は内包が優先される
		@param	[in]	line(sta,end)	判定する線分(始点、終点)
		@param	[in]	fullContain		完全内包とするか(デフォルト：false)
		@param	[in]	intersectOnly	境界線との交差のみにするか(デフォルト：true)
		@return			交差、内包しているか
		@retval	true	している
		@retval	false	していない
	*/
	virtual bool Contains(const mymath::Linef& line, bool fullContain = false, bool intersectOnly = true) const;
	/*
		@brief			線分の交差、内包判定
		@attension		線分の両端が境界線の中にある場合は第4引数により変わる
						完全内包と境界線交差は内包が優先される
		@param	[in]	sta	判定する線分の始点
		@param	[in]	end	判定する線分の終点
		@param	[in]	fullContain		完全内包とするか(デフォルト：false)
		@param	[in]	intersectOnly	境界線との交差のみにするか(デフォルト：true)
		@return			交差、内包しているか
		@retval	true	している
		@retval	false	していない
	*/
	virtual bool Contains(const mymath::Vec3f& sta, const mymath::Vec3f& end, bool fullContain = false, bool intersectOnly = true) const;
	
	/*
		@brief			図形の交差、内包判定
		@attension		線分の両端が境界線の中にある場合は第3引数により変わる
		完全内包と境界線交差は内包が優先される
		@param	[in]	shape			判定する図形
		@param	[in]	fullContain		完全内包とするか(デフォルト：false)
		@param	[in]	intersectOnly	境界線との交差のみにするか(デフォルト：true)
		@return			交差、内包しているか
		@retval	true	している
		@retval	false	していない
	*/
	virtual bool Contains(const mymath::ShapefPtr& shape, bool fullContain = false, bool intersectOnly = true) const;

#pragma endregion // Contains
	//================================================================================

	//================================================================================
#pragma region Intersection
	/*
		@brief	内包している点から一番近い交点の取得
			if(Contains(p))
				point = IntersectionPoint2(p);
		@attention	交差していることが前提
		@param	[in]	point	内包している点
		@return	交点
	*/
	virtual mymath::Vec3f IntersectionPoint2(const mymath::Vec3f& point) const;

	/*
		@brief	線分との交点の取得
			if(Contains(line))
				points = IntersectionPoint2(line);
		@attention	交差していることが前提
		@param	[in]	line(sta,end)	比較線分
		@return	全交点
	*/
	virtual std::vector<mymath::Vec3f> IntersectionPoint2(const mymath::Linef& line) const;
	/*
		@brief	線分との交点の取得
			if(Contains(pos,next))
				points = IntersectionPoint2(pos,next);
		@attention	交差していることが前提
		@param	[in]	sta	比較線分の始点
		@param	[in]	end	比較線分の終点
		@return	全交点
	*/
	virtual std::vector<mymath::Vec3f> IntersectionPoint2(const mymath::Vec3f& sta, const mymath::Vec3f& end) const;

	/*
		@brief	図形との交点の取得
			if(Contains(shape))
				points = IntersectionPoint2(shape);
		@attention	交差していることが前提
		@param	[in]	shape	判定する図形
		@return	全交点
	*/
	virtual std::vector<mymath::Vec3f> IntersectionPoint2(const mymath::ShapefPtr& shape) const;


	/*
		@brief	線分との交点のうち、始点に近い点を取得
			if(Contains(line))
				point = IntersectionPoint2Nearest(line);
		@attention	交差していることが前提
		@param	[in]	line(sta,end)	比較線分
		@return	始点に近い交点
	*/
	virtual mymath::Vec3f IntersectionPoint2Nearest(const mymath::Linef& line) const;
	/*
		@brief	線分との交点のうち、始点に近い点を取得
			if(Contains(pos,next))
				point = IntersectionPoint2Nearest(pos,next);
		@attention	交差していることが前提
		@param	[in]	sta	比較線分の始点
		@param	[in]	end	比較線分の終点
		@return	始点に近い交点
	*/
	virtual mymath::Vec3f IntersectionPoint2Nearest(const mymath::Vec3f& sta, const mymath::Vec3f& end) const;

#pragma endregion // Intersection
	//================================================================================

	
};
typedef std::shared_ptr<IActionPoint> ActPtPtr;

#pragma endregion  // class IActionPoint

#pragma region class CActionCircle
class CActionCircle : public IActionPoint
{
	std::shared_ptr<mymath::Circlef> circle_;
public:
	CActionCircle(float x, float y, float r);
	virtual void step() override;
	virtual void draw() override;
	/*
		@brief	アクションポイントの取得
		@return	アクションポイント
	*/
	virtual mymath::ShapefPtr GetAP() override;
	//================================================================================
#pragma region Contains
	/*
		@brief	点の内包判定
		@param	[in]	point	判定する点
		@return	内包しているか
		@retval true	している
		@retval false	していない
	*/
	virtual bool Contains(const mymath::Vec3f& point) const override;
	/*
		@brief			線分の交差、内包判定
		@attension		線分の両端が境界線の中にある場合は第3引数により変わる
						完全内包と境界線交差は内包が優先される
		@param	[in]	line(sta,end)	判定する線分(始点、終点)
		@param	[in]	fullContain		完全内包とするか(デフォルト：false)
		@param	[in]	intersectOnly	境界線との交差のみにするか(デフォルト：true)
		@return			交差、内包しているか
		@retval	true	している
		@retval	false	していない
	*/
	virtual bool Contains(const mymath::Linef& line, bool fullContain = false, bool intersectOnly = true) const override;
	/*
		@brief			線分の交差、内包判定
		@attension		線分の両端が境界線の中にある場合は第4引数により変わる
						完全内包と境界線交差は内包が優先される
		@param	[in]	sta	判定する線分の始点
		@param	[in]	end	判定する線分の終点
		@param	[in]	fullContain		完全内包とするか(デフォルト：false)
		@param	[in]	intersectOnly	境界線との交差のみにするか(デフォルト：true)
		@return			交差、内包しているか
		@retval	true	している
		@retval	false	していない
	*/
	virtual bool Contains(const mymath::Vec3f& sta, const mymath::Vec3f& end, bool fullContain = false, bool intersectOnly = true) const override;
	
	/*
		@brief			図形の交差、内包判定
		@attension		線分の両端が境界線の中にある場合は第3引数により変わる
		完全内包と境界線交差は内包が優先される
		@param	[in]	shape			判定する図形
		@param	[in]	fullContain		完全内包とするか(デフォルト：false)
		@param	[in]	intersectOnly	境界線との交差のみにするか(デフォルト：true)
		@return			交差、内包しているか
		@retval	true	している
		@retval	false	していない
	*/
	virtual bool Contains(const mymath::ShapefPtr& shape, bool fullContain = false, bool intersectOnly = true) const override;
#pragma endregion	// Contains

	//================================================================================


	//================================================================================
#pragma region Intersection
	/*
		@brief	内包している点から一番近い交点の取得
			if(Contains(p))
				point = IntersectionPoint2(p);
		@attention	交差していることが前提
		@param	[in]	point	内包している点
		@return	交点
	*/
	virtual mymath::Vec3f IntersectionPoint2(const mymath::Vec3f& point) const override;

	/*
		@brief	線分との交点の取得
			if(Contains(line))
				points = IntersectionPoint2(line);
		@attention	交差していることが前提
		@param	[in]	line(sta,end)	比較線分
		@return	全交点
	*/
	virtual std::vector<mymath::Vec3f> IntersectionPoint2(const mymath::Linef& line) const override;
	/*
		@brief	線分との交点の取得
			if(Contains(pos,next))
				points = IntersectionPoint2(pos,next);
		@attention	交差していることが前提
		@param	[in]	sta	比較線分の始点
		@param	[in]	end	比較線分の終点
		@return	全交点
	*/
	virtual std::vector<mymath::Vec3f> IntersectionPoint2(const mymath::Vec3f& sta, const mymath::Vec3f& end) const override;

	/*
		@brief	図形との交点の取得
			if(Contains(shape))
				points = IntersectionPoint2(shape);
		@attention	交差していることが前提
		@param	[in]	shape	判定する図形
		@return	全交点
	*/
	virtual std::vector<mymath::Vec3f> IntersectionPoint2(const mymath::ShapefPtr& shape) const override;

	/*
		@brief	線分との交点のうち、始点に近い点を取得
			if(Contains(line))
				point = IntersectionPoint2Nearest(line);
		@attention	交差していることが前提
		@param	[in]	line(sta,end)	比較線分
		@return	始点に近い交点
	*/
	virtual mymath::Vec3f IntersectionPoint2Nearest(const mymath::Linef& line) const override;
	/*
		@brief	線分との交点のうち、始点に近い点を取得
			if(Contains(pos,next))
				point = IntersectionPoint2Nearest(pos,next);
		@attention	交差していることが前提
		@param	[in]	sta	比較線分の始点
		@param	[in]	end	比較線分の終点
		@return	始点に近い交点
	*/
	virtual mymath::Vec3f IntersectionPoint2Nearest(const mymath::Vec3f& sta, const mymath::Vec3f& end) const override;


#pragma endregion // Intersection
	//================================================================================

};

#pragma endregion // class CActionCircle

#pragma region class CActionPolygon
class CActionPolygon : public IActionPoint
{
private:
	std::shared_ptr<mymath::Polyf> polygon_;
public:
	const std::vector<mymath::Vec3f>& vertexes;
private:
public:
	CActionPolygon(const std::vector<mymath::Vec3f>& points);
	CActionPolygon(const mymath::Polyf& polygon);
	virtual void step() override;
	virtual void draw() override;

	/*
		@brief	アクションポイントの取得
		@return	アクションポイント
	*/
	virtual mymath::ShapefPtr GetAP() override;

	/*
		@brief	vertexes_の情報を元にポリゴン(閉路)を成形
		@return	ポリゴンの枠
	*/
	std::vector<mymath::Linef> MakeLines() const;
	
	//================================================================================
#pragma region Contains
	/*
		@brief	点の内包判定
		@param	[in]	point	判定する点
		@return	内包しているか
		@retval true	している
		@retval false	していない
	*/
	virtual bool Contains(const mymath::Vec3f& point) const override;
	/*
		@brief			線分の交差、内包判定
		@attension		線分の両端が境界線の中にある場合は第3引数により変わる
						完全内包と境界線交差は内包が優先される
		@param	[in]	line(sta,end)	判定する線分(始点、終点)
		@param	[in]	fullContain		完全内包とするか(デフォルト：false)
		@param	[in]	intersectOnly	境界線との交差のみにするか(デフォルト：true)
		@return			交差、内包しているか
		@retval	true	している
		@retval	false	していない
	*/
	virtual bool Contains(const mymath::Linef& line, bool fullContain = false, bool intersectOnly = true) const override;
	/*
		@brief			線分の交差、内包判定
		@attension		線分の両端が境界線の中にある場合は第4引数により変わる
						完全内包と境界線交差は内包が優先される
		@param	[in]	sta	判定する線分の始点
		@param	[in]	end	判定する線分の終点
		@param	[in]	fullContain		完全内包とするか(デフォルト：false)
		@param	[in]	intersectOnly	境界線との交差のみにするか(デフォルト：true)
		@return			交差、内包しているか
		@retval	true	している
		@retval	false	していない
	*/
	virtual bool Contains(const mymath::Vec3f& sta, const mymath::Vec3f& end, bool fullContain = false, bool intersectOnly = true) const override;
	
	/*
		@brief			図形の交差、内包判定
		@attension		線分の両端が境界線の中にある場合は第3引数により変わる
		完全内包と境界線交差は内包が優先される
		@param	[in]	shape			判定する図形
		@param	[in]	fullContain		完全内包とするか(デフォルト：false)
		@param	[in]	intersectOnly	境界線との交差のみにするか(デフォルト：true)
		@return			交差、内包しているか
		@retval	true	している
		@retval	false	していない
	*/
	virtual bool Contains(const mymath::ShapefPtr& shape, bool fullContain = false, bool intersectOnly = true) const override;
#pragma endregion	// Contains

	//================================================================================
	//================================================================================
#pragma region Intersection
	/*
		@brief	内包している点から一番近い交点の取得
			if(Contains(p))
				point = IntersectionPoint2(p);
		@attention	交差していることが前提
		@param	[in]	point	内包している点
		@return	交点
	*/
	virtual mymath::Vec3f IntersectionPoint2(const mymath::Vec3f& point) const override;

	/*
		@brief	線分との交点の取得
			if(Contains(line))
				points = IntersectionPoint2(line);
		@attention	交差していることが前提
		@param	[in]	line(sta,end)	比較線分
		@return	全交点
	*/
	virtual std::vector<mymath::Vec3f> IntersectionPoint2(const mymath::Linef& line) const override;
	/*
		@brief	線分との交点の取得
			if(Contains(pos,next))
				points = IntersectionPoint2(pos,next);
		@attention	交差していることが前提
		@param	[in]	sta	比較線分の始点
		@param	[in]	end	比較線分の終点
		@return	全交点
	*/
	virtual std::vector<mymath::Vec3f> IntersectionPoint2(const mymath::Vec3f& sta, const mymath::Vec3f& end) const override;

	/*
		@brief	図形との交点の取得
			if(Contains(shape))
				points = IntersectionPoint2(shape);
		@attention	交差していることが前提
		@param	[in]	shape	判定する図形
		@return	全交点
	*/
	virtual std::vector<mymath::Vec3f> IntersectionPoint2(const mymath::ShapefPtr& shape) const override;

	/*
		@brief	線分との交点のうち、始点に近い点を取得
			if(Contains(line))
				point = IntersectionPoint2Nearest(line);
		@attention	交差していることが前提
		@param	[in]	line(sta,end)	比較線分
		@return	始点に近い交点
	*/
	virtual mymath::Vec3f IntersectionPoint2Nearest(const mymath::Linef& line) const override;
	/*
		@brief	線分との交点のうち、始点に近い点を取得
			if(Contains(pos,next))
				point = IntersectionPoint2Nearest(pos,next);
		@attention	交差していることが前提
		@param	[in]	sta	比較線分の始点
		@param	[in]	end	比較線分の終点
		@return	始点に近い交点
	*/
	virtual mymath::Vec3f IntersectionPoint2Nearest(const mymath::Vec3f& sta, const mymath::Vec3f& end) const override;


#pragma endregion // Intersection
	//================================================================================

};

#pragma endregion // class CActionPolygon


#endif