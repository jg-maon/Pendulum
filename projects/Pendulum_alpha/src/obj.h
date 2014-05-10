#ifndef DEF_OBJ_H
#define DEF_OBJ_H

#ifndef DEF_CHARBASE_H
#include "CharBase.h"
#endif

#ifndef DEF_MYMATH_HPP
#include "MyMath.hpp"
#endif

//���C�u����
#include "../../../lib/gplib.h"
using namespace gplib;

//3D�����O�����C�u����
#include "../../../lib/dx_media/dx_graphics3d.h"
#include "../../../lib/dx_media/dx3dmeshmanager.h"

#include <memory>
#include <string>
#include <vector>

#define EraseObject(obj)	((obj).erase( \
								std::remove_if( \
									(obj).begin(), (obj).end(), \
									std::mem_fn(&Base::isDestroy)), \
								(obj).end()))
									
					


class CGameManager;

/*
	@brief	�S�����n���N���X
*/
class Base
{
public:
	enum class Status
	{
		run, destroy
	};
	typedef std::vector<mymath::ShapefPtr> Collisions;
private:
	//�Q�[���}�l�[�W�����������߂̃N���X
	static CGameManager* gm_;
protected:
	std::string name_;
	Status status_;
protected:
	CGameManager* gm() const;
	void gm(CGameManager* gm);
public:
	Base(const std::string& name);
	virtual ~Base() = 0;
	bool isDestroy() const;
	bool FindName(const std::string& name) const;
	virtual void step() = 0;
	virtual void draw() = 0;
	virtual void kill();
	virtual void hit(const std::shared_ptr<Base>& rival);
	virtual Collisions GetCollisionAreas();
};
typedef std::shared_ptr<Base> ObjPtr;

class TempObject : public Base
{
public:
	TempObject(CGameManager *game):
		Base("Temp")
	{
		gm(game);
	}
	~TempObject(){}
	void step() override{};
	void draw() override{};

};

/*
	@brief	�I�u�W�F�N�g���N���X
*/
class IObject : public Base
{
protected:
	charabase::CharBase obj_;
protected:
	/*
		@brief	obj����ʓ������f
		@param	[in]	border	��ʒ[����̗]��
		@return	��ʓ��ɑ��݂��邩
		@retval	true	��ʓ�
		@retval	false	��ʊO
	*/
	bool InScreen(int border = 0) const;
	/*
		@brief		�t�@�C�����琔�l���i�[
					halfWidth�AhalfHeight�Ή�
		@attension	ifstream��currentPosition�ɒ���
		@param		[in/out]	f		���̓t�@�C��
		@param		[in]		obj		�����̃T�C�Y�p
		@param		[out]		value	���ʊi�[�p
		@return		EOF��
		@retval		true		EOF
		@retval		false		EOF�łȂ�
	*/
	template<class T>	bool LoadValue(std::ifstream& f, const charabase::CharBase& obj, T& value);
public:
	IObject(const std::string& name);
	virtual ~IObject() = 0;
	virtual void step() = 0;
	virtual void draw() = 0;
	virtual void kill();
	virtual void hit(const ObjPtr& rival);
	const charabase::CharBase& obj() const;
};

/*
@brief		�t�@�C�����琔�l���i�[
halfWidth�AhalfHeight�Ή�
@attension	ifstream��currentPosition�ɒ���
@param		[in/out]	f		���̓t�@�C��
@param		[in]		obj		�����̃T�C�Y�p
@param		[out]		value	���ʊi�[�p
@return		EOF��
@retval		true		EOF
@retval		false		EOF�łȂ�
*/
template<class T>	bool IObject::LoadValue(std::ifstream& f, const charabase::CharBase& obj, T& value)
{
	std::string buf;
	bool minus = false;	// �}�C�i�X
	f >> buf;
	if(buf[0] == '-')
	{
		buf.erase(buf.begin(),buf.begin()+1);	// -����蕥��
		minus = true;
	}
	// �l���i�[
	if(buf == "halfWidth")
	{
		value = static_cast<T>(obj.HalfWidth());
	}
	else if(buf == "halfHeight")
	{
		value = static_cast<T>(obj.HalfHeight());
	}
	else
	{
		std::stringstream ss(buf);
		ss >> value;
	}
	if(minus)
	{
		value = -value;
	}
	return f.eof();
}


/*
	@brief	�����蔻��t���I�u�W�F�N�g���N���X
*/
class IColObject : public IObject
{
protected:
	Collisions collisions_;		// �����蔻��̈�
public:
	const Collisions& collisions;	// �摜�����蔻��̈�
protected:
	/*
		@brief	�����蔻��̈�̍폜
		@return �Ȃ�
	*/
	void ClearCollisions();
public:
	IColObject(const std::string& name);
	virtual ~IColObject() = 0;
	virtual void step() = 0;
	virtual void draw() = 0;
	virtual void hit(const ObjPtr& rival);
	
	/*
		@brief	�����蔻��̊i�[
		@attension	ifstream��currentPosition�ɒ���
					if(FindChunk(f,"#Collision"))
						LoadCollisions(f);
		@param	[in/out]	f	�I�[�v���ς݃t�@�C��
		@return		EOF��
		@retval		true		EOF
		@retval		false		EOF�łȂ�
	*/
	bool LoadCollisions(std::ifstream& f);

	/*
		@brief	�����蔻��̈�̎擾
		@return	�����蔻��̈�
	*/
	virtual Collisions GetCollisionAreas() const;

	/*
		@brief	�����蔻��̈�̑��
		@param	[in]	collisions	�V���������蔻��̈�
		@return	�Ȃ�
	*/
	void SetCollisionAreas(const Collisions& collisions);
};



#endif