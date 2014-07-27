#ifndef DEF_FADE_H
#define DEF_FADE_H


class CFade
{
	static unsigned char r_;			// �t�F�[�h�F
	static unsigned char g_;			// �t�F�[�h�F
	static unsigned char b_;			// �t�F�[�h�F
	static float a_;			// alpha�l
	static bool fadeInOn_;		// �t�F�[�h�C����
	static bool fadeOutOn_;	// �t�F�[�h�A�E�g��
public:

	/*
		@brief	�F�ύX
		@param	[in]	r	��
		@param	[in]	g	��
		@param	[in]	b	��
		@return	�Ȃ�
	*/
	static void ChangeColor(unsigned char r, unsigned char g, unsigned char b);

	/*
		@brief	�F�ύX
		@param	[in]	argb	D3DCOLOR_ARGB�w��łł���悤�ɂ����F
		@return	�Ȃ�
	*/
	static void ChangeColor(unsigned long argb);

	/*
		@brief	�t�F�[�h�C���J�n
		@return	�Ȃ�
	*/
	static void StartFadeIn();

	/*
		@brief	�t�F�[�h�A�E�g�J�n
		@return	�Ȃ�
	*/
	static void StartFadeOut();

	/*
		@brief	�t�F�[�h�C��
		@param	[in]	speed	���x(1�t���[���̕ψ�)
		@return	�t�F�[�h�C��������������
		@retval	true	�t�F�[�h�C������
		@retval	false	�t�F�[�h�C����
	*/
	static bool FadeIn(float speed);
	
	/*
		@brief	�t�F�[�h�A�E�g
		@param	[in]	speed	���x(1�t���[���̕ψ�)
		@return	�t�F�[�h�A�E�g������������
		@retval	true	�t�F�[�h�A�E�g����
		@retval	false	�t�F�[�h�A�E�g��
	*/
	static bool FadeOut(float speed);

	/*
		@brief	�t�F�[�h�C���E�A�E�g�`��
		@attention	���t���[���Ăё�����
		@return	�Ȃ�
	*/
	static void draw();
	
	/*
		@brief	�t�F�[�h�C������
		@return	�t�F�[�h�C������
		@retval	true	�t�F�[�h�C����
		@retval	false	�t�F�[�h�C�����łȂ�
	*/
	static bool IsFadeInning();
	/*
		@brief	�t�F�[�h�A�E�g����
		@return	�t�F�[�h�A�E�g����
		@retval	true	�t�F�[�h�A�E�g��
		@retval	false	�t�F�[�h�A�E�g���łȂ�
	*/
	static bool IsFadeOuting();

	/*
		@brief	�t�F�[�h�C���E�A�E�g�ǂ��炩���s����
		@return	���s����
		@retval	true	�t�F�[�h���s��
		@retval	false	�t�F�[�h���s���łȂ�
	*/
	static bool IsFading();

};


#endif