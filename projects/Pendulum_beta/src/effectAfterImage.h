#ifndef DEF_EFFECTAFTERIMAGE_H
#define DEF_EFFECTAFTERIMAGE_H

#ifndef DEF_EFFECTBASE_HPP
#include "effectBase.hpp"
#endif

#ifndef DEF_MYMATH_HPP
#include "MyMath.hpp"
#endif


/*
	@brief	�c��
*/
class CEffectAfterImage : public IEffect
{
private:
	/*
		@brief	�������̃I�u�W�F�N�g
	*/
	class CAfterImageObject : public IObject
	{
	private:
		float addScale_;	// �g�k��
		float decrease_;	// ������
		int renderMode_;	// �`����@
	public:
		CAfterImageObject() :
			IObject("AfterImageObject")
		{}
		/*
			@brief	CharBase�ꎞ�ϊ��p
		*/
		CAfterImageObject(const charabase::CharBase& obj, bool turnFlag) :
			IObject("AfterImageObject")
		{
			obj_ = obj;
			turnFlag_ = turnFlag;
		}

		/*
			@brief	vector�o�^�p
		*/
		CAfterImageObject(const std::weak_ptr<IObject>& obj, bool turnFlag, float dec, int renderMode, float scale) :
			IObject("AfterImageObject")
			, decrease_(dec)
			, renderMode_(renderMode)
			, addScale_(scale)
		{
			obj_ = obj.lock()->obj();
			turnFlag_ = turnFlag;
		}

		void step() override
		{
			__super::step();
			obj_.scale += addScale_;
			obj_.alpha -= decrease_;
			if (obj_.alpha < 0.f)
				obj_.SetUse(false);
		}
		void draw() override
		{
			graph::Draw_SetRenderMode(renderMode_);
			__super::draw();
			graph::Draw_EndRenderMode();
		}
	};
private:
	std::vector<CAfterImageObject> images_;	// �c��

	/*
		@brief	�G�t�F�N�g����
		@param	[in]	obj			�c���p���I�u�W�F�N�g
		@param	[in]	sta			�n�_���S���W
		@param	[in]	end			�I�_���S���W
		@param	[in]	num			��Ԑ�
		@param	[in]	renderMode	�`����@
		@param	[in]	alpha		�ő�s�����x
		@param	[in]	time		�ő�s�����x��0�ɂȂ�܂ł̎���
		@param	[in]	scale		�ŏI�g�嗦
	*/
	void Create(const std::weak_ptr<IObject>& obj, const mymath::Vec3f& sta, const mymath::Vec3f& end, int num, int renderMode, float alpha, float time, float scale);


public:
	/*
		@brief	�G�t�F�N�g����
		@param	[in]	obj			�c���p���I�u�W�F�N�g
		@param	[in]	turnFlag	�I�u�W�F�N�g���]�t���O
		@param	[in]	sta			�n�_���S���W
		@param	[in]	end			�I�_���S���W
		@param	[in]	num			��Ԑ�
		@param	[in]	renderMode	�`����@(�f�t�H���g:�������`��)
		@param	[in]	alpha		�ő�s�����x(�f�t�H���g:220.f)
		@param	[in]	time		�ő�s�����x��0�ɂȂ�܂ł̎���(�f�t�H���g:1�b�ŏ���)
		@param	[in]	scale		�ŏI�g�嗦(�f�t�H���g1.f)		
	*/
	CEffectAfterImage(const charabase::CharBase& obj, bool turnFlag, const mymath::Vec3f& sta, const mymath::Vec3f& end, int num, int renderMode = gplib::RENDER_MODE::NORMAL, float alpha = 220.f, float time = 1.f, float scale = 1.f);
	/*
		@brief	�G�t�F�N�g����
		@param	[in]	obj			�c���p���I�u�W�F�N�g
		@param	[in]	sta			�n�_���S���W
		@param	[in]	end			�I�_���S���W
		@param	[in]	num			��Ԑ�
		@param	[in]	renderMode	�`����@(�f�t�H���g:�������`��)
		@param	[in]	alpha		�ő�s�����x(�f�t�H���g:220.f)
		@param	[in]	time		�ő�s�����x��0�ɂȂ�܂ł̎���(�f�t�H���g:1�b�ŏ���)
		@param	[in]	scale		�ŏI�g�嗦(�f�t�H���g1.f)
	*/
	CEffectAfterImage(const std::weak_ptr<IObject>& obj, const mymath::Vec3f& sta, const mymath::Vec3f& end, int num, int renderMode = gplib::RENDER_MODE::NORMAL, float alpha = 220.f, float time = 1.f, float scale = 1.f);

	void step() override;
	void draw() override;

};


#endif