#ifndef DEF_FIRESHOT_H
#define DEF_FIRESHOT_H

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
	@brief	‰Î‰Š’e
*/
class CFireShot : public IAttack
{
private:

public:
	/*
		@brief	‰Î‰Š’e
	*/
	CFireShot();

	virtual void step() override;
	virtual void draw() override;

	/*
		@brief	‰Î‰Š’e‚Ì”­Ë
		@param	[in]	pos			’e‚Ì‰ŠúÀ•W
		@param	[in]	vec			’e‚Ì”­Ë•ûŒü(ƒxƒNƒgƒ‹)
		@param	[in]	speed		’e‚Ì‘¬‚³
		@return	‚È‚µ
	*/
	void CreateAttack(const mymath::Vec3f& pos, const mymath::Vec3f& vec, float speed) const;
	

};

#endif