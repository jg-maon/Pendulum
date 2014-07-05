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
IScene()
{
	//InsertObject(ObjPtr(new CCollision()));

	// ���C�����Ŏg���I�u�W�F�N�g���n��������
	// �v���C���[��G��Stage�N���X������Ă����
	auto& objs = gm()->GetObjects("StageMng Collision", ' ');
	for (auto& obj : objs)
		obj->start();

	// �X�e�[�W�}�l�[�W�����A���݃X�e�[�W��BGM�����擾
	auto& sm = std::dynamic_pointer_cast<CStageMng>(gm()->GetObj(typeid(CStageMng)));
	bgmResname_ = sm->getStageBGM();
	

	start();
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
	{
		auto& objs = gm()->GetObjects("StageMng Collision Player EnemyMng", ' ');
		for (auto& obj : objs)
			obj->stop();
	}
	// �������U������������
	{
		auto& objs = gm()->GetObjects("Atk_");
		for (auto& obj : objs)
			obj->kill();
	}

	{
		// �X�R�A�}�l�[�W���ɃX�R�A��ǉ�
		auto& sm = std::dynamic_pointer_cast<CScoreMng>(gm()->GetObj(typeid(CScoreMng)));
		sm->score(100);
	}
	return new CSceneEnd();
}



#pragma endregion
// CSceneMain methods

