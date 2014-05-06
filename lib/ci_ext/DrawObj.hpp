#pragma once

#include "../gplib.h"
#include "vec3.hpp"
#include "color.hpp"

namespace ci_ext
{

template<typename T>
class DrawObj
{
	Vec3<T> pos_;
	Vec3<T> scale_;
	IMGOBJ img_;
	Vec3<T> src_;
	Vec3<T> srcSize_;
	Color color_;
	T degree_;
	DrawObj() {}
public:
	DrawObj(
		const Vec3<T>& pos, const Vec3<T>& scale, const IMGOBJ img,
		const Vec3<T>& src, const Vec3<T>& srcSize,
		const Color& color, const T degree) 
		:
		pos_(pos),
		scale_(scale),
		img_(img),
		src_(src),
		srcSize_(srcSize),
		color_(color),
		degree_(degree)
	{}

	void draw() const
	{
		Draw_GraphicsCenter(
			pos_.ix(), pos_.iy(), 
			pos_.z(),	img_,
			src_.ix(), src_.iy(),
			srcSize_.ix(), srcSize_.iy(),
			static_cast<int>(degree_), nullptr,
			scale_.x(), scale_.y(),
			color_.a(), color_.r(), color_.g(), color_.b());
	}

	void setPos(const Vec3<T>& pos)
	{
		pos_ = pos;
	}

	void setPos(const T x, const T y, const T z)
	{
		pos_ = Vec3<T>(x, y, z);
	}

	void offsetPos(const Vec3<T>& offset)
	{
		pos_ += offset;
	}

	void setScale(const Vec3<T>& scale)
	{
		scale_ = scale;
	}

	void offsetScale(const Vec3<T>& offset)
	{
		scale_ += offset;
	}

	void setSrcInfo(const Vec3<T>& src, const Vec3<T>& size)
	{
		src_ = src;
		size_ = size;
	}

	void setDegree(const T degree)
	{
		degree_ = degree;
	}

	void offsetDegree(const T offset)
	{
		T temp0   = static_cast<T>(0);
		T temp360 = static_cast<T>(360);
		degree_ += offset;
		if(degree_ > temp360)    degree_ -= temp360;
		else if(degree_ < temp0) degree_ += temp360;
	}

	void setColor(const Color& color)
	{
		color_ = color;
	}
	void offsetColor(const Color& offset)
	{
		color_ += offset;
	}

	void setImg(const IMGOBJ img)
	{
		img_ = img;
	}

	Vec3<T> pos()     const { return pos_;     }
	Vec3<T> scale()   const { return scale_;   }
	Vec3<T> src()     const { return src_;     }
	Vec3<T> srcSize() const { return srcSize_; }
	T degree()        const { return degree_;  }
	Color color()     const { return color_;   }
	IMGOBJ img()      const { return img_;     }
	T x()             const { return pos_.x(); }
	T y()             const { return pos_.y(); }
	T z()             const { return pos_.z(); }
	int ix()          const { return pos_.ix();}
	int iy()          const { return pos_.iy();}
	int iz()          const { return pos_.iz();}
	DrawObj obj()     const { return *this;    }
};

}