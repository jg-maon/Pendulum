#ifndef DEF_CHARBASE_H
#define DEF_CHARBASE_H

//#define DEF_CHARBASE_EXT

#ifndef DEF_MYMATH_HPP
#include "MyMath.hpp"
#endif

#include <memory>
#include <string>

namespace charabase
{
// アニメーション用クラス
class Anim
{
public:
	int		no;				// 現在のアニメNO　この値は0,1,2,3,4,とカウントするだけ
	float	cnt;			// アニメ更新カウンタ
	float	speed;			// アニメ更新速度
	int		max;			// アニメ枚数　この値にnoが来ると0からカウント

	/*
		@brief	アニメーションの設定
		@param	[in]	max	最大値
		@param	[in]	speed	1フレームあたりの増加量
	*/
	Anim(int max = 0, float speed = 0.f);

	/*
		@brief	アニメーションの設定
		@param	[in]	max	最大値
		@param	[in]	speed	1フレームあたりの増加量
		@return	なし
	*/
	void set(int max, float speed);

	/*
		@brief	アニメーション更新
				アニメの順序を変更したい場合は以下
				int animTable[] = {0,1,0,2};
				//実際に利用する場合は
				animTable[obj.anim.no]
				StepAnim(obj.anim);
				
		@return	アニメーション終了したか
		@retval	true	アニメーション一巡
		@ratval	false	アニメーション中
	*/
	bool step();

	/*
		@brief		アニメーションの一時停止
		@attention	speedを0にするだけなので、再開時は再度setが必要
		@return		なし
	*/
	void stop();
};

class CharBase
{
public:
	mymath::Vec3f	pos;			// 座標,奥行き
	mymath::Vec3f	add;			// 移動量
	
	mymath::Vec3i	size;			// 1コマ幅高さ
	mymath::Vec3i	src;			// 抽出位置
	
	std::string	resname;			// 使用イメージ名
	mymath::Vec3f	scale;			// 拡大率
	float	angle;					// 角度
	float	alpha;					// アルファ
	float	r,g,b;					// RGB
	bool	show;					// 利用フラグ
	
	CharBase(const mymath::Vec3f& _pos,					// 座標
			const mymath::Vec3f& _add,					// 移動量
			const std::string& name="",					// 画像
			int w=0,int h=0,							// 画像サイズ
			float sx=1.0f,float sy=1.0f,				// 拡大率
			float deg=0.f,								// 回転角
			int srcX=0,									// 横要素番号(列)
			int srcY=0);								// 縦要素番号(行)

	CharBase(float x=0.f,float y=0.f,float z=0.f,		// 座標
			float adx=0.f,float ady=0.f,				// 移動量
			const std::string& name="",					// 画像
			int w=0,int h=0,							// 画像サイズ
			float sx=1.0f,float sy=1.0f,				// 拡大率
			float deg=0.f,								// 回転角
			int srcX = 0,								// 横要素番号(列)
			int srcY = 0);								// 縦要素番号(行)


	/*
		@brief	描画時の(拡大率を適用した)サイズの取得
		@return	描画サイズ
	*/ 
	mymath::Vec3f GetSize() const;
	/*
		@brief	半分の幅の取得
		@return	半分の幅
	*/
	float HalfWidth() const;
	/*
		@brief	半分の高さの取得
		@return	半分の高さ
	*/
	float HalfHeight() const;
	/*
		@brief	描画時の半分のサイズの取得
		@return	描画時の半分のサイズ
	*/
	mymath::Vec3f HalfSize() const;

	enum MODE{Center,LeftTop};
	/*
		@brief	判定矩形の取得
		@param	[in] mode	座標を中心とするか、左上とするか
		@return	判定矩形
	*/
	mymath::Rectf GetRect(MODE mode = Center) const;
	/*
		@brief	画像の描画
		@param	[in] mode	座標を中心とするか、左上とするか
		@return	なし
	*/
	void draw(MODE mode = Center) const;
	/*
		@brief	画像の描画 カメラ座標に影響されない
		@param	[in] mode	座標を中心とするか、左上とするか
		@return	なし
	*/
	void drawNC(MODE mode = Center) const;

	/*
		@brief	移動
		@param	[in] b3D	3D移動するか
		@return	なし
	*/
	void Move(bool b3D = false);					// 移動

	/*
		@brief	使用中かを返す
		@return	使用中か
		@retval	true	使用中
		@retval	false	未使用
	*/
	bool CheckUse();

	/*
		@brief	使用フラグを設定
		@param	[in] useFlag	使用フラグ
		@return	なし
	*/
	void SetUse(bool useFlag);


};


#ifdef DEF_CHARBASE_EXT
#include <wtypes.h>	//  RECT用

extern void MoveChar(CharBase& cb);
extern bool CheckUseChar(const CharBase& cb);
extern void NoUseChar(CharBase& cb);
extern void UseChar(CharBase& cb);
extern void MakeRectLeftTop(RECT &rt, const CharBase& cb, int startx = 0, int starty = 0, int endx = 0, int endy = 0);
extern void MakeRect(RECT &rt, const CharBase& cb, int startx = 0, int starty = 0, int endx = 0, int endy = 0);
extern void Draw_Char(const CharBase& cb);
extern void Draw_Char2(const CharBase& cb);
extern void Draw_CharNC(const CharBase& cb);
extern void Draw_Char2NC(const CharBase& cb);
extern bool HitCheck(const CharBase& cb1, const CharBase& cb2);

extern void	InitCharBase(CharBase& cb, float x, float y, float z, float addx, float addy, float addz,
	const std::string& resname, int srcX, int srcY, int srcZ, int w, int h,
	float angle = 0.f, float scaleX = 1.0f, float scaleY = 1.0f, float scaleZ = 1.0f,
	float alpha = 255.f, float red = 255.f, float green = 255.f, float blue = 255.f);
extern void InitCharBase(CharBase& cb);

extern void SetAnim(Anim& anim, int max, float speed);
extern bool StepAnim(Anim& anim);
extern void StopAnim(Anim& anim);

#endif

typedef std::shared_ptr<CharBase> CharPtr;
}	// namespace charabase


#endif