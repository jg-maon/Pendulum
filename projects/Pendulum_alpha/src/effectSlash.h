#ifndef DEF_EFFECTSLASH_H
#define DEF_EFFECTSLASH_H

#ifndef DEF_EFFECTBASE_HPP
#include "effectBase.hpp"
#endif

#ifndef DEF_MYMATH_HPP
#include "MyMath.hpp"
#endif

/*
	@brief	�a���G�t�F�N�g
*/
class CEffectSlash : public IEffect
{
	float animCnt_;			// �A�j���[�V�����J�E���^
	float addAngle_;		// ��]�`��
public:
	/*
		@brief	��������
		@param	[in]	pos		���S���W
		@param	[in]	scale	�g�嗦(�f�t�H���g�F1.0f�{)
	*/
	CEffectSlash(const mymath::Vec3f& pos, float scale = 1.f);
	
	void step() override;
	void draw() override;

};


#endif