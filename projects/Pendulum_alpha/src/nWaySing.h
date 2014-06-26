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
	@brief	ƒVƒ‡ƒbƒg
*/
class CSing : public IAttack
{
private:
	int cnt_;
	mymath::Vec3f	acceleration_;	// ‰Á‘¬“x

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
		@brief	‰Šú‰»
		@param	[in]	sing			’e‚Ìî•ñ
		@param	[in]	acceleration	‰Á‘¬“x
	*/
	CSing(const CSing& sing, const mymath::Vec3f& acceleration = mymath::Vec3f()) :
		IAttack("Atk_Sing")
		, acceleration_(acceleration)
	{
		obj_ = sing.obj();
		init();
	}
	/*
		@brief	‰Šú‰»
		@param	[in]	sing			’e‚Ìî•ñ
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
@brief	NWay’e
*/
class CNWaySing : public IAttack
{
private:
	const CSing sing_;			// ’e‚Ìî•ñ
public:
	/*
	@brief	NWay’e‚Ì¶¬
	@param	[in]	base		’e‚Ìî•ñ(‰æ‘œî•ñ,“–‚½‚è”»’è)
	*/
	CNWaySing(const CSing& base);

	virtual void step() override;
	virtual void draw() override;

	/*
		@brief	NWay’e‚Ì”­Ë
		‚Ç‚ÌˆÊ’u‚©‚ç ‰½ŒÂ ‚Ç‚ÌŠp“x‚É ‚Ç‚ê‚­‚ç‚¢‚ÌŠÔŠu‚Å ‚Ç‚ê‚­‚ç‚¢‚Ì‘¬‚³‚Å ‰Á‘¬“x‚Í‚Ç‚ê‚­‚ç‚¢‚Å ‚Ç‚Ì‚æ‚¤‚ÉU‚ç‚¹‚é‚©
		@param	[in]	pos			’e‚Ì‰ŠúÀ•W
		@param	[in]	n			’e”
		@param	[in]	angle		’e‚Ì”­ËŠp“x(’PˆÊ:Degree)
		@param	[in]	speed		’e‚Ì‘¬‚³(‰‘¬“x)
		@param	[in]	acc			’e‚Ì‰Á‘¬“x
		@param	[in]	centerFlag	’e‚ğ¶‰E‚É‚Ó‚é‚©(ƒfƒtƒHƒ‹ƒg:true)
		@return	‚È‚µ
	*/
	void CreateAttack(const mymath::Vec3f& pos, int n, float angle, float speed, float acc, bool centerFlag = true);


};

#endif