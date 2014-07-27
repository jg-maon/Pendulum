#ifndef DEF_EFFECTEXPLOSION_H
#define DEF_EFFECTEXPLOSION_H

#ifndef DEF_EFFECTBASE_HPP
#include "effectBase.hpp"
#endif

#ifndef DEF_MYMATH_HPP
#include "MyMath.hpp"
#endif

/*
	@brief	�����G�t�F�N�g
*/
class CEffectExplosion : public IEffect
{
	float animCnt_;			// �A�j���[�V�����J�E���^
	float addAngle_;		// ��]�`��
public:
	/*
		@brief	��������
		@param	[in]	pos		���S���W
		@param	[in]	scale	�g�嗦(�f�t�H���g�F1.0f�{)
	*/
	CEffectExplosion(const mymath::Vec3f& pos, float scale = 1.f);
	
	void step() override;
	void draw() override;

};


#endif