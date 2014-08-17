#ifndef DEF_PLAYER_H
#define DEF_PLAYER_H

#ifndef DE_CHARACTERBASE_H
#include "characterBase.h"
#endif

#ifndef DEF_MYMATH_HPP
#include "MyMath.hpp"
#endif

#ifndef DEF_ACTIONPOINT_H
#include "actionPoint.h"
#endif

#ifndef DEF_CHARBASE_H
#include "charBase.h"
#endif

#ifndef DEF_COMMON_H
#include "common.h"
#endif



class CPlayer : public ICharacter
{
public:

	enum class AttackType	// �U���̎��
	{
		SLASH, 
		TACKLE,
	};	
	enum UseBtn	// �g�p�{�^��
	{
		MOVE_BTN =	gplib::input::KEY_MOUSE_LBTN,	// �ړ�
		ATK_BTN	=	gplib::input::KEY_MOUSE_RBTN,	// �U��
	};
	struct LoadInfo
	{
		std::string armImg;		// �r�摜
		float armRotateX;		// �r��]���SX
		float armRotateY;		// �r��]���SY
		float armX;				// �r�I�t�Z�b�gX
		float armY;				// �r�I�t�Z�b�gY

		std::string chainImg;	// ���摜

		float MAX_G;			// �ő�d�͑��x
		float TENSION;			// �t�b�N�̒���(����)
		float DOWN_TENSION;		// ���͌�����
		float DOWN_SPEED;		// �ړ�������(1-DOWN_SPEED)
		float MAX_VX;			// ���������̍ő呬�x(�Q�[���Ƃ��Đ��藧�o�����X�����p)
		float MAX_VY;			// ���������̍ő呬�x(�Q�[���Ƃ��Đ��藧�o�����X�����p)
		float CHAIN_TIME[2];	// Chain�P�\����
		
		float invincibleTime;			// ���G����
		
		int health;				// ����HP
		int power;				// �����U����

		float maxAttackRadius;	// �ő�U���͈�
		float addRadius;		// �U���͈͑�����
	};
private:
	enum class MotionType		// ���[�V�����ԍ�
	{
		FALL,			// ����(�ʏ펞)
		HANG,			// ���̋@����
		ATTACK,			// �U����
		DAMAGE,			// ��e��

		MOTION_NUM,		// ���[�V������
	};
	enum
	{
		ZOOM_TIME = 7,				// �J�����Y�[���E�F�C�g����(unit[/10sec])
		MAX_SLASH = 15,				// �ō��a�����K��
		MAX_CHAIN = 999,			// �ō�Chain��
	};
private:
	/*
	static const float MAX_G;		// �ő�d�͑��x
	static const float GRAVITY_ACC;	// �d�͉����x
	static const float TENSION;		// �t�b�N�̒���(����)
	static const float DOWN_TENSION;// ���͌�����
	static const float DOWN_SPEED;	// �ړ�������(1-DOWN_SPEED)
	static const float MAX_VX;		// ���������̍ő呬�x(�Q�[���Ƃ��Đ��藧�o�����X�����p)
	static const float MAX_VY;		// ���������̍ő呬�x(�Q�[���Ƃ��Đ��藧�o�����X�����p)

	static const float CHAIN_TIME[2];	// Chain�P�\����
	//*/
	
	LoadInfo loadInfo_;					// �t�@�C������擾����ϒl

	float animTime_;					// �A�j���[�V�����p�J�E���^
	bool isCameraZoom_;					// �J�����Y�[���t���O
	float zoomTime_;					// �J�����Y�[���E�F�C�g����

	mymath::Vec3f startPos_;			// �X�e�[�W�J�n�����W

	float gravity_;						// �d��
	bool gravityF_;						// �d�͏��������邩(�ǂɎh�����Ă鎞�Ȃǂ̏����p)
	float tensionAcc_;					// ���͂̉����x
	float hangAcc_;						// �Ԃ牺�����Ă��鎞�̐U��q�^���̉����x
	
	bool isHanging_;					// �Ԃ牺�����Ă��邩
	mymath::Vec3f hangPoint_;			// �Ԃ牺�����Ă���x�_
	
	charabase::Anim motionAnim_;		// ���[�V�����A�j���[�V�����p

	charabase::CharPtr attackObj_;		// �U���͈͉摜
	float attackRadius_;				// �U���͈�
	bool isAttacking_;					// �U������

	//bool invincible_;					// ���G����
	float invincibleTime_;				// ���G����
	float invincibleAnim_;				// ���G�_�ŃA�j���[�V��������

	common::DispState chainState_;		// Chain�����摜�\�����
	
	//--------------------------------------------------
	charabase::CharPtr chainMsg_;		// Chain�����摜
	mymath::Vec3f numberPos_;			// Chain�������W
	float numberAnimTime_;				// Chain�����A�j���[�V�����p�J�E���g1
	mymath::Vec3f chainStaPos_;			// Chain���������ʒu
	float chainAnimTime_;				// Chain�����A�j���[�V�����p�J�E���g
	float chainTime_;					// Chain�c�莞��
	
	//--------------------------------------------------
	int chainCnt_;				// Chain��

	//--------------------------------------------------


private:

	/*
		@brief	�o��A�j���[�V����
		@return	�Ȃ�
	*/
	void EnterAnimation();

	/*
		@brief	�ޏ�A�j���[�V����
		@return	�Ȃ�
	*/
	void ExitAnimation();

	/*
		@brief	�L�[����
		@return	�Ȃ�
	*/
	void key();

	/*
		@brief	�L�����̈ړ�
		@return �Ȃ�
	*/
	void move();

	/*
		@brief	�t�b�N���O��
		@return	�Ȃ�
	*/
	void UnHang();
public:
	/*
		@brief	��I�u�W�F�N�g����
				DB���������ɌĂ΂��
	*/
	CPlayer();

	/*
		@param	[in]	x		�������W
		@param	[in]	y		�������W
		@param	[in]	z		�������W
	*/
	CPlayer(float x, float y, float z = 0.2f);
	
	/*
		@brief	���̏�����
		@param	[in]	x		�������W
		@param	[in]	y		�������W
		@param	[in]	z		�������W
		@return	�Ȃ�
	*/
	void init(float x, float y, float z);

	~CPlayer();
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
		@brief	�����蔻��̈�̎擾
		@return	�����蔻��̈�
	*/
	virtual Collisions GetDamageAreas() const override;
	
	/*
		@brief	�U���̓����蔻��̈�̎擾
		@return	�U���̓����蔻��̈�
	*/
	virtual Collisions GetAttackAreas() const override;

	/*
		@brief	�v���C���[�I�u�W�F�N�g���擾
		@return	�|�C���^
		@retval	nullptr	�v���C���[�I�u�W�F�N�g������Ă��Ȃ��ꍇ
		@retval	�v���C���[�̃|�C���^
	*/
	static ObjPtr GetPtr();

	/*
		@brief	�U���ҋ@���t���O�̎擾
		@return	�U���ҋ@���t���O
		@retval	true	�U���ҋ@��
		@retval	false	�U���ҋ@���łȂ�
	*/
	bool isAttacking() const;

	/*
		@brief	���G�����擾
		@return	���G����
		@retval	true	���G��
		@retval	false	���G���łȂ�
	*/
	bool isInvincible() const;

	/*
		@brief	�v���C���[��񏉊���
		@param	[in]	info	���[�h���Ă������ꗗ
		@return	�Ȃ�
	*/
	void SetInfo(const LoadInfo& info);

	/*
		@brief	�t�b�N�̎x�_�̐ݒ�
		@param	[in]	pos	�x�_���W
		@return	�Ȃ�
	*/
	void SetHangPoint(const mymath::Vec3f& pos);

	/*
		@brief	�_���[�W���Z
		@param	[in]	dam	�_���[�W��
		@return ���S������
		@retval	true	���S
		@retval	false	�c��
	*/
	//bool ApplyDamage(int dam) override;
	
	/*
		@brief	�U��
		@param	[in]	type	�U���̎��
		@param	[in]	pos		�G�̍��W
		@return	�Ȃ�
	*/
	void ApplyAttack(AttackType type, const mymath::Vec3f& pos);

	/*
		@brief	�G��|����
		@return	�Ȃ�
	*/
	void KilledEnemy();
	

	/*
		@brief	Chain���̎擾
		@return	Chain��
	*/
	int getChain() const;

	/*
		@brief	�{�[�i�X�X�R�A�p�m�[�_���[�W
		@return	�m�[�_���[�W�N���A��
		@retval	true	�m�[�_���[�W
		@retval	false	�_���[�W����
	*/
	bool isNoDamage() const;

};

#endif