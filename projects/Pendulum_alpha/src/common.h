#ifndef DEF_COMMON_H
#define DEF_COMMON_H

#ifndef DEF_DEFINE_H
#include "define.h"
#endif


#include <vector>

#define arrayof(x) (sizeof((x)) / sizeof((x)[0]))

// POINT�\���̂̉��Z�q�I�[�o�[���[�h
extern bool operator==(const POINT& p1, const POINT& p2);

extern bool operator!=(const POINT& p1, const POINT& p2);

extern POINT operator+(const POINT& p1, const POINT& p2);

namespace common
{
// ������̒u��
std::string StrReplace(const std::string& str, const std::string& search, const std::string& replace);

// ���x���܂Ńt�@�C���X�g���[����i�߂�
bool FindChunk(std::ifstream& f, const std::string& find);

// �t�@�C���X�g���[����擪�ɖ߂�
std::ifstream& SeekSet(std::ifstream& f);
// �t�@�C���X�g���[����擪�ɖ߂�
std::ofstream& SeekSet(std::ofstream& f);

//======================================================
#pragma region �֐��̃I�[�o�[���[�h��

// �o�̓}�j�s�����[�^���f�t�H���g�̏�Ԃɖ߂�

class floatFixed
{
	int m_value;
public:
	floatFixed(int value){ m_value = value; }
	ostream& operator()(ostream& ost) const
	{
		ost << setprecision(m_value);	// ���x�w��
		ost << fixed;					// �Œ菬���_�`��
		return ost;
	}
};

inline ostream& operator << (ostream& ost, const floatFixed& fx);
// �����ɗ^�������l��L�������ɂ����
inline floatFixed fixval(int value);

// ostream�������ݒ�ɖ߂�
inline ostream& orgset(ostream& ost);

// �L�������t�����������_�𕶎���Ƃ��Ď擾����
template<typename T_> 
inline string getFloatString(T_ f, int val = 8, int w = 8);
template<typename T_> 
inline string getFloatString(T_ f, int val = 8, int w = 8, char fill = '0');

#pragma endregion	// �֐��̃I�[�o�[���[�h��





///////////////////////////////////////////////////////////////
//
// Color
//
///////////////////////////////////////////////////////////////
namespace color
{
/*
	@brief	ARGB���e�F�ɕ���
	@param	[in]	argb	����������F
	@param	[out]	a		Alpha
	@param	[out]	r		Red
	@param	[out]	g		Green
	@param	[out]	b		Blue
	@return	�Ȃ�
*/
template <class T> void Color_SeparateARGB(D3DCOLOR argb, T& a, T& r, T& g, T& b)
{
	a = static_cast<T>(argb >> 24 & 0xff);
	r = static_cast<T>(argb >> 16 & 0xff);
	g = static_cast<T>(argb >> 8 & 0xff);
	b = static_cast<T>(argb & 0xff);
}

/*
	@brief	AHSV��H�`�����l���̂݉��Z
	@return AHSV(D3DCOLOR)
	@retval A = A
			R = H
			G = S
			B = V
*/
inline D3DCOLOR	Color_AddHChannel(D3DCOLOR ahsv, int value);
//-----------------------------
#define	Color_ARGBFromAHSV	Color_AHSVtoARGB
/*
	@brief AHSV -> ARGB
	@return ARGB
*/
inline D3DCOLOR	Color_AHSVtoARGB(float alpha, float h, float s, float v);
inline D3DCOLOR	Color_AHSVtoARGB(D3DCOLOR ahsv);
//-----------------------------
#define	Color_RGBFromHSV	Color_HSVtoRGB
/*
	@brief HSV -> RGB
	@return RGB�z��
	@retval [0] = R
	@retval [1] = G
	@retval [2] = B
*/
inline std::vector<float>	Color_HSVtoRGB(float h, float s, float v);
/*
	@brief ARGB -> AHSV
	@return AHSV(D3DCOLOR)
	@retval A = A
			R = H
			G = S
			B = V
*/
#define	Color_AHSVFromARGB	Color_ARGBtoAHSV
inline D3DCOLOR Color_ARGBtoAHSV(float alpha, float red, float green, float blue);
inline D3DCOLOR	Color_ARGBtoAHSV(D3DCOLOR argb);
//-----------------------------
#define	Color_HSVFromRGB	Color_RGBtoHSV
/*
	@brief RGB -> HSV
	@return HSV�z��
	@retval [0] = H(0~360)
	@retval [1] = S
	@retval [2] = V
*/
inline std::vector<float>	Color_RGBtoHSV(float red, float green, float blue);
}
///////////////////////////////////////////////////////////////



}	// namespace common
#endif