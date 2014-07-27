#ifndef DEF_NWAYSING_H
#define DEF_NWAYSING_H

#ifndef DEF_ATTACKBASE_HPP
#include "attackBase.hpp"
#endif

#ifndef DEF_MYMATH_HPP
#include "MyMath.hpp"
#endif

#include <vector>

/*
	@brief	����
*/
class CSing : public IAttack
{
private:
	int cnt_;
	float acceleration_;		// �����x
	float	addAngle_;	// �p���x

private:
	void init()
	{
		start();
		force_ = 2;
		cnt_ = 0;
		
		obj_.src(0, 0);
		//obj_.src(0, math::GetRandom<int>(0, 2));

		//collisions_.push_back(mymath::ShapefPtr(new mymath::Circlef(0.f, 0.f, obj_.pos.z, obj_.HalfHeight())));
		collisions_.push_back(mymath::ShapefPtr(new mymath::Rectf(obj_.GetRect())));
	}
public:
	/*
		@brief	������
		@param	[in]	sing		�e�̏��
		@param	[in]	acc			�����x[unit:px/sec](�f�t�H���g:100)
		@param	[in]	addAngle	�p���x[unit:Degree](�f�t�H���g:360) 
	*/
	CSing(const CSing& sing, float acc = 100.f, float addAngle = 360.f) :
		IAttack("Atk_Sing")
		, acceleration_(acc)
		, addAngle_(addAngle)
	{
		obj_ = sing.obj();
		init();
	}

	/*
		@brief	������
		@param	[in]	sing		�e�̏��
	*/
	CSing(const charabase::CharBase& sing) :
		IAttack("Atk_Sing")
	{
		obj_ = sing;
		init();
	}

	virtual void step() override
	{
		//obj_.add += acceleration_;

		obj_.angle += addAngle_ * system::FrameTime;
		if (obj_.angle >= 360.f)
			obj_.angle -= 360.f;
		else if (obj_.angle < 0.f)
			obj_.angle += 360.f;

		obj_.add.x += math::ROUND_X(obj_.angle, (float)cnt_);
		obj_.add.y += math::ROUND_Y(obj_.angle, (float)cnt_);

		obj_.Move();
		if (++cnt_ > 60)
		{
			kill();
		}
	}

	virtual void draw() override
	{
		obj_.draw();
	}

	virtual void hit(const ObjPtr& rival) override
	{
		if (rival->FindName("Player"))
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
	@brief	NWay����
*/
class CNWaySing : public IAttack
{
private:
	const CSing sing_;			// �����̏��
public:
	/*
		@brief	NWay�����̐���
		@param	[in]	base		�����̏��(�摜���,�����蔻��)
	*/
	CNWaySing(const CSing& base);

	virtual void step() override;
	virtual void draw() override;

	/*
		@brief	NWay�����̔���
				�ǂ̈ʒu���� ���� �ǂ̊p�x�� �ǂꂭ�炢�̊Ԋu�� �ǂꂭ�炢�̊p���x�� �����x�͂ǂꂭ�炢��
		@param	[in]	pos			�����̏������W
		@param	[in]	n			������
		@param	[in]	angle		�����̔��ˊp�x(�P��:Degree)
		@param	[in]	acc			�����̉����x
		@param	[in]	addAngle	�����̊p���x
		@return	�Ȃ�
	*/
	void CreateAttack(const mymath::Vec3f& pos, int n, float angle, float acc, float addAngle);


};

#endif