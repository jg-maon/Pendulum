#ifndef DEF_NWAYSHOT_H
#define DEF_NWAYSHOT_H

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
class CShot : public IAttack
{
private:
	int cnt_;
	mymath::Vec3f	acceleration_;	// ‰Á‘¬“x

private:
	void init()
	{
		cnt_ = 0;
	}
public:
	/*
		@brief	‰Šú‰»
		@param	[in]	shot			’e‚Ìî•ñ
	*/
	CShot(const charabase::CharBase& shot):
		IAttack("Atk_Shot")
	{
		obj_ = shot;
	}
	/*
		@brief	‰Šú‰»
		@param	[in]	shot			’e‚Ìî•ñ
		@param	[in]	acceleration	‰Á‘¬“x
	*/
	CShot(const CShot& shot, const mymath::Vec3f& acceleration):
		IAttack("Atk_Shot")
		,cnt_(0)
		,acceleration_(acceleration)
	{
		obj_ = shot.obj();
	}
	virtual void step() override
	{
		obj_.Move();
		obj_.add += acceleration_;
		if(++cnt_ > 1200)
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
		if(rival->FindName("Player"))
		{
			kill();
		}
	}
};
/*
	@brief	NWay’e
*/
class CNWayShot : public IAttack
{
private:
	const CShot shot_;			// ’e‚Ìî•ñ
public:
	/*
		@brief	NWay’e‚Ì¶¬
		@param	[in]	base		’e‚Ìî•ñ(‰æ‘œî•ñ,“–‚½‚è”»’è)
	*/
	CNWayShot(const CShot& base);

	virtual void step() override;
	virtual void draw() override;

	/*
		@brief	NWay’e‚Ì”­Ë
			‚Ç‚ÌˆÊ’u‚©‚ç ‰½ŒÂ ‚Ç‚ÌŠp“x‚É ‚Ç‚ê‚­‚ç‚¢‚ÌŠÔŠu‚Å ‚Ç‚ê‚­‚ç‚¢‚Ì‘¬‚³‚Å ‰Á‘¬“x‚Í‚Ç‚ê‚­‚ç‚¢‚Å ‚Ç‚Ì‚æ‚¤‚ÉU‚ç‚¹‚é‚©
		@param	[in]	pos			’e‚Ì‰ŠúÀ•W
		@param	[in]	n			’e”
		@param	[in]	angle		’e‚Ì”­ËŠp“x(’PˆÊ:Degree)
		@param	[in]	interval	’e‚ÌŠÔŠuŠp“x(’PˆÊ:Degree)
		@param	[in]	speed		’e‚Ì‘¬‚³(‰‘¬“x)
		@param	[in]	acc			’e‚Ì‰Á‘¬“x
		@param	[in]	centerFlag	’e‚ğ¶‰E‚É‚Ó‚é‚©(ƒfƒtƒHƒ‹ƒg:true)
		@return	‚È‚µ
	*/
	void CreateAttack(const mymath::Vec3f& pos, int n, float angle, float interval, float speed, float acc, bool centerFlag = true);
	

};

#endif