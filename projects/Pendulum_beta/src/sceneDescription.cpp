#include "sceneDescription.h"

#include "define.h"


//======================================
// CSceneDescription methods
#pragma region


// コンストラクタ
CSceneDescription::CSceneDescription():
IScene("SceneDescription", "img_description", "bgm_title")
//, sceneDescriptionBg_(0.f, 0.f, 1.f, 0.f, 0.f, "img_endbg", system::WINW, system::WINH, 1.f, 1.f, 0.f, 0, 0)
{

}
CSceneDescription::~CSceneDescription()
{
}


bool CSceneDescription::update()
{
	if (input::CheckPush(input::KEY_MOUSE_LBTN))
	{
		return true;
	}
	return false;
}

void CSceneDescription::start()
{
	__super::start();
}

int CSceneDescription::NextScene() const
{
	return CSceneMng::Scene::STAGESELECT;
}




#pragma endregion
// CSceneDescription methods
