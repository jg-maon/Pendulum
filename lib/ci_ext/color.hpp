#pragma once

namespace ci_ext
{

union ColorU
{
  unsigned int val;
  struct 
  {
    unsigned char b;
    unsigned char g;
    unsigned char r;
    unsigned char a;
  };
};

class Color
{
  ColorU val_;
  Color() {}
public:
  Color(const unsigned int val)
  {
    set(val);
  }
  Color(unsigned char a, unsigned char r, unsigned char g, unsigned char b)
  {
    set(a, r, g, b);
  }
  unsigned int val() const { return val_.val; }
  operator int() const { return val_.val; }
  operator unsigned int() const { return val_.val; }
  operator long() const { return val_.val; }
  operator unsigned long() const { return val_.val; }
  unsigned char a() const { return val_.a; }
  unsigned char r() const { return val_.r; }
  unsigned char g() const { return val_.g; }
  unsigned char b() const { return val_.b; }
  void a(unsigned char a) { val_.a = a; }
  void r(unsigned char r) { val_.r = r; }
  void g(unsigned char g) { val_.g = g; }
  void b(unsigned char b) { val_.b = b; }
  void adda(unsigned char a) { val_.a += a; }
  void addr(unsigned char r) { val_.r += r; }
  void addg(unsigned char g) { val_.g += g; }
  void addb(unsigned char b) { val_.b += b; }
  void set(const unsigned int val) { val_.val = val; }
  void set(unsigned char a, unsigned char r, unsigned char g, unsigned char b) 
  {
    val_.a = a;
    val_.r = r;
    val_.g = g;
    val_.b = b;
  }
  Color& operator=(const Color& color)
  {
    set(val_.val);
    return *this;
  }

  Color& operator=(const unsigned int color)
  {
    set(color);
    return *this;
  }

  Color& operator+=(const Color& color)
  {
    val_.a += color.val_.a;
    val_.r += color.val_.r;
    val_.g += color.val_.g;
    val_.b += color.val_.b;
    return *this;
  }

  Color operator+(const Color& color)
  {
    return Color(val_.val + color.val_.val);
  }

  Color& operator-=(const Color& color)
  {
    val_.a -= color.val_.a;
    val_.r -= color.val_.r;
    val_.g -= color.val_.g;
    val_.b -= color.val_.b;
    return *this;
  }

  Color operator-(const Color& color)
  {
    return Color(val_.val - color.val_.val);
  }
  bool operator==(const Color& color)
  {
    return (val_.val == color.val_.val);
  }

  bool operator==(const unsigned int c) const
  {
    return (val_.val == c);
  }
};

template<typename T>
int GetAlpha(T color)
{
  return (color >> 24) & 0xFF;
}

template<typename T>
int GetRed(T color)
{
  return (color >> 16) & 0xFF;
}

template<typename T>
int GetGreen(T color)
{
  return (color >> 8) & 0xFF;
}

template<typename T>
int GetBlue(T color)
{
  return color & 0xFF;
}

template<typename T>
int SetColor(T a, T r, T g, T b)
{
  int color = ((a & 0xFF) << 24) ||
              ((r & 0xFF) << 16) ||
              ((g & 0xFF) << 8)  ||
               (b & 0xff);
  return color;
}

}