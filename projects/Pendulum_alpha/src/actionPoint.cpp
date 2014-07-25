#ifdef _DEBUG

//#define D_CORD_TEST		// 座標描画
#endif
//#define D_CIRCLE_TEST		// CActionCircle内判定円の描画
#define D_POLYGON_TEST		// CActionPolygon描画

#ifdef D_CIRCLE_TEST
#define DEF_SHAPE_DRAW
#define USE_CIRCLE_EXT
#endif

#include "actionPoint.h"
#include "define.h"

#include <typeinfo>


#pragma region IActionPoint methods
IActionPoint::IActionPoint(const std::string& name, float x, float y) :
IObject(name)
{
	obj_.pos.x = x;
	obj_.pos.y = y;
	obj_.pos.z = 0.8f;
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

bool IActionPoint::Contains(const mymath::Linef& line, bool fullContain, bool intersectOnly) const
{
	return false;
}

bool IActionPoint::Contains(const mymath::Vec3f& sta, const mymath::Vec3f& end, bool fullContain, bool intersectOnly) const
{
	return false; 
}

bool IActionPoint::Contains(const mymath::ShapefPtr& shape, bool fullContain, bool intersectOnly) const
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

std::vector<mymath::Vec3f> IActionPoint::IntersectionPoint2(const mymath::ShapefPtr& shape) const
{
	return std::vector<mymath::Vec3f>();
}

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

CActionCircle::CActionCircle(float x, float y, float r) :
IActionPoint("ActionCircle", x, y)
, circle_(new mymath::Circlef(x, y, obj_.pos.z, r))
{
	obj_.resname = "img_actionCircle200";
	obj_.size(512, 512);
	obj_.scale = r * 2.f / 400.f;
}

void CActionCircle::step()
{
	
}
void CActionCircle::draw()
{
#ifdef D_CIRCLE_TEST
	//if(InScreen(static_cast<int>(circle_.radius)))
	mymath::Rectf cr = camera::GetScreenRect();
	if (cr.Contains(*circle_))
	{
		circle_->draw(0xff4050ff);
	}
#endif
#ifdef D_CORD_TEST
	std::stringstream ss;
	ss	<< "(" << std::setw(4) << (int)circle_->center.x
		<< "," << std::setw(4) << (int)circle_->center.y
		<< ")"
		<< "r:" << std::setw(4) << (int)circle_->radius;

	Draw_FontText(	(int)circle_->center.x,
		(int)circle_->center.y,
		circle_->center.z,
		ss.str(),-1,0);
#endif
	mymath::Recti rt = camera::GetScreenRect();
	//if (rt.)
	if(InScreen())
		obj_.draw();
}

mymath::ShapefPtr CActionCircle::GetAP()
{
	return circle_;
}


bool CActionCircle::Contains(const mymath::Vec3f& point) const
{
	mymath::Circlef tmp = *circle_;
	tmp.radius += adjust / 10.f;
	return circle_->Contains(point) || tmp.Contains(point);
}

bool CActionCircle::Contains(const mymath::Linef& line, bool fullContain, bool intersectOnly) const
{
	mymath::Circlef tmp = *circle_;
	tmp.radius += adjust / 10.f;
	return circle_->Contains(line, fullContain, intersectOnly) || tmp.Contains(line, fullContain, intersectOnly);
}

bool CActionCircle::Contains(const mymath::Vec3f& sta, const mymath::Vec3f& end, bool fullContain, bool intersectOnly) const
{
	mymath::Circlef tmp = *circle_;
	tmp.radius += adjust / 10.f;
	return circle_->Contains(sta, end, fullContain, intersectOnly) || tmp.Contains(sta, end, fullContain, intersectOnly);
}

bool CActionCircle::Contains(const mymath::ShapefPtr& shape, bool fullContain, bool intersectOnly) const
{
	const auto& id = typeid(*shape);
	if (id == typeid(mymath::Circlef))
	{
		return circle_->Contains(*std::dynamic_pointer_cast<mymath::Circlef>(shape), fullContain, intersectOnly);
	}
	else if (id == typeid(mymath::Rectf))
	{
		return circle_->Contains(*std::dynamic_pointer_cast<mymath::Rectf>(shape), fullContain, intersectOnly);
	}
	else if (id == typeid(mymath::Polyf))
	{
		return circle_->Contains(*std::dynamic_pointer_cast<mymath::Polyf>(shape), fullContain, intersectOnly);
	}
	return false;
}
//-------------------------------------------

mymath::Vec3f CActionCircle::IntersectionPoint2(const mymath::Vec3f& point) const
{
	if (circle_->Contains(point))
		return circle_->IntersectionPoint2(point);
	// 補正地点(円外)で交差
	// 点と中心で更にベクトルを生成
	mymath::Linef line(point, circle_->center);
	return circle_->IntersectionPoint2Nearest(line);
}

std::vector<mymath::Vec3f> CActionCircle::IntersectionPoint2(const mymath::Linef& line) const
{
	if (circle_->Contains(line))
		return circle_->IntersectionPoint2(line);
	// 補正地点(円外)で交差
	// 中心に向けて終点を移動させて交点を求める
	auto& cx = circle_->center.x;
	auto& cy = circle_->center.y;
	auto& lx = line.end.x;
	auto& ly = line.end.y;
	if (lx > cx)
	{
		// 終点を左に動かしつつ交点を探す
		for (float x = lx; x > cx; x -= 1.f)
		{
			mymath::Vec3f p(x, ly);
			if (circle_->Contains(line.sta, p))
				return circle_->IntersectionPoint2(line.sta, p);
		}
	}
	else if (lx < cx)
	{
		// 終点を右に動かしつつ交点を探す
		for (float x = lx; x < cx; x += 1.f)
		{
			mymath::Vec3f p(x, ly);
			if (circle_->Contains(line.sta, p))
				return circle_->IntersectionPoint2(line.sta, p);
		}
	}
	if (cy < ly)
	{
		// 終点を上に動かしつつ交点を探す
		for (float y = ly; y > cy; y -= 1.f)
		{
			mymath::Vec3f p(lx, y);
			if (circle_->Contains(line.sta, p))
				return circle_->IntersectionPoint2(line.sta, p);
		}
	}
	else if (cy > ly)
	{
		// 終点を下に動かしつつ交点を探す
		for (float y = ly; y < cy; y += 1.f)
		{
			mymath::Vec3f p(lx, y);
			if (circle_->Contains(line.sta, p))
				return circle_->IntersectionPoint2(line.sta, p);
		}
	}

	return std::vector<mymath::Vec3f>(1, circle_->center);
}

std::vector<mymath::Vec3f> CActionCircle::IntersectionPoint2(const mymath::Vec3f& sta, const mymath::Vec3f& end) const
{
	return IntersectionPoint2(mymath::Linef(sta, end));
}

std::vector<mymath::Vec3f> CActionCircle::IntersectionPoint2(const mymath::ShapefPtr& shape) const
{
	const auto& id = typeid(*shape);
	if (id == typeid(mymath::Circlef))
	{
		return circle_->IntersectionPoint2(*std::dynamic_pointer_cast<mymath::Circlef>(shape));
	}
	else if (id == typeid(mymath::Rectf))
	{
		return circle_->IntersectionPoint2(*std::dynamic_pointer_cast<mymath::Rectf>(shape));
	}
	else if (id == typeid(mymath::Polyf))
	{
		return circle_->IntersectionPoint2(*std::dynamic_pointer_cast<mymath::Polyf>(shape));
	}
	return std::vector<mymath::Vec3f>();
}

mymath::Vec3f CActionCircle::IntersectionPoint2Nearest(const mymath::Linef& line) const
{
	if (circle_->Contains(line))
		return circle_->IntersectionPoint2Nearest(line);
	// 補正地点(円外)で交差
	auto intersections = IntersectionPoint2(line);
	mymath::Vec3f min = intersections[0];
	for (size_t i = 1; i < intersections.size(); ++i)
	{
		mymath::Vec3f d1 = min - line.sta;
		mymath::Vec3f d2 = intersections[i] - line.sta;
		if (mymath::PYTHA(d1.x, d1.y) > mymath::PYTHA(d2.x, d2.y))
			min = intersections[i];
	}
	return min;
}

mymath::Vec3f CActionCircle::IntersectionPoint2Nearest(const mymath::Vec3f& sta, const mymath::Vec3f& end) const
{
	return IntersectionPoint2Nearest(mymath::Linef(sta, end));
}

#pragma endregion // CActionCircle methods

//=============================================================
//=============================================================

#pragma region CActionPolygon methods

CActionPolygon::CActionPolygon(const std::vector<mymath::Vec3f>& points) :
IActionPoint("ActionPolygon")
, polygon_(new mymath::Polyf(points))
, vertexes(polygon_->points)
{
}

CActionPolygon::CActionPolygon(const mymath::Polyf& polygon) :
IActionPoint("ActionPolygon")
, polygon_(new mymath::Polyf(polygon))
, vertexes(polygon_->points)
{
}


std::vector<mymath::Linef> CActionPolygon::MakeLines() const
{
	std::vector<mymath::Linef> lines;
	lines.reserve(polygon_->size());
	for (size_t sta = 0; sta < polygon_->size(); ++sta)
	{
		size_t end = (sta + 1) % polygon_->size();
		lines.push_back(
			mymath::Linef(
			polygon_->points[sta],
			polygon_->points[end]));
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
	for (const auto& line : lines)
	{
		// 完全に画面外の場合スキップ
		//if(!rect.Contains(line)) continue;
		graph::Draw_Line(int(line.sta.x), int(line.sta.y),
			int(line.end.x), int(line.end.y),
			line.sta.z,
			ARGB(255, 255, 0, 0), 1);

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

mymath::ShapefPtr CActionPolygon::GetAP()
{
	return polygon_;
}

bool CActionPolygon::Contains(const mymath::Vec3f& point) const
{
	return polygon_->Contains(point);
}
bool CActionPolygon::Contains(const mymath::Linef& line, bool fullContain, bool intersectOnly) const
{
	return polygon_->Contains(line, fullContain, intersectOnly);
}

bool CActionPolygon::Contains(const mymath::Vec3f& sta, const mymath::Vec3f& end, bool fullContain, bool intersectOnly) const
{
	return polygon_->Contains(sta, end, fullContain, intersectOnly);
}

bool CActionPolygon::Contains(const mymath::ShapefPtr& shape, bool fullContain, bool intersectOnly) const
{
	std::vector<mymath::Linef> lines = MakeLines();
	const auto& id = typeid(*shape);
	if (id == typeid(mymath::Circlef))
	{
		// Circle
		return 	polygon_->Contains(*std::dynamic_pointer_cast<mymath::Circlef>(shape), fullContain, intersectOnly);

	}
	else if (id == typeid(mymath::Rectf))
	{
		// Rect
		return 	polygon_->Contains(*std::dynamic_pointer_cast<mymath::Rectf>(shape), fullContain, intersectOnly);
	}
	else if (id == typeid(mymath::Polyf))
	{
		// Polygon
		return 	polygon_->Contains(*std::dynamic_pointer_cast<mymath::Polyf>(shape), fullContain, intersectOnly);
	}
	return false;

}

mymath::Vec3f CActionPolygon::IntersectionPoint2(const mymath::Vec3f& point) const
{
	return polygon_->IntersectionPoint2(point);
}

std::vector<mymath::Vec3f> CActionPolygon::IntersectionPoint2(const mymath::Linef& line) const
{
	return polygon_->IntersectionPoint2(line);
}

std::vector<mymath::Vec3f> CActionPolygon::IntersectionPoint2(const mymath::Vec3f& sta, const mymath::Vec3f& end) const
{
	return polygon_->IntersectionPoint2(sta, end);
}

std::vector<mymath::Vec3f> CActionPolygon::IntersectionPoint2(const mymath::ShapefPtr& shape) const
{
	const auto& id = typeid(*shape);
	if (id == typeid(mymath::Circlef))
	{
		return polygon_->IntersectionPoint2(*std::dynamic_pointer_cast<mymath::Circlef>(shape));
	}
	else if (id == typeid(mymath::Rectf))
	{
		return polygon_->IntersectionPoint2(*std::dynamic_pointer_cast<mymath::Rectf>(shape));
	}
	else if (id == typeid(mymath::Polyf))
	{
		return polygon_->IntersectionPoint2(*std::dynamic_pointer_cast<mymath::Polyf>(shape));
	}
	return std::vector<mymath::Vec3f>();
}


mymath::Vec3f CActionPolygon::IntersectionPoint2Nearest(const mymath::Linef& line) const
{
	return polygon_->IntersectionPoint2Nearest(line);
}

mymath::Vec3f CActionPolygon::IntersectionPoint2Nearest(const mymath::Vec3f& sta, const mymath::Vec3f& end) const
{
	return polygon_->IntersectionPoint2Nearest(sta, end);
}

#pragma endregion // CActionPolygon methods