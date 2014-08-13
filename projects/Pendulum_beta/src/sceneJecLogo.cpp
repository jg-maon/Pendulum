#include "sceneJecLogo.h"

#include "define.h"		// ライブラリ
#include "sceneMng.h"


CSceneJecLogo::CSceneJecLogo() :
IScene("SceneJecLogo", "img_jecLogo", "", 1.f, 1.f)
{
	
}

CSceneJecLogo::~CSceneJecLogo()
{

}

bool CSceneJecLogo::update()
{
	if (time::Time_GetTimerCount(0) >= WAIT_TIME / 10.f)
	{
		CFade::ChangeColor(0xff000000);
		return true;
	}
	return false;
}

void CSceneJecLogo::start()
{
	__super::start();
	camera::SetScale(1.f);
	time::Time_ResetTimer(0);
	CFade::ChangeColor(0xff000000);
}

int CSceneJecLogo::NextScene() const
{
	return CSceneMng::Scene::TITLE;
}