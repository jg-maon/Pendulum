#include "sceneMng.h"

//------------------------
// ƒV[ƒ“
#include "sceneJecLogo.h"
#include "sceneTitle.h"
#include "sceneStageSelect.h"
#include "sceneMain.h"
#include "sceneEnd.h"
#include "sceneNameInput.h"
//------------------------



CSceneMng::CSceneMng() :
Base("SceneMng")
, nowScene_(TITLE)
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
	if (scenes_[nowScene_]->isChangeScene())
	{
		scenes_[nowScene_]->stop();
		nowScene_ = scenes_[nowScene_]->NextScene();
		scenes_[nowScene_]->start();
	}
}

void CSceneMng::draw()
{
	scenes_[nowScene_]->draw();
}