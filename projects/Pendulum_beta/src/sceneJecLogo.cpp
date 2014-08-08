#include "sceneJecLogo.h"

#include "define.h"		// ƒ‰ƒCƒuƒ‰ƒŠ
#include "sceneMng.h"


CSceneJecLogo::CSceneJecLogo() :
IScene("SceneJecLogo", "img_jecLogo", "")
{

}

CSceneJecLogo::~CSceneJecLogo()
{

}

bool CSceneJecLogo::update()
{
	if (time::Time_GetTimerCount(0) >= WAIT_TIME / 10.f)
	{
		return true;
	}
	return false;
}

void CSceneJecLogo::start()
{
	__super::start();
	camera::SetScale(1.f);
	time::Time_ResetTimer(0);
}

int CSceneJecLogo::NextScene() const
{
	return CSceneMng::Scene::TITLE;
}