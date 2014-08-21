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
// �A�j���[�V�����p�N���X
class Anim
{
private:
	enum class State{ play, stop };
	State state_;
public:
	int		no;				// ���݂̃A�j��NO�@���̒l��0,1,2,3,4,�ƃJ�E���g���邾��
	float	cnt;			// �A�j���X�V�J�E���^
	float	speed;			// �A�j���X�V���x
	int		max;			// �A�j�������@���̒l��no�������0����J�E���g

	/*
		@brief	�A�j���[�V�����̐ݒ�
		@param	[in]	max	�ő�l
		@param	[in]	speed	1�t���[��������̑�����
	*/
	Anim(int max = 0, float speed = 0.f);

	/*
		@brief	�A�j���[�V�����̐ݒ�
		@param	[in]	max	�ő�l
		@param	[in]	speed	1�t���[��������̑�����
		@return	�Ȃ�
	*/
	void set(int max, float speed);

	/*
		@brief	�A�j���[�V�����X�V
				�A�j���̏�����ύX�������ꍇ�͈ȉ�
				int animTable[] = {0,1,0,2};
				//���ۂɗ��p����ꍇ��
				animTable[obj.anim.no]
				StepAnim(obj.anim);
				
		@return	�A�j���[�V�����I��������
		@retval	true	�A�j���[�V�����ꏄ
		@ratval	false	�A�j���[�V������
	*/
	bool step();
	
	/*
		@brief		�A�j���[�V�����̍ĊJ
		@attention	�A�j���[�V���������̍ĊJ�����邾���Ȃ̂Œl�����ς������ꍇ��set���Ă�
		@return		�Ȃ�
	*/
	void start();
	/*
		@brief		�A�j���[�V�����̈ꎞ��~
					�ĊJ����start���Ă�
		@return		�Ȃ�
	*/
	void stop();

	/*
		@brief	�A�j���[�V�������I�����Ă��邩
		@return	�A�j���[�V�������I�����Ă��邩
		@retval	true	�A�j���[�V�����I��
		@ratval	false	�A�j���[�V������
	*/
	bool isStoped() const;
};

// �摜���
class BaseData
{
public:
	std::string resname;			// �g�p�C���[�W��
	mymath::Vec3i size;				// 1�R�}������
	BaseData(const std::string& resname = "", const mymath::Vec3i& size = mymath::Vec3i());
	BaseData(const std::string& resname, int width, int height);
	// �t�@�C���X�g���[������̊Ǘ����ƃT�C�Y����
	friend std::ifstream& operator >> (std::ifstream& f, BaseData& d)
	{
		f >> d.resname >> d.size.x >> d.size.y;
		ImageSize(d);
		return f;
	}
	/*
		@brief	�摜�T�C�Y�̒���
		@return	�Ȃ�
	*/
	static void ImageSize(BaseData& bd);
};

// �摜�I�u�W�F�N�g
class CharBase
{
public:
	mymath::Vec3f	pos;			// ���W,���s��
	mymath::Vec3f	add;			// �ړ���
	
	mymath::Vec3i	size;			// 1�R�}������
	mymath::Vec3i	src;			// ���o�ʒu
	
	std::string	resname;			// �g�p�C���[�W��
	mymath::Vec3f	scale;			// �g�嗦
	float	angle;					// �p�x
	std::weak_ptr<POINT>	center;	// ���S�_
	float	alpha;					// �A���t�@
	float	r,g,b;					// RGB
	bool	show;					// ���p�t���O
	
	CharBase(const mymath::Vec3f& _pos,				// ���W
		const mymath::Vec3f& _add,					// �ړ���
		const std::string& name = "",				// �摜
		int w = 0, int h = 0,						// �摜�T�C�Y
		float sx = 1.0f, float sy = 1.0f,			// �g�嗦
		float deg = 0.f,							// ��]�p
		int srcX = 0,								// ���v�f�ԍ�(��), ���o�n�_X���W
		int srcY = 0,								// �c�v�f�ԍ�(�s), ���o�n�_Y���W
		const std::weak_ptr<POINT>& center = std::shared_ptr<POINT>(nullptr)	// ���S�_
		);

	CharBase(float x=0.f,float y=0.f,float z=0.f,		// ���W
			float adx=0.f,float ady=0.f,				// �ړ���
			const std::string& name="",					// �摜
			int w=0,int h=0,							// �摜�T�C�Y
			float sx=1.0f,float sy=1.0f,				// �g�嗦
			float deg=0.f,								// ��]�p
			int srcX = 0,								// ���v�f�ԍ�(��), ���o�n�_X���W
			int srcY = 0,								// �c�v�f�ԍ�(�s), ���o�n�_Y���W
			const std::weak_ptr<POINT>& center = std::shared_ptr<POINT>(nullptr)	// ���S�_
		);

	// �摜���ȊO�����l
	CharBase(const BaseData& bd);

	/*
		@brief	�摜���̏㏑��
		@param	[in]	bd	�摜���
		@return	�Ȃ�
	*/
	void SetImgData(const BaseData& bd);

	/*
		@brief	�`�掞��(�g�嗦��K�p����)�T�C�Y�̎擾
		@return	�`��T�C�Y
	*/ 
	mymath::Vec3f GetSize() const;
	/*
		@brief	�����̕��̎擾
		@return	�����̕�
	*/
	float HalfWidth() const;
	/*
		@brief	�����̍����̎擾
		@return	�����̍���
	*/
	float HalfHeight() const;
	/*
		@brief	�`�掞�̔����̃T�C�Y�̎擾
		@return	�`�掞�̔����̃T�C�Y
	*/
	mymath::Vec3f HalfSize() const;

	enum MODE{Center,LeftTop};
	/*
		@brief	�����`�̎擾
		@param	[in]	mode	���W�𒆐S�Ƃ��邩�A����Ƃ��邩(�f�t�H���g:Center)
		@return	�����`
	*/
	mymath::Rectf GetRect(MODE mode = Center) const;
	/*
		@brief	�摜�̕`��
		@param	[in]	mode		���W�𒆐S�Ƃ��邩�A����Ƃ��邩(�f�t�H���g:Center)
		@param	[in]	turnFlag	���]�`�悷�邩(�f�t�H���g:false)
		@return	�Ȃ�
	*/
	void draw(MODE mode = Center, bool turnFlag = false) const;
	/*
		@brief	�摜�̕`�� �J�������W�ɉe������Ȃ�
		@param	[in]	mode		���W�𒆐S�Ƃ��邩�A����Ƃ��邩(�f�t�H���g:Center)
		@param	[in]	turnFlag	���]�`�悷�邩(�f�t�H���g:false)
		@return	�Ȃ�
	*/
	void drawNC(MODE mode = Center, bool turnFlag = false) const;
	/*
		@brief	�摜�̕`��
				�摜�̒��o���@�����W�Ǘ��ɂȂ�
		@param	[in]	mode		���W�𒆐S�Ƃ��邩�A����Ƃ��邩(�f�t�H���g:Center)
		@param	[in]	turnFlag	���]�`�悷�邩(�f�t�H���g:false)
		@return	�Ȃ�
	*/
	void draw2(MODE mode = Center, bool turnFlag = false) const;
	/*
		@brief	�摜�̕`�� �J�������W�ɉe������Ȃ�
				�摜�̒��o���@�����W�Ǘ��ɂȂ�
		@param	[in]	mode		���W�𒆐S�Ƃ��邩�A����Ƃ��邩(�f�t�H���g:Center)
		@param	[in]	turnFlag	���]�`�悷�邩(�f�t�H���g:false)
		@return	�Ȃ�
	*/
	void draw2NC(MODE mode = Center, bool turnFlag = false) const;

	/*
		@brief	�ړ�
		@param	[in] b3D	3D�ړ����邩(�f�t�H���g:false)
		@return	�Ȃ�
	*/
	void Move(bool b3D = false);					// �ړ�

	/*
		@brief	�g�p������Ԃ�
		@return	�g�p����
		@retval	true	�g�p��
		@retval	false	���g�p
	*/
	bool CheckUse() const;

	/*
		@brief	�g�p�t���O��ݒ�
		@param	[in] useFlag	�g�p�t���O
		@return	�Ȃ�
	*/
	void SetUse(bool useFlag);


};

// �A�j���[�V�����I�u�W�F�N�g
class AnimObject
{
public:
	enum class AnimType
	{
		MOVE_X,		// �ړ�
		MOVE_Y,		// �ړ�
		ROTATE,		// ��]
		ANIM_X,		// �摜�A�j���[�V����
		ANIM_Y,		// �摜�A�j���[�V����
	}animType;
	charabase::CharBase obj;		// �I�u�W�F�N�g
	charabase::Anim anim;			// �A�j���[�V�����p
	std::vector<float> animTbl;		// �A�j���[�V�����e�[�u��
	/*
		@brief	�A�j���[�V�����X�V
		@return	�Ȃ�
	*/
	void step();
};

#ifdef DEF_CHARBASE_EXT
#include <wtypes.h>	//  RECT�p

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