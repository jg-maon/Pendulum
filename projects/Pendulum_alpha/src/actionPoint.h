#ifndef DEF_ACTIONPOINT_H
#define DEF_ACTIONPOINT_H

#ifndef DEF_OBJ_H
#include "Obj.h"
#endif

#ifndef DEF_MYMATH_HPP
#include "MyMath.hpp"
#endif

#include <vector>	// Polygon�p

#pragma region class IActionPoint
class IActionPoint : public IObject
{
public:
	IActionPoint(const std::string& name, float x=0.f, float y=0.f);
	virtual ~IActionPoint() = 0;
	virtual void step() = 0;
	virtual void draw() = 0;
	/*
		@brief	�A�N�V�����|�C���g�̎擾
		@return	�A�N�V�����|�C���g
	*/
	virtual mymath::ShapefPtr GetAP() = 0;
	//================================================================================
#pragma region Contains
	/*
		@brief	�_�̓����
		@param	[in]	point	���肷��_
		@return	����Ă��邩
		@retval true	���Ă���
		@retval false	���Ă��Ȃ�
	*/
	virtual bool Contains(const mymath::Vec3f& point) const;
	/*
		@brief			�����̌����A�����
		@attension		�����̗��[�����E���̒��ɂ���ꍇ�͑�3�����ɂ��ς��
						���S����Ƌ��E�������͓���D�悳���
		@param	[in]	line(sta,end)	���肷�����(�n�_�A�I�_)
		@param	[in]	fullContain		���S����Ƃ��邩(�f�t�H���g�Ffalse)
		@param	[in]	intersectOnly	���E���Ƃ̌����݂̂ɂ��邩(�f�t�H���g�Ftrue)
		@return			�����A����Ă��邩
		@retval	true	���Ă���
		@retval	false	���Ă��Ȃ�
	*/
	virtual bool Contains(const mymath::Linef& line, bool fullContain = false, bool intersectOnly = true) const;
	/*
		@brief			�����̌����A�����
		@attension		�����̗��[�����E���̒��ɂ���ꍇ�͑�4�����ɂ��ς��
						���S����Ƌ��E�������͓���D�悳���
		@param	[in]	sta	���肷������̎n�_
		@param	[in]	end	���肷������̏I�_
		@param	[in]	fullContain		���S����Ƃ��邩(�f�t�H���g�Ffalse)
		@param	[in]	intersectOnly	���E���Ƃ̌����݂̂ɂ��邩(�f�t�H���g�Ftrue)
		@return			�����A����Ă��邩
		@retval	true	���Ă���
		@retval	false	���Ă��Ȃ�
	*/
	virtual bool Contains(const mymath::Vec3f& sta, const mymath::Vec3f& end, bool fullContain = false, bool intersectOnly = true) const;
	
	/*
		@brief			�}�`�̌����A�����
		@attension		�����̗��[�����E���̒��ɂ���ꍇ�͑�3�����ɂ��ς��
		���S����Ƌ��E�������͓���D�悳���
		@param	[in]	shape			���肷��}�`
		@param	[in]	fullContain		���S����Ƃ��邩(�f�t�H���g�Ffalse)
		@param	[in]	intersectOnly	���E���Ƃ̌����݂̂ɂ��邩(�f�t�H���g�Ftrue)
		@return			�����A����Ă��邩
		@retval	true	���Ă���
		@retval	false	���Ă��Ȃ�
	*/
	virtual bool Contains(const mymath::ShapefPtr& shape, bool fullContain = false, bool intersectOnly = true) const;

#pragma endregion // Contains
	//================================================================================

	//================================================================================
#pragma region Intersection
	/*
		@brief	����Ă���_�����ԋ߂���_�̎擾
			if(Contains(p))
				point = IntersectionPoint2(p);
		@attention	�������Ă��邱�Ƃ��O��
		@param	[in]	point	����Ă���_
		@return	��_
	*/
	virtual mymath::Vec3f IntersectionPoint2(const mymath::Vec3f& point) const;

	/*
		@brief	�����Ƃ̌�_�̎擾
			if(Contains(line))
				points = IntersectionPoint2(line);
		@attention	�������Ă��邱�Ƃ��O��
		@param	[in]	line(sta,end)	��r����
		@return	�S��_
	*/
	virtual std::vector<mymath::Vec3f> IntersectionPoint2(const mymath::Linef& line) const;
	/*
		@brief	�����Ƃ̌�_�̎擾
			if(Contains(pos,next))
				points = IntersectionPoint2(pos,next);
		@attention	�������Ă��邱�Ƃ��O��
		@param	[in]	sta	��r�����̎n�_
		@param	[in]	end	��r�����̏I�_
		@return	�S��_
	*/
	virtual std::vector<mymath::Vec3f> IntersectionPoint2(const mymath::Vec3f& sta, const mymath::Vec3f& end) const;

	/*
		@brief	�}�`�Ƃ̌�_�̎擾
			if(Contains(shape))
				points = IntersectionPoint2(shape);
		@attention	�������Ă��邱�Ƃ��O��
		@param	[in]	shape	���肷��}�`
		@return	�S��_
	*/
	virtual std::vector<mymath::Vec3f> IntersectionPoint2(const mymath::ShapefPtr& shape) const;


	/*
		@brief	�����Ƃ̌�_�̂����A�n�_�ɋ߂��_���擾
			if(Contains(line))
				point = IntersectionPoint2Nearest(line);
		@attention	�������Ă��邱�Ƃ��O��
		@param	[in]	line(sta,end)	��r����
		@return	�n�_�ɋ߂���_
	*/
	virtual mymath::Vec3f IntersectionPoint2Nearest(const mymath::Linef& line) const;
	/*
		@brief	�����Ƃ̌�_�̂����A�n�_�ɋ߂��_���擾
			if(Contains(pos,next))
				point = IntersectionPoint2Nearest(pos,next);
		@attention	�������Ă��邱�Ƃ��O��
		@param	[in]	sta	��r�����̎n�_
		@param	[in]	end	��r�����̏I�_
		@return	�n�_�ɋ߂���_
	*/
	virtual mymath::Vec3f IntersectionPoint2Nearest(const mymath::Vec3f& sta, const mymath::Vec3f& end) const;

#pragma endregion // Intersection
	//================================================================================

	
};
typedef std::shared_ptr<IActionPoint> ActPtPtr;

#pragma endregion  // class IActionPoint

#pragma region class CActionCircle
class CActionCircle : public IActionPoint
{
	std::shared_ptr<mymath::Circlef> circle_;
public:
	CActionCircle(float x, float y, float r);
	virtual void step() override;
	virtual void draw() override;
	/*
		@brief	�A�N�V�����|�C���g�̎擾
		@return	�A�N�V�����|�C���g
	*/
	virtual mymath::ShapefPtr GetAP() override;
	//================================================================================
#pragma region Contains
	/*
		@brief	�_�̓����
		@param	[in]	point	���肷��_
		@return	����Ă��邩
		@retval true	���Ă���
		@retval false	���Ă��Ȃ�
	*/
	virtual bool Contains(const mymath::Vec3f& point) const override;
	/*
		@brief			�����̌����A�����
		@attension		�����̗��[�����E���̒��ɂ���ꍇ�͑�3�����ɂ��ς��
						���S����Ƌ��E�������͓���D�悳���
		@param	[in]	line(sta,end)	���肷�����(�n�_�A�I�_)
		@param	[in]	fullContain		���S����Ƃ��邩(�f�t�H���g�Ffalse)
		@param	[in]	intersectOnly	���E���Ƃ̌����݂̂ɂ��邩(�f�t�H���g�Ftrue)
		@return			�����A����Ă��邩
		@retval	true	���Ă���
		@retval	false	���Ă��Ȃ�
	*/
	virtual bool Contains(const mymath::Linef& line, bool fullContain = false, bool intersectOnly = true) const override;
	/*
		@brief			�����̌����A�����
		@attension		�����̗��[�����E���̒��ɂ���ꍇ�͑�4�����ɂ��ς��
						���S����Ƌ��E�������͓���D�悳���
		@param	[in]	sta	���肷������̎n�_
		@param	[in]	end	���肷������̏I�_
		@param	[in]	fullContain		���S����Ƃ��邩(�f�t�H���g�Ffalse)
		@param	[in]	intersectOnly	���E���Ƃ̌����݂̂ɂ��邩(�f�t�H���g�Ftrue)
		@return			�����A����Ă��邩
		@retval	true	���Ă���
		@retval	false	���Ă��Ȃ�
	*/
	virtual bool Contains(const mymath::Vec3f& sta, const mymath::Vec3f& end, bool fullContain = false, bool intersectOnly = true) const override;
	
	/*
		@brief			�}�`�̌����A�����
		@attension		�����̗��[�����E���̒��ɂ���ꍇ�͑�3�����ɂ��ς��
		���S����Ƌ��E�������͓���D�悳���
		@param	[in]	shape			���肷��}�`
		@param	[in]	fullContain		���S����Ƃ��邩(�f�t�H���g�Ffalse)
		@param	[in]	intersectOnly	���E���Ƃ̌����݂̂ɂ��邩(�f�t�H���g�Ftrue)
		@return			�����A����Ă��邩
		@retval	true	���Ă���
		@retval	false	���Ă��Ȃ�
	*/
	virtual bool Contains(const mymath::ShapefPtr& shape, bool fullContain = false, bool intersectOnly = true) const override;
#pragma endregion	// Contains

	//================================================================================


	//================================================================================
#pragma region Intersection
	/*
		@brief	����Ă���_�����ԋ߂���_�̎擾
			if(Contains(p))
				point = IntersectionPoint2(p);
		@attention	�������Ă��邱�Ƃ��O��
		@param	[in]	point	����Ă���_
		@return	��_
	*/
	virtual mymath::Vec3f IntersectionPoint2(const mymath::Vec3f& point) const override;

	/*
		@brief	�����Ƃ̌�_�̎擾
			if(Contains(line))
				points = IntersectionPoint2(line);
		@attention	�������Ă��邱�Ƃ��O��
		@param	[in]	line(sta,end)	��r����
		@return	�S��_
	*/
	virtual std::vector<mymath::Vec3f> IntersectionPoint2(const mymath::Linef& line) const override;
	/*
		@brief	�����Ƃ̌�_�̎擾
			if(Contains(pos,next))
				points = IntersectionPoint2(pos,next);
		@attention	�������Ă��邱�Ƃ��O��
		@param	[in]	sta	��r�����̎n�_
		@param	[in]	end	��r�����̏I�_
		@return	�S��_
	*/
	virtual std::vector<mymath::Vec3f> IntersectionPoint2(const mymath::Vec3f& sta, const mymath::Vec3f& end) const override;

	/*
		@brief	�}�`�Ƃ̌�_�̎擾
			if(Contains(shape))
				points = IntersectionPoint2(shape);
		@attention	�������Ă��邱�Ƃ��O��
		@param	[in]	shape	���肷��}�`
		@return	�S��_
	*/
	virtual std::vector<mymath::Vec3f> IntersectionPoint2(const mymath::ShapefPtr& shape) const override;

	/*
		@brief	�����Ƃ̌�_�̂����A�n�_�ɋ߂��_���擾
			if(Contains(line))
				point = IntersectionPoint2Nearest(line);
		@attention	�������Ă��邱�Ƃ��O��
		@param	[in]	line(sta,end)	��r����
		@return	�n�_�ɋ߂���_
	*/
	virtual mymath::Vec3f IntersectionPoint2Nearest(const mymath::Linef& line) const override;
	/*
		@brief	�����Ƃ̌�_�̂����A�n�_�ɋ߂��_���擾
			if(Contains(pos,next))
				point = IntersectionPoint2Nearest(pos,next);
		@attention	�������Ă��邱�Ƃ��O��
		@param	[in]	sta	��r�����̎n�_
		@param	[in]	end	��r�����̏I�_
		@return	�n�_�ɋ߂���_
	*/
	virtual mymath::Vec3f IntersectionPoint2Nearest(const mymath::Vec3f& sta, const mymath::Vec3f& end) const override;


#pragma endregion // Intersection
	//================================================================================

};

#pragma endregion // class CActionCircle

#pragma region class CActionPolygon
class CActionPolygon : public IActionPoint
{
private:
	std::shared_ptr<mymath::Polyf> polygon_;
public:
	const std::vector<mymath::Vec3f>& vertexes;
private:
public:
	CActionPolygon(const std::vector<mymath::Vec3f>& points);
	CActionPolygon(const mymath::Polyf& polygon);
	virtual void step() override;
	virtual void draw() override;

	/*
		@brief	�A�N�V�����|�C���g�̎擾
		@return	�A�N�V�����|�C���g
	*/
	virtual mymath::ShapefPtr GetAP() override;

	/*
		@brief	vertexes_�̏������Ƀ|���S��(�H)�𐬌`
		@return	�|���S���̘g
	*/
	std::vector<mymath::Linef> MakeLines() const;
	
	//================================================================================
#pragma region Contains
	/*
		@brief	�_�̓����
		@param	[in]	point	���肷��_
		@return	����Ă��邩
		@retval true	���Ă���
		@retval false	���Ă��Ȃ�
	*/
	virtual bool Contains(const mymath::Vec3f& point) const override;
	/*
		@brief			�����̌����A�����
		@attension		�����̗��[�����E���̒��ɂ���ꍇ�͑�3�����ɂ��ς��
						���S����Ƌ��E�������͓���D�悳���
		@param	[in]	line(sta,end)	���肷�����(�n�_�A�I�_)
		@param	[in]	fullContain		���S����Ƃ��邩(�f�t�H���g�Ffalse)
		@param	[in]	intersectOnly	���E���Ƃ̌����݂̂ɂ��邩(�f�t�H���g�Ftrue)
		@return			�����A����Ă��邩
		@retval	true	���Ă���
		@retval	false	���Ă��Ȃ�
	*/
	virtual bool Contains(const mymath::Linef& line, bool fullContain = false, bool intersectOnly = true) const override;
	/*
		@brief			�����̌����A�����
		@attension		�����̗��[�����E���̒��ɂ���ꍇ�͑�4�����ɂ��ς��
						���S����Ƌ��E�������͓���D�悳���
		@param	[in]	sta	���肷������̎n�_
		@param	[in]	end	���肷������̏I�_
		@param	[in]	fullContain		���S����Ƃ��邩(�f�t�H���g�Ffalse)
		@param	[in]	intersectOnly	���E���Ƃ̌����݂̂ɂ��邩(�f�t�H���g�Ftrue)
		@return			�����A����Ă��邩
		@retval	true	���Ă���
		@retval	false	���Ă��Ȃ�
	*/
	virtual bool Contains(const mymath::Vec3f& sta, const mymath::Vec3f& end, bool fullContain = false, bool intersectOnly = true) const override;
	
	/*
		@brief			�}�`�̌����A�����
		@attension		�����̗��[�����E���̒��ɂ���ꍇ�͑�3�����ɂ��ς��
		���S����Ƌ��E�������͓���D�悳���
		@param	[in]	shape			���肷��}�`
		@param	[in]	fullContain		���S����Ƃ��邩(�f�t�H���g�Ffalse)
		@param	[in]	intersectOnly	���E���Ƃ̌����݂̂ɂ��邩(�f�t�H���g�Ftrue)
		@return			�����A����Ă��邩
		@retval	true	���Ă���
		@retval	false	���Ă��Ȃ�
	*/
	virtual bool Contains(const mymath::ShapefPtr& shape, bool fullContain = false, bool intersectOnly = true) const override;
#pragma endregion	// Contains

	//================================================================================
	//================================================================================
#pragma region Intersection
	/*
		@brief	����Ă���_�����ԋ߂���_�̎擾
			if(Contains(p))
				point = IntersectionPoint2(p);
		@attention	�������Ă��邱�Ƃ��O��
		@param	[in]	point	����Ă���_
		@return	��_
	*/
	virtual mymath::Vec3f IntersectionPoint2(const mymath::Vec3f& point) const override;

	/*
		@brief	�����Ƃ̌�_�̎擾
			if(Contains(line))
				points = IntersectionPoint2(line);
		@attention	�������Ă��邱�Ƃ��O��
		@param	[in]	line(sta,end)	��r����
		@return	�S��_
	*/
	virtual std::vector<mymath::Vec3f> IntersectionPoint2(const mymath::Linef& line) const override;
	/*
		@brief	�����Ƃ̌�_�̎擾
			if(Contains(pos,next))
				points = IntersectionPoint2(pos,next);
		@attention	�������Ă��邱�Ƃ��O��
		@param	[in]	sta	��r�����̎n�_
		@param	[in]	end	��r�����̏I�_
		@return	�S��_
	*/
	virtual std::vector<mymath::Vec3f> IntersectionPoint2(const mymath::Vec3f& sta, const mymath::Vec3f& end) const override;

	/*
		@brief	�}�`�Ƃ̌�_�̎擾
			if(Contains(shape))
				points = IntersectionPoint2(shape);
		@attention	�������Ă��邱�Ƃ��O��
		@param	[in]	shape	���肷��}�`
		@return	�S��_
	*/
	virtual std::vector<mymath::Vec3f> IntersectionPoint2(const mymath::ShapefPtr& shape) const override;

	/*
		@brief	�����Ƃ̌�_�̂����A�n�_�ɋ߂��_���擾
			if(Contains(line))
				point = IntersectionPoint2Nearest(line);
		@attention	�������Ă��邱�Ƃ��O��
		@param	[in]	line(sta,end)	��r����
		@return	�n�_�ɋ߂���_
	*/
	virtual mymath::Vec3f IntersectionPoint2Nearest(const mymath::Linef& line) const override;
	/*
		@brief	�����Ƃ̌�_�̂����A�n�_�ɋ߂��_���擾
			if(Contains(pos,next))
				point = IntersectionPoint2Nearest(pos,next);
		@attention	�������Ă��邱�Ƃ��O��
		@param	[in]	sta	��r�����̎n�_
		@param	[in]	end	��r�����̏I�_
		@return	�n�_�ɋ߂���_
	*/
	virtual mymath::Vec3f IntersectionPoint2Nearest(const mymath::Vec3f& sta, const mymath::Vec3f& end) const override;


#pragma endregion // Intersection
	//================================================================================

};

#pragma endregion // class CActionPolygon


#endif