#pragma once
#include <vector>
#include "color.hpp"

namespace ci_ext
{

template<typename T>
class Rect
{
  Vec3<T> pos_;
  Vec3<T> size_;
  T z_;
public:
  Rect(const Vec3<T>& pos, const Vec3<T>& size, T z = 0) : 
    pos_(pos),
    size_(size),
    z_(z) {}
  Rect(T posX, T posY, T width, T height, T z = 0) :
    pos_(posX, posY),
    size_(width, height),
    z_(z) {}

  Rect<T>& offset(const Rect<T>& offset)
  {
    pos_.offset(offset.x(), offset.y(), offset.z());
    size_.offset(offset.width(), offset.height());
    offsetZ(offset.z());
    return *this;
  }
  Rect<T>& offsetPos(const Vec3<T>& offset)
  {
    pos_ += offset;
    return *this;
  }
  Rect<T>& offsetSize(const Vec3<T>& offset)
  {
    size_ += offset;
    return *this;
  }
  T offsetZ(T z, T offset) const 
  {
    z += offset;
    if(z > 1.0f) z = 1.0f;
    else if(z < 0.0f) z = 0.0f;
    return z;
  }
  Rect<T>& offsetZ(T offset)
  {
    z_ = offsetZ(z_, offset);
    return *this;
  }

  Rect<T>& set(const Rect<T>& r)
  {
    set(r.x(), r.y(), r.width(), r.height(), r.z());
    return *this;
  }
  Rect<T>& set(const Vec3<T>& pos, const Vec3<T> size, T z)
  {
    pos_.set(pos);
    size_.set(size);
    z_ = z;
    return *this;
  }
  Rect<T>& set(T x, T y, T w, T h, T z)
  {
    pos_.set(x, y);
    size_.set(w, h);
    z_ = z;
    return *this;
  }

  Rect<T>& operator=(const Rect<T>& r)
  {
    return set(r);
  }

  RECT getRect_PosSize() const 
  {
    RECT rc = {ix(), iy(), iwidth(), iheight()};
    return rc;
  }
  RECT getRect_Locate() const 
  {
    RECT rc = {ix(), iy(), ix()+iwidth(), iy()+iheight()};
    return rc;
  }

  Rect<T>& operator+=(const Rect<T>& r)
  {
    offset(r);
    return *this;
  }
  Rect<T> operator+(const Rect<T>& r)
  {
    return Rect<T>(x() + r.x(), y() + r.y(), width()+r.width(), height()+r.height(), offsetZ(z(), r.z()));
  }
  Rect<T>& operator-=(const Rect<T>& r)
  {
    offset(-r);
    return *this;
  }
  Rect<T> operator-(const Rect<T>& r)
  {
    return Rect<T>(x() - r.x(), y() - r.y(), width()-r.width(), height()-r.height(), offsetZ(z(), -r.z()));
  }
  Rect<T> operator-()
  {
    return Rect<T>(-x(), -y(), -width(), -height(), -z());
  }

  int ix() const { return pos_.ix(); }
  int iy() const { return pos_.iy(); }
  int iwidth() const { return size_.ix(); }
  int iheight() const { return size_.iy(); }
  T x() const { return pos_.x(); }
  T y() const { return pos_.y(); }
  T width() const { return size_.x(); }
  T height() const { return size_.y(); }

  int iz() const { return static_cast<int>(z_); }
  T z() const { return z_; }

  void draw(Color colorFill) const
  {
    DrawBox(*this, colorFill);
  }
  void drawCenter(Color colorFill) const
  {
    DrawBoxCenter(*this, colorFill);
  }
  void drawOutline(Color colorFill) const
  {
    DrawBoxOutline(*this, colorFill);
  }
  void drawCenterOutline(Color colorFill) const
  {
    DrawBoxCenterOutline(*this, colorFill);
  }
};

template<typename T>
void DrawBoxOutline(const Rect<T>& draw, Color colorFill = 0xFFFFFFFF)
{
  Draw_Box(draw.ix(), draw.iy(), draw.ix() + draw.iwidth(), draw.iy()+draw.iheight(),
    draw.z(),
    colorFill.val(), colorFill.val(),
    1, 0);
}
template<typename T>
void DrawBoxCenterOutline(const Rect<T>& draw, Color colorFill = 0xFFFFFFFF)
{
  int offsetx = static_cast<int>(draw.width() / 2.0f);
  int offsety = static_cast<int>(draw.height() / 2.0f);
  Draw_Box(
    draw.ix() - offsetx,
    draw.iy() - offsety,
    draw.ix() + offsetx, 
    draw.iy() + offsety,
    draw.z(),
    colorFill.val(), colorFill.val(),
    1, 0);
}

template<typename T>
void DrawBox(const Rect<T>& draw, Color colorFill = 0xFFFFFFFF)
{
  Draw_Box(draw.ix(), draw.iy(),
    draw.ix()+draw.iwidth(), draw.iy()+draw.iheight(),
    draw.z(),
    colorFill.val(), colorFill.val(),
    0, 1);
}

template<typename T>
void DrawBoxCenter(const Rect<T>& draw, Color colorFill = 0xFFFFFFFF)
{
  int offsetx = static_cast<int>(draw.width() / 2.0f);
  int offsety = static_cast<int>(draw.height() / 2.0f);
  Draw_Box(
    draw.ix() - offsetx,
    draw.iy() - offsety,
    draw.ix() + offsetx, 
    draw.iy() + offsety,
    draw.z(),
    colorFill.val(), colorFill.val(),
    0, 1);
}

template<typename T>
void DrawGraphics(const Rect<T>& draw, IMGOBJ imgNo, const Rect<T>& src,
                  Color color = 0xFFFFFFFF,
                  const Vec3<T>* center_rotate = 0, const int degree = 0)
{
  POINT pt;
  POINT* p = center_rotate ? &pt: nullptr;
  if(center_rotate)
  {
    pt.x = center_rotate->ix();
    pt.y = center_rotate->iy();
  }
  Draw_Graphics(draw.ix(), draw.iy(), draw.z(),
                imgNo,
                src.ix(), src.iy(),
                src.iwidth(), src.iheight(),
                degree, p,
                draw.width() / src.width(),
                draw.height() / src.height(),
                color.a(), color.r(), color.g(), color.b());
}
template<typename T>
void DrawGraphicsCenter(const Rect<T>& draw, IMGOBJ imgNo, const Rect<T>& src,
                        Color color = 0xFFFFFFFF,
                        const Vec3<T>* center_rotate = 0, const int degree = 0)
{
  POINT pt;
  POINT* p = center_rotate ? &pt: nullptr;
  if(center_rotate)
  {
    pt.x = center_rotate->ix();
    pt.y = center_rotate->iy();
  }
  Draw_GraphicsCenter(draw.ix(), draw.iy(), draw.z(),
                imgNo,
                src.ix(), src.iy(),
                src.iwidth(), src.iheight(),
                degree, p,
                draw.width() / src.width(),
                draw.height() / src.height(),
                color.a(), color.r(), color.g(), color.b());
}

template<typename T>
void DrawGraphics(const Vec3<T>& pos_draw, const Vec3<T>& size_draw,
                  IMGOBJ imgNo,
                  const Vec3<T>& pos_src,  const Vec3<T>& size_src,
                  Color color = 0xFFFFFFFF,
                  const Vec3<T>* center_rotate = 0, const int degree = 0)
{
  POINT pt;
  POINT* p = center_rotate ? &pt: nullptr;
  if(center_rotate)
  {
    pt.x = center_rotate->ix();
    pt.y = center_rotate->iy();
  }
  Draw_Graphics(pos_draw.ix(), pos_draw.iy(), pos_draw.z(),
                imgNo,
                pos_src.ix(), pos_src.iy(),
                size_src.ix(), size_src.iy(),
                degree, p,
                size_draw.x() / size_src.x(),
                size_draw.y() / size_src.y(),
                color.a(), color.r(), color.g(), color.b());
}

template<typename T>
void DrawGraphicsCenter(const Vec3<T>& pos_draw, const Vec3<T>& size_draw,
                  IMGOBJ imgNo,
                  const Vec3<T>& pos_src,  const Vec3<T>& size_src,
                  Color color = 0xFFFFFFFF,
                  const Vec3<T>* center_rotate = 0, const int degree = 0)
{
  POINT pt;
  POINT* p = center_rotate ? &pt: nullptr;
  if(center_rotate)
  {
    pt.x = center_rotate->ix();
    pt.y = center_rotate->iy();
  }
  Draw_GraphicsCenter(pos_draw.ix(),pos_draw.iy(), pos_draw.z(),
                imgNo,
                pos_src.ix(), pos_src.iy(),
                size_src.ix(), size_src.iy(),
                degree, p,
                size_draw.x() / size_src.x(),
                size_draw.y() / size_src.y(),
                color.a(), color.r(), color.g(), color.b());
}
template<typename T>
void DrawGraphicsCenter(const Vec3<T>& pos_draw, IMGOBJ imgNo,
                  const Vec3<T>& pos_src,  const Vec3<T>& size_src,
                  Color color = 0xFFFFFFFF,
									const Vec3<T>& scale = Vec3<T>(1,1,0),
                  const Vec3<T>* center_rotate = 0, const int degree = 0)
{
  Draw_GraphicsCenter(
    pos_draw.ix(),pos_draw.iy(), pos_draw.z(),
    imgNo,
    pos_src.ix(), pos_src.iy(),
    size_src.ix(), size_src.iy(),
    degree, p,
    scale.x(), scale.y(),
    color.a(), color.r(), color.g(), color.b());
}

template<typename T>
void DrawGraphics(const Vec3<T>& pos_draw, IMGOBJ imgNo,
                  const Vec3<T>& pos_src,  const Vec3<T>& size_src,
                  Color color = 0xFFFFFFFF,
									const Vec3<T>& scale = Vec3<T>(1,1,0),
                  const Vec3<T>* center_rotate = 0, const int degree = 0)
{
  Draw_Graphics(
    pos_draw.ix(),pos_draw.iy(), pos_draw.z(),
    imgNo,
    pos_src.ix(), pos_src.iy(),
    size_src.ix(), size_src.iy(),
    degree, p,
    scale.x(), scale.y(),
    color.a(), color.r(), color.g(), color.b());
}

template<typename T>
void DrawLine(const Vec3<T>& point1, const Vec3<T>& point2,
              Color color = 0xFFFFFFFF, int size = 1)
{
		Draw_Line(point1.ix(), point1.iy(),
              point2.ix(), point2.iy(),
              point1.z(),
              color, size); 
}

template<typename T>
void DrawLines(const std::vector<Vec3<T>>& points,
               Color color = 0xFFFFFFFF, int size = 1)
{
  auto p1 = points.begin();
  auto p2 = p1 + 1;
  for(; p2 != points.end(); ++p1, ++p2)
  {
		DrawLine(*p1, *p2, color, size); 
  }
  p2 = points.begin();
	DrawLine(*p1, *p2, color, size); 

}

}