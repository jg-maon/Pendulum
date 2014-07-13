#ifndef DEF_MYMATH_HPP
#define DEF_MYMATH_HPP
#include <cmath>
#include <vector>
#include <memory>

#include <wtypes.h>	//  RECT用

#define DEF_SHAPE_DRAW

#if defined(DEF_SHAPE_DRAW) || defined(USE_CIRCLE_EXT)
#include "../../../lib/gplib.h"
#endif

namespace mymath
{
 const float PI = 3.14159265358979323846f;
 const double PI_D = 3.14159265358979323846;

#ifndef M_PI
#define M_PI   3.14159265358979323846f
#endif
#ifndef M_PI_D
#define M_PI_D 3.14159265358979323846
#endif




/*
// 2乗
#define POW2(x)		( (x)*(x) )
// ピタゴラスの定理のルートなし
#define PYTHA(x,y)	( POW2((x)) + POW2((y)) )
//*/
//*
// 2乗
template <typename T> inline T POW2(const T& x)
{
	return x * x;
}
// ピタゴラスの定理のルートなし
template <typename T1, typename T2>	inline T1 PYTHA(const T1& x, const T2& y)
{ 
	return (x * x) + (y * y); 
}
//*/
// 四捨五入
template<typename T> inline T	round(T a)
{
	return T(int(a + .5 - (a<0.0)));
}


#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
//template<class T>
//inline T max(const T a, const T b)
//{
//	return ((a > b) ? a : b);
//}
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
//template<class T>
//inline T min(const T a, const T b)
//{
//	return ((a < b) ?  a : b);
//}
#endif

#ifndef clamp
#define clamp(x,minVal,maxVal)		( min(max( (minVal), (x)), (maxVal)) )
//template<class T>
//inline T clamp(const T x, const T minVal, const T maxVal)
//{
//	return ( min(max( minVal, x), maxVal) );
//}
#endif

// 円の補助機能を使う際はコメントアウトを外すか外部定義する
//#define USE_CIRCLE_EXT

//=================================================================================================
//=================================================================================================
//=================================================================================================
#pragma region Vec3

template<typename T> 
class Vec3 
{ 
public:
	T x,y,z;

	Vec3(T xx = 0, T yy = 0, T zz = 0) :
		x(xx), y(yy), z(zz)
	{}
	Vec3(const T p[3]) :
		x(p[0]), y(p[1]), z(p[2])
	{}
	Vec3(const POINT& p)
	{
		x = static_cast<T>(p.x);
		y = static_cast<T>(p.y);
		z = static_cast<T>(0);
	}

	template<class T1>
	Vec3(const mymath::Vec3<T1>& v)
	{
		x = static_cast<T>(v.x);
		y = static_cast<T>(v.y);
		z = static_cast<T>(v.z);
	}

	//Vec3(const Vec3<T>& v){ *this = v; }

	// 代入、初期化系
	Vec3<T>&	operator()(T xx, T yy = 0, T zz = 0){ x = xx; y = yy; z = zz; return *this; }
	Vec3<T>&	operator=(T v){ x = v; y = v; z = v; return *this; }
	Vec3<T>&	operator=(const POINT& p){	x = static_cast<T>(p.x); y = static_cast<T>(p.y); return *this;	}

	// キャスト代入
	template<class T1>
	Vec3<T>& operator = (const mymath::Vec3<T1>& v)
	{
		x = static_cast<T>(v.x);
		y = static_cast<T>(v.y);
		z = static_cast<T>(v.z);
		return *this;
	}
	// キャスト
	template<class T1>
	operator Vec3<T1>()
	{
		return Vec3<T1>(static_cast<T>(x),
			static_cast<T>(y),
			static_cast<T>(z));
	}


	// 単項
	Vec3<T>		operator-() const { return Vec3<T>(-x, -y, -z); }

	// 比較 
	bool		operator==(const Vec3<T>& v) const { return (x == v.x && y == v.y && z == v.z); }
	bool		operator!=(const Vec3<T>& v) const { return (x != v.x || y != v.y || z != v.z); }

	// ベクトル操作(ベクトル対ベクトル)
	Vec3<T>		operator+(const Vec3<T>& v) const { return Vec3<T>(x + v.x, y + v.y, z + v.z); }
	Vec3<T>		operator-(const Vec3<T>& v) const { return Vec3<T>(x - v.x, y - v.y, z - v.z); }
	Vec3<T>		operator*(const Vec3<T>& v) const { return Vec3<T>(x * v.x, y * v.y, z * v.z); }
	Vec3<T>		operator/(const Vec3<T>& v) const { return Vec3<T>(x / v.x, y / v.y, z / v.z); }
	Vec3<T>&	operator+=(const Vec3<T>& v){ x += v.x; y += v.y; z += v.z; return *this; }
	Vec3<T>&	operator-=(const Vec3<T>& v){ x -= v.x; y -= v.y; z -= v.z; return *this; }
	Vec3<T>&	operator*=(const Vec3<T>& v){ x *= v.x; y *= v.y; z *= v.z; return *this; }
	Vec3<T>&	operator/=(const Vec3<T>& v){ x /= v.x; y /= v.y; z /= v.z; return *this; }

	// 特殊演算
	T			operator|(const Vec3<T>& v)	{ return Dot(*this, v); }
	Vec3<T>		operator&(const Vec3<T>& v)	{ return Cross(*this, v); }

	// ベクトル操作2(ベクトル対定数)
	Vec3<T>		operator+(T v) const { return Vec3<T>(x + v, y + v, z + v); }
	Vec3<T>		operator-(T v) const { return Vec3<T>(x - v, y - v, z - v); }
	Vec3<T>		operator*(T v) const { return Vec3<T>(x * v, y * v, z * v); }
	Vec3<T>		operator/(T v) const { return Vec3<T>(x / v, y / v, z / v); }
	Vec3<T>&	operator+=(T v){ x += v; y += v; z += v; return *this; }
	Vec3<T>&	operator-=(T v){ x -= v; y -= v; z -= v; return *this; }
	Vec3<T>&	operator*=(T v){ x *= v; y *= v; z *= v; return *this; }
	Vec3<T>&	operator/=(T v){ x /= v; y /= v; z /= v; return *this; }

	// friend関数によるベクトル操作(定数対ベクトル)
	friend Vec3<T>	operator+(T v, const Vec3<T>& obj){ return Vec3<T>(obj.x + v, obj.y + v, obj.z + v); }
	friend Vec3<T>	operator-(T v, const Vec3<T>& obj){ return Vec3<T>(obj.x - v, obj.y - v, obj.z - v); }
	friend Vec3<T>	operator*(T v, const Vec3<T>& obj){ return Vec3<T>(obj.x * v, obj.y * v, obj.z * v); }
	friend Vec3<T>	operator/(T v, const Vec3<T>& obj){ return Vec3<T>(obj.x / v, obj.y / v, obj.z / v); }
	  
	// friend関数による比較
	friend bool		operator==(T v, Vec3<T>& obj){ return (obj.x == v && obj.y == v && obj.z == v); }
	friend bool		operator!=(T v, Vec3<T>& obj){ return (obj.x != v || obj.y != v || obj.z != v); }


#pragma region member methods

	static enum eTmpReplace { X = 0x01, Y = 0x02, Z = 0x04 };
	/*
		@brief	一時的に値を置き換えたベクトルを返す
		@param	[in]	flag	置き換えるベクトルの要素(X,Y,Zで指定、|で複数指定可)
		@param	[in]	v		元ベクトル
		@return	置き換えたベクトル
	*/
	Vec3<T>		TmpReplace(int flag, const Vec3<T>& v)
	{
		return Vec3<T>((flag&X) ? v.x : x, (flag&Y) ? v.y : y, (flag&Z) ? v.z : z);
	}

	/*
		@brief	ベクトルの長さを取得(3D)
		@return 3次元ベクトルの長さ sqrt(x*x + y*y + z*z)
	*/
	float Length() const
	{
		return std::sqrtf(static_cast<float>(x*x + y*y + z*z));
		//return std::sqrtf(std::powf(x,2.f) + std::powf(y,2.f) + std::powf(z,2.f));
	}
	/*
		@brief	ベクトルの長さを取得(2D)
		@return XY平面の2次元ベクトルの長さ sqrt(x*x+y*y)
	*/
	float Length2() const
	{
		return std::sqrtf(static_cast<float>(x*x + y*y));
		//return std::sqrtf(std::powf(x,2.f) + std::powf(y,2.f));
	}

	/*
		@brief	3次元ベクトルを正規化する
		@return	正規化したベクトル
	*/
	Vec3<T>& Normalize()
	{
		float mag = 1.f / Length();
		x = static_cast<T>(static_cast<float>(x)* mag);
		y = static_cast<T>(static_cast<float>(y)* mag);
		z = static_cast<T>(static_cast<float>(z)* mag);
		return *this;
	}
	/*
		@brief	XY平面の2次元ベクトルを正規化する
		@return	正規化したベクトル
	*/
	Vec3<T>& Normalize2()
	{
		float mag = 1.f / Length2();
		x = static_cast<T>(static_cast<float>(x)* mag);
		y = static_cast<T>(static_cast<float>(y)* mag);
		return *this;
	}

#pragma endregion // member methods
	
#pragma region static methods
	static Vec3<T> Right(){		return Vec3<T>( 1, 0, 0); }
	static Vec3<T> Left(){		return Vec3<T>(-1, 0, 0); }
	static Vec3<T> Top(){		return Vec3<T>( 0, 1, 0); }
	static Vec3<T> Bottom(){	return Vec3<T>( 0,-1, 0); }
	static Vec3<T> Front(){		return Vec3<T>( 0, 0, 1); }
	static Vec3<T> Back(){		return Vec3<T>( 0, 0,-1); }

	/*
		@brief ベクトルv1とv2のなす角
		@param	[in]	v1	ベクトル
		@param	[in]	v2	ベクトル
		@return	なす角 [ unit:radian ]
	*/
	static T Angle(const Vec3<T>& v1, const Vec3<T>& v2)
	{
		/*
		float l1 = v1.Length();
		float l2 = v2.Length();
		if (l1 == 0.0f || l2 == 0.0f) return 0.0f;
		return std::acosf(Dot(v1, v2) / (l1 * l2));
		*/
		Vec3<T> v = v2 - v1;
		return std::atan2(v.y, v.x);
	}

	/*
		@brief	引数のベクトルとの内積(v1・v2)
		@param	[in]	v1	ベクトル
		@param	[in]	v2	ベクトル
		@return	内積
	*/
	static T Dot(const Vec3<T>& v1, const Vec3<T>& v2)
	{
		return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
	}
	/*
		@brief	引数のベクトルとのXY平面における内積(v1・v2)
		@param	[in]	v1	ベクトル
		@param	[in]	v2	ベクトル
		@return	内積
	*/
	static T Dot2(const Vec3<T>& v1, const Vec3<T>& v2)
	{
		return (v1.x * v2.x) + (v1.y * v2.y);
	}

	/*
		@brief	引数のベクトルとの外積(v1×v2)
		@param	[in]	v1	ベクトル
		@param	[in]	v2	ベクトル
		@return	外積
	*/
	static Vec3<T> Cross(const Vec3<T>& v1, const Vec3<T>& v2)
	{
		return Vec3<T>((v1.y*v2.z) - (v1.z*v2.y), (v1.z*v2.x) - (v1.x*v2.z), (v1.x*v2.y) - (v1.y*v2.x));
	}
	/*
		@brief	引数のベクトルとのXY平面における外積(v1×v2)
		@param	[in]	v1	ベクトル
		@param	[in]	v2	ベクトル
		@return	外積
	*/
	static T Cross2(const Vec3<T>& v1, const Vec3<T>& v2)
	{
		return (v1.x*v2.y) - (v1.y * v2.x);
	}
	
	/*
		@brief	回転単位ベクトルの習得
		@param	[in]	angle	角度(unit:radian)
		@return 回転後の単位ベクトル
	*/
	static Vec3<T> Rotate(float angle)
	{
		return Vec3<T>(static_cast<T>(std::cosf(angle)), static_cast<T>(std::sinf(angle)));
	}

#pragma endregion // static methods

};
typedef Vec3<int>	Vec3i;
typedef Vec3<float>	Vec3f;

#pragma endregion // Vec3

//=================================================================================================
//=================================================================================================
//=================================================================================================
#pragma region Line

template <typename T>
class Line
{
public:
	Vec3<T> sta;
	Vec3<T> end;
public:
	Line() :
		sta(0), end(0)
	{}
	Line(const Vec3<T>& s, const Vec3<T>& e) :
		sta(s), end(e)
	{}
	Line(T sx, T sy, T sz, T ex, T ey, T ez) :
		sta(Vec3<T>(sx, sy, sz)), end(Vec3<T>(ex, ey, ez))
	{}
	Line(const T s[3], const T e[3]) :
		sta(Vec3<T>(s[0], s[1], s[2])), end(Vec3<T>(e[0], e[1], e[2]))
	{}

	/*
		@brief	交差判定
		@param	[in]	line	対象線分
		@return	交差しているか
		@retval	true	交差している
		@retval	false	交差していない
	*/
	bool Intersect(const Line<T>& line) const
	{
		Vec3<T> v1 = end - sta;
		Vec3<T> v2 = line.end - line.sta;
		return ((v1 & (line.sta - sta)) | (v1 & (line.end - sta))) < 0.f
			&& ((v2 & (sta - line.sta)) | (v2 & (end - line.sta))) < 0.f;
	}
	/*
		@brief	交差判定
		@param	[in]	_sta	始点
		@param	[in]	_end	終点
		@return	交差しているか
		@retval	true	交差している
		@retval	false	交差していない
	*/
	bool Intersect(const Vec3<T>& _sta, const Vec3<T>& _end) const
	{
		Vec3<T> v1 = end - sta;
		Vec3<T> v2 = _end - _sta;
		return ((v1 & (_sta - sta)) | (v1 & (_end - sta))) < 0.f
			&& ((v2 & (sta - _sta)) | (v2 & (end - _sta))) < 0.f;
	}

	/*
		@brief	交点の取得
			if (Intersect(line))
				point = IntersectionPoint2(line);
		@attention	交差していることが前提
		@param	[in]	line	線分
		@return	交点
	*/
	Vec3<T> IntersectionPoint2(const Line<T>& line) const
	{
		/*
		//Vec3<T> v = line.end - line.sta;
		double d1 = std::fabs( ((line.end-line.sta) & (sta-line.sta)).Length() );
		double d2 = std::fabs( ((line.end-line.sta) & (end-line.sta)).Length() );
		double t = d1 / (d1 + d2);
		return sta + ((end - sta) * t);
		*/
		/*
		double r, s;
		double denominator = (end.x - sta.x) * (line.end.y - line.sta.y) - (end.y - sta.y) * (line.end.x - line.sta.x);
		double numeratorR = (sta.y - line.sta.y) * (line.end.x - line.sta.x) - (sta.x - line.sta.x) * (line.end.y - line.sta.y);
		r = numeratorR / denominator;
		double numeratorS = (sta.y - line.sta.y) * (end.x - sta.x) - (sta.x - line.sta.x) * (end.y - sta.y);
		s = numeratorS / denominator;
		return sta + ((end-sta) * r);
		*/

		double ksi = (line.end.y - line.sta.y) * (line.end.x - sta.x) - (line.end.x - line.sta.x) * (line.end.y - sta.y);
		double delta = (end.x - sta.x) * (line.end.y - line.sta.y) - (end.y - sta.y) * (line.end.x - line.sta.x);
		double ramda = ksi / delta;
		return sta + (end - sta) * static_cast<T>(ramda);

		/*
		double s1 = ( (line.end.x - line.sta.x) * (sta.y - line.sta.y) - (line.end.y - line.sta.y) * (sta.x - line.sta.x)) / 2.0;
		double s2 = ( (line.end.x - line.sta.x) * (line.sta.y - end.y) - (line.end.y - line.sta.y) * (line.sta.x - end.x)) / 2.0;

		return sta + (end-sta) * s1 / (s1+s2);
		*/
	}
	/*
		@brief	交点の取得
			if (Intersect(p1,p2))
				point = IntersectionPoint2(p1,p2);
		@attention	交差していることが前提
		@param	[in]	_sta	始点
		@param	[in]	_end	終点
		@return	交点
	*/
	Vec3<T> IntersectionPoint2(const Vec3<T>& _sta, const Vec3<T>& _end) const
	{
		double ksi = (_end.y - _sta.y) * (_end.x - sta.x) - (_end.x - _sta.x) * (_end.y - sta.y);
		double delta = (end.x - sta.x) * (_end.y - _sta.y) - (end.y - sta.y) * (_end.x - _sta.x);
		double ramda = ksi / delta;
		return sta + (end - sta) * static_cast<T>(ramda);
	}
};
typedef Line<float> Linef;
typedef Line<int>	Linei;

#pragma endregion // Line

//=================================================================================================
//=================================================================================================
//=================================================================================================
#pragma region Shape
template<typename Pol>
class Polygon;
template<typename Rec>
class Rect;
template<typename Cir, typename Rad>
class Circle;
/*
	@brief	図形の基底クラス
*/
template<typename T> class Shape 
{
private:
public:
	virtual ~Shape() = 0{}

	/*
		@brief			図形を水平反転させる
		@return			自分自身
	*/
	virtual Shape<T>& ReverseX() = 0{ return *this; }
	/*
		@brief			図形を垂直反転させる
		@return			自分自身
	*/
	virtual Shape<T>& ReverseY() = 0{ return *this; }

	/*
		@brief			図形を値分ずらす
		@param	[in]	offset	ずらす量
		@return			自分自身
	*/
	virtual Shape<T>& Offset(const Vec3<T>& offset) = 0{ return *this; }

	/*
		@brief			図形を回転させる
		@param	[in]	angle	回転角度(unit:radian)
		@return			自分自身
	*/
	virtual Shape<T>& Rotate(float angle) = 0{ return *this; }

	//================================================================================
#pragma region Contains
	/*
		@brief			点の内包判定
		@param	[in]	point	判定する点
		@return			内包しているか
		@retval true	している
		@retval false	していない
	*/
	virtual bool Contains(const Vec3<T>& point) const{ return false; }

	/*
		@brief			線分の交差、内包判定
		@attension		線分の両端が境界線の中にある場合は第3引数により変わる
						完全内包と境界線交差は内包が優先される
		@param	[in]	line(sta,end)	判定する線分(始点、終点)
		@param	[in]	fullContain		完全内包とするか(デフォルト：false)
		@param	[in]	intersectOnly	境界線との交差のみにするか(デフォルト：false)
		@return			交差、内包しているか
		@retval	true	している
		@retval	false	していない
	*/
	virtual bool Contains(const Line<T>& line, bool fullContain = false, bool intersectOnly = false) const{ return false; }
	
	/*
		@brief			線分の交差、内包判定
		@attension		線分の両端が境界線の中にある場合は第4引数により変わる
						完全内包と境界線交差は内包が優先される
		@param	[in]	sta	判定する線分の始点
		@param	[in]	end	判定する線分の終点
		@param	[in]	fullContain		完全内包とするか(デフォルト：false)
		@param	[in]	intersectOnly	境界線との交差のみにするか(デフォルト：false)
		@return			交差、内包しているか
		@retval	true	している
		@retval	false	していない
	*/
	virtual bool Contains(const Vec3<T>& sta, const Vec3<T>& end, bool fullContain = false, bool intersectOnly = false) const{ return false; }
	
	/*
		@brief			多角形の交差、内包判定
		@attension		線分の両端が境界線の中にある場合は第3引数により変わる
						完全内包と境界線交差は内包が優先される
		@param	[in]	poly			判定する多角形
		@param	[in]	fullContain		完全内包とするか(デフォルト：false)
		@param	[in]	intersectOnly	境界線との交差のみにするか(デフォルト：false)
		@return			交差、内包しているか
		@retval	true	している
		@retval	false	していない
	*/
	virtual bool Contains(const Polygon<T>& poly, bool fullContain = false, bool intersectOnly = false) const{ return false; }
	
	/*
		@brief			矩形の交差、内包判定
		@attension		線分の両端が境界線の中にある場合は第3引数により変わる
						完全内包と境界線交差は内包が優先される
		@param	[in]	rect			判定する矩形
		@param	[in]	fullContain		完全内包とするか(デフォルト：false)
		@param	[in]	intersectOnly	境界線との交差のみにするか(デフォルト：false)
		@return			交差、内包しているか
		@retval	true	している
		@retval	false	していない
	*/
	virtual bool Contains(const Rect<T>& rect, bool fullContain = false, bool intersectOnly = false) const{ return false; }

	/*
		@brief			円の交差、内包判定
		@attension		線分の両端が境界線の中にある場合は第3引数により変わる
						完全内包と境界線交差は内包が優先される
		@param	[in]	circ			判定する円
		@param	[in]	fullContain		完全内包とするか(デフォルト：false)
		@param	[in]	intersectOnly	境界線との交差のみにするか(デフォルト：false)
		@return			交差、内包しているか
		@retval	true	している
		@retval	false	していない
	*/
	virtual bool Contains(const Circle<T, T>& circ, bool fullContain = false, bool intersectOnly = false) const{ return false; }
	
	/*
		@brief			図形の交差、内包判定
		@attension		線分の両端が境界線の中にある場合は第3引数により変わる
		完全内包と境界線交差は内包が優先される
		@param	[in]	shape			判定する図形
		@param	[in]	fullContain		完全内包とするか(デフォルト：false)
		@param	[in]	intersectOnly	境界線との交差のみにするか(デフォルト：false)
		@return			交差、内包しているか
		@retval	true	している
		@retval	false	していない
	*/
	bool Contains(const std::shared_ptr<Shape<T>>& shape, bool fullContain = false, bool intersectOnly = false) const
	{
		const auto& id = typeid(*shape);
		if (id == typeid(Circle<T,T>))
		{
			return Contains(*std::dynamic_pointer_cast<Circle<T,T>>(shape));
		}
		else if (id == typeid(Rect<T>))
		{
			return Contains(*std::dynamic_pointer_cast<Rect<T>>(shape));
		}
		else if (id == typeid(Polygon<T>))
		{
			return Contains(*std::dynamic_pointer_cast<Polygon<T>>(shape));
		}
		return false;
	}



#pragma endregion // Contains
	//================================================================================

	//================================================================================
#pragma region Intersection
	/*
		@brief	内包している点から一番近い交点の取得
			if (Contains(p))
				point = IntersectionPoint2(p);
		@attention	交差していることが前提
		@param	[in]	point	内包している点
		@return	交点
	*/
	virtual Vec3<T> IntersectionPoint2(const Vec3<T>& point) const{return Vec3<T>();}
	
	/*
		@brief	線分との交点の取得
			if (Contains(line))
				points = IntersectionPoint2(line);
		@attention	交差していることが前提
		@param	[in]	line(sta,end)	比較線分
		@return	全交点
	*/
	virtual std::vector<Vec3<T>> IntersectionPoint2(const Line<T>& line) const{return std::vector<Vec3<T>>();}
	
	/*
		@brief	線分との交点の取得
			if (Contains(pos,next))
				points = IntersectionPoint2(pos,next);
		@attention	交差していることが前提
		@param	[in]	sta	比較線分の始点
		@param	[in]	end	比較線分の終点
		@return	全交点
	*/
	virtual std::vector<Vec3<T>> IntersectionPoint2(const Vec3<T>& sta, const Vec3<T>& end) const{return std::vector<Vec3<T>>();}
	
	/*
		@brief	多角形との交点の取得
			if (Contains(polygon))
				points = IntersectionPoint2(polygon);
		@attention	交差していることが前提
		@param	[in]	polygon	比較図形
		@return	全交点
	*/
	virtual std::vector<Vec3<T>> IntersectionPoint2(const Polygon<T>& polygon) const{return std::vector<Vec3<T>>();}
	/*
		@brief	矩形との交点の取得
			if (Contains(rect))
				points = IntersectionPoint2(rect);
		@attention	交差していることが前提
		@param	[in]	rect	比較図形
		@return	全交点
	*/
	virtual std::vector<Vec3<T>> IntersectionPoint2(const Rect<T>& rect) const{return std::vector<Vec3<T>>();}
	/*
		@brief	円との交点の取得
			if (Contains(circle))
				points = IntersectionPoint2(circle);
		@attention	交差していることが前提
		@param	[in]	circle	比較図形
		@return	全交点
	*/
	virtual std::vector<Vec3<T>> IntersectionPoint2(const Circle<T, T>& circle) const{ return std::vector<Vec3<T>>(); }

	/*
		@brief	線分との交点のうち、始点に近い点を取得
			if (Contains(line))
				point = IntersectionPoint2Nearest(line);
		@attention	交差していることが前提
		@param	[in]	line(sta,end)	比較線分
		@return	始点に近い交点
	*/
	virtual Vec3<T> IntersectionPoint2Nearest(const Line<T>& line) const {return Vec3<T>();}
	
	/*
		@brief	線分との交点のうち、始点に近い点を取得
			if (Contains(pos,next))
				point = IntersectionPoint2Nearest(pos,next);
		@attention	交差していることが前提
		@param	[in]	sta	比較線分の始点
		@param	[in]	end	比較線分の終点
		@return	始点に近い交点
	*/
	virtual Vec3<T> IntersectionPoint2Nearest(const Vec3<T>& sta, const Vec3<T>& end) const {return Vec3<T>();}
#pragma endregion // Intersection
	//================================================================================

	/*
		@brief	図形の描画
		@param	[in]	color	線の色(デフォルト:白) 
		@param	[in]	size	線の太さ(デフォルト:1)
		@param	[in]	fill	塗りつぶし(Rectのみ)(デフォルト:true)
		@return	なし
	*/
	virtual void draw(DWORD color = -1, int size = 1, bool fill = true) const
	{
	}


};
typedef	Shape<int>		Shapei;
typedef Shape<float>	Shapef;
typedef std::shared_ptr<Shapef> ShapefPtr;

#pragma endregion // Shape


//=================================================================================================
//=================================================================================================
//=================================================================================================



#pragma region Polygon
/*
	@brief	多角形
*/
template <typename T> class Polygon : public Shape<T>
{
public:
	std::vector<Vec3<T>> points;	// ポリゴンを成形する頂点

public:
	/*
		@param	[in]	vertexNum	頂点数
	*/
	Polygon(size_t vertexNum)
	{
		points.resize(vertexNum);
	}
	/*
		@param	[in]	vertexes	頂点群
	*/
	Polygon(const std::vector<Vec3<T>>& vertexes)
	{
		points = vertexes;
	}
	/*
		@param	[in]	vertexes	頂点群
		@param	[in]	size		頂点数
		@attension		頂点数
	*/
	Polygon(const Vec3<T> vertexes[], size_t size)
	{
		points.clear();
		for (size_t i = 0; i < size, ++i)
		{
			points.push_back(vertexes[i]);
		}
	}
	/*
		@param	[in]	xpoints		頂点X座標
		@param	[in]	ypoints		頂点Y座標
		@param	[in]	size		頂点数
		@attension		xpoints,ypointsの要素数は同じにすること
	*/
	Polygon(const T xpoints[], const T ypoints[], size_t size)
	{
		for (size_t i = 0; i < size; ++i)
		{
			Vec3<T> v(xpoints[i], ypoints[i]);
			points.push_back(v);
		}
	}

	/*
		@param	[in]	xpoints		頂点X座標
		@param	[in]	ypoints		頂点Y座標
		@param	[in]	zpoints		頂点Z座標
		@param	[in]	size		頂点数
		@attension		xpoints,ypoints,zpointsの要素数は同じにすること
	*/
	Polygon(const T xpoints[], const T ypoints[], const T zpoints[], size_t size)
	{
		for (size_t i = 0; i < size; ++i)
		{
			Vec3<T> v(xpoints[i], ypoints[i], zpoints[i]);
			points.push_back(v);
		}
	}
	/*
		@param	[in]	xpoints		頂点X座標
		@param	[in]	ypoints		頂点Y座標
	*/
	Polygon(const std::vector<T>& xpoints, const std::vector<T>& ypoints)
	{
		for (size_t i = 0; i < xpoints.size(); ++i)
		{
			Vec3<T> v(xpoints[i], ypoints[i]);
			points.push_back(v);
		}
	}
	/*
		@param	[in]	xpoints		頂点X座標
		@param	[in]	ypoints		頂点Y座標
		@param	[in]	zpoints		頂点Z座標
	*/
	Polygon(const std::vector<T>& xpoints, const std::vector<T>& ypoints, const std::vector<T>& zpoints)
	{
		for (size_t i = 0; i < xpoints.size(); ++i)
		{
			Vec3<T> v(xpoints[i], ypoints[i], zpoints[i]);
			points.push_back(v);
		}
	}

	/*
		@brief	頂点数を返す
		@return	頂点数
	*/
	size_t size() const
	{
		return points.size();
	}
	
	/*
		@brief			図形を水平反転させる
		@return			自分自身
	*/
	virtual Shape<T>& ReverseX() override
	{
		for (auto& point : points)
			point.x = -point.x;
		return *this;
	}
	/*
		@brief			図形を垂直反転させる
		@return			自分自身
	*/
	virtual Shape<T>& ReverseY() override
	{
		for (auto& point : points)
			point.y = -point.y;
		return *this;
	}

	/*
		@brief	図形を値分ずらす
		@param	[in]	offset	ずらす量
		@return	自分自身
	*/
	virtual Shape<T>& Offset(const Vec3<T>& offset) override
	{
		for (auto& point : points)
		{
			point += offset;
		}
		return *this;
	}

	/*
		@brief			図形を回転させる
		@param	[in]	angle	回転角度(unit:radian)
		@return			自分自身
	*/
	virtual Shape<T>& Rotate(float angle) override
	{
		D3DXMATRIX mt;
		D3DXMatrixRotationZ(&mt, angle);
		D3DXVECTOR3 vec;
		for (auto& point : points)
		{
			vec.x = static_cast<float>(point.x);
			vec.y = static_cast<float>(point.y);
			vec.z = static_cast<float>(point.z);
			D3DXVec3TransformCoord(&vec, &vec, &mt);
			point.x = static_cast<T>(vec.x);
			point.y = static_cast<T>(vec.y);
			point.z = static_cast<T>(vec.z);

			//point = Vec3<T>::Rotate(angle) * point;
		}
		return *this; 
	}


	//================================================================================
#pragma region Contains
	/*
		@brief	点の内包判定
		@param	[in]	point	判定する点
		@return	内包しているか
		@retval true	している
		@retval false	していない
	*/
	virtual bool Contains(const Vec3<T>& point) const override
	{
		int cnt = 0;
		int size = static_cast<int>(points.size());
		for (int i = 0; i < size; ++i)
		{
			int j = (i + 1) % size;
			Vec3<T> vec(points[j] - points[i]);	// ab,bc,ca
			Vec3<T> p_p(point - points[i]);		// ap,bp,cp 
			// 外積の向き判定
			cnt += (Vec3<T>::Cross2(vec, p_p) > static_cast<T>(0)) ? 1 : -1;
		}
		return (cnt == size || cnt == -size);
	}
	/*
		@brief			線分の交差、内包判定
		@attension		線分の両端が境界線の中にある場合は第3引数により変わる
						完全内包と境界線交差は内包が優先される
		@param	[in]	line(sta,end)	判定する線分(始点、終点)
		@param	[in]	fullContain		完全内包とするか(デフォルト：false)
		@param	[in]	intersectOnly	境界線との交差のみにするか(デフォルト：false)
		@return			交差、内包しているか
		@retval	true	している
		@retval	false	していない
	*/
	virtual bool Contains(const Line<T>& line, bool fullContain = false, bool intersectOnly = false) const override
	{
		// 完全内包
		if (fullContain)
		{
			// 始点と終点を内包しており、直線が交差していない場合
			return (Contains(line.sta) && Contains(line.end) && !Contains(line, false, true));
		}
		// 外周との交差判定
		std::vector<Vec3<T>> intersections;
		int size = static_cast<int>(points.size());
		for (int i = 0; i < size; ++i)
		{
			int j = (i + 1) % size;
			if (line.Intersect(points[i], points[j]))
			{
				intersections.push_back(line.IntersectionPoint2(points[i], points[j]));
			}
		}
		// 交点が無かった
		if (intersections.empty())
		{
			// 完全内包チェック(始点または終点が内包されていれば完全内包といえる)
			return !intersectOnly && Contains(line.sta);
		}
		return !intersections.empty();
	}
	/*
		@brief			線分の交差、内包判定
		@attension		線分の両端が境界線の中にある場合は第4引数により変わる
						完全内包と境界線交差は内包が優先される
		@param	[in]	sta	判定する線分の始点
		@param	[in]	end	判定する線分の終点
		@param	[in]	fullContain		完全内包とするか(デフォルト：false)
		@param	[in]	intersectOnly	境界線との交差のみにするか(デフォルト：false)
		@return			交差、内包しているか
		@retval	true	している
		@retval	false	していない
	*/
	virtual bool Contains(const Vec3<T>& sta, const Vec3<T>& end, bool fullContain = false, bool intersectOnly = false) const override
	{
		// 完全内包
		if (fullContain)
		{
			// 始点と終点を内包しており、直線が交差していない場合
			return (Contains(sta) && Contains(end) && !Contains(sta, end, false, true));
		}
		// 外周との交差判定
		std::vector<Vec3<T>> intersections;
		Line<T> line(sta, end);
		int size = static_cast<int>(points.size());
		for (int i = 0; i < size; ++i)
		{
			int j = (i + 1) % size;
			if (line.Intersect(points[i], points[j]))
				intersections.push_back(line.IntersectionPoint2(points[i], points[j]));
		}
		// 交点が無かった
		if (intersections.empty())
		{
			// 完全内包チェック(始点または終点が内包されていれば完全内包といえる)
			return !intersectOnly && Contains(sta);
		}
		return !intersections.empty();
	}
	/*
		@brief			多角形の交差、内包判定
		@attension		線分の両端が境界線の中にある場合は第3引数により変わる
						完全内包と境界線交差は内包が優先される
		@param	[in]	poly			判定する多角形
		@param	[in]	fullContain		完全内包とするか(デフォルト：false)
		@param	[in]	intersectOnly	境界線との交差のみにするか(デフォルト：false)
		@return			交差、内包しているか
		@retval	true	している
		@retval	false	していない
	*/
	virtual bool Contains(const Polygon<T>& poly, bool fullContain = false, bool intersectOnly = false) const override
	{
		int destSize = static_cast<int>(poly.points.size());
		int size = static_cast<int>(points.size());
		int conVer = 0;		// 内包頂点数
		// 頂点内包
		for (int i = 0; i < destSize; ++i)
		{
			bool con = Contains(poly.points[i]);
			if (!intersectOnly && !fullContain && con)
				return true;
			else if (con)
			{
				conVer++;
			}
		}
		if (fullContain)
		{
			return conVer == destSize;
		}

		// 辺交差
		for (int i = 0; i < size; ++i)
		{
			int j = (i + 1) % size;
			Line<T> line(points[i], points[j]);
			for (int k = 0; k < destSize; ++k)
			{
				int l = (k + 1) % destSize;
				if (line.Intersect(poly.points[k], poly.points[l]))
					return true;
			}
		}
		return false;
	}
	/*
		@brief			矩形の交差、内包判定
		@attension		線分の両端が境界線の中にある場合は第3引数により変わる
						完全内包と境界線交差は内包が優先される
		@param	[in]	rect			判定する矩形
		@param	[in]	fullContain		完全内包とするか(デフォルト：false)
		@param	[in]	intersectOnly	境界線との交差のみにするか(デフォルト：false)
		@return			交差、内包しているか
		@retval	true	している
		@retval	false	していない
	*/
	virtual bool Contains(const Rect<T>& rect, bool fullContain = false, bool intersectOnly = false) const override
	{
		bool vertextes[4] = { false, false, false, false };	// 頂点の内包
		// 頂点内包
		Vec3<T> rp[4] = {
			Vec3<T>(rect.left, rect.top, points[0].z),
			Vec3<T>(rect.right, rect.top, points[0].z),
			Vec3<T>(rect.right, rect.bottom, points[0].z),
			Vec3<T>(rect.left, rect.bottom, points[0].z),
		};
		for (int i = 0; i < 4; ++i)
		{
			vertextes[i] = Contains(rp[i]);
			if (!intersectOnly && !fullContain && vertextes[i])
				return true;
		}
		if (fullContain)
			return vertextes[0] && vertextes[1] && vertextes[2] && vertextes[3];

		// 辺交差
		int size = static_cast<int>(points.size());
		for (int i = 0; i < size; ++i)
		{
			int j = (i + 1) % size;
			Line<T> line(points[i], points[j]);
			for (int k = 0; k < 4; ++k)
			{
				int l = (k + 1) % 4;
				if (line.Intersect(rp[k], rp[l]))
					return true;
			}
		}
		return false;
	}

	/*
		@brief			円の交差、内包判定
		@attension		線分の両端が境界線の中にある場合は第3引数により変わる
						完全内包と境界線交差は内包が優先される
		@param	[in]	circ			判定する円
		@param	[in]	fullContain		完全内包とするか(デフォルト：false)
		@param	[in]	intersectOnly	境界線との交差のみにするか(デフォルト：false)
		@return			交差、内包しているか
		@retval	true	している
		@retval	false	していない
	*/
	virtual bool Contains(const Circle<T, T>& circ, bool fullContain = false, bool intersectOnly = false) const override
	{
		// 辺交差
		int size = static_cast<int>(points.size());
		for (int i = 0; i < size; ++i)
		{
			int j = (i + 1) % size;
			Line<T> line(points[i], points[j]);
			if (circ.Contains(line))
			{
				return !fullContain;
			}
		}
		// 完全内包(中心がPolygon内にあり、Circleの中にない)
		if (!intersectOnly && Contains(circ.center) && !circ.Contains(*this))
		{
			return true;
		}

		return false;
	}

#pragma endregion // Contains
	//================================================================================

	//================================================================================
#pragma region Intersection
	/*
		@brief	内包している点から一番近い交点の取得
			if (Contains(p))
				point = IntersectionPoint2(p);
		@attention	交差していることが前提
		@param	[in]	point	内包している点
		@return	交点
	*/
	Vec3<T> IntersectionPoint2(const Vec3<T>& point) const override
	{
		int size = static_cast<int>(points.size());
		std::vector<Vec3<T>> vecs;				// 頂点を一周
		std::vector<Vec3<T>> pointVecs;			// 各頂点 -> point
		std::vector<T> crosses;					// 外積
		std::vector<Vec3<T>> normalVecs;		// 垂線ベクトル
		for (int i = 0; i < size; ++i)
		{
			int j = (i + 1) % size;
			Vec3<T> vec = points[j] - points[i];
			Vec3<T> pv = point - points[j];
			vecs.push_back(vec);
			pointVecs.push_back(pv);
			crosses.push_back(Vec3<T>::Dot2(vec, pv));
			normalVecs.push_back((Vec3<T>::Dot(vec.Normalize2(), pv) * vec.Normalize2()) - pv);
		}

		Vec3<T> work;	// 最も近い垂線ベクトル格納用
		// min( min(ab, bc), ca )
		work = normalVecs[0];
		for (int i = 1; i < size; ++i)
		{
			work = (PYTHA(work.x, work.y) < PYTHA(normalVecs[i].x, normalVecs[i].y)) ? work : normalVecs[i];
		}
		return (work + point);

	}

	/*
		@brief	線分との交点の取得
			if (Contains(line))
				points = IntersectionPoint2(line);
		@attention	交差していることが前提
		@param	[in]	line(sta,end)	比較線分
		@return	全交点
	*/
	std::vector<Vec3<T>> IntersectionPoint2(const Line<T>& line) const override
	{
		std::vector<Vec3<T>> intersections;
		int size = static_cast<int>(points.size());
		for (int i = 0; i < size; ++i)
		{
			int j = (i + 1) % size;
			if (line.Intersect(points[i], points[j]))
				intersections.push_back(line.IntersectionPoint2(points[i], points[j]));
		}
		return intersections;
	}
	/*
		@brief	線分との交点の取得
			if (Contains(pos,next))
				points = IntersectionPoint2(pos,next);
		@attention	交差していることが前提
		@param	[in]	sta	比較線分の始点
		@param	[in]	end	比較線分の終点
		@return	全交点
	*/
	std::vector<Vec3<T>> IntersectionPoint2(const Vec3<T>& sta, const Vec3<T>& end) const override
	{
		std::vector<Vec3<T>> intersections;
		Line<T> line(sta, end);
		int size = static_cast<int>(points.size());
		for (int i = 0; i < size; ++i)
		{
			int j = (i + 1) % size;
			if (line.Intersect(points[i], points[j]))
				intersections.push_back(line.IntersectionPoint2(points[i], points[j]));
		}
		return intersections;
	}

		
	/*
		@brief	多角形との交点の取得
			if (Contains(polygon))
				points = IntersectionPoint2(polygon);
		@attention	交差していることが前提
		@param	[in]	polygon	比較図形
		@return	全交点
	*/
	virtual std::vector<Vec3<T>> IntersectionPoint2(const Polygon<T>& polygon) const override
	{
		std::vector<Vec3<T>> intersections;
		int destSize = static_cast<int>(polygon.points.size());
		int size = static_cast<int>(points.size());
		// 辺交差
		intersections.reserve(size);
		for (int i = 0; i < size; ++i)
		{
			int j = (i + 1) % size;
			Line<T> line(points[i], points[j]);
			for (int k = 0; k < destSize; ++k)
			{
				int l = (k + 1) % destSize;
				if (line.Intersect(polygon.points[k], polygon.points[l]))
					intersections.push_back(line.IntersectionPoint2(polygon.points[k], polygon.points[l]));
			}
		}
		return intersections;
	}
	/*
		@brief	矩形との交点の取得
			if (Contains(rect))
				points = IntersectionPoint2(rect);
		@attention	交差していることが前提
		@param	[in]	rect	比較図形
		@return	全交点
	*/
	virtual std::vector<Vec3<T>> IntersectionPoint2(const Rect<T>& rect) const override
	{
		std::vector<Vec3<T>> intersections;
		// 頂点内包
		Vec3<T> rp[4] = {
			Vec3<T>(rect.left, rect.top, points[0].z),
			Vec3<T>(rect.right, rect.top, points[0].z),
			Vec3<T>(rect.right, rect.bottom, points[0].z),
			Vec3<T>(rect.left, rect.bottom, points[0].z),
		};
		// 辺交差
		int size = static_cast<int>(points.size());
		intersections.reserve(size);
		for (int i = 0; i < size; ++i)
		{
			int j = (i + 1) % size;
			Line<T> line(points[i], points[j]);
			for (int k = 0; k < 4; ++k)
			{
				int l = (k + 1) % 4;
				if (line.Intersect(rp[k], rp[l]))
					intersections.push_back(line.IntersectionPoint2(rp[k], rp[l]));
			}
		}
		return intersections;
	}
	/*
		@brief	円との交点の取得
			if (Contains(circle))
				points = IntersectionPoint2(circle);
		@attention	交差していることが前提
		@param	[in]	circle	比較図形
		@return	全交点
	*/
	virtual std::vector<Vec3<T>> IntersectionPoint2(const Circle<T, T>& circle) const override
	{
		std::vector<Vec3<T>> intersections;
		// 辺交差
		int size = static_cast<int>(points.size());
		intersections.reserve(size);
		for (int i = 0; i < size; ++i)
		{
			int j = (i + 1) % size;
			Line<T> line(points[i], points[j]);
			if (circle.Contains(line))
			{
				auto inter = (circle.IntersectionPoint2(line));
				intersections.insert(intersections.end(), inter.begin(), inter.end());
			}
			
		}
		return intersections;
	}



	/*
		@brief	線分との交点のうち、始点に近い点を取得
			if (Contains(line))
				point = IntersectionPoint2Nearest(line);
		@attention	交差していることが前提
		@param	[in]	line(sta,end)	比較線分
		@return	始点に近い交点
	*/
	Vec3<T> IntersectionPoint2Nearest(const Line<T>& line) const override
	{
		std::vector<Vec3<T>> intersections;
		int size = static_cast<int>(points.size());
		for (int i = 0; i < size; ++i)
		{
			int j = (i + 1) % size;
			if (line.Intersect(points[i], points[j]))
				intersections.push_back(line.IntersectionPoint2(points[i], points[j]));
		}
		// 始点に一番近い交点の算出
		Vec3<T> min = intersections[0];
		for (size_t i = 1; i < intersections.size(); ++i)
		{
			Vec3<T> d1 = min - line.sta;
			Vec3<T> d2 = intersections[i] - line.sta;
			if (PYTHA(d1.x, d1.y) > PYTHA(d2.x, d2.y))
				min = intersections[i];
		}
		return min;
	}
	/*
		@brief	線分との交点のうち、始点に近い点を取得
			if (Contains(pos,next))
				point = IntersectionPoint2Nearest(pos,next);
		@attention	交差していることが前提
		@param	[in]	sta	比較線分の始点
		@param	[in]	end	比較線分の終点
		@return	始点に近い交点
	*/
	Vec3<T> IntersectionPoint2Nearest(const Vec3<T>& sta, const Vec3<T>& end) const override
	{
		std::vector<Vec3<T>> intersections;
		Line<T> line(sta, end);
		int size = static_cast<int>(points.size());
		for (int i = 0; i < size; ++i)
		{
			int j = (i + 1) % size;
			if (line.Intersect(points[i], points[j]))
				intersections.push_back(line.IntersectionPoint2(points[i], points[j]));
		}
		// 始点に一番近い交点の算出
		Vec3<T> min = intersections[0];
		for (size_t i = 1; i < intersections.size(); ++i)
		{
			Vec3<T> d1 = min - sta;
			Vec3<T> d2 = intersections[i] - sta;
			if (PYTHA(d1.x, d1.y) > PYTHA(d2.x, d2.y))
				min = intersections[i];
		}
		return min;
	}
#pragma endregion // Intersection
	//================================================================================

#ifdef DEF_SHAPE_DRAW
	
	/*
		@brief	図形の描画
		@param	[in]	color	線の色(デフォルト:白) 
		@param	[in]	size	線の太さ(デフォルト:1)
		@param	[in]	fill	塗りつぶし(Rectのみ)(デフォルト:true)
		@return	なし
	*/
	virtual void draw(DWORD color = -1, int size = 1, bool fill = true) const override
	{
		for (size_t i = 0; i < points.size(); ++i)
		{
			size_t j = (i + 1) % points.size();
			graph::Draw_Line(
				static_cast<int>(points[i].x),
				static_cast<int>(points[i].y),
				static_cast<int>(points[j].x),
				static_cast<int>(points[j].y),
				static_cast<float>(points[i].z),
				color,
				size);
		}
	}
#endif
};
typedef Polygon<float> Polyf;
typedef Polygon<int> Polyi;

#pragma endregion // Polygon


//=================================================================================================
//=================================================================================================
//=================================================================================================
#pragma region Rect
/*
	@brief	矩形
*/
template<typename T> class Rect : public Shape<T>
{
public:
	T left, top, right, bottom;
	Rect(T l=0, T t=0, T r=0, T b=0):
		left(l),top(t),
		right(r),bottom(b)
	{}
	Rect(const Vec3<T>& leftTop, const Vec3<T>& rightBottom):
		left(leftTop.x), top(leftTop.y),
		right(rightBottom.x), bottom(rightBottom.y)
	{}
	Rect(const RECT& rect) :
		left((T)rect.left), top((T)rect.top),
		right((T)rect.right), bottom((T)rect.bottom)
	{}
	

	template<class T1>
	Rect<T>& operator = (const Rect<T1>& rt)
	{
		left = static_cast<T>(rt.left);
		top = static_cast<T>(rt.top);
		right = static_cast<T>(rt.right);
		bottom = static_cast<T>(rt.bottom);
		return *this;
	}

	template<class T1>
	operator Rect<T1>()
	{
		Rect<T1> rt(static_cast<T1>(left),
			static_cast<T1>(top),
			static_cast<T1>(right),
			static_cast<T1>(bottom));

		return rt;
	}
	
	/*
		@brief			図形を水平反転させる
		@return			自分自身
	*/
	virtual Shape<T>& ReverseX() override
	{
		left = -left;
		right = -right;
		return *this;
	}
	/*
		@brief			図形を垂直反転させる
		@return			自分自身
	*/
	virtual Shape<T>& ReverseY() override
	{
		top = -top;
		bottom = -bottom;
		return *this;
	}

	/*
		@brief	図形を値分ずらす
		@param	[in]	offset	ずらす量
		@return	自分自身
	*/
	virtual Shape<T>& Offset(const Vec3<T>& offset) override
	{
		left += offset.x;
		right += offset.x;
		top += offset.y;
		bottom += offset.y;
		return *this;
	}

	/*
		@brief			図形を回転させる
		@attention		Rectは回転しない。回転する四角形を使う場合はPolygonで作ること
		@param	[in]	angle	回転角度(unit:radian)
		@return			自分自身
	*/
	virtual Shape<T>& Rotate(float angle) override
	{
		return *this; 
	}

	//================================================================================
#pragma region Contains
	/*
		@brief	点の内包判定
		@param	[in]	point	判定する点
		@return	内包しているか
		@retval true	している
		@retval false	していない
	*/
	virtual bool Contains(const Vec3<T>& point) const override
	{
		return left <= point.x && point.x <= right
			&& top <= point.y && point.y <= bottom;
	}
	/*
		@brief			線分の交差、内包判定
		@attension		線分の両端が境界線の中にある場合は第3引数により変わる
						完全内包と境界線交差は内包が優先される
		@param	[in]	line(sta,end)	判定する線分(始点、終点)
		@param	[in]	fullContain		完全内包とするか(デフォルト：false)
		@param	[in]	intersectOnly	境界線との交差のみにするか(デフォルト：false)
		@return			交差、内包しているか
		@retval	true	している
		@retval	false	していない
	*/
	virtual bool Contains(const Line<T>& line, bool fullContain = false, bool intersectOnly = false) const override
	{
		// 完全内包
		if (fullContain)
		{
			return (Contains(line.sta) && Contains(line.end));
		}
		// 外周との交差判定
		bool intersection = false;
		Vec3<T> points[4] = {
			Vec3<T>(left, top, line.sta.z),
			Vec3<T>(right, top, line.sta.z),
			Vec3<T>(right, bottom, line.sta.z),
			Vec3<T>(left, bottom, line.sta.z),
		};
		for (int i = 0; i < 4; ++i)
		{
			int j = (i + 1) % 4;
			if (line.Intersect(points[i], points[j]))
			{
				intersection = true;
				break;
			}
		}
		// 交点が無かった
		if (!intersection)
		{
			// 完全内包チェック(始点または終点が内包されていれば完全内包といえる)
			return !intersectOnly && Contains(line.sta);
		}
		return intersection;
	}
	/*
		@brief			線分の交差、内包判定
		@attension		線分の両端が境界線の中にある場合は第4引数により変わる
						完全内包と境界線交差は内包が優先される
		@param	[in]	sta	判定する線分の始点
		@param	[in]	end	判定する線分の終点
		@param	[in]	fullContain		完全内包とするか(デフォルト：false)
		@param	[in]	intersectOnly	境界線との交差のみにするか(デフォルト：false)
		@return			交差、内包しているか
		@retval	true	している
		@retval	false	していない
	*/
	virtual bool Contains(const Vec3<T>& sta, const Vec3<T>& end, bool fullContain = false, bool intersectOnly = false) const override
	{
		// 完全内包
		if (fullContain)
		{
			return (Contains(sta) && Contains(end));
		}
		// 外周との交差判定
		bool intersection = false;
		Line<T> line(sta, end);
		Vec3<T> points[4] = {
			Vec3<T>(left, top, line.sta.z),
			Vec3<T>(right, top, line.sta.z),
			Vec3<T>(right, bottom, line.sta.z),
			Vec3<T>(left, bottom, line.sta.z),
		};
		for (int i = 0; i < 4; ++i)
		{
			int j = (i + 1) % 4;
			if (line.Intersect(points[i], points[j]))
			{
				intersection = true;
				break;
			}
		}
		// 交点が無かった
		if (!intersection)
		{
			// 完全内包チェック(始点または終点が内包されていれば完全内包といえる)
			return !intersectOnly && Contains(sta);
		}
		return intersection;
	}
	/*
		@brief			多角形の交差、内包判定
		@attension		線分の両端が境界線の中にある場合は第3引数により変わる
						完全内包と境界線交差は内包が優先される
		@param	[in]	poly			判定する多角形
		@param	[in]	fullContain		完全内包とするか(デフォルト：false)
		@param	[in]	intersectOnly	境界線との交差のみにするか(デフォルト：false)
		@return			交差、内包しているか
		@retval	true	している
		@retval	false	していない
	*/
	virtual bool Contains(const Polygon<T>& poly, bool fullContain = false, bool intersectOnly = false) const override
	{
		int size = static_cast<int>(poly.points.size());
		int conVer = 0;				// 内包頂点数
		// 頂点内包
		for (int i = 0; i < size; ++i)
		{
			bool con = Contains(poly.points[i]);
			if (!intersectOnly && !fullContain && con)
				return true;
			else if (con)
			{
				conVer++;
			}
		}
		if (fullContain)
		{
			return conVer == size;
		}

		Vec3<T> points[4] = {
			Vec3<T>(left, top, poly.points[0].z),
			Vec3<T>(right, top, poly.points[0].z),
			Vec3<T>(right, bottom, poly.points[0].z),
			Vec3<T>(left, bottom, poly.points[0].z),
		};
		// 辺交差
		for (int i = 0; i < 4; ++i)
		{
			int j = (i + 1) % 4;
			Line<T> line(points[i], points[j]);
			for (int k = 0; k < size; ++k)
			{
				int l = (k + 1) % size;
				if (line.Intersect(poly.points[k], poly.points[l]))
					return true;
			}
		}
		return false;
	}
	/*
		@brief			矩形の交差、内包判定
		@attension		線分の両端が境界線の中にある場合は第3引数により変わる
						完全内包と境界線交差は内包が優先される
		@param	[in]	rect			判定する矩形
		@param	[in]	fullContain		完全内包とするか(デフォルト：false)
		@param	[in]	intersectOnly	境界線との交差のみにするか(デフォルト：false)
		@return			交差、内包しているか
		@retval	true	している
		@retval	false	していない
	*/
	virtual bool Contains(const Rect<T>& rect, bool fullContain = false, bool intersectOnly = false) const override
	{
		if (fullContain || intersectOnly)
		{
			Vec3<T> rp[4] = {
				Vec3<T>(rect.left, rect.top, 0),
				Vec3<T>(rect.right, rect.top, 0),
				Vec3<T>(rect.right, rect.bottom, 0),
				Vec3<T>(rect.left, rect.bottom, 0),
			};
			// 頂点内包
			int conVer = 0;
			for (int i = 0; i < 4; ++i)
			{
				if (Contains(rp[i]))
					conVer++;
			}
			if (fullContain)
				return conVer == 4;
			for (int i = 0; i < 4; ++i)
			{
				int j = (i + 1) % 4;
				if (Contains(rp[i], rp[j]))
					return true;
			}
		}
		else
		{
			if (right < rect.left) return false;
			if (rect.right < left) return false;
			if (bottom < rect.top) return false;
			if (rect.bottom < top) return false;
			return true;
		}
		return false;
	}

	/*
		@brief			円の交差、内包判定
		@attension		線分の両端が境界線の中にある場合は第3引数により変わる
						完全内包と境界線交差は内包が優先される
		@param	[in]	circ			判定する円
		@param	[in]	fullContain		完全内包とするか(デフォルト：false)
		@param	[in]	intersectOnly	境界線との交差のみにするか(デフォルト：false)
		@return			交差、内包しているか
		@retval	true	している
		@retval	false	していない
	*/
	virtual bool Contains(const Circle<T, T>& circ, bool fullContain = false, bool intersectOnly = false) const override
	{
		Vec3<T> points[4] = {
			Vec3<T>(left, top, circ.center.z),
			Vec3<T>(right, top, circ.center.z),
			Vec3<T>(right, bottom, circ.center.z),
			Vec3<T>(left, bottom, circ.center.z),
		};
		// 辺交差;
		for (int i = 0; i < 4; ++i)
		{
			int j = (i + 1) % 4;
			Line<T> line(points[i], points[j]);
			if (circ.Contains(line))
			{
				return !fullContain;
			}
		}
		// 完全内包(中心がPolygon内にあり、Circleの中にない)
		if (!intersectOnly && Contains(circ.center) && !circ.Contains(*this))
		{
			return true;
		}

		return false;

	}

#pragma endregion // Contains
	//================================================================================
	
	//================================================================================
#pragma region Intersection
	/*
		@brief	内包している点から一番近い交点の取得
			if (Contains(p))
				point = IntersectionPoint2(p);
		@attention	交差していることが前提
		@param	[in]	point	内包している点
		@return	交点
	*/
	Vec3<T> IntersectionPoint2(const Vec3<T>& point) const override
	{
		Vec3<T> points[4] = {
			Vec3<T>(left, top, point.z),
			Vec3<T>(right, top, point.z),
			Vec3<T>(right, bottom, point.z),
			Vec3<T>(left, bottom, point.z),
		};
		Vec3<T> ab(points[1] - points[0]);	// p0 -> p1
		Vec3<T> bp(point - points[1]);		// p1 -> point

		Vec3<T> bc(points[2] - points[1]);	// p1 -> p2
		Vec3<T> cp(point - points[2]);		// p2 -> point

		Vec3<T> cd(points[3] - points[2]);	// p2 -> p3
		Vec3<T> dp(point - points[3]);		// p3 -> point

		Vec3<T> da(points[0] - points[3]);	// p3 -> p0
		Vec3<T> ap(point - points[0]);		// p0 -> point


		// z成分のみの外積
		T c1 = Vec3<T>::Cross2(ab, bp);
		T c2 = Vec3<T>::Cross2(bc, cp);
		T c3 = Vec3<T>::Cross2(cd, dp);
		T c4 = Vec3<T>::Cross2(da, ap);

		Vec3<T> d_ab((Vec3<T>::Dot(ab.Normalize2(), bp) * ab.Normalize2()) - bp);
		Vec3<T> d_bc((Vec3<T>::Dot(bc.Normalize2(), cp) * bc.Normalize2()) - cp);
		Vec3<T> d_cd((Vec3<T>::Dot(cd.Normalize2(), dp) * cd.Normalize2()) - dp);
		Vec3<T> d_da((Vec3<T>::Dot(da.Normalize2(), ap) * da.Normalize2()) - ap);
		Vec3<T> work;	// 最も近い垂線ベクトル格納用
		// min( min( min(ab, bc), cd ) , da)
		work = (PYTHA(d_ab.x, d_ab.y) < PYTHA(d_bc.x, d_bc.y)) ? d_ab : d_bc;
		work = (PYTHA(work.x, work.y) < PYTHA(d_cd.x, d_cd.y)) ? work : d_cd;
		work = (PYTHA(work.x, work.y) < PYTHA(d_da.x, d_da.y)) ? work : d_da;
		return (work + point);

	}

	/*
		@brief	線分との交点の取得
			if (Contains(line))
				points = IntersectionPoint2(line);
		@attention	交差していることが前提
		@param	[in]	line(sta,end)	比較線分
		@return	全交点
	*/
	std::vector<Vec3<T>> IntersectionPoint2(const Line<T>& line) const override
	{
		std::vector<Vec3<T>> intersections;
		Vec3<T> points[4] = {
			Vec3<T>(left, top, line.sta.z),
			Vec3<T>(right, top, line.sta.z),
			Vec3<T>(right, bottom, line.sta.z),
			Vec3<T>(left, bottom, line.sta.z),
		};
		for (int i = 0; i < 4; ++i)
		{
			int j = (i + 1) % 4;
			if (line.Intersect(points[i], points[j]))
				intersections.push_back(line.IntersectionPoint2(points[i], points[j]));
		}
		return intersections;
	}
	/*
		@brief	線分との交点の取得
			if (Contains(pos,next))
				points = IntersectionPoint2(pos,next);
		@attention	交差していることが前提
		@param	[in]	sta	比較線分の始点
		@param	[in]	end	比較線分の終点
		@return	全交点
	*/
	std::vector<Vec3<T>> IntersectionPoint2(const Vec3<T>& sta, const Vec3<T>& end) const override
	{
		std::vector<Vec3<T>> intersections;
		Line<T> line(sta, end);
		Vec3<T> points[4] = {
			Vec3<T>(left, top, line.sta.z),
			Vec3<T>(right, top, line.sta.z),
			Vec3<T>(right, bottom, line.sta.z),
			Vec3<T>(left, bottom, line.sta.z),
		};
		for (int i = 0; i < 4; ++i)
		{
			int j = (i + 1) % 4;
			if (line.Intersect(points[i], points[j]))
				intersections.push_back(line.IntersectionPoint2(points[i], points[j]));
		}
		return intersections;
	}

		
	/*
		@brief	多角形との交点の取得
			if (Contains(polygon))
				points = IntersectionPoint2(polygon);
		@attention	交差していることが前提
		@param	[in]	polygon	比較図形
		@return	全交点
	*/
	virtual std::vector<Vec3<T>> IntersectionPoint2(const Polygon<T>& polygon) const override
	{
		std::vector<Vec3<T>> intersections;
		int destSize = static_cast<int>(polygon.points.size());
		int size = 4;
		intersections.reserve(size);
		// 辺交差
		Vec3<T> points[4] = {
			Vec3<T>(left, top, polygon.points[0].z),
			Vec3<T>(right, top, polygon.points[0].z),
			Vec3<T>(right, bottom, polygon.points[0].z),
			Vec3<T>(left, bottom, polygon.points[0].z),
		};
		for (int i = 0; i < size; ++i)
		{
			int j = (i + 1) % size;
			Line<T> line(points[i], points[j]);
			for (int k = 0; k < destSize; ++k)
			{
				int l = (k + 1) % destSize;
				if (line.Intersect(polygon.points[k], polygon.points[l]))
				{
					intersections.push_back(line.IntersectionPoint2(polygon.points[k], polygon.points[l]));
					
				}
			}
		}
		return intersections;
	}
	/*
		@brief	矩形との交点の取得
			if (Contains(rect))
				points = IntersectionPoint2(rect);
		@attention	交差していることが前提
		@param	[in]	rect	比較図形
		@return	全交点
	*/
	virtual std::vector<Vec3<T>> IntersectionPoint2(const Rect<T>& rect) const override
	{
		std::vector<Vec3<T>> intersections;
		// 頂点内包
		Vec3<T> rp[4] = {
			Vec3<T>(rect.left, rect.top, 0),
			Vec3<T>(rect.right, rect.top, 0),
			Vec3<T>(rect.right, rect.bottom, 0),
			Vec3<T>(rect.left, rect.bottom, 0),
		};
		// 辺交差
		int size = 4;
		intersections.reserve(size);
		for (int i = 0; i < size; ++i)
		{
			int j = (i + 1) % size;
			if (Contains(rp[i], rp[j]))
			{
				auto inter = IntersectionPoint2(rp[i], rp[j]);
				intersections.insert(intersections.end(), inter.begin(), inter.end());
			}
		}
		return intersections;
	}
	/*
		@brief	円との交点の取得
			if (Contains(circle))
				points = IntersectionPoint2(circle);
		@attention	交差していることが前提
		@param	[in]	circle	比較図形
		@return	全交点
	*/
	virtual std::vector<Vec3<T>> IntersectionPoint2(const Circle<T, T>& circle) const override
	{
		std::vector<Vec3<T>> intersections;
		// 辺交差
		Vec3<T> points[4] = {
			Vec3<T>(left, top, circle.center.z),
			Vec3<T>(right, top, circle.center.z),
			Vec3<T>(right, bottom, circle.center.z),
			Vec3<T>(left, bottom, circle.center.z),
		};
		int size = 4;
		intersections.reserve(size*2);
		for (int i = 0; i < size; ++i)
		{
			int j = (i + 1) % size;
			Line<T> line(points[i], points[j]);
			if (circle.Contains(line))
			{

				auto inter = (circle.IntersectionPoint2(line));
				intersections.insert(intersections.end(), inter.begin(), inter.end());
			}
			
		}
		return intersections;
	}



	/*
		@brief	線分との交点のうち、始点に近い点を取得
			if (Contains(line))
				point = IntersectionPoint2Nearest(line);
		@attention	交差していることが前提
		@param	[in]	line(sta,end)	比較線分
		@return	始点に近い交点
	*/
	Vec3<T> IntersectionPoint2Nearest(const Line<T>& line) const override
	{
		std::vector<Vec3<T>> intersections;
		Vec3<T> points[4] = {
			Vec3<T>(left, top, line.sta.z),
			Vec3<T>(right, top, line.sta.z),
			Vec3<T>(right, bottom, line.sta.z),
			Vec3<T>(left, bottom, line.sta.z),
		};
		for (int i = 0; i < 4; ++i)
		{
			int j = (i + 1) % 4;
			if (line.Intersect(points[i], points[j]))
				intersections.push_back(line.IntersectionPoint2(points[i], points[j]));
		}
		// 始点に一番近い交点の算出
		Vec3<T> min = intersections[0];
		for (size_t i = 1; i < intersections.size(); ++i)
		{
			Vec3<T> d1 = min - line.sta;
			Vec3<T> d2 = intersections[i] - line.sta;
			if (PYTHA(d1.x, d1.y) > PYTHA(d2.x, d2.y))
				min = intersections[i];
		}
		return min;
	}
	/*
		@brief	線分との交点のうち、始点に近い点を取得
			if (Contains(pos,next))
				point = IntersectionPoint2Nearest(pos,next);
		@attention	交差していることが前提
		@param	[in]	sta	比較線分の始点
		@param	[in]	end	比較線分の終点
		@return	始点に近い交点
	*/
	Vec3<T> IntersectionPoint2Nearest(const Vec3<T>& sta, const Vec3<T>& end) const override
	{
		std::vector<Vec3<T>> intersections;
		Line<T> line(sta, end);
		Vec3<T> points[4] = {
			Vec3<T>(left, top, line.sta.z),
			Vec3<T>(right, top, line.sta.z),
			Vec3<T>(right, bottom, line.sta.z),
			Vec3<T>(left, bottom, line.sta.z),
		};
		for (int i = 0; i < 4; ++i)
		{
			int j = (i + 1) % 4;
			if (line.Intersect(points[i], points[j]))
				intersections.push_back(line.IntersectionPoint2(points[i], points[j]));
		}
		// 始点に一番近い交点の算出
		Vec3<T> min = intersections[0];
		for (size_t i = 1; i < intersections.size(); ++i)
		{
			Vec3<T> d1 = min - sta;
			Vec3<T> d2 = intersections[i] - sta;
			if (PYTHA(d1.x, d1.y) > PYTHA(d2.x, d2.y))
				min = intersections[i];
		}
		return min;
	}
#pragma endregion // Intersection
	//================================================================================

#ifdef DEF_SHAPE_DRAW
	
	/*
		@brief	図形の描画
		@param	[in]	color	線の色(デフォルト:白) 
		@param	[in]	size	線の太さ(デフォルト:1)
		@param	[in]	fill	塗りつぶし(Rectのみ)(デフォルト:true)
		@return	なし
	*/
	virtual void draw(DWORD color = -1, int size = 1, bool fill = true) const override
	{
		graph::Draw_Box(
			static_cast<int>(left),
			static_cast<int>(top),
			static_cast<int>(right),
			static_cast<int>(bottom),
			0.0f,
			color, color, size, fill);

	}
#endif
};
typedef Rect<int>	Recti;
typedef Rect<float>	Rectf;

#pragma endregion // Rect

//=================================================================================================
//=================================================================================================
//=================================================================================================
#pragma region Circle

/*
	@brief	円
	Vec3<T1> center
	T2	radius
*/
template<typename T1, typename T2> class Circle : public Shape<T1>
{
public:
	Vec3<T1> center;
	T2 radius;		// 大きさ(半径)
public:
	Circle() :center(Vec3<T1>()), radius(0)
	{}
	Circle(T1 x, T1 y, T1 z, T2 r) :
		center(Vec3<T1>(x, y, z)), radius(r)
	{}
	Circle(const Vec3<T1>& c, T2 r) :
		center(c), radius(r)
	{}

	
	/*
		@brief			図形を水平反転させる
		@attention		真円なので意味なし。
		@return			自分自身
	*/
	virtual Shape<T1>& ReverseX() override
	{
		return *this;
	}
	/*
		@brief			図形を垂直反転させる
		@attention		真円なので意味なし。
		@return			自分自身
	*/
	virtual Shape<T1>& ReverseY() override
	{
		return *this;
	}

	/*
		@brief	図形を値分ずらす
		@param	[in]	offset	ずらす量
		@return	自分自身
	*/
	virtual Shape<T1>& Offset(const Vec3<T1>& offset) override
	{
		center += offset;
		return *this;
	}

	/*
		@brief			図形を回転させる
		@attention		真円なので意味なし。
		@param	[in]	angle	回転角度(unit:radian)
		@return			自分自身
	*/
	virtual Shape<T1>& Rotate(float angle) override
	{
		return *this; 
	}


	//================================================================================
#pragma region Contains
	/*
		@brief	点の内包判定
		@param	[in]	point	判定する点
		@return	内包しているか
		@retval	true	内包している
		@retval	false	内包していない
	*/
	bool Contains(const Vec3<T1>& point) const override
	{
	 const Vec3<T1>& c = center;
	 const Vec3<T1>& p = point;
		return (PYTHA(p.x - c.x, p.y - c.y) <= POW2(radius));
	}

	/*
		@brief			線分の内包判定
		@attension		線分の両端が円の中にある場合は第3引数により変わる
						完全内包と円周交差は内包が優先される
		@param	[in]	line(sta,end)	判定する線分(始点、終点)
		@param	[in]	fullContain		完全内包とするか(デフォルト：false)
		@param	[in]	intersectOnly	円周との交差のみにするか(デフォルト：false)
		@return			内包しているか
		@retval	true	している
		@retval	false	していない
	*/
	virtual bool Contains(const Line<T1>& line, bool fullContain = false, bool intersectOnly = false) const override
	{
		Vec3<T1> sp(center - line.sta); // 線分始点から円中心へのベクトル(start->pos)
		Vec3<T1> ep(center - line.end); // 線分終点から円中心へのベクトル(end->pos)
		bool isInner = (PYTHA(sp.x, sp.y) < POW2(radius))
			&& (PYTHA(ep.x, ep.y) < POW2(radius));
		// 完全内包(円の内側)
		if (fullContain)
		{
			return isInner;
		}
		if (isInner)
		{
			return !intersectOnly;
		}
		Vec3<T1> nv((line.end - line.sta).Normalize2());	// 線分の単位ベクトル(normalize-vector)
		// 円中心から線分への垂線 d=((n・p)*n)-p
		Vec3<T1> dist((Vec3<T1>::Dot(nv, sp) * nv) - sp);

		if (PYTHA(dist.x, dist.y) > POW2(radius))
		{
			// 垂線判定(円の外側)
			return false;
		}
		else if ((PYTHA(sp.x, sp.y) > POW2(radius) && PYTHA(ep.x, ep.y) > POW2(radius))
			&& (((sp.x > 0.f) ? 1.f : -1.f) == ((ep.x > 0.f) ? 1.f : -1.f))
			&& (((sp.y > 0.f) ? 1.f : -1.f) == ((ep.y > 0.f) ? 1.f : -1.f)))
		{
			// 符号判定(中心点をまたがない)
			return false;
		}
		return true;
	}
	/*
		@brief			線分の内包判定
		@attension		線分の両端が円の中にある場合は第4引数により変わる
						完全内包と円周交差は内包が優先される
		@param	[in]	sta	判定する線分の始点
		@param	[in]	end	判定する線分の終点
		@param	[in]	fullContain		完全内包とするか(デフォルト：false)
		@param	[in]	intersectOnly	円周との交差のみにするか(デフォルト：false)
		@return			内包しているか
		@retval	true	している
		@retval	false	していない
	*/
	virtual bool Contains(const Vec3<T1>& sta, const Vec3<T1>& end, bool fullContain=false, bool intersectOnly=false) const override
	{
		Vec3<T1> sp(center - sta); // 線分始点から円中心へのベクトル
		Vec3<T1> ep(center - end); // 線分終点から円中心へのベクトル
		bool isInner = (PYTHA(sp.x, sp.y) < POW2(radius))
			&& (PYTHA(ep.x, ep.y) < POW2(radius));
		// 完全内包(円の内側)
		if (fullContain)
		{
			return isInner;
		}
		if (isInner)
		{
			return !intersectOnly;
		}
		Vec3<T1> nv((end - sta).Normalize2());	// 線分の単位ベクトル
		// 円中心から線分への垂線 d=((n・p)n)-p
		Vec3<T1> dist((Vec3<T1>::Dot(nv, sp) * nv) - sp);
		if (PYTHA(dist.x, dist.y) > POW2(radius))
		{
			// 垂線判定(円の外側)
			return false;
		}
		else if ((PYTHA(sp.x, sp.y) > POW2(radius) && PYTHA(ep.x, ep.y) > POW2(radius))
			&& (((sp.x > 0.f) ? 1.f : -1.f) == ((ep.x > 0.f) ? 1.f : -1.f))
			&& (((sp.y > 0.f) ? 1.f : -1.f) == ((ep.y > 0.f) ? 1.f : -1.f)))
		{
			// 符号判定(中心点をまたがない)
			return false;
		}
		return true;
	}



	/*
		@brief			多角形の交差、内包判定
		@attension		線分の両端が境界線の中にある場合は第3引数により変わる
						完全内包と境界線交差は内包が優先される
		@param	[in]	poly			判定する多角形
		@param	[in]	fullContain		完全内包とするか(デフォルト：false)
		@param	[in]	intersectOnly	境界線との交差のみにするか(デフォルト：false)
		@return			交差、内包しているか
		@retval	true	している
		@retval	false	していない
	*/
	virtual bool Contains(const Polygon<T1>& poly, bool fullContain = false, bool intersectOnly = false) const override
	{
		int size = static_cast<int>(poly.points.size());
		int conVer = 0;				// 内包頂点数
		// 頂点内包
		for (int i = 0; i < size; ++i)
		{
			bool con = Contains(poly.points[i]);
			if (!intersectOnly && !fullContain && con)
				return true;
			else if (con)
			{
				conVer++;
			}
		}
		if (fullContain)
		{
			return conVer == size;
		}

		// 辺交差
		for (int i = 0; i < size; ++i)
		{
			int j = (i + 1) % size;
			if (Contains(poly.points[i], poly.points[j]))
				return true;
		}
		return false;
	}


	/*
		@brief			矩形の交差、内包判定
		@attension		線分の両端が境界線の中にある場合は第3引数により変わる
						完全内包と境界線交差は内包が優先される
		@param	[in]	rect			判定する矩形
		@param	[in]	fullContain		完全内包とするか(デフォルト：false)
		@param	[in]	intersectOnly	境界線との交差のみにするか(デフォルト：false)
		@return			交差、内包しているか
		@retval	true	している
		@retval	false	していない
	*/
	virtual bool Contains(const Rect<T1>& rect, bool fullContain = false, bool intersectOnly = false) const override
	{
		bool vertextes[4] = { false, false, false, false };	// 頂点の内包
		Vec3<T1> rp[4] = {
			Vec3<T1>(rect.left, rect.top, 0),
			Vec3<T1>(rect.right, rect.top, 0),
			Vec3<T1>(rect.right, rect.bottom, 0),
			Vec3<T1>(rect.left, rect.bottom, 0),
		};
		// 頂点内包
		for (int i = 0; i < 4; ++i)
		{
			vertextes[i] = Contains(rp[i]);
			if (!intersectOnly && !fullContain && vertextes[i])
				return true;
		}
		if (fullContain)
			return vertextes[0] && vertextes[1] && vertextes[2] && vertextes[3];

		// 辺交差
		for (int i = 0; i < 4; ++i)
		{
			int j = (i + 1) % 4;
			if (Contains(rp[i], rp[j]))
				return true;
		}
		return false;
	}

	/*
		@brief			円の交差、内包判定
		@attension		線分の両端が境界線の中にある場合は第3引数により変わる
						完全内包と境界線交差は内包が優先される
		@param	[in]	circ			判定する円
		@param	[in]	fullContain		完全内包とするか(デフォルト：false)
		@param	[in]	intersectOnly	境界線との交差のみにするか(デフォルト：false)
		@return			交差、内包しているか
		@retval	true	している
		@retval	false	していない
	*/
	virtual bool Contains(const Circle<T1, T2>& circ, bool fullContain = false, bool intersectOnly = false) const override
	{
		// 辺交差or内包
		Vec3<T1> dist = circ.center - center;
		if (PYTHA(dist.x, dist.y) <= POW2(radius + circ.radius))
		{
			if (fullContain)
			{
				// 完全内包
				return dist.Length2() + circ.radius < radius;
			}
			else if (intersectOnly)
			{
				// 完全内包でない
				return dist.Length2() + circ.radius >= radius;
			}
			return true;
		}
		return false;
	}


#pragma endregion // Contains
	//================================================================================

	//================================================================================
#pragma region Intersection
	/*
		@brief	内包している点から一番近い交点の取得
			if (Contains(p))
				point = IntersectionPoint2(p);
		@attention	交差していることが前提
		@param	[in]	point	内包している点
		@return	交点
	*/
	Vec3<T1> IntersectionPoint2(const Vec3<T1>& point) const override
	{
		// 距離
		Vec3<T1> dist = point - center;
		// 点へのベクトルを正規化して円中心からr半径倍の位置に伸ばす
		return  (center + (dist.Normalize2() * radius));
	}

	/*
		@brief	線分との交点の取得
			if (Contains(line))
				points = IntersectionPoint2(line);
		@attention	交差していることが前提
		@param	[in]	line(sta,end)	比較線分
		@return	全交点
	*/
	std::vector<Vec3<T1>> IntersectionPoint2(const Line<T1>& line) const override
	{
		std::vector<Vec3<T1>> intersections; // 交点格納用
		Vec3<T1> sp(center - line.sta); // 線分始点から円中心へのベクトル(start->pos)
		Vec3<T1> ep(center - line.end); // 線分終点から円中心へのベクトル(end->pos)
		Vec3<T1> nv((line.end - line.sta).Normalize2());	// 線分の単位ベクトル(normalize-vector)
		// 円中心から線分への垂線 d=((n・p)*n)-p
		Vec3<T1> dist((Vec3<T1>::Dot(nv, sp) * nv) - sp);

		if (PYTHA(dist.x, dist.y) == POW2(radius))
		{
			intersections.push_back(dist + center);	// 一接点
		}
		else
		{
			float d_rad;	// 垂線の角度
			float i_rad;	// 垂線から交点の角度
			if (PYTHA(dist.x, dist.y) > 0.0f)
			{
				d_rad = std::atan2f(static_cast<float>(dist.y), static_cast<float>(dist.x));
				i_rad = std::acosf(dist.Length2() / radius);
			}
			else
			{
				// 線分が円中心を通る場合
				d_rad = std::atan2f(static_cast<float>(line.end.y), static_cast<float>(line.end.x));
				i_rad = PI / 2.0f;
			}

			Vec3<T1> temp[] =
			{
				// 垂線と無限線分を中心からのT字と考えたとき,それぞれ線分の右,左の交点
				Vec3<T1>(static_cast<T1>(static_cast<float>(radius)* std::cosf(d_rad + i_rad)),
				static_cast<T1>(static_cast<float>(radius)* std::sinf(d_rad + i_rad))),
				Vec3<T1>(static_cast<T1>(static_cast<float>(radius)* std::cosf(d_rad - i_rad)),
				static_cast<T1>(static_cast<float>(radius)* std::sinf(d_rad - i_rad))),
			};

			if (PYTHA(sp.x, sp.y) >= POW2(radius) && PYTHA(ep.x, ep.y) >= POW2(radius))
			{
				for (const auto& t : temp)
				{
					intersections.push_back(t + center);	// 二交点
				}
			}
			else
			{
				// 一交点
				if (PYTHA(sp.x, sp.y) >= POW2(radius))
				{
					// 円外の座標から交点までの距離
					Vec3<T1> a(temp[0] - sp);
					Vec3<T1> b(temp[1] - sp);
					intersections.push_back(((PYTHA(a.x, a.y) > PYTHA(b.x, b.y)) ? temp[0] : temp[1]) + center);
				}
				else
				{
					// 円外の座標から交点までの距離
					Vec3<T1> a(temp[0] - ep);
					Vec3<T1> b(temp[1] - ep);
					intersections.push_back(((PYTHA(a.x, a.y) > PYTHA(b.x, b.y)) ? temp[0] : temp[1]) + center);
				}
			}
		}
		return intersections;
	}
	/*
		@brief	線分との交点の取得
			if (Contains(pos,next))
				points = IntersectionPoint2(pos,next);
		@attention	交差していることが前提
		@param	[in]	sta	比較線分の始点
		@param	[in]	end	比較線分の終点
		@return	全交点
	*/
	std::vector<Vec3<T1>> IntersectionPoint2(const Vec3<T1>& sta, const Vec3<T1>& end) const override
	{
		std::vector<Vec3<T1>> intersections; // 交点格納用
		Vec3<T1> sp(center - sta); // 線分始点から円中心へのベクトル(start->pos)
		Vec3<T1> ep(center - end); // 線分終点から円中心へのベクトル(end->pos)
		Vec3<T1> nv((end - sta).Normalize2());	// 線分の単位ベクトル(normalize-vector)
		// 円中心から線分への垂線 d=((n・p)*n)-p
		Vec3<T1> dist((Vec3<T1>::Dot(nv, sp) * nv) - sp);

		if (PYTHA(dist.x, dist.y) == POW2(radius))
		{
			intersections.push_back(dist + center);	// 一接点
		}
		else
		{
			float d_rad;	// 垂線の角度
			float i_rad;	// 垂線から交点の角度
			if (PYTHA(dist.x, dist.y) > 0.0f)
			{
				d_rad = std::atan2f(static_cast<float>(dist.y), static_cast<float>(dist.x));
				i_rad = std::acosf(dist.Length2() / radius);
			}
			else
			{
				// 線分が円中心を通る場合
				d_rad = std::atan2f(static_cast<float>(end.y), static_cast<float>(end.x));
				i_rad = PI / 2.0f;
			}

			Vec3<T1> temp[] =
			{
				// 垂線と無限線分を中心からのT字と考えたとき,それぞれ線分の右,左の交点
				Vec3<T1>(static_cast<T1>(static_cast<float>(radius)* std::cosf(d_rad + i_rad)),
				static_cast<T1>(static_cast<float>(radius)* std::sinf(d_rad + i_rad))),
				Vec3<T1>(static_cast<T1>(static_cast<float>(radius)* std::cosf(d_rad - i_rad)),
				static_cast<T1>(static_cast<float>(radius)* std::sinf(d_rad - i_rad))),
			};

			if (PYTHA(sp.x, sp.y) >= POW2(radius) && PYTHA(ep.x, ep.y) >= POW2(radius))
			{
				for (const auto& t : temp)
				{
					intersections.push_back(t + center);	// 二交点
				}
			}
			else
			{
				// 一交点
				if (PYTHA(sp.x, sp.y) >= POW2(radius))
				{
					// 円外の座標から交点までの距離
					Vec3<T1> a(temp[0] - sp);
					Vec3<T1> b(temp[1] - sp);
					intersections.push_back(((PYTHA(a.x, a.y) > PYTHA(b.x, b.y)) ? temp[0] : temp[1]) + center);
				}
				else
				{
					// 円外の座標から交点までの距離
					Vec3<T1> a(temp[0] - ep);
					Vec3<T1> b(temp[1] - ep);
					intersections.push_back(((PYTHA(a.x, a.y) > PYTHA(b.x, b.y)) ? temp[0] : temp[1]) + center);
				}
			}
		}
		return intersections;
	}

	
		
	/*
		@brief	多角形との交点の取得
			if (Contains(polygon))
				points = IntersectionPoint2(polygon);
		@attention	交差していることが前提
		@param	[in]	polygon	比較図形
		@return	全交点
	*/
	virtual std::vector<Vec3<T1>> IntersectionPoint2(const Polygon<T1>& polygon) const override
	{
		std::vector<Vec3<T1>> intersections;
		int size = static_cast<int>(polygon.points.size());
		intersections.reserve(size*2);
		// 辺交差
		for (int i = 0; i < size; ++i)
		{
			int j = (i + 1) % size;
			if (Contains(polygon.points[i], polygon.points[j]))
			{
				auto inter = IntersectionPoint2(polygon.points[i], polygon.points[j]);
				intersections.insert(intersections.end(), inter.begin(), inter.end());
			}
		}
		return intersections;
	}
	/*
		@brief	矩形との交点の取得
			if (Contains(rect))
				points = IntersectionPoint2(rect);
		@attention	交差していることが前提
		@param	[in]	rect	比較図形
		@return	全交点
	*/
	virtual std::vector<Vec3<T1>> IntersectionPoint2(const Rect<T1>& rect) const override
	{
		std::vector<Vec3<T1>> intersections;
		// 頂点内包
		Vec3<T1> rp[4] = {
			Vec3<T1>(rect.left, rect.top, 0),
			Vec3<T1>(rect.right, rect.top, 0),
			Vec3<T1>(rect.right, rect.bottom, 0),
			Vec3<T1>(rect.left, rect.bottom, 0),
		};
		// 辺交差
		int size = 4;
		intersections.reserve(size*2);
		for (int i = 0; i < size; ++i)
		{
			int j = (i + 1) % size;
			if (Contains(rp[i], rp[j]))
			{
				auto inter = IntersectionPoint2(rp[i], rp[j]);
				intersections.insert(intersections.end(), inter.begin(), inter.end());

			}
		}
		return intersections;
	}
	/*
		@brief	円との交点の取得
			if (Contains(circle))
				points = IntersectionPoint2(circle);
		@attention	交差していることが前提
		@param	[in]	circle	比較図形
		@return	全交点
	*/
	virtual std::vector<Vec3<T1>> IntersectionPoint2(const Circle<T1, T2>& circle) const override
	{
		std::vector<Vec3<T1>> intersections;
		intersections.reserve(2);

		Vec3<T1> dist = circle.center - center;

		T1 c = (PYTHA(center.x, center.y) - POW2(radius))
			- (PYTHA(circle.center.x, circle.center.y) - POW2(circle.radius)) / static_cast<T1>(2);

		T1 d = dist.x*center.x + dist.y*center.y + c;
		T1 tmp = static_cast<T1>(sqrt( PYTHA(dist.x, dist.y)*POW2(radius) - d*d ));
		intersections.push_back(mymath::Vec3<T1>(
			(-dist.x*d + dist.y*tmp) / PYTHA(dist.x, dist.y) + center.x,
			(-dist.y*d - dist.x*tmp) / PYTHA(dist.x, dist.y) + center.y));
		intersections.push_back(mymath::Vec3<T1>(
			(-dist.x*d - dist.y*tmp) / PYTHA(dist.x, dist.y) + center.x,
			(-dist.y*d + dist.x*tmp) / PYTHA(dist.x, dist.y) + center.y));

		return intersections;
	}



	/*
		@brief	線分との交点のうち、始点に近い点を取得
			if (Contains(line))
				point = IntersectionPoint2Nearest(line);
		@attention	交差していることが前提
		@param	[in]	line(sta,end)	比較線分
		@return	始点に近い交点
	*/
	Vec3<T1> IntersectionPoint2Nearest(const Line<T1>& line) const override
	{
		std::vector<Vec3<T1>> intersections; // 交点格納用
		Vec3<T1> sp(center - line.sta); // 線分始点から円中心へのベクトル(start->pos)
		Vec3<T1> ep(center - line.end); // 線分終点から円中心へのベクトル(end->pos)
		Vec3<T1> nv((line.end - line.sta).Normalize2());	// 線分の単位ベクトル(normalize-vector)
		// 円中心から線分への垂線 d=((n・p)*n)-p
		Vec3<T1> dist((Vec3<T1>::Dot(nv, sp) * nv) - sp);

		if (PYTHA(dist.x, dist.y) == POW2(radius))
		{
			intersections.push_back(dist + center);	// 一接点
		}
		else
		{
			float d_rad;	// 垂線の角度
			float i_rad;	// 垂線から交点の角度
			if (PYTHA(dist.x, dist.y) > 0.0f)
			{
				d_rad = std::atan2f(static_cast<float>(dist.y), static_cast<float>(dist.x));
				i_rad = std::acosf(dist.Length2() / radius);
			}
			else
			{
				// 線分が円中心を通る場合
				d_rad = std::atan2f(static_cast<float>(line.end.y), static_cast<float>(line.end.x));
				i_rad = PI / 2.0f;
			}

			Vec3<T1> temp[] =
			{
				// 垂線と無限線分を中心からのT字と考えたとき,それぞれ線分の右,左の交点
				Vec3<T1>(static_cast<T1>(static_cast<float>(radius)* std::cosf(d_rad + i_rad)),
				static_cast<T1>(static_cast<float>(radius)* std::sinf(d_rad + i_rad))),
				Vec3<T1>(static_cast<T1>(static_cast<float>(radius)* std::cosf(d_rad - i_rad)),
				static_cast<T1>(static_cast<float>(radius)* std::sinf(d_rad - i_rad))),
			};

			if (PYTHA(sp.x, sp.y) >= POW2(radius) && PYTHA(ep.x, ep.y) >= POW2(radius))
			{
				for (const auto& t : temp)
				{
					intersections.push_back(t + center);	// 二交点
				}
			}
			else
			{
				// 一交点
				if (PYTHA(sp.x, sp.y) >= POW2(radius))
				{
					// 円外の座標から交点までの距離
					Vec3<T1> a(temp[0] - sp);
					Vec3<T1> b(temp[1] - sp);
					intersections.push_back(((PYTHA(a.x, a.y) > PYTHA(b.x, b.y)) ? temp[0] : temp[1]) + center);
				}
				else
				{
					// 円外の座標から交点までの距離
					Vec3<T1> a(temp[0] - ep);
					Vec3<T1> b(temp[1] - ep);
					intersections.push_back(((PYTHA(a.x, a.y) > PYTHA(b.x, b.y)) ? temp[0] : temp[1]) + center);
				}
			}
		}
		// 始点に一番近い交点の算出
		Vec3<T1> min = intersections[0];
		for (size_t i = 1; i < intersections.size(); ++i)
		{
			Vec3<T1> d1 = min - line.sta;
			Vec3<T1> d2 = intersections[i] - line.sta;
			if (PYTHA(d1.x, d1.y) > PYTHA(d2.x, d2.y))
				min = intersections[i];
		}
		return min;
	}


	/*
		@brief	線分との交点のうち、始点に近い点を取得
			if (Contains(pos,next))
				point = IntersectionPoint2Nearest(pos,next);
		@attention	交差していることが前提
		@param	[in]	sta	比較線分の始点
		@param	[in]	end	比較線分の終点
		@return	始点に近い交点
	*/
	Vec3<T1> IntersectionPoint2Nearest(const Vec3<T1>& sta, const Vec3<T1>& end) const override
	{
		std::vector<Vec3<T1>> intersections; // 交点格納用
		Vec3<T1> sp(center - sta); // 線分始点から円中心へのベクトル(start->pos)
		Vec3<T1> ep(center - end); // 線分終点から円中心へのベクトル(end->pos)
		Vec3<T1> nv((end - sta).Normalize2());	// 線分の単位ベクトル(normalize-vector)
		// 円中心から線分への垂線 d=((n・p)*n)-p
		Vec3<T1> dist((Vec3<T1>::Dot(nv, sp) * nv) - sp);

		if (PYTHA(dist.x, dist.y) == POW2(radius))
		{
			intersections.push_back(dist + center);	// 一接点
		}
		else
		{
			float d_rad;	// 垂線の角度
			float i_rad;	// 垂線から交点の角度
			if (PYTHA(dist.x, dist.y) > 0.0f)
			{
				d_rad = std::atan2f(static_cast<float>(dist.y), static_cast<float>(dist.x));
				i_rad = std::acosf(dist.Length2() / radius);
			}
			else
			{
				// 線分が円中心を通る場合
				d_rad = std::atan2f(static_cast<float>(end.y), static_cast<float>(end.x));
				i_rad = PI / 2.0f;
			}

			Vec3<T1> temp[] =
			{
				// 垂線と無限線分を中心からのT字と考えたとき,それぞれ線分の右,左の交点
				Vec3<T1>(static_cast<T1>(static_cast<float>(radius)* std::cosf(d_rad + i_rad)),
				static_cast<T1>(static_cast<float>(radius)* std::sinf(d_rad + i_rad))),
				Vec3<T1>(static_cast<T1>(static_cast<float>(radius)* std::cosf(d_rad - i_rad)),
				static_cast<T1>(static_cast<float>(radius)* std::sinf(d_rad - i_rad))),
			};

			if (PYTHA(sp.x, sp.y) >= POW2(radius) && PYTHA(ep.x, ep.y) >= POW2(radius))
			{
				for (const auto& t : temp)
				{
					intersections.push_back(t + center);	// 二交点
				}
			}
			else
			{
				// 一交点
				if (PYTHA(sp.x, sp.y) >= POW2(radius))
				{
					// 円外の座標から交点までの距離
					Vec3<T1> a(temp[0] - sp);
					Vec3<T1> b(temp[1] - sp);
					intersections.push_back(((PYTHA(a.x, a.y) > PYTHA(b.x, b.y)) ? temp[0] : temp[1]) + center);
				}
				else
				{
					// 円外の座標から交点までの距離
					Vec3<T1> a(temp[0] - ep);
					Vec3<T1> b(temp[1] - ep);
					intersections.push_back(((PYTHA(a.x, a.y) > PYTHA(b.x, b.y)) ? temp[0] : temp[1]) + center);
				}
			}
		}
		// 始点に一番近い交点の算出
		Vec3<T1> min = intersections[0];
		for (size_t i = 1; i < intersections.size(); ++i)
		{
			Vec3<T1> d1 = min - sta;
			Vec3<T1> d2 = intersections[i] - sta;
			if (PYTHA(d1.x, d1.y) > PYTHA(d2.x, d2.y))
				min = intersections[i];
		}
		return min;
	}

#pragma endregion // Intersection
	//================================================================================

#ifdef USE_CIRCLE_EXT
	/*
		@brief	円の描画
		@param	[in]	center_x	中心X座標
		@param	[in]	center_y	中心Y座標
		@param	[in]	center_z	中心Z座標
		@param	[in]	radius		半径
		@param	[in]	color		線色(デフォルト:白)
		@param	[in]	size		線の太さ(デフォルト:1)
		@return	なし
	*/
	static void draw(T1 center_x, T1 center_y, T1 center_z, T2 radius, D3DCOLOR color = 0xffffffff, int size = 1)
	{
		for(int i=0; i<360; ++i)
		{
			float rad = static_cast<float>(i)	* 2.0f * PI / 360.0f;
			float ra2 = static_cast<float>(i+1)	* 2.0f * PI / 360.0f;
			POINT sta = { LONG(std::cosf(rad) * radius + center_x), LONG(-std::sinf(rad) * radius + center_y) };
			POINT end = { LONG(std::cosf(ra2) * radius + center_x), LONG(-std::sinf(ra2) * radius + center_y) };
			graph::Draw_Line(	(int)sta.x,
				(int)sta.y,
				(int)end.x,
				(int)end.y,
				center_z,
				color, size);
		}
	}
#endif

#ifdef DEF_SHAPE_DRAW
	
	/*
		@brief	図形の描画
		@param	[in]	color	線の色(デフォルト:白) 
		@param	[in]	size	線の太さ(デフォルト:1)
		@param	[in]	fill	塗りつぶし(Rectのみ)(デフォルト:true)
		@return	なし
	*/
	virtual void draw(DWORD color = -1, int size = 1, bool fill = true) const override
	{
		const float resolution = 120.f;
		const int add = static_cast<int>(360.f / resolution);
		for (int i = 0; i<360; i += add)
		{
			float rad = static_cast<float>(i)* 2.0f * PI / 360.0f;
			float ra2 = static_cast<float>(i + add) * 2.0f * PI / 360.0f;
			POINT sta = { LONG(std::cosf(rad) * radius + center.x), LONG(-std::sinf(rad) * radius + center.y) };
			POINT end = { LONG(std::cosf(ra2) * radius + center.x), LONG(-std::sinf(ra2) * radius + center.y) };
			graph::Draw_Line(
				static_cast<int>(sta.x),
				static_cast<int>(sta.y),
				static_cast<int>(end.x),
				static_cast<int>(end.y),
				static_cast<float>(center.z),
				color, size);
		}
	}
#endif
};
typedef Circle<int,int> Circlei;
typedef Circle<float,float> Circlef;
#pragma endregion // Circle


}
#endif
/*
template<class T>
strict Matrix
{
	T m[4][4];
};
template<class T>
struct Quaternion
{
	T w, x, y, z;

	Quaternion(){}
	Quaternion( float sx, float sy, float sz, float sw ) : x(sx), y(sy), z(sz), w(sw) {}

	//	単位クォータニオン生成
	void Identity(){ x = y = z = 0; w = 1; }
	
	
	//クォータニオンの計算
	Quaternion operator*(Quaternion qua);

	//回転クォータニオン
	Quaternion CreateRotationQuaternion(double radian, Vector3f Axis);

	//位置クォータニオン
	Quaternion CreateXYZToQuaternion(double PosX, double PosY, double PosZ);

	//クォータニオンから回転行列へ
	MATRIX QuaternionToMatrix();
};
*/
