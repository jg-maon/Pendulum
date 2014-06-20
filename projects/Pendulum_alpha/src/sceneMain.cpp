#include "sceneMain.h"
#include "define.h"

//#include "Collision.h"		// Collision���o�p
#include "enemyMng.h"
#include "setting.h"

#include "Fade.h"

#include "sceneEnd.h"		// �V�[���J��



//======================================
// CSceneMain methods
#pragma region
// �R���X�g���N�^
CSceneMain::CSceneMain():
IScene("","bgm_main")
{
	//InsertObject(ObjPtr(new CCollision()));

	// ���C�����Ŏg���I�u�W�F�N�g���n��������
	auto& objs = gm()->GetObjects("StageMng Collision Player EnemyMng", ' ');
	for (auto& obj : objs)
		obj->start();

}
CSceneMain::~CSceneMain()
{
}

// �`��
void CSceneMain::draw()
{
	// �������Ȃ�(�X�e�[�W�}�l�[�W���[��draw�ɔC��
}

// ����
bool CSceneMain::update()
{
	// �����A�N�V�������N�����ăV�[�����؂�ւ��Ƃ�
	if (input::CheckPush(input::KEY_BTN0))
	{
		return true;
	}
	return false;
}

IScene* CSceneMain::NextScene()
{
	// ���C�����Ŏg���I�u�W�F�N�g���~������
	auto& objs = gm()->GetObjects("StageMng Collision", ' ');
	for (auto& obj : objs)
		obj->stop();
	return new CSceneEnd();
}



#pragma endregion
// CSceneMain methods

