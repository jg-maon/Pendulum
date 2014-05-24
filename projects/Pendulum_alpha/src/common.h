#ifndef DEF_COMMON_H
#define DEF_COMMON_H

#ifndef DEF_DEFINE_H
#include "define.h"
#endif


#include <vector>

#define array_size(x) (sizeof((x)) / sizeof((x)[0]))

namespace common
{
// 文字列の置換
std::string StrReplace(const std::string& str, const std::string& search, const std::string& replace);

// ラベルまでファイルストリームを進める
bool FindChunk(std::ifstream& f, const std::string& find);

// ファイルストリームを先頭に戻す
std::ifstream& SeekSet(std::ifstream& f);
// ファイルストリームを先頭に戻す
std::ofstream& SeekSet(std::ofstream& f);

//======================================================
#pragma region 関数のオーバーロード類

// POINT構造体の演算子オーバーロード
inline bool operator==(const POINT& p1, const POINT& p2);

inline bool operator!=(const POINT& p1, const POINT& p2);

inline POINT operator+(const POINT& p1, const POINT& p2);

// 出力マニピュレータをデフォルトの状態に戻す

class floatFixed
{
	int m_value;
public:
	floatFixed(int value){ m_value = value; }
	ostream& operator()(ostream& ost) const
	{
		ost << setprecision(m_value);	// 精度指定
		ost << fixed;					// 固定小数点形式
		return ost;
	}
};

inline ostream& operator << (ostream& ost, const floatFixed& fx);
// 引数に与えた数値を有効数字にするの
inline floatFixed fixval(int value);

// ostreamを初期設定に戻す
inline ostream& orgset(ostream& ost);

// 有効数字付き浮動小数点を文字列として取得する
template<typename T_> 
inline string getFloatString(T_ f, int val = 8, int w = 8);
template<typename T_> 
inline string getFloatString(T_ f, int val = 8, int w = 8, char fill = '0');

#pragma endregion	// 関数のオーバーロード類





///////////////////////////////////////////////////////////////
//
// Color
//
///////////////////////////////////////////////////////////////
namespace color
{
/*
	@brief	ARGBを各色に分解
	@param	[in]	argb	分解させる色
	@param	[out]	a		Alpha
	@param	[out]	r		Red
	@param	[out]	g		Green
	@param	[out]	b		Blue
	@return	なし
*/
template <class T> void Color_SeparateARGB(D3DCOLOR argb, T& a, T& r, T& g, T& b)
{
	a = static_cast<T>(argb >> 24 & 0xff);
	r = static_cast<T>(argb >> 16 & 0xff);
	g = static_cast<T>(argb >> 8 & 0xff);
	b = static_cast<T>(argb & 0xff);
}

/*
	@brief	AHSVのHチャンネルのみ加算
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
	@return RGB配列
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
	@return HSV配列
	@retval [0] = H(0~360)
	@retval [1] = S
	@retval [2] = V
*/
inline std::vector<float>	Color_RGBtoHSV(float red, float green, float blue);
}
///////////////////////////////////////////////////////////////



}	// namespace common
#endif