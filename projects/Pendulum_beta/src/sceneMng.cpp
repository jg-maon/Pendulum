#include "sceneMng.h"

//------------------------
// �V�[��
#include "sceneJecLogo.h"
#include "sceneTitle.h"
#include "sceneStageSelect.h"
#include "sceneMain.h"
#include "sceneEnd.h"
#include "sceneNameInput.h"
//------------------------

bool IScene::isSoftReset_ = false;
float IScene::resetTime_;
int IScene::bgmVolume_ = 100;
IScene::State IScene::state_;

CSceneMng::CSceneMng() :
Base("SceneMng")
, nowScene_(JEC)
{
	scenes_.resize(SCENE_NUM);
	scenes_[JEC]			= ScenePtr(new CSceneJecLogo());
	scenes_[TITLE]			= ScenePtr(new CSceneTitle());
	scenes_[STAGESELECT]	= ScenePtr(new CSceneStageSelect());
	scenes_[MAIN]			= ScenePtr(new CSceneMain());
	scenes_[END]			= ScenePtr(new CSceneEnd());
	scenes_[NAMEINPUT]		= ScenePtr(new CSceneNameInput());

	scenes_[nowScene_]->start();

}
CSceneMng::~CSceneMng()
{
}

void CSceneMng::step()
{
	scenes_[nowScene_]->step();
	if (IScene::isChangeScene())
	{
		// �Œ��̃V�[���I��
		scenes_[nowScene_]->stop();

		nowScene_ = scenes_[nowScene_]->NextScene();
		
		if (IScene::isSoftReset())	// �\�t�g���Z�b�g
			nowScene_ = JEC;
		
		// ���̃V�[���J�n
		scenes_[nowScene_]->start();
	}
}

void CSceneMng::draw()
{
	scenes_[nowScene_]->draw();
}