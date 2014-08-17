#include "sceneMain.h"
#include "define.h"

//#include "Collision.h"		// Collision���o�p
#include "enemyMng.h"
#include "setting.h"
#include "scoreMng.h"
#include "Fade.h"

#include "stageMng.h"




//======================================
// CSceneMain methods
#pragma region
// �R���X�g���N�^
CSceneMain::CSceneMain() :
IScene("SceneMain")
{
}


CSceneMain::~CSceneMain()
{
}
void CSceneMain::start()
{
	auto cursor = gm()->cursor();
	cursor.obj.SetUse(true);
	cursor.obj.r = 255.f;
	cursor.obj.g = 255.f;
	cursor.obj.b = 0.f;
	gm()->cursor(cursor);

	// ���C�����Ŏg���I�u�W�F�N�g���n��������
	// �v���C���[��G��Stage�N���X������Ă����
	auto& objs = gm()->GetObjects("StageMng Collision ScoreMng", ' ');
	for (auto& obj : objs)
		obj->start();

	// �X�e�[�W�}�l�[�W�����A���݃X�e�[�W��BGM�����擾
	auto& sm = std::dynamic_pointer_cast<CStageMng>(gm()->GetObj(typeid(CStageMng)));
	bgmResname_ = sm->getStageBGM();

	__super::start();
}

// �`��
void CSceneMain::draw()
{
	// �������Ȃ�(�X�e�[�W�}�l�[�W���[��draw�ɔC����)
}

// ����
bool CSceneMain::update()
{
	const auto& sm = gm()->stageMng();

	// �����A�N�V�������N�����ăV�[�����؂�ւ��Ƃ�
	if (sm->isEndStage())
	{
		return true;
	}

	return false;
}

int CSceneMain::NextScene() const
{
	// ���C�����Ŏg�����I�u�W�F�N�g���~������
	{
		auto& objs = gm()->GetObjects("StageMng Collision ScoreMng Player EnemyMng Pickup", ' ');
		for (auto& obj : objs)
			obj->stop();
	}
	// �������U������������
	{
		auto& objs = gm()->GetObjects("Atk_");
		for (auto& obj : objs)
			obj->kill();
	}


	return CSceneMng::Scene::END;
}



#pragma endregion
// CSceneMain methods

