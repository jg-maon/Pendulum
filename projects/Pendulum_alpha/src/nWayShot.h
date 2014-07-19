#ifndef DEF_NWAYSHOT_H
#define DEF_NWAYSHOT_H

#ifdef _DEBUG
#define DEF_SHAPE_DRAW
#endif


#ifndef DEF_ATTACKBASE_HPP
#include "attackBase.hpp"
#endif

#ifndef DEF_MYMATH_HPP
#include "MyMath.hpp"
#endif

#include <vector>

/*
	@brief	�V���b�g
*/
class CShot : public IAttack
{
private:
	int cnt_;
	mymath::Vec3f	acceleration_;	// �����x

private:
	void init()
	{
		force_ = 1;
		start();
		cnt_ = 0;
		collisions_.push_back(mymath::ShapefPtr(new mymath::Circlef(0.f, 0.f, obj_.pos.z, obj_.HalfHeight())));
	}
public:
	/*
		@brief	������
		@param	[in]	shot			�e�̏��
		@param	[in]	acceleration	�����x
	*/
	CShot(const CShot& shot, const mymath::Vec3f& acceleration = mymath::Vec3f()) :
		IAttack("Atk_Shot")
		, acceleration_(acceleration)
	{
		obj_ = shot.obj();
		init();
	}
	/*
		@brief	������
		@param	[in]	shot			�e�̏��
	*/
	CShot(const charabase::CharBase& shot):
		IAttack("Atk_Shot")
	{
		obj_ = shot;
		init();
	}
	virtual void step() override
	{
		obj_.Move();
		obj_.add += acceleration_;
		if(++cnt_ > 300)
		{
			kill();
		}
	}
	virtual void draw() override
	{
		obj_.draw();
#if defined(DEF_SHAPE_DRAW) && defined(_DEBUG)
		const auto& cols = GetDamageAreas();
		for (const auto& col : cols)
		{
			col->Offset(mymath::Vec3f(0.f, 0.f, -0.1f));
			col->draw();
		}
#endif
	}
	virtual void hit(const ObjPtr& rival) override
	{
		if(rival->FindName("Player"))
		{
			kill();
		}
		else if (rival->FindName("ActionPolygon"))
		{
			kill();
		}
	}
};
/*
	@brief	NWay�e
*/
class CNWayShot : public IAttack
{
private:
	const CShot shot_;			// �e�̏��
public:
	/*
		@brief	NWay�e�̐���
		@param	[in]	base		�e�̏��(�摜���,�����蔻��)
	*/
	CNWayShot(const CShot& base);

	virtual void step() override;
	virtual void draw() override;

	/*
		@brief	NWay�e�̔���
			�ǂ̈ʒu���� ���� �ǂ̊p�x�� �ǂꂭ�炢�̊Ԋu�� �ǂꂭ�炢�̑����� �����x�͂ǂꂭ�炢�� �ǂ̂悤�ɎU�点�邩
		@param	[in]	pos			�e�̏������W
		@param	[in]	n			�e��
		@param	[in]	angle		�e�̔��ˊp�x(�P��:Degree)
		@param	[in]	interval	�e�̊Ԋu�p�x(�P��:Degree)
		@param	[in]	speed		�e�̑���(�����x)
		@param	[in]	acc			�e�̉����x
		@param	[in]	centerFlag	�e�����E�ɂӂ邩(�f�t�H���g:true)
		@return	�Ȃ�
	*/
	void CreateAttack(const mymath::Vec3f& pos, int n, float angle, float interval, float speed, float acc, bool centerFlag = true);
	

};

#endif