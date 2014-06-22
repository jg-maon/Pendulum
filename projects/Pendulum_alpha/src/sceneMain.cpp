#include "sceneMain.h"
#include "define.h"

//#include "Collision.h"		// Collision���o�p
#include "enemyMng.h"
#include "setting.h"
#include "scoreMng.h"
#include "Fade.h"

#include "stageMng.h"

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
	// �������Ȃ�(�X�e�[�W�}�l�[�W���[��draw�ɔC����)
}

// ����
bool CSceneMain::update()
{
	const auto& sm = CStageMng::GetPtr();

	// �����A�N�V�������N�����ăV�[�����؂�ւ��Ƃ�
	if (sm->isEndStage())
	{
		return true;
	}
	return false;
}

IScene* CSceneMain::NextScene()
{
	// ���C�����Ŏg�����I�u�W�F�N�g���~������
	auto& objs = gm()->GetObjects("StageMng Collision Player EnemyMng", ' ');
	for (auto& obj : objs)
		obj->stop();
	// �X�R�A�}�l�[�W���ɃX�R�A��ǉ�
	auto& sm = std::dynamic_pointer_cast<CScoreMng>(gm()->GetObj(typeid(CScoreMng)));
	sm->score(100);
	return new CSceneEnd();
}



#pragma endregion
// CSceneMain methods

