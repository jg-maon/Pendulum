#include "../../../lib/gplib.h"
#include "charBase.h"
using namespace mymath;
using namespace gplib;
namespace charabase
{
#pragma region Anim methods
Anim::Anim(int max, float speed)
{
	cnt = 0.f;
	this->max = max;
	this->speed = speed;
	no = 0;
}
void Anim::set(int max, float speed)
{
	cnt = 0.f;
	this->max = max;
	this->speed = speed;
	no = 0;
}
bool Anim::step()
{
	if ((cnt += speed) >= 1.f)
	{
		cnt = 0.f;
		if (++no > max)
		{
			no = 0;
			return true;
		}
	}
	return false;
}
void Anim::stop()
{
	speed = 0.f;
}

#pragma endregion Anim methods

#pragma region CharBase methods

CharBase::CharBase(const mymath::Vec3f& _pos,		// 座標
	const mymath::Vec3f& _add,						// 移動量
	const std::string& name,				// 画像
	int w, int h,							// 画像サイズ
	float sx, float sy,						// 拡大率
	float deg,								// 回転角
	int srcX,								// 横要素番号(列)
	int srcY								// 縦要素番号(行)
	) :
	pos(_pos)
	, add(_add)
	, resname(name)
	, size(w, h)
	, scale(sx, sy)
	, angle(deg)
	, src(srcX, srcY)
	, show(true)
{
	alpha = r = g = b = 255.f;
}

CharBase::CharBase(float x, float y, float z,		// 座標
	float adx, float ady,							// 移動量
	const std::string& name,						// 画像
	int w, int h,									// 画像サイズ
	float sx, float sy,								// 拡大率
	float deg,										// 回転角
	int srcX,										// 横要素番号(列)
	int srcY										// 縦要素番号(行)
	) :
	pos(x, y, z)
	, add(adx, ady)
	, resname(name)
	, size(w, h)
	, scale(sx, sy)
	, angle(deg)
	, src(srcX, srcY)
	, show(true)
{
	alpha = r = g = b = 255.f;
}

mymath::Vec3f CharBase::GetSize() const
{
	return mymath::Vec3f(size.x*scale.x, size.y*scale.y, size.z*scale.z);
}

float CharBase::HalfWidth() const
{
	return size.x*scale.x / 2.0f;
}

float CharBase::HalfHeight() const
{
	return size.y*scale.y / 2.0f;
}
mymath::Vec3f CharBase::HalfSize() const
{
	return GetSize() / 2.0f;
}

mymath::Rectf CharBase::GetRect(MODE mode) const
{
	if (mode == MODE::Center)
	{
		mymath::Rectf ret(	pos - HalfSize(),
					pos + HalfSize());
		return ret;
	}
	else if (mode == MODE::LeftTop)
	{
		mymath::Rectf ret(pos, pos + GetSize());
		return ret;
	}
	else
	{
		return mymath::Rectf();
	}
}

void CharBase::draw(MODE mode) const
{
	if(mode == MODE::Center)
	{
		graph::Draw_Graphics(
					pos.x, pos.y, pos.z,
					resname,
					size.x * src.x,
					size.y * src.y,
					size.x, size.y,
					static_cast<int>(angle), nullptr,
					scale.x, scale.y,
					static_cast<u_char>(alpha),
					static_cast<u_char>(r),
					static_cast<u_char>(g),
					static_cast<u_char>(b));
	}
	else
	{
		graph::Draw_GraphicsLeftTop(
					pos.x, pos.y, pos.z,
					resname,
					size.x * src.x,
					size.y * src.y,
					size.x, size.y,
					static_cast<int>(angle), nullptr,
					scale.x, scale.y,
					static_cast<u_char>(alpha),
					static_cast<u_char>(r),
					static_cast<u_char>(g),
					static_cast<u_char>(b));
	}
}
void CharBase::drawNC(MODE mode) const
{
	if (mode == MODE::Center)
	{
		graph::Draw_GraphicsNC(
					pos.x, pos.y, pos.z,
					resname,
					size.x * src.x,
					size.y * src.y,
					size.x, size.y,
					static_cast<int>(angle), nullptr,
					scale.x, scale.y,
					static_cast<u_char>(alpha),
					static_cast<u_char>(r),
					static_cast<u_char>(g),
					static_cast<u_char>(b));
	}
	else
	{
		graph::Draw_GraphicsLeftTopNC(
					pos.x, pos.y, pos.z,
					resname,
					size.x * src.x,
					size.y * src.y,
					size.x, size.y,
					static_cast<int>(angle), nullptr,
					scale.x, scale.y,
					static_cast<u_char>(alpha),
					static_cast<u_char>(r),
					static_cast<u_char>(g),
					static_cast<u_char>(b));
	}
}

void CharBase::Move(bool b3D)
{
	mymath::Vec3f v(add * system::FrameTime);
	if(b3D)
	{
		pos += v;
	}
	else
	{
		pos.x += v.x;
		pos.y += v.y;
	}
}

bool CharBase::CheckUse()
{
	return (show);
}

void CharBase::SetUse(bool useFlag)
{
	show = useFlag;
}

#pragma endregion // CharBase methods

//------------------------------------------------------
//------------------------------------------------------
//------------------------------------------------------
//------------------------------------------------------
//
#pragma region CharBase利用時汎用処理
#ifdef DEF_CHARBASE_EXT
//------------------------------------------------------
//アニメーション設定
void charabase::SetAnim(Anim& anim, int max, float speed)
{
	anim.cnt = 0.f;
	anim.max = max;
	anim.speed = speed;
	anim.no = 0;
}

//アニメ更新
//この関数はnoの値を更新する。noの値は
//CharBase.src.xに代入する。
//アニメの終了をした場合に真を返す
//アニメの順序を変更したい場合は以下
/*
StepAnim(obj.anim);
//以下でアニメーション指定可能
int animTable[] = {0,1,0,2};
//実際に利用する場合は
animTable[obj.anim.no]
StepAnim(obj.anim);

*/
//
bool charabase::StepAnim(Anim& anim)
{
	anim.cnt += anim.speed;
	if (anim.cnt >= 1.f){
		anim.cnt = 0;
		anim.no++;
		if (anim.no >= anim.max){
			anim.no = 0;
			return true;
		}
	}
	return false;
}

//----------------------
//アニメーション停止
//speedを0にするだけなので、再開時は再度SetAnimが必要
void charabase::StopAnim(Anim& anim)
{
	anim.speed = 0.f;
}

//------------------------------------------------------
//CharBase初期化処理
//
/*
//基本となるチップのサイズと利用する画像
BaseData base = {CHARWID,CHARHGT,charIMG};
//初期化　指定外の値は初期値になる。
//この場合はwid,hgt,img以外は初期値
InitCharBase(player.pos,base);

//この場合はwid,hgt,img,x,y以外は初期値
InitCharBase(player.pos,base,STARTPOSX,STARTPOSY);

利用する際は可視フラグshowも偽になっているため、
UseCharで可視状態にすること。
*/
void charabase::InitCharBase(CharBase& cb, float x, float y, float z, float addx, float addy, float addz,
	const std::string& resname, int srcX, int srcY, int srcZ, int w, int h,
	float angle, float scaleX, float scaleY, float scaleZ,
	float alpha, float red, float green, float blue)
{
	cb.pos.x = x;
	cb.pos.y = y;
	cb.pos.z = z;
	cb.add.x = addx;
	cb.add.y = addy;
	cb.add.z = addz;
	cb.scale.x = scaleX;
	cb.scale.y = scaleY;
	cb.scale.y = scaleZ;
	cb.src.x = srcX;
	cb.src.y = srcY;
	cb.src.z = srcZ;
	cb.resname = resname;
	cb.size.x = w;
	cb.size.y = h;
	cb.angle = angle;
	cb.alpha = alpha;
	cb.r = red;
	cb.g = green;
	cb.b = blue;
	cb.show = false;

}

//------------------------------------------------------
//CharBase初期化処理
//すべて０にする。
void charabase::InitCharBase(CharBase& cb)
{
	CharBase zeroCharBase = {};
	cb = zeroCharBase;
}


//----------------------
//利用チェック
bool charabase::CheckUseChar(const CharBase& cb)
{
	return (cb.show == true);
}

//----------------------
//汎用移動
void charabase::MoveChar(CharBase& cb)
{
	cb.pos.x += cb.add.x;
	cb.pos.y += cb.add.y;
	cb.pos.z += cb.add.z;
}

//----------------------
//汎用消去処理
void charabase::NoUseChar(CharBase& cb)
{
	cb.show = false;
}

//----------------------
//汎用出現処理
void charabase::UseChar(CharBase& cb)
{
	cb.show = true;
}
//----------------------
//判定用矩形作成 左上座標基準
//大きさがクリッピングサイズと異なる場合に利用する。
//startx,starty : 左上点からのオフセット値、値分左上から加算される
//endx,endy : 右下点からのオフセット値、値分右下から減算される
void charabase::MakeRectLeftTop(RECT &rt, const CharBase& cb, int startx, int starty, int endx, int endy)
{
	SetRect(&rt, (int)cb.pos.x + startx, (int)cb.pos.y + starty,
		(int)cb.pos.x + cb.size.x - endx, (int)cb.pos.y + cb.size.y - endy);
}

//----------------------
//判定用矩形作成 中央座標基準
//大きさがクリッピングサイズと異なる場合に利用する。
//startx,starty : 左上点からのオフセット値、値分左上から加算される
//endx,endy : 右下点からのオフセット値、値分右下から減算される
void charabase::MakeRect(RECT &rt, const CharBase& cb, int startx, int starty, int endx, int endy)
{
	SetRect(&rt, (int)cb.pos.x - cb.size.x / 2 + startx, (int)cb.pos.y - cb.size.y / 2 + starty,
		(int)cb.pos.x + cb.size.x / 2 - endx, (int)cb.pos.y + cb.size.y / 2 - endy);
}

//----------------------
//汎用描画処理 中央基準
void charabase::Draw_Char(const CharBase& pos)
{
	graph::Draw_Graphics(
		pos.pos.x, pos.pos.y, pos.pos.z,
		pos.resname,
		pos.size.x * pos.src.x,
		pos.size.y * pos.src.y,
		pos.size.x, pos.size.y,
		(int)pos.angle, nullptr,
		pos.scale.x, pos.scale.y,
		(u_char)pos.alpha, (u_char)pos.r, (u_char)pos.g, (u_char)pos.b);
}

//----------------------
//汎用描画処理 左上基準
void charabase::Draw_Char2(const CharBase& pos)
{
	graph::Draw_GraphicsLeftTop(
		pos.pos.x, pos.pos.y, pos.pos.z,
		pos.resname,
		pos.size.x * pos.src.x,
		pos.size.y * pos.src.y,
		pos.size.x, pos.size.y,
		(int)pos.angle, nullptr,
		pos.scale.x, pos.scale.y,
		(u_char)pos.alpha, (u_char)pos.r, (u_char)pos.g, (u_char)pos.b);
}


//----------------------
//汎用描画処理 中央基準
void charabase::Draw_CharNC(const CharBase& pos)
{

	graph::Draw_GraphicsNC(
		pos.pos.x, pos.pos.y, pos.pos.z,
		pos.resname,
		pos.size.x * pos.src.x,
		pos.size.y * pos.src.y,
		pos.size.x, pos.size.y,
		(int)pos.angle, nullptr,
		pos.scale.x, pos.scale.y,
		(u_char)pos.alpha, (u_char)pos.r, (u_char)pos.g, (u_char)pos.b);
}

//----------------------
//汎用描画処理 左上基準
void charabase::Draw_Char2NC(const CharBase& pos)
{
	graph::Draw_GraphicsLeftTopNC(
		pos.pos.x, pos.pos.y, pos.pos.z,
		pos.resname,
		pos.size.x * pos.src.x,
		pos.size.y * pos.src.y,
		pos.size.x, pos.size.y,
		(int)pos.angle, nullptr,
		pos.scale.x, pos.scale.y,
		(u_char)pos.alpha, (u_char)pos.r, (u_char)pos.g, (u_char)pos.b);
}
//----------------------
//あたりチェック
bool charabase::HitCheck(const CharBase& v1, const CharBase& v2)
{
	RECT rt1;
	RECT rt2;
	charabase::MakeRect(rt1, v1);
	charabase::MakeRect(rt2, v2);
	//デバッグ----------------
	//	Draw_CkRect(rt1);
	//	Draw_CkRect(rt2);
	//ここまで----------------
	if (math::RectCheck(&rt1, &rt2)){
		return true;
	}
	else{
		return false;
	}
}
//ここまで
//------------------------------------------------------
#endif
#pragma endregion CharBase汎用処理
//------------------------------------------------------
}	// namespace charabase
