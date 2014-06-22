#ifndef DEF_CHARAOBJBASE_HPP
#define DEF_CHARAOBJBASE_HPP

#ifndef DEF_OBJ_H
#include "Obj.h"
#endif

/*
	@brief	�L�����N�^�[�I�u�W�F�N�g���N���X(�v���C���[�A�G�ANPC?)
	@attension	step����ICharacter::step���Ăяo��
*/
class ICharacter : public IColObject
{

protected:
	int health_;				// �̗�
	int power_;					// �U����

	mymath::Vec3f prePos_;		// 1�t���[���O�̍��W

public:
	/*
		@brief	�I�u�W�F�N�g�̐���
		@param	[in]	name	���ʖ�
	*/
	ICharacter(const std::string& name) :
		IColObject(name)
		, health_(1)
		, power_(0)
		, prePos_(obj_.pos)
	{}
	/*
		@brief	�C���^�[�t�F�[�X�����邽�߂̏������z�֐�
	*/
	~ICharacter() = 0{}
	/*
		@brief		�h���㋤�ʏ���
					1�t���[���O�̍��W�Ɍ��ݍ��W���i�[
		@attension	�h�����step���ŌĂяo������
		@return		�Ȃ�
	*/
	virtual void step() = 0{ prePos_ = obj_.pos; }
	/*
		@brief	�`�揈��
		@return	�Ȃ�
	*/
	virtual void draw() = 0{}
	/*
		@brief	�I�u�W�F�N�g���m�̏d�Ȃ菈��
		@param	[in]	rival	�d�Ȃ��Ă��鑊��I�u�W�F�N�g
		@return	�Ȃ�
	*/
	virtual void hit(const ObjPtr& rival){ __super::hit(rival); }

	/*
		@brief	���[���h���W�̓����蔻��̈�̎擾
		@return	���[���h���W�̓����蔻��̈�
	*/
	virtual Collisions GetCollisionAreas() const {return __super::GetCollisionAreas();}

	/*
		@brief	�U���͂̎擾
		@return	�␳��̍U����
	*/
	virtual int getPower() const {return power_;}

	/*
		@brief	1�t���[���O���W�̎擾
		@return	1�t���[���O�̍��W
	*/
	const mymath::Vec3f& prePos() const{ return prePos_; }
};

#endif