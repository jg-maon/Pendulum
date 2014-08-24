#include "fireShot.h"
#include "define.h"

#include "MyMath.hpp"

#ifndef DEF_EFFECTBASE_HPP
#include "effectBase.hpp"
#endif

/*
	@brief	�Ή��e�I�u�W�F�N�g
*/
class CFire : public IAttack
{
	enum class State
	{
		move, explode,
	}state_;
private:
	charabase::Anim anim_;

	
public:

	/*
		@brief	������
		@param	[in]	force	�З�
		@param	[in]	obj		�I�u�W�F�N�g���
	*/
	CFire(int force, const charabase::CharBase& obj) :
		IAttack("Atk_Fire")
		, anim_(21, 21.f / 1.5f * system::FrameTime)
		, state_(State::move)
	{
		force_ = force;
		obj_ = obj;
		start();
	}

	/*
		@brief	�X�V����
		@return	�Ȃ�
	*/
	virtual void step() override
	{
		// �q�b�g�X�g�b�v���̓X�L�b�v
		if (!isUpdatable())
			return;

		bool animEnd = anim_.step();
		const int animRow = 16;
		obj_.src.x = anim_.no % animRow;
		obj_.src.y = anim_.no / animRow;
		switch (state_)
		{
		case CFire::State::move:
			break;
		case CFire::State::explode:
			if (animEnd) 
				kill();
			break;
		default:
			break;
		}

		// �ړ�
		obj_.Move();

	}

	/*
		@brief	�`�揈��
	*/
	virtual void draw() override
	{
		graph::Draw_SetRenderMode(RENDER_MODE::HALFADD);
		obj_.draw();
		graph::Draw_EndRenderMode();
	}

	virtual void hit(const ObjPtr& rival) override
	{
		if (rival->FindName("Player"))
		{
			anim_.max = 64;
			anim_.no = 22;
			state_ = State::explode;
		}
	}

};


CFireShot::CFireShot() :
IAttack("Atk_FireShot")
{
	force_ = 3;
}

void CFireShot::step()
{
}
void CFireShot::draw()
{
}


void CFireShot::CreateAttack(const mymath::Vec3f& pos, const mymath::Vec3f& vec, float speed) const
{
	// �e
	charabase::CharBase shot(charabase::BaseData("img_fireShot", 256, 256));
	// �傫��
	shot.scale = 0.5f;
	// ���W
	shot.pos = pos;
	// ���ˊp
	float rad = vec.Angle2(pos, vec);
	shot.angle = math::Calc_RadToDegree(rad);
	if (shot.angle >= 360.f)
		shot.angle -= 360.f;
	else if (shot.angle < 0.f)
		shot.angle += 360.f;
	// ���ˊp�ɍ��킹�ď����x������x�̌�����ς���
	const float c = std::cosf(rad);
	const float s = std::sinf(rad);
	shot.add.x = speed *  c;
	shot.add.y = speed * -s;
	shot.add.z = 0.f;

	// �o�^
	gm()->AddObject(ObjPtr(new CFire(force_, shot)));
}



