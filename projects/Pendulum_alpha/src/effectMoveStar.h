#ifndef DEF_EFFECTMOVESTAR_H
#define DEF_EFFECTMOVESTAR_H

#ifndef DEF_EFFECTBASE_HPP
#include "effectBase.hpp"
#endif

#ifndef DEF_MYMATH_HPP
#include "MyMath.hpp"
#endif

/*
	@brief	�����G�t�F�N�g
*/
class CEffectMoveStar : public IEffect
{
	float animCnt_;			// �A�j���[�V�����J�E���^
	float animTime_;		// �A�j���[�V��������
public:
	/*
		@brief	��������
		@param	[in]	pos		���S���W
		@param	[in]	add		�ړ���
		@param	[in]	time	����
		@param	[in]	scale	�g�嗦(�f�t�H���g�F1.0f�{)
	*/
	CEffectMoveStar(const mymath::Vec3f& pos, const mymath::Vec3f& add, float time, float scale = 1.f);

	void step() override;
	void draw() override;

};


#endif