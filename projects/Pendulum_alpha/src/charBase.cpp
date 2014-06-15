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

CharBase::CharBase(const mymath::Vec3f& _pos,		// ���W
	const mymath::Vec3f& _add,						// �ړ���
	const std::string& name,				// �摜
	int w, int h,							// �摜�T�C�Y
	float sx, float sy,						// �g�嗦
	float deg,								// ��]�p
	int srcX,								// ���v�f�ԍ�(��)
	int srcY								// �c�v�f�ԍ�(�s)
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

CharBase::CharBase(float x, float y, float z,		// ���W
	float adx, float ady,							// �ړ���
	const std::string& name,						// �摜
	int w, int h,									// �摜�T�C�Y
	float sx, float sy,								// �g�嗦
	float deg,										// ��]�p
	int srcX,										// ���v�f�ԍ�(��)
	int srcY										// �c�v�f�ԍ�(�s)
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
#pragma region CharBase���p���ėp����
#ifdef DEF_CHARBASE_EXT
//------------------------------------------------------
//�A�j���[�V�����ݒ�
void charabase::SetAnim(Anim& anim, int max, float speed)
{
	anim.cnt = 0.f;
	anim.max = max;
	anim.speed = speed;
	anim.no = 0;
}

//�A�j���X�V
//���̊֐���no�̒l���X�V����Bno�̒l��
//CharBase.src.x�ɑ������B
//�A�j���̏I���������ꍇ�ɐ^��Ԃ�
//�A�j���̏�����ύX�������ꍇ�͈ȉ�
/*
StepAnim(obj.anim);
//�ȉ��ŃA�j���[�V�����w��\
int animTable[] = {0,1,0,2};
//���ۂɗ��p����ꍇ��
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
//�A�j���[�V������~
//speed��0�ɂ��邾���Ȃ̂ŁA�ĊJ���͍ēxSetAnim���K�v
void charabase::StopAnim(Anim& anim)
{
	anim.speed = 0.f;
}

//------------------------------------------------------
//CharBase����������
//
/*
//��{�ƂȂ�`�b�v�̃T�C�Y�Ɨ��p����摜
BaseData base = {CHARWID,CHARHGT,charIMG};
//�������@�w��O�̒l�͏����l�ɂȂ�B
//���̏ꍇ��wid,hgt,img�ȊO�͏����l
InitCharBase(player.pos,base);

//���̏ꍇ��wid,hgt,img,x,y�ȊO�͏����l
InitCharBase(player.pos,base,STARTPOSX,STARTPOSY);

���p����ۂ͉��t���Oshow���U�ɂȂ��Ă��邽�߁A
UseChar�ŉ���Ԃɂ��邱�ƁB
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
//CharBase����������
//���ׂĂO�ɂ���B
void charabase::InitCharBase(CharBase& cb)
{
	CharBase zeroCharBase = {};
	cb = zeroCharBase;
}


//----------------------
//���p�`�F�b�N
bool charabase::CheckUseChar(const CharBase& cb)
{
	return (cb.show == true);
}

//----------------------
//�ėp�ړ�
void charabase::MoveChar(CharBase& cb)
{
	cb.pos.x += cb.add.x;
	cb.pos.y += cb.add.y;
	cb.pos.z += cb.add.z;
}

//----------------------
//�ėp��������
void charabase::NoUseChar(CharBase& cb)
{
	cb.show = false;
}

//----------------------
//�ėp�o������
void charabase::UseChar(CharBase& cb)
{
	cb.show = true;
}
//----------------------
//����p��`�쐬 ������W�
//�傫�����N���b�s���O�T�C�Y�ƈقȂ�ꍇ�ɗ��p����B
//startx,starty : ����_����̃I�t�Z�b�g�l�A�l�����ォ����Z�����
//endx,endy : �E���_����̃I�t�Z�b�g�l�A�l���E�����猸�Z�����
void charabase::MakeRectLeftTop(RECT &rt, const CharBase& cb, int startx, int starty, int endx, int endy)
{
	SetRect(&rt, (int)cb.pos.x + startx, (int)cb.pos.y + starty,
		(int)cb.pos.x + cb.size.x - endx, (int)cb.pos.y + cb.size.y - endy);
}

//----------------------
//����p��`�쐬 �������W�
//�傫�����N���b�s���O�T�C�Y�ƈقȂ�ꍇ�ɗ��p����B
//startx,starty : ����_����̃I�t�Z�b�g�l�A�l�����ォ����Z�����
//endx,endy : �E���_����̃I�t�Z�b�g�l�A�l���E�����猸�Z�����
void charabase::MakeRect(RECT &rt, const CharBase& cb, int startx, int starty, int endx, int endy)
{
	SetRect(&rt, (int)cb.pos.x - cb.size.x / 2 + startx, (int)cb.pos.y - cb.size.y / 2 + starty,
		(int)cb.pos.x + cb.size.x / 2 - endx, (int)cb.pos.y + cb.size.y / 2 - endy);
}

//----------------------
//�ėp�`�揈�� �����
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
//�ėp�`�揈�� ����
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
//�ėp�`�揈�� �����
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
//�ėp�`�揈�� ����
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
//������`�F�b�N
bool charabase::HitCheck(const CharBase& v1, const CharBase& v2)
{
	RECT rt1;
	RECT rt2;
	charabase::MakeRect(rt1, v1);
	charabase::MakeRect(rt2, v2);
	//�f�o�b�O----------------
	//	Draw_CkRect(rt1);
	//	Draw_CkRect(rt2);
	//�����܂�----------------
	if (math::RectCheck(&rt1, &rt2)){
		return true;
	}
	else{
		return false;
	}
}
//�����܂�
//------------------------------------------------------
#endif
#pragma endregion CharBase�ėp����
//------------------------------------------------------
}	// namespace charabase
