#ifndef DEF_GAMESTATUS_H
#define DEF_GAMESTATUS_H

#ifndef DEF_OBJ_H
#include "obj.h"
#endif

#ifndef DEF_COMMON_H
#include "common.h"
#endif

/*
	@brief	���C���Q�[���̏�Ԃ��Ǘ�
*/
class CGameStatus : public Base
{
public:
	/*
		@brief	���A�V�X�e���n
	*/
	struct Environment
	{
		float gravityAcc;			// �d�͉����x
	};
private:

	/*
		@brief	�q�b�g�X�g�b�v�n
	*/
	class HitStop
	{
	private:
		float hitStopTime_;			// �q�b�g�X�g�b�v����
		float maxHitStopTime_;		// �q�b�g�X�g�b�v�̍Œ�����

		float backAlpha_;			// �q�b�g�X�g�b�v���̔w�i�Â�
		float backAnimTime_;		// �w�i�\���A�j���[�V�����p
		common::DispState state_;	// �w�i�\�����
	public:
		/*
			@brief	������
		*/
		HitStop();
		/*
			@brief	������
			@param	[in]	stopTime	�q�b�g�X�g�b�v����
			@return	�Ȃ�
		*/
		void init(float hitStopTime);
		/*
			@brief	�q�b�g�X�g�b�v���ԍX�V����
			@return	�Ȃ�
		*/
		void step();
		/*
			@brief	�q�b�g�X�g�b�v���`�揈��
			@return	�Ȃ�
		*/
		void draw() const;


		/*
			@brief	�q�b�g�X�g�b�v�J�n
			@param	[in]	extendTime	��������
			@return	�Ȃ�
		*/
		void Begin(float extendTime);

		/*
			@brief	�q�b�g�X�g�b�v�̋����I��
			@return	�Ȃ�
		*/
		void End();

		/*
			@brief	�q�b�g�X�g�b�v����
			@return	�q�b�g�X�g�b�v����
			@retval	true	�q�b�g�X�g�b�v��
			@retval	false	�q�b�g�X�g�b�v���łȂ�
		*/
		bool isStopping() const;
	}hitStop_;


	int slowInterval_;			// �q�b�g�X�g�b�v���X�V�t���[��
	int updateCnt_;				// �X�V�v�Z�t���[����(0->MAX)

	Environment environment_;	// �V�X�e��

public:
	/*
		@brief	������
	*/
	CGameStatus();
	~CGameStatus();
	
	/*
		@brief	������
	*/
	virtual void start() override;

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
		@brief	�����擾
		@return	�����
	*/
	const Environment& getEnv() const;

	/*
		@brief	�q�b�g�X�g�b�v�J�n
		@param	[in]	extendTime	��������(�f�t�H���g:0.f=�ʏ�ʂ�)
		@return	�Ȃ�
	*/
	void BeginHitStop(float extendTime = 0.f);

	/*
		@brief	�q�b�g�X�g�b�v�̋����I��
		@return	�Ȃ�
	*/
	void EndHitStop();

	/*
		@brief	�q�b�g�X�g�b�v����
		@return	�q�b�g�X�g�b�v����
		@retval	true	�q�b�g�X�g�b�v��
		@retval	false	�q�b�g�X�g�b�v���łȂ�
	*/
	bool isHitStopping() const;

	/*
		@brief	�X�V���������邩
		@return	�X�V���������邩
		@retval	true	�X�V�������邱�Ƃ��ł���
		@retval	false	�X�V���Ȃ�
	*/
	bool isUpdatable() const;


};


#endif