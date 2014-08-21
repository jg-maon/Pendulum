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
private:
	enum class State{ play, stop };
	State state_;
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
		@brief		アニメーションの再開
		@attention	アニメーション処理の再開をするだけなので値をも変えたい場合はsetを呼ぶ
		@return		なし
	*/
	void start();
	/*
		@brief		アニメーションの一時停止
					再開時はstartを呼ぶ
		@return		なし
	*/
	void stop();

	/*
		@brief	アニメーションが終了しているか
		@return	アニメーションが終了しているか
		@retval	true	アニメーション終了
		@ratval	false	アニメーション中
	*/
	bool isStoped() const;
};

// 画像情報
class BaseData
{
public:
	std::string resname;			// 使用イメージ名
	mymath::Vec3i size;				// 1コマ幅高さ
	BaseData(const std::string& resname = "", const mymath::Vec3i& size = mymath::Vec3i());
	BaseData(const std::string& resname, int width, int height);
	// ファイルストリームからの管理名とサイズ入力
	friend std::ifstream& operator >> (std::ifstream& f, BaseData& d)
	{
		f >> d.resname >> d.size.x >> d.size.y;
		ImageSize(d);
		return f;
	}
	/*
		@brief	画像サイズの調整
		@return	なし
	*/
	static void ImageSize(BaseData& bd);
};

// 画像オブジェクト
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
	std::weak_ptr<POINT>	center;	// 中心点
	float	alpha;					// アルファ
	float	r,g,b;					// RGB
	bool	show;					// 利用フラグ
	
	CharBase(const mymath::Vec3f& _pos,				// 座標
		const mymath::Vec3f& _add,					// 移動量
		const std::string& name = "",				// 画像
		int w = 0, int h = 0,						// 画像サイズ
		float sx = 1.0f, float sy = 1.0f,			// 拡大率
		float deg = 0.f,							// 回転角
		int srcX = 0,								// 横要素番号(列), 抽出始点X座標
		int srcY = 0,								// 縦要素番号(行), 抽出始点Y座標
		const std::weak_ptr<POINT>& center = std::shared_ptr<POINT>(nullptr)	// 中心点
		);

	CharBase(float x=0.f,float y=0.f,float z=0.f,		// 座標
			float adx=0.f,float ady=0.f,				// 移動量
			const std::string& name="",					// 画像
			int w=0,int h=0,							// 画像サイズ
			float sx=1.0f,float sy=1.0f,				// 拡大率
			float deg=0.f,								// 回転角
			int srcX = 0,								// 横要素番号(列), 抽出始点X座標
			int srcY = 0,								// 縦要素番号(行), 抽出始点Y座標
			const std::weak_ptr<POINT>& center = std::shared_ptr<POINT>(nullptr)	// 中心点
		);

	// 画像情報以外初期値
	CharBase(const BaseData& bd);

	/*
		@brief	画像情報の上書き
		@param	[in]	bd	画像情報
		@return	なし
	*/
	void SetImgData(const BaseData& bd);

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
		@param	[in]	mode	座標を中心とするか、左上とするか(デフォルト:Center)
		@return	判定矩形
	*/
	mymath::Rectf GetRect(MODE mode = Center) const;
	/*
		@brief	画像の描画
		@param	[in]	mode		座標を中心とするか、左上とするか(デフォルト:Center)
		@param	[in]	turnFlag	反転描画するか(デフォルト:false)
		@return	なし
	*/
	void draw(MODE mode = Center, bool turnFlag = false) const;
	/*
		@brief	画像の描画 カメラ座標に影響されない
		@param	[in]	mode		座標を中心とするか、左上とするか(デフォルト:Center)
		@param	[in]	turnFlag	反転描画するか(デフォルト:false)
		@return	なし
	*/
	void drawNC(MODE mode = Center, bool turnFlag = false) const;
	/*
		@brief	画像の描画
				画像の抽出方法が座標管理になる
		@param	[in]	mode		座標を中心とするか、左上とするか(デフォルト:Center)
		@param	[in]	turnFlag	反転描画するか(デフォルト:false)
		@return	なし
	*/
	void draw2(MODE mode = Center, bool turnFlag = false) const;
	/*
		@brief	画像の描画 カメラ座標に影響されない
				画像の抽出方法が座標管理になる
		@param	[in]	mode		座標を中心とするか、左上とするか(デフォルト:Center)
		@param	[in]	turnFlag	反転描画するか(デフォルト:false)
		@return	なし
	*/
	void draw2NC(MODE mode = Center, bool turnFlag = false) const;

	/*
		@brief	移動
		@param	[in] b3D	3D移動するか(デフォルト:false)
		@return	なし
	*/
	void Move(bool b3D = false);					// 移動

	/*
		@brief	使用中かを返す
		@return	使用中か
		@retval	true	使用中
		@retval	false	未使用
	*/
	bool CheckUse() const;

	/*
		@brief	使用フラグを設定
		@param	[in] useFlag	使用フラグ
		@return	なし
	*/
	void SetUse(bool useFlag);


};

// アニメーションオブジェクト
class AnimObject
{
public:
	enum class AnimType
	{
		MOVE_X,		// 移動
		MOVE_Y,		// 移動
		ROTATE,		// 回転
		ANIM_X,		// 画像アニメーション
		ANIM_Y,		// 画像アニメーション
	}animType;
	charabase::CharBase obj;		// オブジェクト
	charabase::Anim anim;			// アニメーション用
	std::vector<float> animTbl;		// アニメーションテーブル
	/*
		@brief	アニメーション更新
		@return	なし
	*/
	void step();
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