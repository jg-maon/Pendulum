
#include "characterBase.h"

std::weak_ptr<CStageMng> ICharacter::sm_;


ICharacter::ICharacter(const std::string& name) :
	IColObject(name)
	, health_(1)
	, power_(0)
	, prePos_(obj_.pos)
{}

ICharacter::~ICharacter()
{}

void ICharacter::step()
{
	prePos_ = obj_.pos; 
}

void ICharacter::draw()
{}


std::shared_ptr<CStageMng> ICharacter::sm()
{
	return sm_.lock();
}

void ICharacter::sm(const std::shared_ptr<CStageMng>& sm)
{
	sm_ = sm;
}





void ICharacter::hit(const ObjPtr& rival)
{
	__super::hit(rival); 
}

Base::Collisions ICharacter::GetDamageAreas() const
{
	return __super::GetDamageAreas(); 
}

int ICharacter::getPower() const
{
	return power_; 
}

#ifdef DEF_PREPOS

const mymath::Vec3f& ICharacter::prePos() const
{
	return prePos_;
}

#endif

mymath::Vec3f ICharacter::nextPos() const
{
	return obj_.pos + (obj_.add * system::FrameTime);
}

