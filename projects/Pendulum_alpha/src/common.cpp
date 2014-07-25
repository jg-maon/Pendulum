#include "common.h"
#include "myMath.hpp"
#include <sstream>
#include <iomanip>


#pragma region POINT�\���̂̉��Z�q�I�[�o�[���[�h
bool operator==(const POINT& p1, const POINT& p2)
{
	return p1.x == p2.x && p1.y == p2.y;
}

bool operator!=(const POINT& p1, const POINT& p2)
{
	return p1.x != p2.x || p1.y != p2.y;
}

POINT operator+(const POINT& p1, const POINT& p2)
{
	POINT ret = {
		p1.x + p2.x,
		p1.y + p2.y };
	return ret;
}

#pragma endregion // POINT



namespace common
{
std::vector<charabase::CharBase> GetNumberImageObjects(int num, const charabase::CharBase& obj, Align align)
{
	// num�̌������擾
	int digit = 1;
	{
		int work = num;
		while (work /= 10)
		{
			digit++;
		}
	}
	// �����ɃI�u�W�F�N�g�z�u
	std::vector<charabase::CharBase> retObjects(digit, obj);
	if (align == Align::RIGHT)
	{
		// �E����
		int work = num;
		for (int i = 1; i <= digit; ++i)
		{
			// ���ʌ����珇�ɔz�u
			auto& ret = retObjects[digit - i];
			ret.pos.x -= i * obj.size.x;
			//ret.pos.y -= 0.f;
			ret.src.x = (work % 10);
			ret.src.y = 0;

			// �����㏸
			work /= 10;
		}
	}
	else
	{
		// ������
		std::string work = std::to_string(num);
		for (int i = 0; i < digit; ++i)
		{
			// ��ʌ����珇�ɔz�u
			auto& ret = retObjects[i];
			ret.pos.x += i * obj.size.x;
			//ret.pos.y -= 0.f;
			ret.src.x = ((work[i] - '0') % 10);
			ret.src.y = 0;
		}
	}

	return retObjects;
}

//  �������u������
std::string StrReplace(const std::string& str, const std::string& search, const std::string& replace)
{
	std::string ret = str;
	std::string::size_type  pos(str.find(search));

	while (pos != std::string::npos)
	{
		ret.replace(pos, search.length(), replace);
		pos = ret.find(search, pos + replace.length());
	}

	return ret;
}

bool FindChunk(std::ifstream& f, const std::string& find)
{
	while (!f.eof())
	{
		std::string label;
		f >> label;
		if (find == label)
		{
			return true;
		}
	}
	return false;
}

std::ifstream& SeekSet(std::ifstream& f)
{
	f.clear();
	f.seekg(0);
	return f;
}
std::ofstream& SeekSet(std::ofstream& f)
{
	f.clear();
	f.seekp(0);
	return f;
}


//======================================================
#pragma region �֐��̃I�[�o�[���[�h��

#pragma region floatFixed
inline ostream& operator << (ostream& ost, const floatFixed& fx)
{
	return fx(ost);
}
// �����ɗ^�������l��L�������ɂ����
inline floatFixed fixval(int value)
{
	return floatFixed(value);
}

// ostream�������ݒ�ɖ߂�
inline std::ostream& orgset(std::ostream& ost)
{
	// ���������_�̈�ʌ`���\��
	ost.setf((std::ios_base::fmtflags)0, std::ios_base::floatfield);
	ost << dec << nouppercase;	// 10�i���A�������\��
	return ost;
}

// �L�������t�����������_�𕶎���Ƃ��Ď擾����
template<typename T_>
inline string getFloatString(T_ f, int val, int w)
{
	std::ostringstream ss;
	ss << std::right << std::setw(w) << std::fixval(val) << f;
	return ss.str();
}
template<typename T_>
inline string getFloatString(T_ f, int val, int w, char fill)
{
	std::ostringstream ss;
	ss << std::right << std::setfill(fill) << std::setw(w) << std::fixval(val) << f;
	return ss.str();
}

#pragma endregion // floatFixed

#pragma endregion // �֐��̃I�[�o�[���[�h��

//======================================================

///////////////////////////////////////////////////////////////
//
// Color
//
///////////////////////////////////////////////////////////////
namespace color
{


/*
	@brief	AHSV��H�`�����l���̂݉��Z
	@return AHSV(D3DCOLOR)
	@retval A = A
	R = H
	G = S
	B = V
*/
inline D3DCOLOR	Color_AddHChannel(D3DCOLOR ahsv, int value)
{
	int a = ahsv >> 24 & 0xff;
	int h = ahsv >> 16 & 0xff;
	int s = ahsv >> 8 & 0xff;
	int v = ahsv & 0xff;
	h = static_cast<int>(h * 360.f / 255.f);	// 0~360���Z
	h = (h + value) % 360;
	h = static_cast<int>(h * 255.f / 360.f);	// 0~255

	return D3DCOLOR_ARGB(a, h, s, v);
}
//-----------------------------

/*
	@brief AHSV -> ARGB
	@return ARGB
*/
inline D3DCOLOR	Color_AHSVtoARGB(float alpha, float h, float s, float v)
{
	float f;
	int c0 = static_cast<int>(mymath::round(v));
	int c1, c2, c3;
	int i = static_cast<int>(std::floorf(h / 60.0f)) % 6;
	f = (h / 60.0f) - i;

	c1 = static_cast<int>(mymath::round(v * (1.0f - (s / 255.0f))));
	c2 = static_cast<int>(mymath::round(v * (1.0f - (s / 255.0f) * f)));
	c3 = static_cast<int>(mymath::round(v * (1.0f - (s / 255.0f) * (1.0f - f))));

	int a = static_cast<int>(mymath::round(alpha));
	int rgb[3];
	switch (i)
	{
	case 0: rgb[0] = c0; rgb[1] = c3; rgb[2] = c1; break;
	case 1: rgb[0] = c2; rgb[1] = c0; rgb[2] = c1; break;
	case 2: rgb[0] = c1; rgb[1] = c0; rgb[2] = c3; break;
	case 3: rgb[0] = c1; rgb[1] = c2; rgb[2] = c0; break;
	case 4: rgb[0] = c3; rgb[1] = c1; rgb[2] = c0; break;
	case 5: rgb[0] = c0; rgb[1] = c1; rgb[2] = c2; break;
	}

	return D3DCOLOR_ARGB(a, rgb[0], rgb[1], rgb[2]);
}
inline D3DCOLOR	Color_AHSVtoARGB(D3DCOLOR ahsv)
{
	float a, h, s, v;
	Color_SeparateARGB(ahsv, a, h, s, v);
	return Color_AHSVtoARGB(a, h, s, v);
}
//-----------------------------
/*
	@brief HSV -> RGB
	@return RGB�z��
	@retval [0] = R
	@retval [1] = G
	@retval [2] = B
*/
inline std::vector<float>	Color_HSVtoRGB(float h, float s, float v)
{
	std::vector<float> color(3);
	D3DCOLOR argb = Color_AHSVtoARGB(0.f, h, s, v);
	color[0] = static_cast<float>(argb >> 16 & 0xff);
	color[1] = static_cast<float>(argb >> 8 & 0xff);
	color[2] = static_cast<float>(argb & 0xff);
	return color;
}
/*
	@brief ARGB -> AHSV
	@return AHSV(D3DCOLOR)
	@retval A = A
	R = H
	G = S
	B = V
*/
inline D3DCOLOR Color_ARGBtoAHSV(float alpha, float red, float green, float blue)
{
	int hsv[3];
	float max = max(red, max(green, blue));
	float min = min(red, min(green, blue));
	int a = static_cast<int>(alpha);

	// h
	if (max == 0.f){
		hsv[0] = 0;
	}
	else if (max == red){
		hsv[0] = static_cast<int>(60.f * (green - blue) / (max - min) + 360.f) % 360;
	}
	else if (max == green){
		hsv[0] = static_cast<int>(60.f * (blue - red) / (max - min)) + 120;
	}
	else if (max == blue){
		hsv[0] = static_cast<int>(60.f * (red - green) / (max - min)) + 240;
	}
	hsv[0] = static_cast<int>(hsv[0] * 255.f / 360.f);

	// s
	if (max == 0){
		hsv[1] = 0;
	}
	else{
		hsv[1] = static_cast<int>(255.f * ((max - min) / max));
	}

	// v
	hsv[2] = static_cast<int>(max);

	return D3DCOLOR_ARGB(a, hsv[0], hsv[1], hsv[2]);
}
inline D3DCOLOR	Color_ARGBtoAHSV(D3DCOLOR argb)
{
	float a, r, g, b;
	Color_SeparateARGB(argb, a, r, g, b);
	return Color_ARGBtoAHSV(a, r, g, b);
}
//-----------------------------

/*
	@brief RGB -> HSV
	@return HSV�z��
	@retval [0] = H(0~360)
	@retval [1] = S
	@retval [2] = V
*/
inline std::vector<float>	Color_RGBtoHSV(float red, float green, float blue)
{
	std::vector<float> color(3);
	D3DCOLOR argb = Color_ARGBtoAHSV(0.f, red, green, blue);
	color[0] = static_cast<float>(argb >> 16 & 0xff);
	color[1] = static_cast<float>(argb >> 8 & 0xff);
	color[2] = static_cast<float>(argb & 0xff);
	return color;
}

}// namespace color
///////////////////////////////////////////////////////////////


}	// namespace common