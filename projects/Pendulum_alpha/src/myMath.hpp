#ifndef DEF_MYMATH_HPP
#define DEF_MYMATH_HPP
#include <cmath>
#include <vector>
#include <memory>

#include <wtypes.h>	//  RECT用

namespace mymath
{
	const float PI = 3.14159265358979323846f;
	const double PI_D = 3.14159265358979323846;
/*
#ifndef PI
#define PI   3.14159265358979323846f
#endif
#ifndef PI_D
#define PI_D 3.14159265358979323846
#endif
*/




// 2乗
//#define POW2(x)		( (x)*(x) )
// ピタゴラスの定理のルートなし
//#define PYTHA(x,y)	( POW2((x)) + POW2((y)) )

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
//inline clamp(const T x, const T minVal, const T maxVal)
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

	Vec3(T xx = 0, T yy = 0 , T zz = 0):
		x(xx), y(yy), z(zz)
	{}
	Vec3(const T p[3]):
		x(p[0]), y(p[1]), z(p[2])
	{}

	//Vec3(const Vec3<T>& v){ *this = v; }

	// 代入、初期化系
	Vec3<T>&	operator=(T v){ x=v;y=v;z=v; return *this; } 
	Vec3<T>&	operator()(T xx, T yy = 0 , T zz = 0){ x=xx; y=yy; z=zz; return *this; }

	// 単項
	Vec3<T>		operator-() const { return Vec3<T>(-x,-y,-z); }

	// 比較 
	bool		operator==(const Vec3<T>& v) const { return (x==v.x && y==v.y && z==v.z); } 
	bool		operator!=(const Vec3<T>& v) const { return (x!=v.x || y!=v.y || z!=v.z); } 

	// ベクトル操作(ベクトル対ベクトル)
	Vec3<T>		operator+(const Vec3<T>& v) const { return Vec3<T>(x+v.x, y+v.y, z+v.z); } 
	Vec3<T>		operator-(const Vec3<T>& v) const { return Vec3<T>(x-v.x, y-v.y, z-v.z); } 
	Vec3<T>		operator*(const Vec3<T>& v) const { return Vec3<T>(x*v.x, y*v.y, z*v.z); } 
	Vec3<T>		operator/(const Vec3<T>& v) const { return Vec3<T>(x/v.x, y/v.y, z/v.z); } 
	Vec3<T>&	operator+=(const Vec3<T>& v){ x+=v.x; y+=v.y; z+=v.z; return *this; } 
	Vec3<T>&	operator-=(const Vec3<T>& v){ x-=v.x; y-=v.y; z-=v.z; return *this; } 
	Vec3<T>&	operator*=(const Vec3<T>& v){ x*=v.x; y*=v.y; z*=v.z; return *this; } 
	Vec3<T>&	operator/=(const Vec3<T>& v){ x/=v.x; y/=v.y; z/=v.z; return *this; } 

	// 特殊演算
	T			operator|( const Vec3<T>& v )	{ return Dot( *this, v ); }
	Vec3<T>		operator&( const Vec3<T>& v )	{ return Cross( *this, v ); }

	// ベクトル操作2(ベクトル対定数)
	Vec3<T>		operator+(T v) const { return Vec3<T>(x+v,y+v,z+v); } 
	Vec3<T>		operator-(T v) const { return Vec3<T>(x-v,y-v,z-v); } 
	Vec3<T>		operator*(T v) const { return Vec3<T>(x*v,y*v,z*v); } 
	Vec3<T>		operator/(T v) const { return Vec3<T>(x/v,y/v,z/v); } 
	Vec3<T>&	operator+=(T v){ x+=v; y+=v; z+=v; return *this; }
	Vec3<T>&	operator-=(T v){ x-=v; y-=v; z-=v; return *this; }
	Vec3<T>&	operator*=(T v){ x*=v; y*=v; z*=v; return *this; }
	Vec3<T>&	operator/=(T v){ x/=v; y/=v; z/=v; return *this; }

	// friend関数によるベクトル操作(定数対ベクトル)
	friend Vec3<T>	operator+(T v, const Vec3<T>& obj){ return Vec3<T>(obj.x+v, obj.y+v, obj.z+v); } 
	friend Vec3<T>	operator-(T v, const Vec3<T>& obj){ return Vec3<T>(obj.x-v, obj.y-v, obj.z-v); } 
	friend Vec3<T>	operator*(T v, const Vec3<T>& obj){ return Vec3<T>(obj.x*v, obj.y*v, obj.z*v); } 
	friend Vec3<T>	operator/(T v, const Vec3<T>& obj){ return Vec3<T>(obj.x/v, obj.y/v, obj.z/v); } 
	  
	// friend関数による比較
	friend bool		operator==(T v, Vec3<T>& obj){ return (obj.x==v && obj.y==v && obj.z==v); } 
	friend bool		operator!=(T v, Vec3<T>& obj){ return (obj.x!=v || obj.y!=v || obj.z!=v); } 
	
	// キャスト
	template<class T1>
	Vec3<T>& operator = (const Vec3<T1>& v)
	{
		x = static_cast<T>(v.x);
		y = static_cast<T>(v.y);
		z = static_cast<T>(v.z);
		return *this;
	}
	template<class T1>
	operator Vec3<T1>()
	{
		return Vec3<T1>(static_cast<T>(x),
						static_cast<T>(y),
						static_cast<T>(z));
	}

#pragma region member methods

	static enum eTmpReplace { X=0x01, Y=0x02, Z=0x04 };
	/*
		@brief	一時的に値を置き換えたベクトルを返す
		@param	[in]	flag	置き換えるベクトルの要素(X,Y,Zで指定、|で複数指定可)
		@param	[in]	v		元ベクトル
		@return	置き換えたベクトル
	*/
	Vec3<T>		TmpReplace( int flag, const Vec3<T>& v )
	{
		return Vec3<T>( (flag&X)?v.x:x, (flag&Y)?v.y:y, (flag&Z)?v.z:z );
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
		x = static_cast<T>(static_cast<float>(x) * mag);
		y = static_cast<T>(static_cast<float>(y) * mag);
		z = static_cast<T>(static_cast<float>(z) * mag);
		return *this;
	}
	/*
		@brief	XY平面の2次元ベクトルを正規化する
		@return	正規化したベクトル
	*/
	Vec3<T>& Normalize2()
	{
		float mag = 1.f / Length2();
		x = static_cast<T>(static_cast<float>(x) * mag);
		y = static_cast<T>(static_cast<float>(y) * mag);
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
		@return	なす角
	*/
	static float Angle(const Vec3<T>& v1, const Vec3<T>& v2)
	{
		float l1 = v1.Length();
		float l2 = v2.Length();
		if( l1 == 0.0f || l2 == 0.0f ) return 0.0f;
		return std::acosf( Dot(v1 , v2) / (l1 * l2) );
	}

	/*
		@brief	引数のベクトルとの内積(v1・v2)
		@param	[in]	v1	ベクトル
		@param	[in]	v2	ベクトル
		@return	内積
	*/
	static T Dot(const Vec3<T>& v1, const Vec3<T>& v2)
	{
		return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
	}
	/*
		@brief	引数のベクトルとのXY平面における内積(v1・v2)
		@param	[in]	v1	ベクトル
		@param	[in]	v2	ベクトル
		@return	内積
	*/
	static T Dot2(const Vec3<T>& v1, const Vec3<T>& v2)
	{
		return v1.x*v2.x + v1.y*v2.y;
	}
	/*
		@brief	引数のベクトルとの外積(v1×v2)
		@param	[in]	v1	ベクトル
		@param	[in]	v2	ベクトル
		@return	外積
	*/
	static Vec3<T> Cross(const Vec3<T>& v1, const Vec3<T>& v2)
	{
		return Vec3<T>( (v1.y*v2.z)-(v1.z*v2.y), (v1.z*v2.x)-(v1.x*v2.z), (v1.x*v2.y)-(v1.y*v2.x) );
	}
	/*
		@brief	引数のベクトルとのXY平面における外積(v1×v2)
		@param	[in]	v1	ベクトル
		@param	[in]	v2	ベクトル
		@return	外積
	*/
	static T Cross2(const Vec3<T>& v1, const Vec3<T>& v2)
	{
		return (v1.y*v2.x) - (v1.y * v2.x);
	}
	
	/*
		@brief	回転単位ベクトルの習得
		@param	[in]	angle	角度(unit:radian)
		@return 回転後の単位ベクトル
	*/
	static Vec3<T> Rotate(float angle)
	{
		return Vec3<T>( std::cosf(angle), std::sinf(angle) );
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
	Line():
		sta(0), end(0)
	{}
	Line(const Vec3<T>& s, const Vec3<T>& e):
		sta(s), end(e)
	{}
	Line(T sx, T sy, T sz, T ex, T ey, T ez):
		sta(Vec3<T>(sx,sy,sz)), end(Vec3<T>(ex,ey,ez))
	{}
	Line(const T s[3], const T e[3]):
		sta(Vec3<T>(s[0],s[1],s[2])), end(Vec3<T>(e[0],e[1],e[2]))
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
		return ( (v1 & (line.sta-sta)) | (v1 & (line.end-sta)) ) < 0.f
			&& ( (v2 & (sta-line.sta)) | (v2 & (end-line.sta)) ) < 0.f;
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
		return ( (v1 & (_sta-sta)) | (v1 & (_end-sta)) ) < 0.f
			&& ( (v2 & (sta-_sta)) | (v2 & (end-_sta)) ) < 0.f;
	}

	/*
		@brief	交点の取得
			if(Intersect(line))
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
		return sta + (end-sta) * static_cast<T>(ramda);
		
		/*
		double s1 = ( (line.end.x - line.sta.x) * (sta.y - line.sta.y) - (line.end.y - line.sta.y) * (sta.x - line.sta.x)) / 2.0;
		double s2 = ( (line.end.x - line.sta.x) * (line.sta.y - end.y) - (line.end.y - line.sta.y) * (line.sta.x - end.x)) / 2.0;

		return sta + (end-sta) * s1 / (s1+s2);
		*/
	}
	/*
		@brief	交点の取得
			if(Intersect(p1,p2))
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
		return sta + (end-sta) * static_cast<T>(ramda);
	}
};
typedef Line<float> Linef;
typedef Line<int>	Linei;

#pragma endregion // Line

//=================================================================================================
//=================================================================================================
//=================================================================================================
#pragma region Shape
template<typename Tri>
class Triangle;
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
		@brief	図形を値分ずらす
		@param	[in]	offset	ずらす量
		@return	自分自身
	*/
	virtual Shape<T>& Offset(const Vec3<T>& offset) = 0{return *this;}

	/*
		@brief	点の内包判定
		@param	[in]	point	判定する点
		@return	内包しているか
		@retval true	している
		@retval false	していない
	*/
	virtual bool Contains(const Vec3<T>& point)	const{return false;}

	/*
		@brief	線分の交差、内包判定
		@param	[in]	line(sta,end)	判定する線分
		@param	[in]	fullContain		完全内包とするか(デフォルト：false)
		@return	交差、内包しているか
		@retval	true	している
		@retval	false	していない
	*/
	virtual bool Contains(const Line<T>& line, bool fullContain=false)	const{return false;}
	
	/*
		@brief	線分の交差、内包判定
		@param	[in]	sta				判定する線分の始点
		@param	[in]	end				判定する線分の終点
		@param	[in]	fullContain		完全内包とするか(デフォルト：false)
		@return	交差、内包しているか
		@retval	true	している
		@retval	false	していない
	*/
	virtual bool Contains(const Vec3<T>& sta, const Vec3<T>& end, bool fullContain=false)	const{return false;}
	
	/*
		@brief	三角形の交差、内包判定
		@param	[in]	tri				判定する三角形
		@param	[in]	fullContain		完全内包とするか(デフォルト：false)
		@return	交差、内包しているか
		@retval	true	している
		@retval	false	していない
	*/
	virtual bool Contains(const Triangle<T>& tri, bool fullContain=false)	const{return false;}
	
	/*
		@brief	矩形の交差、内包判定
		@param	[in]	rect			判定する矩形
		@param	[in]	fullContain		完全内包とするか(デフォルト：false)
		@return	交差、内包しているか
		@retval	true	している
		@retval	false	していない
	*/
	virtual bool Contains(const Rect<T>& rect, bool fullContain=false)		const{return false;}
	//virtual bool Contains(const Circle<T,T>& c, bool fullContain=false)	const{return false;}
	
	/*
		@brief	内包している点から一番近い交点の取得
			if(Contains(p))
				point = IntersectionPoint2(p);
		@attention	交差していることが前提
		@param	[in]	point	内包している点
		@return	交点
	*/
	virtual Vec3<T> IntersectionPoint2(const Vec3<T>& point)	const{return Vec3<T>();}
	
	/*
		@brief	線分との交点の取得
			if(Contains(line))
				points = IntersectionPoint2(line);
		@attention	交差していることが前提
		@param	[in]	line(sta,end)	比較線分
		@return	全交点
	*/
	virtual std::vector<Vec3<T>> IntersectionPoint2(const Line<T>& line)	const{return std::vector<Vec3<T>>();}
	
	/*
		@brief	線分との交点の取得
			if(Contains(pos,next))
				points = IntersectionPoint2(pos,next);
		@attention	交差していることが前提
		@param	[in]	sta	比較線分の始点
		@param	[in]	end	比較線分の終点
		@return	全交点
	*/
	virtual std::vector<Vec3<T>> IntersectionPoint2(const Vec3<T>& sta, const Vec3<T>& end)	const{return std::vector<Vec3<T>>();}
	
	/*
		@brief	線分との交点のうち、始点に近い点を取得
			if(Contains(line))
				point = IntersectionPoint2Nearest(line);
		@attention	交差していることが前提
		@param	[in]	line(sta,end)	比較線分
		@return	始点に近い交点
	*/
	virtual Vec3<T> IntersectionPoint2Nearest(const Line<T>& line)	const {return Vec3<T>();}
	
	/*
		@brief	線分との交点のうち、始点に近い点を取得
			if(Contains(pos,next))
				point = IntersectionPoint2Nearest(pos,next);
		@attention	交差していることが前提
		@param	[in]	sta	比較線分の始点
		@param	[in]	end	比較線分の終点
		@return	始点に近い交点
	*/
	virtual Vec3<T> IntersectionPoint2Nearest(const Vec3<T>& sta, const Vec3<T>& end)	const {return Vec3<T>();}
	
};
typedef	Shape<int>		Shapei;
typedef Shape<float>	Shapef;
typedef std::shared_ptr<Shapef> ShapefPtr;

#pragma endregion // Shape


//=================================================================================================
//=================================================================================================
//=================================================================================================

#pragma region Triangle
/*
	@brief	三角形
*/
template <typename T> class Triangle : public Shape<T>
{
private:

public:
	Vec3<T> points[3];	// ポリゴンを整形する頂点
	Triangle()
	{
		points[0] = 0;
		points[1] = 0;
		points[2] = 0;
	}
	Triangle(const Vec3<T>& aa, const Vec3<T>& bb, const Vec3<T>& cc)
	{
		points[0] = aa;
		points[1] = bb;
		points[2] = cc;
	}
	Triangle(T ax, T ay, T az, T bx, T by, T bz, T cx, T cy, T cz):
		points[0](Vec3<T>(ax, ay, az)),
		points[1](Vec3<T>(bx, by, bz)),
		points[2](Vec3<T>(cx, cy, cz))
	{}
	Triangle(T a[3], T b[3], T c[3]):
		points[0](Vec3<T>(a[0], a[1], a[2])),
		points[1](Vec3<T>(b[0], b[1], b[2])),
		points[2](Vec3<T>(c[0], c[1], c[2]))
	{}
	
	/*
		@brief	図形を値分ずらす
		@param	[in]	offset	ずらす量
		@return	自分自身
	*/
	virtual Shape<T>& Offset(const Vec3<T>& offset) override
	{
		for(auto& point : points)
		{
			point += offset;
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
	bool Contains(const Vec3<T>& point) const override
	{
		int cnt = 0;
		for(int i=0; i<3; ++i)
		{
			int j = (i+1) % 3;
			Vec3<T> vec(points[j] - points[i]);	// ab,bc,ca
			Vec3<T> p_p(point - points[i]);		// ap,bp,cp 
			// 外積の向き判定
			cnt += (Vec3<T>::Dot2(vec,p_p) > static_cast<T>(0)) ? 1 : -1;
		}
		return (cnt == 3 || cnt == -3);
	}
	/*
		@brief	線分の交差、内包判定
		@param	[in]	line(sta,end)	判定する線分
		@param	[in]	fullContain		完全内包とするか(デフォルト：false)
		@return	交差、内包しているか
		@retval	true	している
		@retval	false	していない
	*/
	bool Contains(const Line<T>& line, bool fullContain = false) const override
	{
		// 完全内包
		if(fullContain)
		{
			return ( Contains(line.sta) && Contains(line.end) );
		}
		// 外周との交差判定
		std::vector<Vec3<T>> intersections;
		for(int i = 0; i < 3; ++i)
		{
			int j = (i+1) % 3;
			if(line.Intersect(points[i], points[j]))
			{
				intersections.push_back(line.IntersectionPoint2(points[i],points[j]));
			}
		}
		// 交点が無かった
		if(intersections.empty())
		{
			// 完全内包チェック(始点または終点が内包されていれば完全内包といえる)
			return Contains(line.sta);
		}
		return true;
	}
	/*
		@brief	線分の交差、内包判定
		@param	[in]	sta				判定する線分の始点
		@param	[in]	end				判定する線分の終点
		@param	[in]	fullContain		完全内包とするか(デフォルト：false)
		@return	交差、内包しているか
		@retval	true	している
		@retval	false	していない
	*/
	bool Contains(const Vec3<T>& sta, const Vec3<T>& end, bool fullContain = false) const override
	{
		// 完全内包
		if(fullContain)
		{
			return ( Contains(sta) && Contains(end) );
		}
		// 外周との交差判定
		std::vector<Vec3<T>> intersections;
		Line<T> line(sta, end);
		for(int i = 0; i < 3; ++i)
		{
			int j = (i+1) % 3;
			if(line.Intersect(points[i],points[j]))
				intersections.push_back(line.IntersectionPoint2(points[i],points[j]));
		}
		// 交点が無かった
		if(intersections.empty())
		{
			// 完全内包チェック(始点または終点が内包されていれば完全内包といえる)
			return Contains(sta);
		}
		return true;
	}
	/*
		@brief	三角形の交差、内包判定
		@param	[in]	tri				判定する三角形
		@param	[in]	fullContain		完全内包とするか(デフォルト：false)
		@return	交差、内包しているか
		@retval	true	している
		@retval	false	していない
	*/
	bool Contains(const Triangle<T>& tri, bool fullContain = false) const override
	{
		bool vertextes[3]={false,false,false};	// 頂点の内包
		// 頂点内包
		for(int i=0; i<3; ++i)
		{
			vertextes[i] = Contains(tri.points[i]);
			if(!fullContain && vertextes[i])
				return true;
		}
		if(fullContain)
			return vertextes[0] && vertextes[1] && vertextes[2];
		
		// 辺交差
		for(int i=0; i<3; ++i)
		{
			int j = (i+1) % 3;
			Line<T> line(points[i],points[j]);
			for(int k = 0; k < 3; ++k)
			{
				int l = (k+1) % 3;
				if(line.Intersect(tri.points[k], tri.points[l]))
					return true;
			}
		}
		return false;
	}
	/*
		@brief	矩形の交差、内包判定
		@param	[in]	rect			判定する矩形
		@param	[in]	fullContain		完全内包とするか(デフォルト：false)
		@return	交差、内包しているか
		@retval	true	している
		@retval	false	していない
	*/
	bool Contains(const Rect<T>& rect, bool fullContain = false) const override
	{
		bool vertextes[4]={false,false,false,false};	// 頂点の内包
		// 頂点内包
		Vec3<T> rp[4] = {
			Vec3<T>(rect.left,	rect.top,	points[0].z),
			Vec3<T>(rect.right,	rect.top,	points[0].z),
			Vec3<T>(rect.left,	rect.bottom,points[0].z),
			Vec3<T>(rect.right,	rect.bottom,points[0].z)
		};
		for(int i=0; i<4; ++i)
		{
			vertextes[i] = Contains(rp[i]);
			if(!fullContain && vertextes[i])
				return true;
		}
		if(fullContain)
			return vertextes[0] && vertextes[1] && vertextes[2] && vertextes[3];
		
		// 辺交差
		for(int i=0; i<3; ++i)
		{
			int j = (i+1) % 3;
			Line<T> line(points[i],points[j]);
			for(int k = 0; k < 4; ++k)
			{
				int l = (k+1) % 4;
				if(line.Intersect(rp[k], rp[l]))
					return true;
			}
		}
		return false;
	}
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
	Vec3<T> IntersectionPoint2(const Vec3<T>& point) const override
	{
		Vec3<T> ab(points[1] - points[0]);	// p0 -> p1
		Vec3<T> bp(point - points[1]);		// p1 -> point

		Vec3<T> bc(points[2] - points[1]);	// p1 -> p2
		Vec3<T> cp(point - points[2]);		// p2 -> point

		Vec3<T> ca(points[0] - points[2]);	// p2 -> p0
		Vec3<T> ap(point - points[0]);		// p0 -> point

		// z成分のみの外積
		T c1((ab.x * bp.y) - (ab.y * bp.x));
		T c2((bc.x * cp.y) - (bc.y * cp.x));
		T c3((ca.x * ap.y) - (ca.y * ap.x));
		
		Vec3<T> d_ab((Vec3<T>::Dot(ab.Normalize2(),bp) * ab.Normalize2()) - bp);			
		Vec3<T> d_bc((Vec3<T>::Dot(bc.Normalize2(),cp) * bc.Normalize2()) - cp);
		Vec3<T> d_ca((Vec3<T>::Dot(ca.Normalize2(),ap) * ca.Normalize2()) - ap);
		Vec3<T> work;	// 最も近い垂線ベクトル格納用
		// min( min(ab, bc), ca )
		work = (PYTHA(d_ab.x,d_ab.y) < PYTHA(d_bc.x,d_bc.y))? d_ab : d_bc;
		work = (PYTHA(work.x,work.y) < PYTHA(d_ca.x,d_ca.y))? work : d_ca;
		return (work + point);

	}

	/*
		@brief	線分との交点の取得
			if(Contains(line))
				points = IntersectionPoint2(line);
		@attention	交差していることが前提
		@param	[in]	line(sta,end)	比較線分
		@return	全交点
	*/
	std::vector<Vec3<T>> IntersectionPoint2(const Line<T>& line) const override
	{
		std::vector<Vec3<T>> intersections;
		for(int i=0; i<3; ++i)
		{
			int j = (i+1) % 3;
			if(line.Intersect(points[i],points[j]))
				intersections.push_back(line.IntersectionPoint2(points[i],points[j]));
		}
		return intersections;
	}
	/*
		@brief	線分との交点の取得
			if(Contains(pos,next))
				points = IntersectionPoint2(pos,next);
		@attention	交差していることが前提
		@param	[in]	sta	比較線分の始点
		@param	[in]	end	比較線分の終点
		@return	全交点
	*/
	std::vector<Vec3<T>> IntersectionPoint2(const Vec3<T>& sta, const Vec3<T>& end) const override
	{
		std::vector<Vec3<T>> intersections;
		Line<T> line(sta,end);
		for(int i=0; i<3; ++i)
		{
			int j = (i+1) % 3;
			if(line.Intersect(points[i],points[j]))
				intersections.push_back(line.IntersectionPoint2(points[i],points[j]));
		}
		return intersections;
	}


	/*
		@brief	線分との交点のうち、始点に近い点を取得
			if(Contains(line))
				point = IntersectionPoint2Nearest(line);
		@attention	交差していることが前提
		@param	[in]	line(sta,end)	比較線分
		@return	始点に近い交点
	*/
	Vec3<T> IntersectionPoint2Nearest(const Line<T>& line) const override
	{
		std::vector<Vec3<T>> intersections;
		for(int i=0; i<3; ++i)
		{
			int j = (i+1) % 3;
			if(line.Intersect(points[i],points[j]))
				intersections.push_back(line.IntersectionPoint2(points[i],points[j]));
		}
		// 始点に一番近い交点の算出
		Vec3<T> min = intersections[0];
		for(size_t i = 1; i < intersections.size(); ++i)
		{
			Vec3<T> d1 = min - line.sta;
			Vec3<T> d2 = intersections[i] - line.sta;
			if(PYTHA(d1.x, d1.y) > PYTHA(d2.x, d2.y))
				min = intersections[i];
		}
		return min;
	}
	/*
		@brief	線分との交点のうち、始点に近い点を取得
			if(Contains(pos,next))
				point = IntersectionPoint2Nearest(pos,next);
		@attention	交差していることが前提
		@param	[in]	sta	比較線分の始点
		@param	[in]	end	比較線分の終点
		@return	始点に近い交点
	*/
	Vec3<T> IntersectionPoint2Nearest(const Vec3<T>& sta, const Vec3<T>& end) const override
	{
		std::vector<Vec3<T>> intersections;
		Line<T> line(sta,end);
		for(int i=0; i<3; ++i)
		{
			int j = (i+1) % 3;
			if(line.Intersect(points[i],points[j]))
				intersections.push_back(line.IntersectionPoint2(points[i],points[j]));
		}
		// 始点に一番近い交点の算出
		Vec3<T> min = intersections[0];
		for(size_t i = 1; i < intersections.size(); ++i)
		{
			Vec3<T> d1 = min - sta;
			Vec3<T> d2 = intersections[i] - sta;
			if(PYTHA(d1.x, d1.y) > PYTHA(d2.x, d2.y))
				min = intersections[i];
		}
		return min;
	}
#pragma endregion // Intersection
	//================================================================================
	
};
typedef Triangle<float> Trif;
typedef Triangle<int> Trii;

#pragma endregion // Triangle

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
	Rect(const RECT& rect):
		left((T)rect.left), top((T)rect.top),
		right((T)rect.right), bottom((T)rect.bottom)
	{}
	
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

	//================================================================================
#pragma region Contains
	/*
		@brief	点の内包判定
		@param	[in]	point	判定する点
		@return	内包しているか
		@retval true	している
		@retval false	していない
	*/
	bool Contains(const Vec3<T>& point) const override
	{
		return left <= point.x && point.x <= right
			&& top	<= point.y && point.y <= bottom;
	}
	/*
		@brief	線分の交差、内包判定
		@param	[in]	line(sta,end)	判定する線分
		@param	[in]	fullContain		完全内包とするか(デフォルト：false)
		@return	交差、内包しているか
		@retval	true	している
		@retval	false	していない
	*/
	bool Contains(const Line<T>& line, bool fullContain = false) const override
	{
		// 完全内包
		if(fullContain)
		{
			return ( Contains(line.sta) && Contains(line.end) );
		}
		// 外周との交差判定
		bool intersection = false;
		Vec3<T> points[4] = {
			Vec3<T>(left,	top,	line.sta.z),
			Vec3<T>(right,	top,	line.sta.z),
			Vec3<T>(left,	bottom,	line.sta.z),
			Vec3<T>(right,	bottom,	line.sta.z)
		};
		for(int i = 0; i < 4; ++i)
		{
			int j = (i+1) % 4;
			if(line.Intersect(points[i],points[j]))
			{
				intersection = true;
				break;
			}
		}
		// 交点が無かった
		if(!intersection)
		{
			// 完全内包チェック(始点または終点が内包されていれば完全内包といえる)
			return Contains(line.sta);
		}
		return true;
	}
	/*
		@brief	線分の交差、内包判定
		@param	[in]	sta				判定する線分の始点
		@param	[in]	end				判定する線分の終点
		@param	[in]	fullContain		完全内包とするか(デフォルト：false)
		@return	交差、内包しているか
		@retval	true	している
		@retval	false	していない
	*/
	bool Contains(const Vec3<T>& sta, const Vec3<T>& end, bool fullContain = false) const override
	{
		// 完全内包
		if(fullContain)
		{
			return ( Contains(sta) && Contains(end) );
		}
		// 外周との交差判定
		bool intersection = false;
		Line<T> line(sta,end);
		Vec3<T> points[4] = {
			Vec3<T>(left,	top,	line.sta.z),
			Vec3<T>(right,	top,	line.sta.z),
			Vec3<T>(left,	bottom,	line.sta.z),
			Vec3<T>(right,	bottom,	line.sta.z)
		};
		for(int i = 0; i < 4; ++i)
		{
			int j = (i+1) % 4;
			if(line.Intersect(points[i],points[j]))
			{
				intersection = true;
				break;
			}
		}
		// 交点が無かった
		if(!intersection)
		{
			// 完全内包チェック(始点または終点が内包されていれば完全内包といえる)
			return Contains(sta);
		}
		return true;
	}
	/*
		@brief	三角形の交差、内包判定
		@param	[in]	tri				判定する三角形
		@param	[in]	fullContain		完全内包とするか(デフォルト：false)
		@return	交差、内包しているか
		@retval	true	している
		@retval	false	していない
	*/
	bool Contains(const Triangle<T>& tri, bool fullContain = false) const override
	{
		bool vertextes[3]={false,false,false};	// 頂点の内包
		Vec3<T> points[4] = {
			Vec3<T>(left,	top,	tri.points[0].z),
			Vec3<T>(right,	top,	tri.points[0].z),
			Vec3<T>(left,	bottom,	tri.points[0].z),
			Vec3<T>(right,	bottom,	tri.points[0].z)
		};
		// 頂点内包
		for(int i=0; i<3; ++i)
		{
			vertextes[i] = Contains(tri.points[i]);
			if(!fullContain && vertextes[i])
				return true;
		}
		if(fullContain)
			return vertextes[0] && vertextes[1] && vertextes[2];
		
		// 辺交差
		for(int i=0; i<4; ++i)
		{
			int j = (i+1) % 4;
			Line<T> line(points[i],points[j]);
			for(int k = 0; k < 3; ++k)
			{
				int l = (k+1) % 3;
				if(line.Intersect(tri.points[k], tri.points[l]))
					return true;
			}
		}
		return false;
	}
	/*
		@brief	矩形の交差、内包判定
		@param	[in]	rect			判定する矩形
		@param	[in]	fullContain		完全内包とするか(デフォルト：false)
		@return	交差、内包しているか
		@retval	true	している
		@retval	false	していない
	*/
	bool Contains(const Rect<T>& rect, bool fullContain = false) const override
	{
		if(fullContain)
		{
			bool vertextes[4]={false,false,false,false};	// 頂点の内包
			Vec3<T> rp[4] = {
				Vec3<T>(rect.left,rect.top,0),
				Vec3<T>(rect.right,rect.top,0),
				Vec3<T>(rect.left,rect.bottom,0),
				Vec3<T>(rect.right,rect.bottom,0)
			};
			// 頂点内包
			for(int i=0; i<4; ++i)
			{
				if(Contains(rp[i])) continue;
				return false;
			}
			return true;
		}
		else
		{
			if(left > rect.right) return false;
			if(rect.left > right) return false;
			if(top > rect.bottom) return false;
			if(rect.top > bottom) return false;
			return true;
		}
		/*
		bool vertextes[4]={false,false,false,false};	// 頂点の内包
		Vec3<T> points[4] = {
			Vec3<T>(left,top,0),
			Vec3<T>(right,top,0),
			Vec3<T>(left,bottom,0),
			Vec3<T>(right,bottom,0)
		};
		Vec3<T> rp[4] = {
			Vec3<T>(rect.left,rect.top,0),
			Vec3<T>(rect.right,rect.top,0),
			Vec3<T>(rect.left,rect.bottom,0),
			Vec3<T>(rect.right,rect.bottom,0)
		};
		// 頂点内包
		for(int i=0; i<4; ++i)
		{
			vertextes[i] = Contains(rp[i]);
			if(!fullContain && vertextes[i])
				return true;
		}
		if(fullContain)
			return vertextes[0] && vertextes[1] && vertextes[2] && vertextes[3];
		
		// 辺交差
		for(int i=0; i<4; ++i)
		{
			int j = (i+1) % 4;
			Line<T> line(points[i],points[j]);
			for(int k = 0; k < 4; ++k)
			{
				int l = (k+1) % 4;
				if(line.Intersect(rp[k], rp[l]))
					return true;
			}
		}
		*/
	}
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
	Vec3<T> IntersectionPoint2(const Vec3<T>& point) const override
	{
		Vec3<T> points[4] = {
			Vec3<T>(left,	top,	point.z),
			Vec3<T>(right,	top,	point.z),
			Vec3<T>(left,	bottom,	point.z),
			Vec3<T>(right,	bottom,	point.z)
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
		
		Vec3<T> d_ab((Vec3<T>::Dot(ab.Normalize2(),bp) * ab.Normalize2()) - bp);			
		Vec3<T> d_bc((Vec3<T>::Dot(bc.Normalize2(),cp) * bc.Normalize2()) - cp);
		Vec3<T> d_cd((Vec3<T>::Dot(cd.Normalize2(),dp) * cd.Normalize2()) - dp);
		Vec3<T> d_da((Vec3<T>::Dot(da.Normalize2(),ap) * da.Normalize2()) - ap);
		Vec3<T> work;	// 最も近い垂線ベクトル格納用
		// min( min( min(ab, bc), cd ) , da)
		work = (PYTHA(d_ab.x,d_ab.y) < PYTHA(d_bc.x,d_bc.y))? d_ab : d_bc;
		work = (PYTHA(work.x,work.y) < PYTHA(d_cd.x,d_cd.y))? work : d_cd;
		work = (PYTHA(work.x,work.y) < PYTHA(d_da.x,d_da.y))? work : d_da;
		return (work + point);

	}

	/*
		@brief	線分との交点の取得
			if(Contains(line))
				points = IntersectionPoint2(line);
		@attention	交差していることが前提
		@param	[in]	line(sta,end)	比較線分
		@return	全交点
	*/
	std::vector<Vec3<T>> IntersectionPoint2(const Line<T>& line) const override
	{
		std::vector<Vec3<T>> intersections;
		Vec3<T> points[4] = {
			Vec3<T>(left,	top,	line.sta.z),
			Vec3<T>(right,	top,	line.sta.z),
			Vec3<T>(left,	bottom,	line.sta.z),
			Vec3<T>(right,	bottom,	line.sta.z)
		};
		for(int i=0; i<4; ++i)
		{
			int j = (i+1) % 4;
			if(line.Intersect(points[i],points[j]))
				intersections.push_back(line.IntersectionPoint2(points[i],points[j]));
		}
		return intersections;
	}
	/*
		@brief	線分との交点の取得
			if(Contains(pos,next))
				points = IntersectionPoint2(pos,next);
		@attention	交差していることが前提
		@param	[in]	sta	比較線分の始点
		@param	[in]	end	比較線分の終点
		@return	全交点
	*/
	std::vector<Vec3<T>> IntersectionPoint2(const Vec3<T>& sta, const Vec3<T>& end) const override
	{
		std::vector<Vec3<T>> intersections;
		Line<T> line(sta,end);
		Vec3<T> points[4] = {
			Vec3<T>(left,	top,	line.sta.z),
			Vec3<T>(right,	top,	line.sta.z),
			Vec3<T>(left,	bottom,	line.sta.z),
			Vec3<T>(right,	bottom,	line.sta.z)
		};
		for(int i=0; i<4; ++i)
		{
			int j = (i+1) % 4;
			if(line.Intersect(points[i],points[j]))
				intersections.push_back(line.IntersectionPoint2(points[i],points[j]));
		}
		return intersections;
	}


	/*
		@brief	線分との交点のうち、始点に近い点を取得
			if(Contains(line))
				point = IntersectionPoint2Nearest(line);
		@attention	交差していることが前提
		@param	[in]	line(sta,end)	比較線分
		@return	始点に近い交点
	*/
	Vec3<T> IntersectionPoint2Nearest(const Line<T>& line) const override
	{
		std::vector<Vec3<T>> intersections;
		Vec3<T> points[4] = {
			Vec3<T>(left,	top,	line.sta.z),
			Vec3<T>(right,	top,	line.sta.z),
			Vec3<T>(left,	bottom,	line.sta.z),
			Vec3<T>(right,	bottom,	line.sta.z)
		};
		for(int i=0; i<4; ++i)
		{
			int j = (i+1) % 4;
			if(line.Intersect(points[i],points[j]))
				intersections.push_back(line.IntersectionPoint2(points[i],points[j]));
		}
		// 始点に一番近い交点の算出
		Vec3<T> min = intersections[0];
		for(size_t i = 1; i < intersections.size(); ++i)
		{
			Vec3<T> d1 = min - line.sta;
			Vec3<T> d2 = intersections[i] - line.sta;
			if(PYTHA(d1.x, d1.y) > PYTHA(d2.x, d2.y))
				min = intersections[i];
		}
		return min;
	}
	/*
		@brief	線分との交点のうち、始点に近い点を取得
			if(Contains(pos,next))
				point = IntersectionPoint2Nearest(pos,next);
		@attention	交差していることが前提
		@param	[in]	sta	比較線分の始点
		@param	[in]	end	比較線分の終点
		@return	始点に近い交点
	*/
	Vec3<T> IntersectionPoint2Nearest(const Vec3<T>& sta, const Vec3<T>& end) const override
	{
		std::vector<Vec3<T>> intersections;
		Line<T> line(sta,end);
		Vec3<T> points[4] = {
			Vec3<T>(left,	top,	line.sta.z),
			Vec3<T>(right,	top,	line.sta.z),
			Vec3<T>(left,	bottom,	line.sta.z),
			Vec3<T>(right,	bottom,	line.sta.z)
		};
		for(int i=0; i<4; ++i)
		{
			int j = (i+1) % 4;
			if(line.Intersect(points[i],points[j]))
				intersections.push_back(line.IntersectionPoint2(points[i],points[j]));
		}
		// 始点に一番近い交点の算出
		Vec3<T> min = intersections[0];
		for(size_t i = 1; i < intersections.size(); ++i)
		{
			Vec3<T> d1 = min - sta;
			Vec3<T> d2 = intersections[i] - sta;
			if(PYTHA(d1.x, d1.y) > PYTHA(d2.x, d2.y))
				min = intersections[i];
		}
		return min;
	}
#pragma endregion // Intersection
	//================================================================================
	
	template<class T1>
	Rect<T>& operator = (const Rect<T1>& rt)
	{
		left	= static_cast<T>(rt.left);
		top		= static_cast<T>(rt.top);
		right	= static_cast<T>(rt.right);
		bottom	= static_cast<T>(rt.bottom);
		return *this;
	}
	template<class T1>
	operator Rect<T1>()
	{
		Rect<T1> rt(	static_cast<T1>(left),
						static_cast<T1>(top),
						static_cast<T1>(right),
						static_cast<T1>(bottom));

		return rt;
	}

	/*
		@brief　矩形をずらす
		@param	[in]	ofs_x	水平方向にそれぞれに加算する量
		@param	[in]	ofs_y	鉛直方向にそれぞれに加算する量
		return ずらしたあとの自身(連続して書けるように)
	
	Rect<T>& offset(T ofs_x, T ofs_y)
	{
		left	+= ofs_x;
		right	+= ofs_x;
		top		+= ofs_y;
		bottom	+= ofs_y;
		return *this;
	}*/
};
typedef Rect<int>	Recti;
typedef Rect<float>	Rectf;

#pragma endregion // Rect

//=================================================================================================
//=================================================================================================
//=================================================================================================
#pragma region Circle

#ifdef USE_CIRCLE_EXT
#include "../../../lib/gplib.h"
#endif
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
	Circle():center(Vec3<T1>()),radius(0)
	{}
	Circle(T1 x, T1 y, T1 z, T2 r):
		center(Vec3<T1>(x,y,z)),radius(r)
	{}
	Circle(const Vec3<T1>& c, T2 r):
		center(c), radius(r)
	{}

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
		return (PYTHA(p.x-c.x, p.y-c.y) <= POW2(radius) );
	}

	/*
		@brief	線分の交差、内包判定
		@param	[in]	line(sta,end)	判定する線分
		@param	[in]	fullContain		完全内包とするか(デフォルト：false)
		@return	交差、内包しているか
		@retval	true	している
		@retval	false	していない
	*/
	bool Contains(const Line<T1>& line, bool fullContain = false) const override
	{
		Vec3<T1> sp(center - line.sta); // 線分始点から円中心へのベクトル(start->pos)
		Vec3<T1> ep(center - line.end); // 線分終点から円中心へのベクトル(end->pos)
		bool isInner = (PYTHA(sp.x,sp.y) < POW2(radius))
					&& (PYTHA(ep.x,ep.y) < POW2(radius));
		// 完全内包(円の内側)
		if(fullContain)
		{
			return isInner;
		}
		if(isInner)
		{
			return false;
		}
		Vec3<T1> nv((line.end - line.sta).Normalize2());	// 線分の単位ベクトル(normalize-vector)
		// 円中心から線分への垂線 d=((n・p)*n)-p
		Vec3<T1> dist((Vec3<T1>::Dot(nv,sp) * nv) - sp);
		
		if(PYTHA(dist.x,dist.y) > POW2(radius))
		{
			// 垂線判定(円の外側)
			return false;
		}
		else if((PYTHA(sp.x,sp.y) > POW2(radius) && PYTHA(ep.x,ep.y) > POW2(radius))
			&&	(((sp.x > 0.f)? 1.f : -1.f) == ((ep.x > 0.f)? 1.f : -1.f))
			&&	(((sp.y > 0.f)? 1.f : -1.f) == ((ep.y > 0.f)? 1.f : -1.f)))
		{
			// 符号判定(中心点をまたがない)
			return false;
		}
		return true;
	}
	/*
		@brief	線分の交差、内包判定
		@param	[in]	sta				判定する線分の始点
		@param	[in]	end				判定する線分の終点
		@param	[in]	fullContain		完全内包とするか(デフォルト：false)
		@return	交差、内包しているか
		@retval	true	している
		@retval	false	していない
	*/
	bool Contains(const Vec3<T1>& sta, const Vec3<T1>& end, bool fullContain = false) const override
	{
		Vec3<T1> sp(center - sta); // 線分始点から円中心へのベクトル
		Vec3<T1> ep(center - end); // 線分終点から円中心へのベクトル
		bool isInner = (PYTHA(sp.x,sp.y) < POW2(radius))
					&& (PYTHA(ep.x,ep.y) < POW2(radius));
		// 完全内包(円の内側)
		if(fullContain)
		{
			return isInner;
		}
		if(isInner)
		{
			return false;
		}
		Vec3<T1> nv((end - sta).Normalize2());	// 線分の単位ベクトル
		// 円中心から線分への垂線 d=((n・p)n)-p
		Vec3<T1> dist((Vec3<T1>::Dot(nv,sp) * nv) - sp);
		if(PYTHA(dist.x,dist.y) > POW2(radius))
		{
			// 垂線判定(円の外側)
			return false;
		}
		else if((PYTHA(sp.x,sp.y) > POW2(radius) && PYTHA(ep.x,ep.y) > POW2(radius))
			&&	(((sp.x > 0.f)? 1.f : -1.f) == ((ep.x > 0.f)? 1.f : -1.f))
			&&	(((sp.y > 0.f)? 1.f : -1.f) == ((ep.y > 0.f)? 1.f : -1.f)))
		{
			// 符号判定(中心点をまたがない)
			return false;
		}
		return true;
	}
	

	/*
		@brief	線分の内包判定
		@attension	線分の両端が円の中にある場合は第3引数により変わる
					完全内包と円周交差は内包が優先される 
		@param	[in]	line(sta,end)	判定する線分(始点、終点)
		@param	[in]	fullContain		完全内包とするか
		@param	[in]	intersectOnly	円周との交差のみにするか
		@return	内包しているか
		@retval	true	している
		@retval	false	していない
	*/
	bool Contains(const Line<T1>& line, bool fullContain, bool intersectOnly) const
	{
		Vec3<T1> sp(center - line.sta); // 線分始点から円中心へのベクトル(start->pos)
		Vec3<T1> ep(center - line.end); // 線分終点から円中心へのベクトル(end->pos)
		bool isInner = (PYTHA(sp.x,sp.y) < POW2(radius))
					&& (PYTHA(ep.x,ep.y) < POW2(radius));
		// 完全内包(円の内側)
		if(fullContain)
		{
			return isInner;
		}
		if(isInner)
		{
			return !intersectOnly;
		}
		Vec3<T1> nv((line.end - line.sta).Normalize2());	// 線分の単位ベクトル(normalize-vector)
		// 円中心から線分への垂線 d=((n・p)*n)-p
		Vec3<T1> dist((Vec3<T1>::Dot(nv,sp) * nv) - sp);
		
		if(PYTHA(dist.x,dist.y) > POW2(radius))
		{
			// 垂線判定(円の外側)
			return false;
		}
		else if((PYTHA(sp.x,sp.y) > POW2(radius) && PYTHA(ep.x,ep.y) > POW2(radius))
			&&	(((sp.x > 0.f)? 1.f : -1.f) == ((ep.x > 0.f)? 1.f : -1.f))
			&&	(((sp.y > 0.f)? 1.f : -1.f) == ((ep.y > 0.f)? 1.f : -1.f)))
		{
			// 符号判定(中心点をまたがない)
			return false;
		}
		return true;
	}
	/*
		@brief	線分の内包判定
		@attension	線分の両端が円の中にある場合は第4引数により変わる
					完全内包と円周交差は内包が優先される
		@param	[in]	sta	判定する線分の始点
		@param	[in]	end	判定する線分の終点
		@param	[in]	fullContain		完全内包とするか
		@param	[in]	intersectOnly	円周との交差のみにするか
		@return	内包しているか
		@retval	true	している
		@retval	false	していない
	*/
	bool Contains(const Vec3<T1>& sta, const Vec3<T1>& end, bool fullContain, bool intersectOnly) const
	{
		Vec3<T1> sp(center - sta); // 線分始点から円中心へのベクトル
		Vec3<T1> ep(center - end); // 線分終点から円中心へのベクトル
		bool isInner = (PYTHA(sp.x,sp.y) < POW2(radius))
					&& (PYTHA(ep.x,ep.y) < POW2(radius));
		// 完全内包(円の内側)
		if(fullContain)
		{
			return isInner;
		}
		if(isInner)
		{
			return !intersectOnly;
		}
		Vec3<T1> nv((end - sta).Normalize2());	// 線分の単位ベクトル
		// 円中心から線分への垂線 d=((n・p)n)-p
		Vec3<T1> dist((Vec3<T1>::Dot(nv,sp) * nv) - sp);
		if(PYTHA(dist.x,dist.y) > POW2(radius))
		{
			// 垂線判定(円の外側)
			return false;
		}
		else if((PYTHA(sp.x,sp.y) > POW2(radius) && PYTHA(ep.x,ep.y) > POW2(radius))
			&&	(((sp.x > 0.f)? 1.f : -1.f) == ((ep.x > 0.f)? 1.f : -1.f))
			&&	(((sp.y > 0.f)? 1.f : -1.f) == ((ep.y > 0.f)? 1.f : -1.f)))
		{
			// 符号判定(中心点をまたがない)
			return false;
		}
		return true;
	}


	
	/*
		@brief	三角形の交差、内包判定
		@param	[in]	tri				判定する三角形
		@param	[in]	fullContain		完全内包とするか(デフォルト：false)
		@return	交差、内包しているか
		@retval	true	している
		@retval	false	していない
	*/
	bool Contains(const Triangle<T1>& tri, bool fullContain = false) const override
	{
		bool vertextes[3]={false,false,false};	// 頂点の内包
		// 頂点内包
		for(int i=0; i<3; ++i)
		{
			vertextes[i] = Contains(tri.points[i]);
			if(!fullContain && vertextes[i])
				return true;
		}
		if(fullContain)
			return vertextes[0] && vertextes[1] && vertextes[2];
		
		// 辺交差
		for(int i = 0; i < 3; ++i)
		{
			int j = (i+1) % 3;
			if(Contains(tri.points[i], tri.points[j]))
				return true;
		}
		return false;
	}
	/*
		@brief	矩形の交差、内包判定
		@param	[in]	rect			判定する矩形
		@param	[in]	fullContain		完全内包とするか(デフォルト：false)
		@return	交差、内包しているか
		@retval	true	している
		@retval	false	していない
	*/
	bool Contains(const Rect<T1>& rect, bool fullContain = false) const override
	{
		bool vertextes[4]={false,false,false,false};	// 頂点の内包
		Vec3<T1> rp[4] = {
			Vec3<T1>(rect.left,rect.top,0),
			Vec3<T1>(rect.right,rect.top,0),
			Vec3<T1>(rect.left,rect.bottom,0),
			Vec3<T1>(rect.right,rect.bottom,0)
		};
		// 頂点内包
		for(int i=0; i<4; ++i)
		{
			vertextes[i] = Contains(rp[i]);
			if(!fullContain && vertextes[i])
				return true;
		}
		if(fullContain)
			return vertextes[0] && vertextes[1] && vertextes[2] && vertextes[3];
		
		// 辺交差
		for(int i = 0; i < 4; ++i)
		{
			int j = (i+1) % 4;
			if(Contains(rp[i], rp[j]))
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
			if(Contains(p))
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
		return  (center + (dist.Normalize2() * radius) );
	}

	/*
		@brief	線分との交点の取得
			if(Contains(line))
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
		Vec3<T1> dist((Vec3<T1>::Dot(nv,sp) * nv) - sp);
		
		if(PYTHA(dist.x,dist.y) == POW2(radius))
		{
			intersections.push_back(dist + center);	// 一接点
		}
		else
		{
			float d_rad;	// 垂線の角度
			float i_rad;	// 垂線から交点の角度
			if(PYTHA(dist.x,dist.y) > 0.0f)
			{
				d_rad = atan2f(dist.y,dist.x);
				i_rad = acosf(dist.Length2() / radius);
			}
			else
			{
				// 線分が円中心を通る場合
				d_rad = atan2f(line.end.y,line.end.x);
				i_rad = PI/2.0f;
			}

			Vec3<T1> temp[] =
			{
				// 垂線と無限線分を中心からのT字と考えたとき,それぞれ線分の右,左の交点
				Vec3<T1>(	static_cast<T1>(static_cast<float>(radius) * std::cosf(d_rad + i_rad)),
							static_cast<T1>(static_cast<float>(radius) * std::sinf(d_rad + i_rad))),
				Vec3<T1>(	static_cast<T1>(static_cast<float>(radius) * std::cosf(d_rad - i_rad)),
							static_cast<T1>(static_cast<float>(radius) * std::sinf(d_rad - i_rad))),
			};

			if(PYTHA(sp.x,sp.y) >= POW2(radius) && PYTHA(ep.x,ep.y) >= POW2(radius))
			{
				for(const auto& t : temp)
				{
					intersections.push_back(t + center);	// 二交点
				}
			}
			else
			{
				// 一交点
				if(PYTHA(sp.x,sp.y) >= POW2(radius))
				{
					// 円外の座標から交点までの距離
					Vec3<T1> a(temp[0] - sp);
					Vec3<T1> b(temp[1] - sp);
					intersections.push_back(((PYTHA(a.x,a.y) > PYTHA(b.x,b.y))? temp[0] : temp[1]) + center);
				}
				else
				{
					// 円外の座標から交点までの距離
					Vec3<T1> a(temp[0] - ep);
					Vec3<T1> b(temp[1] - ep);
					intersections.push_back(((PYTHA(a.x,a.y) > PYTHA(b.x,b.y))? temp[0] : temp[1]) + center);
				}
			}
		}
		return intersections;
	}
	/*
		@brief	線分との交点の取得
			if(Contains(pos,next))
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
		Vec3<T1> dist((Vec3<T1>::Dot(nv,sp) * nv) - sp);
		
		if(PYTHA(dist.x,dist.y) == POW2(radius))
		{
			intersections.push_back(dist + center);	// 一接点
		}
		else
		{
			float d_rad;	// 垂線の角度
			float i_rad;	// 垂線から交点の角度
			if(PYTHA(dist.x,dist.y) > 0.0f)
			{
				d_rad = atan2f(dist.y,dist.x);
				i_rad = acosf(dist.Length2() / radius);
			}
			else
			{
				// 線分が円中心を通る場合
				d_rad = atan2f(end.y,end.x);
				i_rad = PI/2.0f;
			}

			Vec3<T1> temp[] =
			{
				// 垂線と無限線分を中心からのT字と考えたとき,それぞれ線分の右,左の交点
				Vec3<T1>(	static_cast<T1>(static_cast<float>(radius) * std::cosf(d_rad + i_rad)),
							static_cast<T1>(static_cast<float>(radius) * std::sinf(d_rad + i_rad))),
				Vec3<T1>(	static_cast<T1>(static_cast<float>(radius) * std::cosf(d_rad - i_rad)),
							static_cast<T1>(static_cast<float>(radius) * std::sinf(d_rad - i_rad))),
			};

			if(PYTHA(sp.x,sp.y) >= POW2(radius) && PYTHA(ep.x,ep.y) >= POW2(radius))
			{
				for(const auto& t : temp)
				{
					intersections.push_back(t + center);	// 二交点
				}
			}
			else
			{
				// 一交点
				if(PYTHA(sp.x,sp.y) >= POW2(radius))
				{
					// 円外の座標から交点までの距離
					Vec3<T1> a(temp[0] - sp);
					Vec3<T1> b(temp[1] - sp);
					intersections.push_back(((PYTHA(a.x,a.y) > PYTHA(b.x,b.y))? temp[0] : temp[1]) + center);
				}
				else
				{
					// 円外の座標から交点までの距離
					Vec3<T1> a(temp[0] - ep);
					Vec3<T1> b(temp[1] - ep);
					intersections.push_back(((PYTHA(a.x,a.y) > PYTHA(b.x,b.y))? temp[0] : temp[1]) + center);
				}
			}
		}
		return intersections;
	}


	/*
		@brief	線分との交点のうち、始点に近い点を取得
			if(Contains(line))
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
		Vec3<T1> dist((Vec3<T1>::Dot(nv,sp) * nv) - sp);
		
		if(PYTHA(dist.x,dist.y) == POW2(radius))
		{
			intersections.push_back(dist + center);	// 一接点
		}
		else
		{
			float d_rad;	// 垂線の角度
			float i_rad;	// 垂線から交点の角度
			if(PYTHA(dist.x,dist.y) > 0.0f)
			{
				d_rad = atan2f(dist.y,dist.x);
				i_rad = acosf(dist.Length2() / radius);
			}
			else
			{
				// 線分が円中心を通る場合
				d_rad = atan2f(line.end.y,line.end.x);
				i_rad = PI/2.0f;
			}

			Vec3<T1> temp[] =
			{
				// 垂線と無限線分を中心からのT字と考えたとき,それぞれ線分の右,左の交点
				Vec3<T1>(	static_cast<T1>(static_cast<float>(radius) * std::cosf(d_rad + i_rad)),
							static_cast<T1>(static_cast<float>(radius) * std::sinf(d_rad + i_rad))),
				Vec3<T1>(	static_cast<T1>(static_cast<float>(radius) * std::cosf(d_rad - i_rad)),
							static_cast<T1>(static_cast<float>(radius) * std::sinf(d_rad - i_rad))),
			};

			if(PYTHA(sp.x,sp.y) >= POW2(radius) && PYTHA(ep.x,ep.y) >= POW2(radius))
			{
				for(const auto& t : temp)
				{
					intersections.push_back(t + center);	// 二交点
				}
			}
			else
			{
				// 一交点
				if(PYTHA(sp.x,sp.y) >= POW2(radius))
				{
					// 円外の座標から交点までの距離
					Vec3<T1> a(temp[0] - sp);
					Vec3<T1> b(temp[1] - sp);
					intersections.push_back(((PYTHA(a.x,a.y) > PYTHA(b.x,b.y))? temp[0] : temp[1]) + center);
				}
				else
				{
					// 円外の座標から交点までの距離
					Vec3<T1> a(temp[0] - ep);
					Vec3<T1> b(temp[1] - ep);
					intersections.push_back(((PYTHA(a.x,a.y) > PYTHA(b.x,b.y))? temp[0] : temp[1]) + center);
				}
			}
		}
		// 始点に一番近い交点の算出
		Vec3<T1> min = intersections[0];
		for(size_t i = 1; i < intersections.size(); ++i)
		{
			Vec3<T1> d1 = min - line.sta;
			Vec3<T1> d2 = intersections[i] - line.sta;
			if(PYTHA(d1.x, d1.y) > PYTHA(d2.x, d2.y))
				min = intersections[i];
		}
		return min;
	}
	/*
		@brief	線分との交点のうち、始点に近い点を取得
			if(Contains(pos,next))
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
		Vec3<T1> dist((Vec3<T1>::Dot(nv,sp) * nv) - sp);
		
		if(PYTHA(dist.x,dist.y) == POW2(radius))
		{
			intersections.push_back(dist + center);	// 一接点
		}
		else
		{
			float d_rad;	// 垂線の角度
			float i_rad;	// 垂線から交点の角度
			if(PYTHA(dist.x,dist.y) > 0.0f)
			{
				d_rad = atan2f(dist.y,dist.x);
				i_rad = acosf(dist.Length2() / radius);
			}
			else
			{
				// 線分が円中心を通る場合
				d_rad = atan2f(end.y,end.x);
				i_rad = PI/2.0f;
			}

			Vec3<T1> temp[] =
			{
				// 垂線と無限線分を中心からのT字と考えたとき,それぞれ線分の右,左の交点
				Vec3<T1>(	static_cast<T1>(static_cast<float>(radius) * std::cosf(d_rad + i_rad)),
							static_cast<T1>(static_cast<float>(radius) * std::sinf(d_rad + i_rad))),
				Vec3<T1>(	static_cast<T1>(static_cast<float>(radius) * std::cosf(d_rad - i_rad)),
							static_cast<T1>(static_cast<float>(radius) * std::sinf(d_rad - i_rad))),
			};

			if(PYTHA(sp.x,sp.y) >= POW2(radius) && PYTHA(ep.x,ep.y) >= POW2(radius))
			{
				for(const auto& t : temp)
				{
					intersections.push_back(t + center);	// 二交点
				}
			}
			else
			{
				// 一交点
				if(PYTHA(sp.x,sp.y) >= POW2(radius))
				{
					// 円外の座標から交点までの距離
					Vec3<T1> a(temp[0] - sp);
					Vec3<T1> b(temp[1] - sp);
					intersections.push_back(((PYTHA(a.x,a.y) > PYTHA(b.x,b.y))? temp[0] : temp[1]) + center);
				}
				else
				{
					// 円外の座標から交点までの距離
					Vec3<T1> a(temp[0] - ep);
					Vec3<T1> b(temp[1] - ep);
					intersections.push_back(((PYTHA(a.x,a.y) > PYTHA(b.x,b.y))? temp[0] : temp[1]) + center);
				}
			}
		}
		// 始点に一番近い交点の算出
		Vec3<T1> min = intersections[0];
		for(size_t i = 1; i < intersections.size(); ++i)
		{
			Vec3<T1> d1 = min - sta;
			Vec3<T1> d2 = intersections[i] - sta;
			if(PYTHA(d1.x, d1.y) > PYTHA(d2.x, d2.y))
				min = intersections[i];
		}
		return min;
	}
#pragma endregion // Intersection
	//================================================================================
	
#ifdef USE_CIRCLE_EXT
	void draw(D3DCOLOR color = 0xffffffff)
	{
		for(int i=0; i<360; ++i)
		{
			float rad = static_cast<float>(i)	* 2.0f * PI / 360.0f;
			float ra2 = static_cast<float>(i+1)	* 2.0f * PI / 360.0f;
			POINT sta = { LONG(std::cosf(rad) * radius + center.x), LONG(-std::sinf(rad) * radius + center.y) };
			POINT end = { LONG(std::cosf(ra2) * radius + center.x), LONG(-std::sinf(ra2) * radius + center.y) };
			Draw_Line(	(int)sta.x,
						(int)sta.y,
						(int)end.x,
						(int)end.y,
						0.f,
						color,1);
		}
	}

	static void draw(T1 center_x, T1 center_y, T1 center_z, T2 radius, D3DCOLOR color = 0xffffffff)
	{
		for(int i=0; i<360; ++i)
		{
			float rad = static_cast<float>(i)	* 2.0f * PI / 360.0f;
			float ra2 = static_cast<float>(i+1)	* 2.0f * PI / 360.0f;
			POINT sta = { LONG(std::cosf(rad) * radius + center_x), LONG(-std::sinf(rad) * radius + center_y) };
			POINT end = { LONG(std::cosf(ra2) * radius + center_x), LONG(-std::sinf(ra2) * radius + center_y) };
			Draw_Line(	(int)sta.x,
						(int)sta.y,
						(int)end.x,
						(int)end.y,
						center_z,
						color,1);
		}
	}
//#else
//	void draw(unsigned long color = 0xffffffff)
//	{
//	}
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
