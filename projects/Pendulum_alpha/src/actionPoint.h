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
	virtual ~IActionPoint() = 0 {}
	virtual void step() override {}
	virtual void draw() override {}
	/*
		@brief	点と円の衝突検知 + 衝突時の一番近い円周上の点の取得
			if(Contains(out, point))
				point = out;
		@param	[out]	out		一番近い円周上の点
		@param	[in]	point	衝突しているか検知する点
		@return	衝突しているか
		@retval	true	衝突している
		@retval	false	衝突していない
	*/
	virtual bool Contains(mymath::Vec3f& out, const mymath::Vec3f& point)const{return false;}
	/*
		@brief	線分と円の衝突検知 + 衝突時始点に近い方の交点の取得
			if(Contains(out, pos, next))
				next = out;
		@param	[out]	out				始点に近い方の交点
		@param	[in]	line(sta,end)	衝突検知線分
		@return	衝突しているか
		@retval	true	衝突している
		@retval	false	衝突していない
	*/
	virtual bool Contains(mymath::Vec3f& out, const mymath::Linef& line)const{return false;}
	/*
		@brief	線分と円の衝突検知 + 衝突時始点に近い方の交点の取得
			if(Contains(out, pos, next))
				next = out;
		@param	[out]	out				始点に近い方の交点
		@param	[in]	line(sta,end)	衝突検知線分
		@return	衝突しているか
		@retval	true	衝突している
		@retval	false	衝突していない
	*/
	virtual bool Contains(mymath::Vec3f& out, const mymath::Vec3f& sta, const mymath::Vec3f& end)const{return false;}
	
	/*
		@brief	線分との衝突検知 + 衝突時の交点の取得
			if(Contains(out, line))
				next = out;
		@param	[out]	out		交点
		@param	[in]	line	衝突検知線分
		@return	衝突しているか
		@retval	true	衝突している
		@retval	false	衝突していない
	*/
	virtual bool Contains(std::vector<mymath::Vec3f>& out, const mymath::Linef& line)const{return false;}
	/*
		@brief	線分との衝突検知 + 衝突時の交点の取得
			if(Contains(out, pos, next))
				next = out;
		@param	[out]	out		交点
		@param	[in]	sta,end	衝突検知線分
		@return	衝突しているか
		@retval	true	衝突している
		@retval	false	衝突していない
	*/
	virtual bool Contains(std::vector<mymath::Vec3f>& out, const mymath::Vec3f& sta, const mymath::Vec3f& end)const{return false;}

	virtual bool Contains(const mymath::ShapefPtr& shape) const { return false; }
	
};
typedef std::shared_ptr<IActionPoint> ActPtPtr;

#pragma endregion  // class IActionPoint

#pragma region class CActionCircle
class CActionCircle : public IActionPoint
{
	mymath::Circlef circle_;
public:
	CActionCircle(float x, float y, float r);
	virtual void step() override;
	virtual void draw() override;
	/*
		@brief	点と円の衝突検知 + 衝突時の一番近い円周上の点の取得
			if(Contains(out, point))
				point = out;
		@param	[out]	out		一番近い円周上の点
		@param	[in]	point	衝突しているか検知する点
		@return	衝突しているか
		@retval	true	衝突している
		@retval	false	衝突していない
	*/
	bool Contains(mymath::Vec3f& out, const mymath::Vec3f& point) const override;
	/*
		@brief	線分と円の衝突検知 + 衝突時始点に近い方の交点の取得
			if(Contains(out, pos, next))
				next = out;
		@param	[out]	out				始点に近い方の交点
		@param	[in]	line(sta,end)	衝突検知線分
		@return	衝突しているか
		@retval	true	衝突している
		@retval	false	衝突していない
	*/
	bool Contains(mymath::Vec3f& out, const mymath::Linef& line) const override;
	/*
		@brief	線分と円の衝突検知 + 衝突時始点に近い方の交点の取得
			if(Contains(out, pos, next))
				next = out;
		@param	[out]	out				始点に近い方の交点
		@param	[in]	line(sta,end)	衝突検知線分
		@return	衝突しているか
		@retval	true	衝突している
		@retval	false	衝突していない
	*/
	bool Contains(mymath::Vec3f& out, const mymath::Vec3f& sta, const mymath::Vec3f& end) const override;
	
	/*
		@brief	線分との衝突検知 + 衝突時の交点の取得
			if(Contains(out, line))
				next = out;
		@param	[out]	out		交点
		@param	[in]	line	衝突検知線分
		@return	衝突しているか
		@retval	true	衝突している
		@retval	false	衝突していない
	*/
	bool Contains(std::vector<mymath::Vec3f>& out, const mymath::Linef& line) const override;
	/*
		@brief	線分との衝突検知 + 衝突時の交点の取得
			if(Contains(out, pos, next))
				next = out;
		@param	[out]	out		交点
		@param	[in]	sta,end	衝突検知線分
		@return	衝突しているか
		@retval	true	衝突している
		@retval	false	衝突していない
	*/
	bool Contains(std::vector<mymath::Vec3f>& out, const mymath::Vec3f& sta, const mymath::Vec3f& end) const override;
	
};

#pragma endregion // class CActionCircle

#pragma region class CActionPolygon
class CActionPolygon : public IActionPoint
{
private:
	std::vector<mymath::Vec3f> vertexes_;	// ポリゴンを成形する頂点
public:
	const std::vector<mymath::Vec3f>& vertexes;
private:
	/*
		@brief	vertexes_の情報を元にポリゴン(閉路)を成形
		@return	ポリゴンの枠
	*/
	std::vector<mymath::Linef> MakeLines() const;
	/*
		@brief	vertexes_の情報を元に三角形ポリゴンを成形
				vertexes_[0]から各頂点に伸びる
		@return	三角形ポリゴン群
	*/
	std::vector<mymath::Trif> MakeTriangles() const;
public:
	CActionPolygon(const std::vector<mymath::Vec3f>& points);
	virtual void step() override;
	virtual void draw() override;
	
	//================================================================================
#pragma region Contains
	/*
		@brief	線分との衝突検知 + 衝突時始点に近い方の交点の取得
			if(Contains(out, line))
				next = out;
		@param	[out]	out		始点に近い方の交点
		@param	[in]	line	衝突検知線分
		@return	衝突しているか
		@retval	true	衝突している
		@retval	false	衝突していない
	*/
	bool Contains(mymath::Vec3f& out, const mymath::Linef& line) const override;
	/*
		@brief	線分との衝突検知 + 衝突時始点に近い方の交点の取得
			if(Contains(out, pos, next))
				next = out;
		@param	[out]	out		始点に近い方の交点
		@param	[in]	sta,end	衝突検知線分
		@return	衝突しているか
		@retval	true	衝突している
		@retval	false	衝突していない
	*/
	bool Contains(mymath::Vec3f& out, const mymath::Vec3f& sta, const mymath::Vec3f& end) const override;
	
	/*
		@brief	線分との衝突検知 + 衝突時の交点の取得
			if(Contains(out, line))
				next = out;
		@param	[out]	out		交点
		@param	[in]	line	衝突検知線分
		@return	衝突しているか
		@retval	true	衝突している
		@retval	false	衝突していない
	*/
	bool Contains(std::vector<mymath::Vec3f>& out, const mymath::Linef& line) const override;
	/*
		@brief	線分との衝突検知 + 衝突時の交点の取得
			if(Contains(out, pos, next))
				next = out;
		@param	[out]	out		交点
		@param	[in]	sta,end	衝突検知線分
		@return	衝突しているか
		@retval	true	衝突している
		@retval	false	衝突していない
	*/
	bool Contains(std::vector<mymath::Vec3f>& out, const mymath::Vec3f& sta, const mymath::Vec3f& end) const override;
	

	bool Contains(const mymath::ShapefPtr& shape) const override;

#pragma endregion // Contains
	//================================================================================
	
	//================================================================================
#pragma region Intersection
	/*
		@brief	線分との交点のうち、始点に近い点を取得
			if(Contains(line))
				point = IntersectionPoint2Nearest(line);
		@attention	交差していることが前提
		@param	[in]	line(sta,end)	比較線分
		@return	始点に近い交点
	*/
	mymath::Vec3f IntersectionPoint2Nearest(const mymath::Linef& line) const;
	/*
		@brief	線分との交点のうち、始点に近い点を取得
			if(Contains(pos,next))
				point = IntersectionPoint2Nearest(pos,next);
		@attention	交差していることが前提
		@param	[in]	sta	比較線分の始点
		@param	[in]	end	比較線分の終点
		@return	始点に近い交点
	*/
	mymath::Vec3f IntersectionPoint2Nearest(const mymath::Vec3f& sta, const mymath::Vec3f& end) const;

	#pragma endregion // Intersection
	//================================================================================
	
};

#pragma endregion // class CActionPolygon


#endif