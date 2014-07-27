#ifndef DEF_SCENENAMEINPUT_H
#define DEF_SCENENAMEINPUT_H

#ifndef DEF_SCENEBASE_HPP
#include "sceneBase.hpp"
#endif

class CSceneNameInput : public IScene
{

private:
	//======================================
	// �萔
	//======================================
	enum kbData
	{

		KB_WIDTH = 50,
		KB_HEIGHT = 50,

	};


	enum selectOffset{

		OFFSET_W = 10,
		OFFSET_H = 10,

	};


	enum chgData
	{

		CHG_WIDTH = 150,
		CHG_HEIGHT = 50,

	};

	static const mymath::Vec3f KEYBOARD_POS;			// �L�[�{�[�h�̍��Wx,y,z
	static const mymath::Vec3f CHANGE_POS;				// �����L���ύX�{�^���̍��Wx,y,z
	static const mymath::Vec3f DISPLAY_POS;				// �f�B�X�v���C�̍��Wx,y,z
	static const float FLASHING_TIME;
	static const std::string KEYBOARD_MARKDATA[4];		// �L�[�{�[�h�L���f�[�^�z��
	static const std::string KEYBOARD_NAME[3];			// �L�[�{�[�h�{�^���摜���@	[0]:�\������,[1]:�I��,[2]:�{�^���ʉ�
	static const std::string CHANGE_NAME[3];			// �����L���ϊ��{�^���摜���@[0]:�\������,[1]:�I��,[2]:�{�^���ʉ�
	static const std::string DISPLAY_NAME[2];			// �L�[�{�[�h�̉摜��		[0]:�\������,[1]:�_�ŉ摜


public:

	//�{�^���̑���
	enum class BtnAttribute
	{
		NONE,
		KEYBOARD_DATA,		// �L�[�{�[�h�ł̕�������
		KEYBOARD_DELETE,	// �L�[�{�[�h�ł̍폜
		CHANGE_UP_LOW,		// �L�[�{�[�h�ł̑啶���������ϊ�
		CHANGE_LETTER,
		CHANGE_MARK,		// �L�[�{�[�h�ł̕����L���ϊ�

	};

	//======================================
	// �I�u�W�F�N�g�N���X
	//======================================

	class CButton : public IObject
	{

	public:

		//�{�^���̏��
		enum class BtnState
		{

			NONE,			// �����Ȃ�
			SELECT,			// �}�E�X����������
			PRESS,			// ������Ă�����
			RELEASE,		// ���ꂽ���

		};

		//  �{�^�����m�̊Ԋu
		enum BtnInterval{

			INTERVAL_W = 5,				// ���Ԋu
			INTERVAL_H = 5,				// �����Ԋu


		};

	private:
		BtnState pushState_;			// �{�^���̏��
		BtnAttribute attribute_;		// �{�^���̑���
		char charData_;					// �����f�[�^
		bool isCapital_;				// �啶���������p�@true:�啶��

		charabase::CharBase btn_;		// �{�^���ʉ��p
		charabase::CharBase select_;	// �{�^���I��p

	private:

		/*
			@brief	�{�^����������Ă���
			@return	�{�^���������ꂽ�u�Ԃ�
			@retval	true	�{�^���������ꂽ
			@retval	false	�{�^���������ꂽ�u�ԂłȂ�
		*/
		//bool CheckRelease();

		/*
			@brief	�{�^���������ꑱ���Ă���
			@return	�{�^���������ꑱ���Ă��邩
			@retval	true	�{�^����������Ă���
			@retval	false	�{�^����������Ă��Ȃ�
		*/
		//bool CheckPress();

		/*
			@brief	�}�E�X���{�^���ɏ���Ă���
			@return	�Ȃ�
		*/
		bool CheckSelect();

		/*
			@brief	�{�^���ƃ}�E�X�̊֌W
			@return	�Ȃ�
		*/
		void CheckState();
	
	public:
		CButton();
		~CButton();
		/*
			@brief	�I�u�W�F�N�g������
			���S����Ƌ��E�������͓���D�悳���
			@param	[in]	pos			[0][0]�̍��W X,Y (������̎��͂��̍��WX,Y�j
			@param	[in]	w			�{�^���̕�
			@param	[in]	h			�{�^���̍���
			@param	[in]	atr			�{�^���̑���
			@param	[in]	resnames[3]	�{�^���̉摜��(�v�f[0]:�\������ [1]:�I���� [2]:�{�^���� [3]:�{�^����)
			@param	[in]	col			�{�^�����z�񂾂����ꍇ�̍s��(��Ȃ�0)
			@param	[in]	row			�{�^�����z�񂾂����ꍇ�̗�(��Ȃ�0)
			@return	�Ȃ�
		*/
		void init(const mymath::Vec3f& pos, int w, int h, BtnAttribute atr, const string resnames[3],int col , int row);

		
		/*
			@brief	�{�^���̏���
			@return	�Ȃ�
		*/
		void step() override;

		/*
			@brief	�{�^���̕`��
			@return	�Ȃ�
		*/
		void draw() override;


		/*
			@brief	�啶���������{�^���̏�����
			@return	�Ȃ�
		*/
		void UpLowInit();

		/*
			@brief	�L�[�{�[�h�{�^���啶���������ω�
			@return	�Ȃ�
		*/
		void ChangeLetterUpLow();

		/*
			@brief	�L�[�{�[�h�{�^���啶���ω�
			@param	[in]	col			�{�^�����z�񂾂����ꍇ�̍s��(��Ȃ�0)
			@param	[in]	row			�{�^�����z�񂾂����ꍇ�̗�(��Ȃ�0)
			@return	�Ȃ�
		*/
		void ChangeLetter(int row, int col);

		/*
			@brief	�L�[�{�[�h�{�^���L���ω�
			@param	[in]	col			�{�^�����z�񂾂����ꍇ�̍s��(��Ȃ�0)
			@param	[in]	row			�{�^�����z�񂾂����ꍇ�̗�(��Ȃ�0)
			@return	�Ȃ�
		*/
		void ChangeMark(int row, int col);

		/*
			@brief	�{�^���̕`��ύX����
			@return	�����ꂽ���ǂ���
			@retval	true	�����ꂽ
			@retval	false	����ȊO
		*/
		bool isReleased();

		/*
			@brief	�啶���������t���O�𔽓]������
			@return	�Ȃ�
		*/
		void ChangeCapital();

		/*
			@brief	���蓖�Ă�ꂽ�����擾
			@return	����
		*/
		char getChar() const;

		/*
			@brief	�{�^�������̎擾
			@return	�{�^������
		*/
		BtnAttribute getAtt() const;
		
		/*
			@brief	�{�^�������̕ύX
			@return	�Ȃ�
		*/
		void setAtt(BtnAttribute att);

	};


	//======================================
	// �f�B�X�v���C�p�N���X
	//======================================

	class CDisplay : public IObject
	{

	public:

		//�{�^���̏��
		enum class DispState
		{

			NONE,			// �����Ȃ�
			FLASH,			// �_��
			DISP,			// �\��

		};
	private:

		enum DispSize
		{
			DISP_WIDTH = 50,			// �f�B�X�v���C��
			DISP_HEIGHT = 50,			// �f�B�X�v���C����
		};

	private:

		charabase::CharBase flash_;		// �t���b�V������

		DispState dispState_;			// �\�����
		float phaseTime_;				// �f�B�X�v���C�_�ŗp����

	private:

		/*
			@brief	�_�ŏ���
			@return	�Ȃ�
		*/
		void StepFlashing();



	public:
		CDisplay();
		~CDisplay();
	

		/*
			@brief	��ԕύX
			@param	[in]	ds_	�\�����
			@return	�Ȃ�
		*/
		void ChangeState(DispState ds_);


		/*
			@brief	�f�[�^�̓���
			@param	[in]	cb_	���͏��
			@return	�Ȃ�
		*/
		void LoadLetter(const charabase::CharBase& cb_);

		/*
			@brief	�f�B�X�v���C�̏�����
			@param	[in]	resnames	[0]:obj_ / [1]:flash_
			@param	[in]	row			��
			@return	�Ȃ�
		*/
		void init(const string resnames[2], int row);

		/*
			@brief	�f�B�X�v���C�̏���
			@return	�Ȃ�
		*/
		void step() override;

		/*
			@brief	�f�B�X�v���C�̕`��
			@return	�Ȃ�
		*/
		void draw() override;


	};


private:

	enum class State
	{
		INPUT,
		CHECK,
		END,

	};

	enum
	{
		NAME_LEN = 6,	// ������
	};

	State state_;

	CButton keyButtons[4][7];		// �L�[�{�[�h�{�^��
	CButton changeMark[2];			// �����L���ύX�{�^��

	CDisplay display[NAME_LEN];		// �f�B�X�v���C


	//charabase::CharBase bg_;		// �w�i

	std::string name_;				// ���͂��ꂽ���O�̊i�[�ꏊ
	int wordcount_;					// ������

private:

	/*
		@brief	�{�^���������ꂽ�Ƃ��̊e����
		@return	�Ȃ�
	*/
	void IsRelease(const CButton& cb_);


	/*
		@brief	����
		@return	�Ȃ�
	*/
	void stepInput();


	/*
		@brief	�f�B�X�v���C�̏�ԕύX
		@return	�Ȃ�
	*/
	void DisplayReflesh();


	/*
		@brief	�L�[���͊m�F
		@return	�Ȃ�
	*/
	//bool InputNextKey();



protected:
	/*
		@brief	���C���X�V����
		@return	���C���I����
		@retval	true	���C���I��(�t�F�[�h�A�E�g�J�n)
		@retval	false	���C��������
	*/
	bool update() override;


public:
	CSceneNameInput();
	~CSceneNameInput();
	/*
		@brief	�`��
		@return	�Ȃ�
	*/
	void draw() override;

	/*
		@brief	������
		@return	�Ȃ�
	*/
	void start() override;
	
	/*
		@brief	���̃V�[���ɐ؂�ւ��u�ԂɌĂ΂��
		@return	���̃V�[��
	*/
	int NextScene() const override;
};


#endif