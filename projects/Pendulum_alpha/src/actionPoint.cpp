#ifdef _DEBUG
#define D_CIRCLE_TEST		// CActionCircleì‡îªíËâ~ÇÃï`âÊ
#define D_POLYGON_TEST		// CActionPolygonï`âÊ

//#define D_CORD_TEST		// ç¿ïWï`âÊ
#endif

#ifdef D_CIRCLE_TEST
#define DEF_SHAPE_DRAW
#define USE_CIRCLE_EXT
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

IActionPoint::~IActionPoint()
{
}

void IActionPoint::step()
{
}
void IActionPoint::draw()
{
}

bool IActionPoint::Contains(const mymath::Vec3f& point) const
{
	return false;
}

bool IActionPoint::Contains(const mymath::Linef& line) const
{
	return false;
}

bool IActionPoint::Contains(const mymath::Vec3f& sta, const mymath::Vec3f& end) const
{
	return false; 
}

bool IActionPoint::Contains(const mymath::ShapefPtr& shape) const
{
	return false;
}

mymath::Vec3f IActionPoint::IntersectionPoint2(const mymath::Vec3f& point) const
{
	return mymath::Vec3f();
}

std::vector<mymath::Vec3f> IActionPoint::IntersectionPoint2(const mymath::Linef& line) const
{
	return std::vector<mymath::Vec3f>();
}

std::vector<mymath::Vec3f> IActionPoint::IntersectionPoint2(const mymath::Vec3f& sta, const mymath::Vec3f& end) const
{
	return std::vector<mymath::Vec3f>();
}

//std::vector<mymath::Vec3f> IActionPoint::IntersectionPoint2(const mymath::ShapefPtr& shape) const
//{
//	return std::vector<mymath::Vec3f>();
//}

mymath::Vec3f IActionPoint::IntersectionPoint2Nearest(const mymath::Linef& line) const
{
	return mymath::Vec3f();
}

mymath::Vec3f IActionPoint::IntersectionPoint2Nearest(const mymath::Vec3f& sta, const mymath::Vec3f& end) const
{
	return mymath::Vec3f();
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

bool CActionCircle::Contains(const mymath::Vec3f& point) const
{
	return circle_.Contains(point);
}

bool CActionCircle::Contains(const mymath::Linef& line) const
{
	return circle_.Contains(line,false,true);
}

bool CActionCircle::Contains(const mymath::Vec3f& sta, const mymath::Vec3f& end) const
{
	return circle_.Contains(sta,end,false,true);
}

bool CActionCircle::Contains(const mymath::ShapefPtr& shape) const
{
	const auto& id = typeid(*shape);
	if (id == typeid(mymath::Circlef))
	{
		return circle_.Contains(*std::dynamic_pointer_cast<mymath::Circlef>(shape));
	}
	else if (id == typeid(mymath::Rectf))
	{
		return circle_.Contains(*std::dynamic_pointer_cast<mymath::Rectf>(shape));
	}
	else if (id == typeid(mymath::Polyf))
	{
		return circle_.Contains(*std::dynamic_pointer_cast<mymath::Polyf>(shape));
	}
	return false;
}
//-------------------------------------------

mymath::Vec3f CActionCircle::IntersectionPoint2(const mymath::Vec3f& point) const
{
	return circle_.IntersectionPoint2(point);
}

std::vector<mymath::Vec3f> CActionCircle::IntersectionPoint2(const mymath::Linef& line) const
{
	return circle_.IntersectionPoint2(line);
}

std::vector<mymath::Vec3f> CActionCircle::IntersectionPoint2(const mymath::Vec3f& sta, const mymath::Vec3f& end) const
{
	return circle_.IntersectionPoint2(sta, end);
}

//std::vector<mymath::Vec3f> CActionCircle::IntersectionPoint2(const mymath::ShapefPtr& shape) const
//{
//	const auto& id = typeid(*shape);
//	if (id == typeid(mymath::Circlef))
//	{
//		return circle_.IntersectionPoint2(*std::dynamic_pointer_cast<mymath::Circlef>(shape));
//	}
//	else if (id == typeid(mymath::Rectf))
//	{
//		return circle_.IntersectionPoint2(*std::dynamic_pointer_cast<mymath::Rectf>(shape));
//	}
//	else if (id == typeid(mymath::Polyf))
//	{
//		return circle_.IntersectionPoint2(*std::dynamic_pointer_cast<mymath::Polyf>(shape));
//	}
//	return std::vector<mymath::Vec3f>();
//}

mymath::Vec3f CActionCircle::IntersectionPoint2Nearest(const mymath::Linef& line) const
{
	return circle_.IntersectionPoint2Nearest(line);
}

mymath::Vec3f CActionCircle::IntersectionPoint2Nearest(const mymath::Vec3f& sta, const mymath::Vec3f& end) const
{
	return circle_.IntersectionPoint2Nearest(sta, end);
}

#pragma endregion // CActionCircle methods

//=============================================================
//=============================================================

#pragma region CActionPolygon methods

CActionPolygon::CActionPolygon(const std::vector<mymath::Vec3f>& points) :
	IActionPoint("ActionPolygon")
	, polygon_(points)
	, vertexes(polygon_.points)
{
}

CActionPolygon::CActionPolygon(const mymath::Polyf& polygon):
	IActionPoint("ActionPolygon")
	, polygon_(polygon)
	, vertexes(polygon_.points)
{
}


std::vector<mymath::Linef> CActionPolygon::MakeLines() const
{
	std::vector<mymath::Linef> lines;
	for(size_t sta = 0; sta < polygon_.size(); ++sta)
	{
		size_t end = (sta+1) % polygon_.size();
		lines.push_back(
			mymath::Linef(
				polygon_.points[sta],
				polygon_.points[end]));
	}
	return lines;
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
		// äÆëSÇ…âÊñ äOÇÃèÍçáÉXÉLÉbÉv
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

bool CActionPolygon::Contains(const mymath::Vec3f& point) const
{
	return polygon_.Contains(point);
}
bool CActionPolygon::Contains(const mymath::Linef& line) const
{
	return polygon_.Contains(line, false, true);
}

bool CActionPolygon::Contains(const mymath::Vec3f& sta, const mymath::Vec3f& end) const
{
	return polygon_.Contains(sta, end, false, true);
}

bool CActionPolygon::Contains(const mymath::ShapefPtr& shape) const
{
	std::vector<mymath::Linef> lines = MakeLines();
	const auto& id = typeid(*shape);
	if(id == typeid(mymath::Circlef))
	{
		// Circle
		return 	polygon_.Contains(*std::dynamic_pointer_cast<mymath::Circlef>(shape));
		
	}
	else if(id == typeid(mymath::Rectf))
	{
		// Rect
		return 	polygon_.Contains(*std::dynamic_pointer_cast<mymath::Rectf>(shape));
	}
	else if (id == typeid(mymath::Polyf))
	{
		// Polygon
		return 	polygon_.Contains(*std::dynamic_pointer_cast<mymath::Polyf>(shape));
	}
	return false;

}

mymath::Vec3f CActionPolygon::IntersectionPoint2(const mymath::Vec3f& point) const
{
	return polygon_.IntersectionPoint2(point);
}

std::vector<mymath::Vec3f> CActionPolygon::IntersectionPoint2(const mymath::Linef& line) const
{
	return polygon_.IntersectionPoint2(line);
}

std::vector<mymath::Vec3f> CActionPolygon::IntersectionPoint2(const mymath::Vec3f& sta, const mymath::Vec3f& end) const
{
	return polygon_.IntersectionPoint2(sta, end);
}


mymath::Vec3f CActionPolygon::IntersectionPoint2Nearest(const mymath::Linef& line) const
{
	return polygon_.IntersectionPoint2Nearest(line);
}

mymath::Vec3f CActionPolygon::IntersectionPoint2Nearest(const mymath::Vec3f& sta, const mymath::Vec3f& end) const
{
	return polygon_.IntersectionPoint2Nearest(sta, end);
}

#pragma endregion // CActionPolygon methods