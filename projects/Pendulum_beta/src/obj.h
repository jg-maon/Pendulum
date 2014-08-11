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
		run,		// step,draw�����s
		idle,		// step,draw�����s���Ȃ�
		update,		// step�̂ݎ��s
		disp,		// draw�̂ݎ��s
		destroy,	// �폜
	};
	typedef std::vector<mymath::ShapefPtr> Collisions;		// �����蔻��̈�
	typedef std::vector<Collisions> MotionCollisions;		// ���[�V�������̓����蔻��̈�
private:
	//�Q�[���}�l�[�W�����������߂̃N���X
	static CGameManager* gm_;
protected:
	std::string name_;
	Status status_;
protected:
	/*
		@brief	�Q�[���}�l�[�W���̎擾
		@return	�Q�[���}�l�[�W��
	*/
	CGameManager* gm() const;
	/*
		@brief	�Q�[���}�l�[�W���̐ݒ�
		@param	[in]	gm	�Q�[���}�l�[�W��
		@return	�Ȃ�
	*/
	void gm(CGameManager* gm);
public:
	/*
		@brief	�I�u�W�F�N�g�̐���
		@param	[in]	name	���ʖ�
	*/
	Base(const std::string& name);
	/*
		@brief	�C���^�[�t�F�[�X�����邽�߂̏������z�֐�
	*/
	virtual ~Base() = 0;
	/*
		@brief	�I�u�W�F�N�g�������Ώۂ��擾
		@return	�I�u�W�F�N�g�̏����Ώۏ��
		@retval	true	�����Ώ�
		@retval	false	�����ΏۂłȂ�
	*/
	bool isDestroy() const;
	
	
	/*
		@brief	�I�u�W�F�N�g���̌���
		@return	�w�肵�����O���I�u�W�F�N�g���Ɋ܂܂�Ă��邩
		@retval	true	�܂܂�Ă���
		@retval	false	�܂܂�Ă��Ȃ�
	*/
	bool FindName(const std::string& name) const;
	/*
		@brief	�I�u�W�F�N�g�̎��ʖ����擾
		@return	�I�u�W�F�N�g���ʖ�
	*/
	const std::string& getName() const;
	/*
		@brief	�X�V����
		@return	�Ȃ�
	*/
	virtual void step() = 0;
	/*
		@brief	�`�揈��
		@return	�Ȃ�
	*/
	virtual void draw() = 0;
	/*
		@brief	�I�u�W�F�N�g������
		@return	�Ȃ�
	*/
	virtual void kill();
	/*
		@brief	�I�u�W�F�N�g���n��������
		@return	�Ȃ�
	*/
	virtual void start();
	/*
		@brief	�I�u�W�F�N�g���~������
		@return	�Ȃ�
	*/
	virtual void stop();

	/*
		@brief	�I�u�W�F�N�g��`��݂̂ɐݒ肷��
		@return	�Ȃ�
	*/
	virtual void SetStatusDisp();

	/*
		@brief	�I�u�W�F�N�g�̏�Ԃ�ύX����
		@param	[in]	status	���
		@return	�Ȃ�
	*/
	//void setStatus(Status status);
	
	/*
		@brief	��Ԃ̎擾
		@return	�I�u�W�F�N�g�̏��
	*/
	Status getStatus() const;

	/*
		@brief	�I�u�W�F�N�g���m�̏d�Ȃ菈��
		@param	[in]	rival	�d�Ȃ��Ă��鑊��I�u�W�F�N�g
		@return	�Ȃ�
	*/
	virtual void hit(const std::shared_ptr<Base>& rival);
	/*
		@brief	���[���h���W�̐H�炢����̈�̎擾
		@return	���[���h���W�̓����蔻��̈�
	*/
	virtual Collisions GetDamageAreas() const;
	/*
		@brief	���[���h���W�̃X�e�[�W�����蔻��̈�̎擾
		@return	���[���h���W�̃X�e�[�W�����蔻��̈�
	*/
	virtual Collisions GetStageCollisions() const;

};
typedef std::shared_ptr<Base> ObjPtr;

/*
	@brief	Base::gm�������p�I�u�W�F�N�g
*/
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
private:
protected:
	bool turnFlag_;				// ���]�`�悷�邩
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
		@brief	�X�V�������邱�Ƃ��ł��邩
		@return	�X�V���������邩
		@retval	true	�X�V
		@retval	false	�X�V�ł��Ȃ�
	*/
	bool isUpdatable() const;

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
	/*
		@brief	�I�u�W�F�N�g�̐���
		@param	[in]	name	���ʖ�
	*/
	IObject(const std::string& name);
	/*
		@brief	�C���^�[�t�F�[�X�����邽�߂̏������z�֐�
	*/
	virtual ~IObject() = 0;
	
	/*
		@brief	�X�V����
		@return	�Ȃ�
	*/
	virtual void step() = 0;
	/*
		@brief	�`�揈��
		@return	�Ȃ�
	*/
	virtual void draw();
	/*
		@brief	�I�u�W�F�N�g������
		@return	�Ȃ�
	*/
	virtual void kill();
	/*
		@brief	�I�u�W�F�N�g���m�̏d�Ȃ菈��
		@param	[in]	rival	�d�Ȃ��Ă��鑊��I�u�W�F�N�g
		@return	�Ȃ�
	*/
	virtual void hit(const std::shared_ptr<Base>& rival);
	/*
		@brief	���[���h���W�̐H�炢����̈�̎擾
		@return	���[���h���W�̓����蔻��̈�
	*/
	virtual Collisions GetDamageAreas() const;
	/*
		@brief	���[���h���W�̃X�e�[�W�����蔻��̈�̎擾
		@return	���[���h���W�̃X�e�[�W�����蔻��̈�
	*/
	virtual Collisions GetStageCollisions() const;
	/*
		@brief	�摜�I�u�W�F�N�g�̎擾
		@return	�摜�I�u�W�F�N�g
	*/
	const charabase::CharBase& obj() const;
	/*
		@brief	�摜�I�u�W�F�N�g�̐ݒ�
		@return	�Ȃ�
	*/
	void obj(const charabase::CharBase& o);

	/*
		@brief	�摜���]�`�悳���邩�̃t���O��Ԃ�
		@return	���]�`��t���O
		@retval	true	���]�`�悷��
		@retval	false	���]�`�悵�Ȃ�
	*/
	bool isTurn() const;
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
	Collisions collisions_;			// �H�炢����̈�
	Collisions stageCollisions_;	// �X�e�[�W�Ƃ̓����蔻��̈�
protected:
	/*
		@brief	�����蔻��̈�̍폜
		@return �Ȃ�
	*/
	void ClearCollisions();

	
	/*
		@brief	�����蔻��̊i�[
		@attension	ifstream��currentPosition�ɒ���
					if(FindChunk(f,"#Collision"))
						LoadCollisions(f, collisions_);
					if(FindChunk(f,"#StageCollision"))
						LoadCollisions(f, stageCollisions_);
		@param	[in/out]	f			�I�[�v���ς݃t�@�C��
		@param	[out]		collisions	������i�[����z��
		@return		EOF��
		@retval		true		EOF
		@retval		false		EOF�łȂ�
	*/
	bool LoadCollisions(std::ifstream& f, Collisions& collisions);


	/*
		@brief	���[���h���W�ɕϊ����������蔻��̈�̎擾
		@param	[in]	collisions	���[�J�������蔻��̈�
		@return	���[���h���W�̓����蔻��̈�
	*/
	Collisions GetWorldCollisions(const Collisions& collisions) const;

public:
	/*
		@brief	�I�u�W�F�N�g�̐���
		@param	[in]	name	���ʖ�
	*/
	IColObject(const std::string& name);
	/*
		@brief	�C���^�[�t�F�[�X�����邽�߂̏������z�֐�
	*/
	virtual ~IColObject() = 0;
	/*
		@brief	�X�V����
		@return	�Ȃ�
	*/
	virtual void step() = 0;
	/*
		@brief	�`�揈��
		@return	�Ȃ�
	*/
	virtual void draw() = 0;

	/*
		@brief	�I�u�W�F�N�g���m�̏d�Ȃ菈��
		@param	[in]	rival	�d�Ȃ��Ă��鑊��I�u�W�F�N�g
		@return	�Ȃ�
	*/
	virtual void hit(const std::shared_ptr<Base>& rival);
	
	/*
		@brief	�����蔻��̊i�[
		@attension	ifstream��currentPosition�ɒ���
					if(FindChunk(f,"#Collision"))
						LoadCollisions(f);
		@param	[in/out]	f			�I�[�v���ς݃t�@�C��
		@return		EOF��
		@retval		true		EOF
		@retval		false		EOF�łȂ�
	*/
	bool LoadCollisions(std::ifstream& f);
	
	/*
		@brief	�X�e�[�W�����蔻��̊i�[
		@attension	ifstream��currentPosition�ɒ���
					if(FindChunk(f,"#StageCollision"))
						LoadStageCollisions(f);
		@param	[in/out]	f			�I�[�v���ς݃t�@�C��
		@return		EOF��
		@retval		true		EOF
		@retval		false		EOF�łȂ�
	*/
	bool LoadStageCollisions(std::ifstream& f);


	/*
		@brief	���[���h���W�̐H�炢����̈�̎擾
		@return	���[���h���W�̓����蔻��̈�
	*/
	virtual Collisions GetDamageAreas() const override;
	/*
		@brief	���[���h���W�̃X�e�[�W�����蔻��̈�̎擾
		@return	���[���h���W�̃X�e�[�W�����蔻��̈�
	*/
	virtual Collisions GetStageCollisions() const override;

	/*
		@brief	���[�J�����W�̓����蔻��̈�̑��
		@param	[in]	collisions	�V�������[�J�������蔻��̈�
		@return	�Ȃ�
	*/
	void SetCollisionAreas(const Collisions& collisions);

	/*
		@brief	���[�J�����W�̓����蔻��̈�̑��
		@param	[in]	obj	�V���������蔻��̈���������I�u�W�F�N�g
		@return	�Ȃ�
	*/
	void SetCollisionAreas(const IColObject& obj);

	/*
		@brief	���[�J�����W�̃X�e�[�W�����蔻��̈�̑��
		@param	[in]	collisions	�V�����X�e�[�W�����蔻��̈�
		@return	�Ȃ�
	*/
	void SetStageCollisions(const Collisions& collisions);

	/*
		@brief	�ΏۃI�u�W�F�N�g����X�e�[�W�����蔻��̈���R�s�[����
		@param	[in]	obj	�R�s�[���铖���蔻��̈���������I�u�W�F�N�g
		@return	�Ȃ�
	*/
	void SetStageCollisions(const IColObject& obj);

};



#endif