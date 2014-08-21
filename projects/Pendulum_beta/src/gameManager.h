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

#ifndef DEF_SCENEMNG_H
#include "sceneMng.h"
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
#include "stageMng.h"
#endif

#ifndef DEF_SCOREMNG_H
#include "scoreMng.h"
#endif

#ifndef DEF_GAMESTATUS_H
#include "gameStatus.h"
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
	typedef charabase::AnimObject Cursor;
	
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
	std::weak_ptr<CPlayer> pPlayer_;			// �v���C���[�|�C���^
	std::weak_ptr<CEnemyMng> pEnemyMng_;		// �G�l�~�[�}�l�[�W��
	std::weak_ptr<CStageMng> pStageMng_;		// �X�e�[�W�}�l�[�W��
	std::weak_ptr<CScoreMng> pScoreMng_;		// �X�R�A�}�l�[�W��
	std::weak_ptr<CGameStatus> pGameStatus_;	// �Q�[���X�e�[�^�X
	//*/
#endif
	//=============================================================

	/*
	//�Q�[���N���A��Ԃ𔭓�����B
	bool clear_;
	float count_;
	//*/

	bool showCursor_;					// true:�}�E�X�J�[�\���̕\��

	bool isCursorSyncMouse_;			// �}�E�X�J�[�\�����W�ɃJ�[�\�������킹�邩

	enum CursorSize
	{
		width = 64,
		height = 64,
	};

	
	Cursor cursor_;

	
	std::shared_ptr<CFileMng> fileMng_;					// �t�@�C���}�l�[�W��

	std::unique_ptr<CSceneMng> sceneMng_;				// �V�[���}�l�[�W��

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
	ObjPtr GetObj(const type_info& objinfo) const;

	/*
		@brief	�ǉ�����Ă���S�I�u�W�F�N�g���擾
		@return	�I�u�W�F�N�g�Q
	*/
	std::vector<ObjPtr>& GetObjects();

	/*
		@brief	�^����ꂽ���O�̃I�u�W�F�N�g���擾
				GetObjects("Collision");
		@param	[in] taskName	�T���I�u�W�F�N�g��
		@return	�I�u�W�F�N�g�Q
	*/
	std::vector<ObjPtr> GetObjects(const std::string& taskName) const;
	/*
		@brief	�^����ꂽ���O�̃I�u�W�F�N�g���擾
				GetObjects("Player,Enemy", ",");
		@param	[in] taskName	�T���I�u�W�F�N�g��
		@param	[in] delim		��؂蕶��
		@return	�I�u�W�F�N�g�Q
	*/
	std::vector<ObjPtr> GetObjects(const std::string& taskName, const char delim) const;
	
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
		@brief	�J�[�\�������t���O�ݒ�
		@param	[uin]	sync	�����t���O
		@return	�Ȃ�
	*/
	void SetCursorSynchronicity(bool sync);

	/*
		@brief	�J�[�\���̍��W���擾
		@return	�J�[�\�����W
	*/
	const mymath::Vec3f& GetCursorPos() const;
	
	/*
		@brief		�J�[�\���̍��W�̐ݒ�
		@attension	�����t���O�������I�ɐ܂���
		@param	[in]	pos	�J�[�\�����W
		@return	�Ȃ�
	*/
	void SetCursorPos(const mymath::Vec3f& pos);

	/*
		@brief	�J�[�\���̃X�N���[�����W���擾
		@return	�J�[�\���X�N���[�������W
	*/
	mymath::Vec3f GetCursorPosNC() const;

	/*
		@brief	�J�[�\���I�u�W�F�N�g�̎擾
		@return	�J�[�\���I�u�W�F�N�g
	*/
	const Cursor& cursor() const;

	/*
		@brief	�}�E�X�J�[�\���I�u�W�F�N�g�̐ݒ�
		@param	[in]	newCursor	�V�����J�[�\���I�u�W�F�N�g
		@return	�Ȃ�
	*/
	void cursor(const Cursor& newCursor);


	/*
		@brief	�v���C���[���W�̎擾
		@return	�v���C���[�I�u�W�F�N�g�̍��W
	*/
	mymath::Vec3f GetPlayerPos() const;

	/*
		@brief	�t�@�C���}�l�[�W���̎擾
		@return	�t�@�C���}�l�[�W��
	*/
	std::shared_ptr<CFileMng> fm();

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
	void SetStageMngPtr(const std::weak_ptr<CStageMng>& stagemng);

	/*
		@brief	�X�R�A�}�l�[�W���|�C���^�̐ݒ�
		@param	[in]	scoremng	�X�R�A�}�l�[�W��
		@return	�Ȃ�
	*/
	void SetScoreMngPtr(const std::weak_ptr<CScoreMng>& scoremng);

	/*
		@brief	�Q�[���X�e�[�^�X
		@param	[in]	gamestatus	�Q�[���X�e�[�^�X
		@return	�Ȃ�
	*/
	void SetGameStatusPtr(const std::weak_ptr<CGameStatus>& gamestatus);



	/*
		@brief	�v���C���[�I�u�W�F�N�g�̎擾
		@return	�v���C���[�I�u�W�F�N�g
	*/
	std::shared_ptr<CPlayer> GetPlayer() const;

	/*
		@brief	�G�l�~�[�}�l�[�W���̎擾
		@return	�G�l�~�[�}�l�[�W��
	*/
	std::shared_ptr<CEnemyMng> enemyMng() const;

	
	/*
		@brief	�X�e�[�W�}�l�[�W���̎擾
		@return	�X�e�[�W�}�l�[�W��
	*/
	std::shared_ptr<CStageMng> stageMng() const;
	
	/*
		@brief	�X�R�A�}�l�[�W���̎擾
		@return	�X�R�A�}�l�[�W��
	*/
	std::shared_ptr<CScoreMng> scoreMng() const;

	/*
		@brief	�Q�[���X�e�[�^�X�̎擾
		@return	�Q�[���X�e�[�^�X
	*/
	std::shared_ptr<CGameStatus> gameStatus() const;

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
		return fileMng_->GetData(obj);
	}

};

//extern void InsertObject(const ObjPtr& obj);


#endif

