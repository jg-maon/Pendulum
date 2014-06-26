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
	@brief	�V���b�g
*/
class CSing : public IAttack
{
private:
	int cnt_;
	mymath::Vec3f	acceleration_;	// �����x

private:
	void init()
	{
		force_ = 10;
		start();
		cnt_ = 0;
		collisions_.push_back(mymath::ShapefPtr(new mymath::Circlef(0.f, 0.f, obj_.pos.z, obj_.HalfHeight())));
	}
public:
	/*
		@brief	������
		@param	[in]	sing			�e�̏��
		@param	[in]	acceleration	�����x
	*/
	CSing(const CSing& sing, const mymath::Vec3f& acceleration = mymath::Vec3f()) :
		IAttack("Atk_Sing")
		, acceleration_(acceleration)
	{
		obj_ = sing.obj();
		init();
	}
	/*
		@brief	������
		@param	[in]	sing			�e�̏��
	*/
	CSing(const charabase::CharBase& sing) :
		IAttack("Atk_Sing")
	{
		obj_ = sing;
		init();
	}
	virtual void step() override
	{
		obj_.Move();
		obj_.add += acceleration_;
		if (++cnt_ > 300)
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
	}
};
/*
@brief	NWay�e
*/
class CNWaySing : public IAttack
{
private:
	const CSing sing_;			// �e�̏��
public:
	/*
	@brief	NWay�e�̐���
	@param	[in]	base		�e�̏��(�摜���,�����蔻��)
	*/
	CNWaySing(const CSing& base);

	virtual void step() override;
	virtual void draw() override;

	/*
		@brief	NWay�e�̔���
		�ǂ̈ʒu���� ���� �ǂ̊p�x�� �ǂꂭ�炢�̊Ԋu�� �ǂꂭ�炢�̑����� �����x�͂ǂꂭ�炢�� �ǂ̂悤�ɎU�点�邩
		@param	[in]	pos			�e�̏������W
		@param	[in]	n			�e��
		@param	[in]	angle		�e�̔��ˊp�x(�P��:Degree)
		@param	[in]	speed		�e�̑���(�����x)
		@param	[in]	acc			�e�̉����x
		@param	[in]	centerFlag	�e�����E�ɂӂ邩(�f�t�H���g:true)
		@return	�Ȃ�
	*/
	void CreateAttack(const mymath::Vec3f& pos, int n, float angle, float speed, float acc, bool centerFlag = true);


};

#endif