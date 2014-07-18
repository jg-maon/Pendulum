#ifndef DE_CHARACTERBASE_H
#define DE_CHARACTERBASE_H

#ifndef DEF_OBJ_H
#include "Obj.h"
#endif

#ifndef DEF_STAGEMNG_H
#include "stageMng.h"
#endif

#ifndef DEF_MYMATH_HPP
#include "myMath.hpp"
#endif

#include <string>
#include <memory>

#define DEF_PREPOS

/*
	@brief	�L�����N�^�[�I�u�W�F�N�g���N���X(�v���C���[�A�G�ANPC?)
	@attension	step����ICharacter::step���Ăяo��
*/
class ICharacter : public IColObject
{
private:
	static std::weak_ptr<CStageMng> sm_;			// �X�e�[�W�}�l�[�W���ւ̎Q��
protected:
	int health_;							// �̗�
	int power_;								// �U����
#ifdef DEF_PREPOS
	mymath::Vec3f prePos_;					// 1�t���[���O�̍��W
#endif

	bool gravityF_;							// �d�͏����t���O

protected:
	/*
		@brief	�X�e�[�W�}�l�[�W���̃|�C���^�擾
		@return	�X�e�[�W�}�l�[�W���|�C���^
	*/
	std::shared_ptr<CStageMng> sm();

	/*
		@brief	�X�e�[�W�}�l�[�W���̐ݒ�
		@param	[in]	sm		�X�e�[�W�}�l�[�W��
		@return	�Ȃ�
	*/
	void sm(const std::shared_ptr<CStageMng>& sm);

public:

	/*
		@brief	�I�u�W�F�N�g�̐���
		@param	[in]	name	���ʖ�
	*/
	ICharacter(const std::string& name);
	/*
		@brief	�C���^�[�t�F�[�X�����邽�߂̏������z�֐�
	*/
	~ICharacter() = 0;
	/*
		@brief		�h���㋤�ʏ���
					1�t���[���O�̍��W�Ɍ��ݍ��W���i�[
		@attension	�h�����step���ŌĂяo������
		@return		�Ȃ�
	*/
	virtual void step() = 0;
	/*
		@brief	�`�揈��
		@return	�Ȃ�
	*/
	virtual void draw() = 0;
	/*
		@brief	�I�u�W�F�N�g���m�̏d�Ȃ菈��
		@param	[in]	rival	�d�Ȃ��Ă��鑊��I�u�W�F�N�g
		@return	�Ȃ�
	*/
	virtual void hit(const ObjPtr& rival);

	/*
		@brief	���[���h���W�̐H�炢����̈�̎擾
		@return	���[���h���W�̓����蔻��̈�
	*/
	virtual Collisions GetDamageAreas() const;

	/*
		@brief	�U���͂̎擾
		@return	�␳��̍U����
	*/
	virtual int getPower() const;

#ifdef DEF_PREPOS
	/*
		@brief	1�t���[���O���W�̎擾
		@return	1�t���[���O�̍��W
	*/
	const mymath::Vec3f& prePos() const;

#endif

	/*
		@brief	1�t���[����̍��W�̎擾
		@return	1�t���[����̍��W
	*/
	mymath::Vec3f nextPos() const;

};

/*
	@brief	�X�e�[�W�}�l�[�W���������p�L����
*/
class TempCharacter : public ICharacter
{
public:
	TempCharacter(const std::shared_ptr<CStageMng>& stageMng) :
		ICharacter("")
	{
		sm(stageMng);
	}
	void step() override{}
	void draw() override{}
};

#endif