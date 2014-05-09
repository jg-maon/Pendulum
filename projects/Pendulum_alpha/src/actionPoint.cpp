#ifdef _DEBUG
#define D_CIRCLE_TEST		// CActionCircle内判定円の描画
#define USE_CIRCLE_EXT
#define D_POLYGON_TEST		// CActionPolygon描画

//#define D_CORD_TEST		// 座標描画
#endif
#include "actionPoint.h"
#include "define.h"

#include <typeinfo>


#pragma region IActionPoint methods
IActionPoint::IActionPoint(const std::string& name, float x, float y):
	IObject(name)
{
	obj_.pos.x = x;
	obj_.pos.y = y;
	obj_.pos.z = 0.5f;
}



#pragma endregion	// IActionPoint methods


#pragma region CActionCircle methods

CActionCircle::CActionCircle(float x, float y, float r):
	IActionPoint("ActionCircle", x, y)
	,circle_(x, y, obj_.pos.z, r)
{
}

void CActionCircle::step()
{
	
}
void CActionCircle::draw()
{
#ifdef D_CIRCLE_TEST
	if(InScreen(static_cast<int>(circle_.radius)))
	{
		circle_.draw(0xff4050ff);
	}
#endif
#ifdef D_CORD_TEST
	std::stringstream ss;
	ss	<< "(" << std::setw(4) << (int)circle_.center.x
		<< "," << std::setw(4) << (int)circle_.center.y
		<< ")"
		<< "r:" << std::setw(4) << (int)circle_.radius;

	Draw_FontText(	(int)circle_.center.x,
					(int)circle_.center.y,
					circle_.center.z,
					ss.str(),-1,0);
#endif

}


bool CActionCircle::Contains(mymath::Vec3f& out, const mymath::Vec3f& point) const
{
	if(!circle_.Contains(point)) return false;
	// 交点の計算
	out = circle_.IntersectionPoint2(point);
	return true;
}

bool CActionCircle::Contains(mymath::Vec3f& out, const mymath::Linef& line) const
{
	if(!circle_.Contains(line,false,true)) return false;
	out = circle_.IntersectionPoint2Nearest(line);
	return true;
}

bool CActionCircle::Contains(mymath::Vec3f& out, const mymath::Vec3f& sta, const mymath::Vec3f& end) const
{
	if(!circle_.Contains(sta,end,false,true)) return false;
	out = circle_.IntersectionPoint2Nearest(sta,end);	
	return true;
}

bool CActionCircle::Contains(std::vector<mymath::Vec3f>& out, const mymath::Linef& line) const
{
	if(!circle_.Contains(line,false,true)) return false;
	out = circle_.IntersectionPoint2(line);
	return true;
}

bool CActionCircle::Contains(std::vector<mymath::Vec3f>& out, const mymath::Vec3f& sta, const mymath::Vec3f& end) const
{
	if(!circle_.Contains(sta,end,false,true)) return false;
	out = circle_.IntersectionPoint2(sta,end);
	return true;
}

#pragma endregion // CActionCircle methods

//=============================================================
//=============================================================

#pragma region CActionPolygon methods

CActionPolygon::CActionPolygon(const std::vector<mymath::Vec3f>& points):
	IActionPoint("ActionPolygon")
	,vertexes_(points)
	,vertexes(vertexes_)
{

}

std::vector<mymath::Linef> CActionPolygon::MakeLines() const
{
	std::vector<mymath::Linef> lines;
	for(size_t sta = 0; sta < vertexes_.size(); ++sta)
	{
		size_t end = (sta+1) % vertexes_.size();
		lines.push_back(mymath::Linef(vertexes_[sta], vertexes_[end]));
	}
	return lines;
}

std::vector<mymath::Trif> CActionPolygon::MakeTriangles() const
{
	std::vector<mymath::Trif> triangles;
	for(size_t sta = 1; sta < vertexes_.size()-1; ++sta)
	{
		size_t end = sta+1;
		triangles.push_back(mymath::Trif(vertexes_[0],vertexes_[sta],vertexes_[end]));
	}
	return triangles;
}

void CActionPolygon::step()
{
	
}
void CActionPolygon::draw()
{
#ifdef D_POLYGON_TEST
	std::vector<mymath::Linef> lines = MakeLines();
	mymath::Rectf rect = camera::GetScreenRect();
	for(const auto& line : lines)
	{
		// 完全に画面外の場合スキップ
		//if(!rect.Contains(line)) continue;
		graph::Draw_Line(int(line.sta.x),int(line.sta.y),
					int(line.end.x),int(line.end.y),
					line.sta.z,
					ARGB(255,255,0,0),1);
		
#ifdef D_CORD_TEST
		std::stringstream ss;
		ss << "( " << line.sta.x << " , " << line.sta.y << " )";
		Draw_FontText(line.sta.x,line.sta.y,0.f,ss.str(),-1,0);
#endif
		
	}
#ifdef D_CORD_TEST
	std::stringstream ss;
	ss << "l: " << rect.left << " t: " << rect.top
		<< " r: " << rect.right << " b: " << rect.bottom;
	Draw_FontText(lines[0].sta.x,lines[0].sta.y,0.f,ss.str(),-1,0);
#endif
#endif
}

bool CActionPolygon::Contains(mymath::Vec3f& out,const mymath::Linef& line) const
{
	std::vector<mymath::Linef> lines = MakeLines();
	// 交差
	for(auto& l : lines)
	{
		if(l.Intersect(line))
		{
			out = IntersectionPoint2Nearest(line);
			return true;
		}
	}
	// 内包
	//std::vector<mymath::Trif> tris = MakeTriangles();
	//for(auto& tri : tris)
	//{
	//	if(tri.Contains(line))
	//		return true;
	//}
	// 交点がなかった
	return false;

}
bool CActionPolygon::Contains(mymath::Vec3f& out, const mymath::Vec3f& sta, const mymath::Vec3f& end) const
{
	std::vector<mymath::Linef> lines = MakeLines();
	
	std::vector<mymath::Vec3f> points;	// 交点格納用
	for(auto& l : lines)
	{
		if(l.Intersect(sta, end))
			points.push_back(l.IntersectionPoint2(sta, end));
	}
	// 交点がなかった
	if(points.empty())
		return false;
	// 始点に一番近い交点の算出
	mymath::Vec3f min = points[0];
	for(size_t i = 1; i < points.size(); ++i)
	{
		mymath::Vec3f d1 = min - sta;
		mymath::Vec3f d2 = points[i] - sta;
		if(mymath::PYTHA(d1.x,d1.y) > mymath::PYTHA(d2.x,d2.y))
			min = points[i];
	}
	out = min;
	return true;
}


bool CActionPolygon::Contains(std::vector<mymath::Vec3f>& out, const mymath::Linef& line) const
{
	std::vector<mymath::Linef> lines = MakeLines();
	std::vector<mymath::Vec3f> points;	// 交点格納用
	for(const auto& l : lines)
	{
		if(l.Intersect(line))
			points.push_back(l.IntersectionPoint2(line));
	}
	// 交点がなかった
	if(points.empty())
		return false;
	out = points;
	return true;
}
bool CActionPolygon::Contains(std::vector<mymath::Vec3f>& out, const mymath::Vec3f& sta, const mymath::Vec3f& end) const
{
	std::vector<mymath::Linef> lines = MakeLines();
	
	std::vector<mymath::Vec3f> points;	// 交点格納用
	for(auto& l : lines)
	{
		if(l.Intersect(sta, end))
			points.push_back(l.IntersectionPoint2(sta, end));
	}
	// 交点がなかった
	if(points.empty())
		return false;
	out = points;
	return true;
}

bool CActionPolygon::Contains(const mymath::ShapefPtr& shape) const
{
	std::vector<mymath::Linef> lines = MakeLines();
	const auto& id = typeid(*shape);
	if(id == typeid(mymath::Trif))
	{
		// Triangle
		mymath::Trif& tri = *std::dynamic_pointer_cast<mymath::Trif>(shape);
		// 交差
		for(auto& l : lines)
		{
			if(tri.Contains(l))
			{
				return true;
			}
		}
		
		// 内包
		std::vector<mymath::Trif> mytris = MakeTriangles();
		for(auto& mytri : mytris)
		{
			if(tri.Contains(mytri))
				return true;
			else if(mytri.Contains(tri))
				return true;
		}
	}
	else if(id == typeid(mymath::Circlef))
	{
		// Circle
		mymath::Circlef& cir = *std::dynamic_pointer_cast<mymath::Circlef>(shape);
		// 交差
		for(auto& l : lines)
		{
			if(cir.Contains(l))
			{
				return true;
			}
		}
		
		// 内包
		std::vector<mymath::Trif> mytris = MakeTriangles();
		for(auto& mytri : mytris)
		{
			if(cir.Contains(mytri))
				return true;
			//else if(mytri.Contains(cir))
			//	return true;
		}
	}
	else if(id == typeid(mymath::Rectf))
	{
		// Rect
		mymath::Rectf& rect = *std::dynamic_pointer_cast<mymath::Rectf>(shape);
		// 交差
		for(auto& l : lines)
		{
			if(rect.Contains(l))
			{
				return true;
			}
		}
		
		// 内包
		std::vector<mymath::Trif> mytris = MakeTriangles();
		for(auto& mytri : mytris)
		{
			if(rect.Contains(mytri))
				return true;
			else if(mytri.Contains(rect))
				return true;
		}

	}


	return false;

}

mymath::Vec3f CActionPolygon::IntersectionPoint2Nearest(const mymath::Linef& line) const
{
	std::vector<mymath::Linef> lines = MakeLines();
	std::vector<mymath::Vec3f> points;	// 交点格納用
	for(auto& l : lines)
	{
		if(l.Intersect(line))
			points.push_back(l.IntersectionPoint2(line));
	}
	// 始点に一番近い交点の算出
	mymath::Vec3f min = points[0];
	for(size_t i = 1; i < points.size(); ++i)
	{
		mymath::Vec3f d1 = min - line.sta;
		mymath::Vec3f d2 = points[i] - line.sta;
		if(mymath::PYTHA(d1.x,d1.y) > mymath::PYTHA(d2.x,d2.y))
			min = points[i];
	}
	return min;
}

mymath::Vec3f CActionPolygon::IntersectionPoint2Nearest(const mymath::Vec3f& sta, const mymath::Vec3f& end) const
{
	std::vector<mymath::Linef> lines = MakeLines();
	std::vector<mymath::Vec3f> points;	// 交点格納用
	for(auto& l : lines)
	{
		if(l.Intersect(sta, end))
			points.push_back(l.IntersectionPoint2(sta, end));
	}
	// 始点に一番近い交点の算出
	mymath::Vec3f min = points[0];
	for(size_t i = 1; i < points.size(); ++i)
	{
		mymath::Vec3f d1 = min - sta;
		mymath::Vec3f d2 = points[i] - sta;
		if(mymath::PYTHA(d1.x,d1.y) > mymath::PYTHA(d2.x,d2.y))
			min = points[i];
	}
	return min;
}

#pragma endregion // CActionPolygon methods