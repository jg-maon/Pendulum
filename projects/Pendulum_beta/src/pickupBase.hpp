#ifndef DEF_PICKUPBASE_HPP
#define DEF_PICKUPBASE_HPP

#ifndef DEF_OBJ_H
#include "obj.h"
#endif

/*
	@brief	�A�C�e���n�����N���X
			�H�炢����=�擾����
*/
class IPickup : public IColObject
{
protected:
	enum
	{
		POP_ANIM_TIME = 7,			// �o���A�j���[�V��������(�����蔻��Ȃ�)[unit: /10sec ]
		GET_TIME = 10,				// �擾�܂ł̎���[unit: /10sec ]
	};
	enum class State
	{
		POP,						// �o��
		CHASE,						// �v���C���[��ǂ�������
	};
protected:
	float time_;					// �ėp�^�C�}�[
	IPickup::State state_;			// �A�j���[�V�������

public:
	IPickup(const std::string& name) :
		IColObject(name)
		, time_(0.f)
		, state_(State::POP)
	{

	}
	virtual void step(){}
	virtual void draw() override
	{
		obj_.draw();
	}
	virtual Base::Collisions GetDamageAreas() const override
	{
		switch (state_)
		{
		case IPickup::State::POP:
			break;
		case IPickup::State::CHASE:
			return __super::GetDamageAreas();
			//break;
		default:
			break;
		}
		return Collisions();
	}


};

typedef std::shared_ptr<IPickup> PickupPtr;

#endif