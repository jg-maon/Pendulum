#ifndef DEF_PICKUPJEWELY_H
#define DEF_PICKUPJEWELY_H

#ifndef DFE_PICKUPBASE_HPP
#include "pickupBase.hpp"
#endif

class CPickupJewely : public IPickup
{
private:
	enum
	{
		UNIT_SCORE = 300,
	};
	int score_;					// �������̃X�R�A

	mymath::Vec3f startPos_;	// �o���������W
	
private:
	/*
		@brief	������
		@param	[in]	pos		���W
		@param	[in]	type	���
		return	�Ȃ�
	*/
	void init(const mymath::Vec3f& pos, int type);

	/*
		@brief	Chain���ɉ������{���擾
		@param	[in]	chain	Chain��
		@return	�X�R�A�{��
	*/
	float CalcMagnification(int chain);
public:
	/*
		@brief	DB�ǉ��p
	*/
	CPickupJewely();
	/*
		@brief	�w�肵�����W�ɐ���
		@param	[in]	pos		���W
		@param	[in]	type	���
	*/
	CPickupJewely(const mymath::Vec3f& pos, int type = -1);

	/*
		@brief	�X�V����
		@return	�Ȃ�
	*/
	virtual void step() override;
	/*
		@brief	�`�揈��
		@return	�Ȃ�
	*/
	virtual void draw() override;
	
	/*
		@brief	�I�u�W�F�N�g���m�̏d�Ȃ菈��
		@param	[in]	rival	�d�Ȃ��Ă��鑊��I�u�W�F�N�g
		@return	�Ȃ�
	*/
	virtual void hit(const ObjPtr& rival) override;

	/*
		@brief	�X�R�A�擾
		@return	�X�R�A
	*/
	int getScore() const;
};

#endif