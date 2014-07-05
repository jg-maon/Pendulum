#ifndef DEF_GAMEMANAGER_H
#define DEF_GAMEMANAGER_H

#define DEF_GM_PTR		// �悭�g���|�C���^��ێ�������

//���C�u����
#ifndef DEF_DEFINE_H
#include "define.h"
using namespace gplib;
#endif

#ifndef DEF_OBJ_H
#include "obj.h"
#endif

#ifndef DEF_CHARBASE_H
#include "charBase.h"
#endif

#ifndef DEF_FILEMNG_H
#include "fileMng.h"
#endif

#ifdef DEF_GM_PTR
//*
#ifndef DEF_PLAYER_H
#include "player.h"
#endif

#ifndef DEF_ENEMYMNG_H
#include "enemyMng.h"
#endif

#ifndef DEF_STAGEMNG_H
//#include "stageMng.h"
#endif

#ifndef DEF_SCOREMNG_H
#include "scoreMng.h"
#endif
//*/
#endif

#include	<vector>
#include	<typeinfo>

//-------------------------------------------------
//�Q�[���̊Ǘ����s���B
//Object���p�����Ă���N���X�͑S�ĊǗ������B
class CGameManager : public Base
{
private:
	static mymath::Recti* winRect_;		// �E�B���h�E�T�C�Y

	//�o�����̃I�u�W�F�N�g���Ǘ�
	std::vector<ObjPtr>	objs_;
	//�ǉ��\��̃I�u�W�F�N�g���Ǘ�
	std::vector<ObjPtr>	addObjs_;

	//============================================================
	// �悭�g���I�u�W�F�N�g�͕ʂŊm�ۂ��Ă���
#ifdef DEF_GM_PTR
	//*
	std::weak_ptr<CPlayer> pPlayer_;		// �v���C���[�|�C���^
	std::weak_ptr<CEnemyMng> pEnemyMng_;	// �G�l�~�[�}�l�[�W��
	//std::weak_ptr<CStageMng> pStageMng_;	// �X�e�[�W�}�l�[�W��
	std::weak_ptr<CScoreMng> pScoreMng_;	// �X�R�A�}�l�[�W��
	//*/
#endif
	//=============================================================

	/*
	//�Q�[���N���A��Ԃ𔭓�����B
	bool clear_;
	float count_;
	//*/

	bool showCursor_;					// true:�J�[�\���̕\��

	enum CursorSize
	{
		width = 48,
		height = 48,
	};
	charabase::CharPtr cursor_;			// �J�[�\��
	//charabase::Anim cursorAnim_;		// �J�[�\���A�j���[�V�����p

	
	CFileMng fileMng_;					// �t�@�C���}�l�[�W��

private:

	/*
		@brief	�I�u�W�F�N�g�̒ǉ��A�폜���s��
		@return	�Ȃ�
	*/
	void MargeObjects();

public:

	CGameManager();
	~CGameManager();
	/*
		@brief	������	
		@return	�Ȃ�
	*/
	void init();

	/*
		@brief	�Q�[���}�l�[�W���������A�N��
		@return	�Ȃ�
	*/
	void start() override;

	/*
		@brief	�S�I�u�W�F�N�g�̍X�V����
		@return	�Ȃ�
	*/
	void step() override;
	/*
		@brief	�S�I�u�W�F�N�g�̕`��
		@return	�Ȃ�
	*/
	void draw() override;

	/*
		@brief	�S�I�u�W�F�N�g��ҋ@��Ԃɂ���
		@return	�Ȃ�
	*/
	void AllStop();

	/*
		@brief		�I�u�W�F�N�g�̒ǉ�
		@attention	�I�u�W�F�N�g�����ۂɒǉ������̂�1�t���[����
		@parama		[in]	obj	�ǉ�����I�u�W�F�N�g
		@return		�Ȃ�
	*/
	void AddObject(const ObjPtr& obj);
	/*
		@brief	�I�u�W�F�N�g�̑����ǉ�
		@parama	[in]	obj	�ǉ�����I�u�W�F�N�g
		@return	�Ȃ�
	*/
	void AddObject2(const ObjPtr& obj);
	
	/*
		@brief	�w��̃N���X�������o���ĕԂ�
		@param	[in]	objinfo	�N���X��ID(typeid)
		@return	�I�u�W�F�N�g
	*/
	ObjPtr GetObj(const type_info& objinfo);

	/*
		@brief	�ǉ�����Ă���S�I�u�W�F�N�g���擾
		@return	�I�u�W�F�N�g�Q
	*/
	std::vector<ObjPtr>& GetObjects();

	/*
		@brief	�^����ꂽ���O�̃I�u�W�F�N�g���擾
				GetObjects("Collision");
		@param	[in] taskName	�T���I�u�W�F�N�g��(�O����v)
		@return	�I�u�W�F�N�g�Q
	*/
	std::vector<ObjPtr> GetObjects(const std::string& taskName);
	/*
		@brief	�^����ꂽ���O�̃I�u�W�F�N�g���擾
				GetObjects("Player,Enemy", ",");
		@param	[in] taskName	�T���I�u�W�F�N�g��(�O����v)
		@param	[in] delim		��؂蕶��
		@return	�I�u�W�F�N�g�Q
	*/
	std::vector<ObjPtr> GetObjects(const std::string& taskName, const char delim);
	
	/*
		@brief	�I�u�W�F�N�g�̑S����
		@return	�Ȃ�
	*/
	void ClearObjects();



	/*
	void setClear(bool clear);
	bool getClear() const;
	void StartClear();
	void ClearToChangeScreen(int next);
	//*/

	/*
		@brief	�E�B���h�E�T�C�Y�̎擾
		@return	�E�B���h�E�T�C�Y
	*/
	const mymath::Recti* winRect() const;
	/*
		@brief	�V�����E�B���h�E�T�C�Y�̑��
		@param	[in]	newRect	�V�����E�B���h�E�T�C�Y
		@return	�Ȃ�
	*/
	void winRect(const mymath::Recti& newRect);
	/*
		@brief	�E�B���h�E�T�C�Y�̎Q�Ɛ��ύX
		@param	[in]	newRect	�V�����E�B���h�E�T�C�Y�Q�Ɛ�
		@return	�Ȃ�
	*/
	void winRect(mymath::Recti* newRect);

	/*
		@brief	�}�E�X�J�[�\���̍��W���擾
		@return	�}�E�X�J�[�\�����W
	*/
	const mymath::Vec3f& GetCursorPos() const;

	/*
		@brief	�v���C���[���W�̎擾
		@return	�v���C���[�I�u�W�F�N�g�̍��W
	*/
	mymath::Vec3f GetPlayerPos() const;

	/*
		@brief	�t�@�C���}�l�[�W���̎擾
		@return	�t�@�C���}�l�[�W��
	*/
	CFileMng& fm();

#ifdef DEF_GM_PTR
	/*
		@brief	�v���C���[�|�C���^�̐ݒ�
		@param	[in]	player	�v���C���[
		@return	�Ȃ�
	*/
	void SetPlayerPtr(const std::weak_ptr<CPlayer>& player);

	/*
		@brief	�G�l�~�[�}�l�[�W���|�C���^�̐ݒ�
		@param	[in]	enemymng	�G�l�~�[�}�l�[�W��
		@return	�Ȃ�
	*/
	void SetEnemyMngPtr(const std::weak_ptr<CEnemyMng>& enemymng);

	/*
		@brief	�X�e�[�W�}�l�[�W���|�C���^�̐ݒ�
		@param	[in]	stagemng	�X�e�[�W�}�l�[�W��
		@return	�Ȃ�
	*/
	//void SetStageMngPtr(const std::weak_ptr<CStageMng>& stagemng);

	/*
		@brief	�X�R�A�}�l�[�W���|�C���^�̐ݒ�
		@param	[in]	scoremng	�X�R�A�}�l�[�W��
		@return	�Ȃ�
	*/
	void SetScoreMngPtr(const std::weak_ptr<CScoreMng>& scoremng);

	/*
		@brief	�X�e�[�W�}�l�[�W���̎擾
		@return	�X�e�[�W�}�l�[�W��
	*/
	//CStageMng& stageMng();

	/*
		@brief	�X�R�A�}�l�[�W���̎擾
		@return	�X�R�A�}�l�[�W��
	*/
	CScoreMng& scoreMng();
#endif


	/*
		@brief	���̎擾
				gm()->GetData(*this);
		@param	[in/out]	obj	�i�[����N���X�I�u�W�F�N�g
		@return	����ݒ�ł�����
		@retval	true	�ݒ萬��
		@retval	false	�ݒ莸�s
	*/
	template<class OBJ>	bool GetData(OBJ& obj) const
	{
		return fileMng_.GetData(obj);
	}

};

//extern void InsertObject(const ObjPtr& obj);


#endif

