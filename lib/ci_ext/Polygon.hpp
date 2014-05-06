#pragma once

#include "vec3.hpp"
#include "line.hpp"
#include "triangle.hpp"
#include "math.hpp"
#include <vector>
#include <assert.h>

namespace ci_ext
{

template<typename T>
class Polygon
{
  Vec3<T> center_;
  std::vector<Vec3<T>> points_;
  T degree_;
  Vec3<T> scale_;
public:
  Polygon(Vec3<T> center, std::vector<Vec3<T>>& point, T degree, Vec3<T>& scale) :
    center_(center),
    points_(point),
    degree_(degree),
    scale_(scale){}
  size_t getPointsNum() const { return points_.size(); }
  std::vector<Vec3<T>> getPoints() const
  {
    std::vector<Vec3<T>> ret;
    for(size_t i = 0; i < points_.size(); ++i)
    {
      ret.push_back(getPoint(i));
    }
    return ret;
  }
  Vec3<T> getPoint(size_t n) const
  {
    if(n >= points_.size()) assert(0);
    Vec3f a = points_[n];
    a.scale(scale_).rotateDegree(degree_).translate(center_);
    return a;
  }
  std::vector<Line<T>> getLines() const
  {
    std::vector<Line<T>> ret;
    for(size_t i = 0; i < points_.size(); ++i)
    {
      ret.push_back(getLine(i));
    }
    return ret;
  }
  Line<T> getLine(size_t no) const
  {
    if(no >= points_.size()) assert(0);
    size_t p1 = no;
    size_t p2 = p1+1 >= points_.size()? 0: p1+1;
    Vec3<T> s = getPoint(p1);
    Vec3<T> e = getPoint(p2);
    return Line<T>(s, e);

  }
  std::vector<Triangle<T>> getTriangles() const
  {
    size_t n = points_.size()-2; //多角形の三角形の数は、（頂点数－２）個
    std::vector<Triangle<T>> triangles;
    for(size_t i = 0; i < n; ++i)
    {
      triangles.push_back(getTriangle(i));
    }
    return triangles;
  }
  Triangle<T> getTriangle(size_t no) const
  {
    if(no >= (points_.size()-2)) assert(0);
    return Triangle<T>(getPoint(0), getPoint(no+1), getPoint(no+2));
  }
  void offsetScale(const Vec3<T>& scale)
  {
    scale_ += scale;
  }
  void setScale(const Vec3<T>& scale)
  {
    scale_ = scale;
  }
  void offsetCenter(const Vec3<T>& offset)
  {
    center_ += offset;
  }
  void setCenter(const Vec3<T>& center)
  {
    center_ = center;
  }
  void setAngle(T degree)
  {
    degree_ = degree;
  }
  void offsetAngle(T degree)
  {
    rotate(degree);
  }
  void rotate(T degree)
  {
    degree_ += degree;
    if(degree_ > 360) degree_ -= 360;
    else if(degree_ < 0) degree += 360;
  }

  //各頂点が引数の三角形に内包しているかどうか
  bool intercect(const Triangle<T>& triangle) const
  {
    std::vector<Vec3<T>> points = getPoints();
    for(auto& point: points)
    {
      if(triangle.intersect(point)) 
        return true;
    }
    return false;
  }

  //引数の頂点が内包しているかどうか
  bool intercect(const Vec3<T>& point) const
  {
    std::vector<Triangle<T>> triangles = getTriangles();
    for(auto& triangle: triangles)
    {
      if(triangle.intersect(point)) 
        return true;
    }
    return false;
  }

  //引数のポリゴンに各頂点が内包しているかどうか
  bool intercectPoint(const Polygon<T>& polygon, std::vector<Vec3<T>>* inner = nullptr) const
  {
    bool ret = false;
    std::vector<Triangle<T>> triangles = polygon.getTriangles();
    std::vector<Vec3<T>> points = getPoints();
    for(auto& triangle: triangles)
    {
      for(auto& point: points)
      {
        if(triangle.intersect(point))
        {
          if(inner == nullptr) return true;
          inner->push_back(point);
          ret = true;
        }
      }
    }
    return ret;
  }

  //引数のポリゴンに各辺が交差しているかどうか
  bool intercect(const Polygon<T>& polygon, std::vector<Vec3<T>>* cross = nullptr) const
  {
    bool ret = false;
    std::vector<Line<T>> lines1 = getLines();
    std::vector<Line<T>> lines2 = polygon.getLines();
    for(auto& line1: lines1)
    {
      for(auto& line2: lines2)
      {
        Vec3<T> crossPoint;
        if(CheckCrossLine(line1, line2, &crossPoint))
        {
          if(cross == nullptr) return true;
          cross->push_back(crossPoint);
          ret = true;
        }
      }
    }
/*
    for(size_t cntP1 = 0; cntP1 < getPointsNum(); ++cntP1)
    {
      for(size_t cntP2 = 0; cntP2 < polygon.getPointsNum(); ++cntP2)
      {
        Line<T> l1 = getLine(cntP1);
        Line<T> l2 = polygon.getLine(cntP2);
        Vec3<T> crossPoint;
        if(CheckCrossLine(l1, l2, &crossPoint))
        {
          if(cross == nullptr) return true;
          cross->push_back(crossPoint);
          ret = true;
        }
      }
    }
*/
    return ret;
  }

  void draw(Color color = 0xFFFFFFFF, int size = 1) const
  {
    std::vector<Line<T>> lines = getLines();
    for(auto& line: lines)
    {
      line.draw(color, size);
    }
  }
};

typedef Polygon<float> Polygonf;
typedef Polygon<double> Polygond;

}