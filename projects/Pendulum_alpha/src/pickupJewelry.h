#ifndef DEF_PICKUPJEWELY_H
#define DEF_PICKUPJEWELY_H

#ifndef DFE_PICKUPBASE_HPP
#include "pickupBase.hpp"
#endif

class CPickupJewely : public IPickup
{
public:
	CPickupJewely(const mymath::Vec3f& pos);
	virtual void step() override;
	virtual void hit(const ObjPtr& rival) override;
};

#endif