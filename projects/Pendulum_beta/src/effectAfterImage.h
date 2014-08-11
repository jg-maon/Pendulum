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
		float decrease_;	// ������
	public:
		CAfterImageObject() :
			IObject("AfterImageObject")
		{}
		CAfterImageObject(const charabase::CharBase& obj, bool turnFlag) :
			IObject("AfterImageObject")
		{
			obj_ = obj;
			turnFlag_ = turnFlag;
		}
		CAfterImageObject(const std::weak_ptr<IObject>& obj, bool turnFlag, float dec) :
			IObject("AfterImageObject")
			, decrease_(dec)
		{
			obj_ = obj.lock()->obj();
			turnFlag_ = turnFlag;
		}

		void step() override
		{
			__super::step();
			obj_.alpha -= decrease_;
			if (obj_.alpha < 0.f)
				obj_.SetUse(false);
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
		@param	[in]	alpha		�ő�s�����x(�f�t�H���g:220.f)
		@param	[in]	dec			������(�f�t�H���g:1�b�ŏ���)
	*/
	void Create(const std::weak_ptr<IObject>& obj, const mymath::Vec3f& sta, const mymath::Vec3f& end, int num, float alpha = 220.f, float dec = -1.f);


public:
	/*
		@brief	�G�t�F�N�g����
		@param	[in]	obj			�c���p���I�u�W�F�N�g
		@param	[in]	turnFlag	�I�u�W�F�N�g���]�t���O
		@param	[in]	sta			�n�_���S���W
		@param	[in]	end			�I�_���S���W
		@param	[in]	num			��Ԑ�
		@param	[in]	alpha		�ő�s�����x(�f�t�H���g:220.f)
		@param	[in]	dec			������(�f�t�H���g:1�b�ŏ���)
	*/
	CEffectAfterImage(const charabase::CharBase& obj, bool turnFlag, const mymath::Vec3f& sta, const mymath::Vec3f& end, int num, float alpha = 220.f, float dec = -1.f);
	/*
		@brief	�G�t�F�N�g����
		@param	[in]	obj			�c���p���I�u�W�F�N�g
		@param	[in]	sta			�n�_���S���W
		@param	[in]	end			�I�_���S���W
		@param	[in]	num			��Ԑ�
		@param	[in]	alpha		�ő�s�����x(�f�t�H���g:220.f)
		@param	[in]	dec			������(�f�t�H���g:1�b�ŏ���)
	*/
	CEffectAfterImage(const std::weak_ptr<IObject>& obj, const mymath::Vec3f& sta, const mymath::Vec3f& end, int num, float alpha = 220.f, float dec = -1.f);

	void step() override;
	void draw() override;

};


#endif