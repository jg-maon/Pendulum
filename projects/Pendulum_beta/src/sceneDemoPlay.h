#ifndef DEF_SCENEDEMOPLAY_H
#define DEF_SCENEDEMOPLAY_H

#ifndef DEF_PLAYER_H
#include "player.h"
#endif

#ifndef DEF_EASING_H
#include "easing.h"
#endif

#ifndef DEF_GAMEMANAGER_H
#include "gameManager.h"
#endif
/*
	@brief		�f���v���C�p�N���X
				�^�C�g���V�[������Ă΂��
	@attension	�ʏ�̃V�[�P���X�����Ƃ͏��肪�Ⴄ�̂Œ���
*/
class CSceneDemoPlay  : public Base
{
public:
	/*
		@brief	�f���v���C�p�v���C���[
	*/
	class CDemoPlayer : public CPlayer
	{
		float elapsedTime_;						// �o�ߎ���
		
		enum class TimeActType		// �^�C���e�[�u���p�s���ꗗ
		{
			HANG,		// �Ԃ牺����
			UNHANG,		// �Ԃ牺�������
			ATTACK,		// �U��
		};
		/*
			@brief	�^�C���e�[�u���p
		*/
		struct TableContent
		{
			float time;			// ����
			mymath::Vec3f pos;	// �J�[�\�����W
			CDemoPlayer::TimeActType act;	// �s��
		};

		float preActTime_;						// �ړ��v�Z�p
		std::vector<TableContent> timeTable_;	// �s���ꗗ
		int timeIndex_;							// �^�C���e�[�u���p
	protected:
		/*
			@brief	�I�u�W�F�N�g�ړ�
			@return	�Ȃ�
		*/
		virtual void MoveObject() override;

		/*
			@brief	�L�[����
			@return	�Ȃ�
		*/
		virtual void key() override;

	public:

		/*
			@param	[in]	x		�������W
			@param	[in]	y		�������W
			@param	[in]	z		�������W
		*/
		CDemoPlayer(float x, float y, float z = 0.2f);

		/*
			@brief	�X�V����
			@retrn	�Ȃ�
		*/
		void step() override;


		/*
			@brief	�f���v���C�I����
			@return	�f���v���C�I����
			@retval	true	�f���v���C�I��
			@retval	false	�f���v���C�p��
		*/
		bool isEndDemoPlay() const;
	};

private:
	std::weak_ptr<CDemoPlayer> player_;

	/*
		@brief	�X�^�b�t�N���W�b�g�p
	*/
	struct CreditObject
	{
		float time;					// ����
		mymath::Linef pos;			// �v�Z�p���W
		charabase::CharBase obj;	// �摜
		common::DispState state;	// �\�����
	};
	std::vector<CreditObject> creditObjs_;		// �X�^�b�t�N���W�b�g

public:
	/*
		@brief	����
	*/
	CSceneDemoPlay();
	/*
		@brief	�I������
	*/
	~CSceneDemoPlay();

	
	/*
		@brief	������
		@return	�Ȃ�
	*/
	void start() override;

	/*
		@brief	�I������
		@return	�Ȃ�
	*/
	void stop() override;

	void step() override{}

	/*	
		@brief	�X�V
		@return	�f���v���C�I����
		@retval	true	�f���v���C�I��
		@retval	false	�f���v���C�p����
	*/
	bool update();

	/*
		@brief	�`��
		@return	�Ȃ�
	*/
	void draw();
};


#endif
