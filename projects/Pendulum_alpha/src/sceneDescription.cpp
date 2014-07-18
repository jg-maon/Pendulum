#include "sceneDescription.h"

#include "sceneStageSelect.h"
#include "define.h"


//======================================
// CSceneDescription methods
#pragma region


// �R���X�g���N�^
CSceneDescription::CSceneDescription():
IScene("img_description", "bgm_title")
//, sceneDescriptionBg_(0.f, 0.f, 1.f, 0.f, 0.f, "img_endbg", system::WINW, system::WINH, 1.f, 1.f, 0.f, 0, 0)
{

}
CSceneDescription::~CSceneDescription()
{
}



// �`��
void CSceneDescription::draw()
{
	__super::draw();
	
}

// ����
bool CSceneDescription::update()
{
	if (input::CheckPush(input::KEY_MOUSE_RBTN))
	{
		return true;
	}
	return false;
}

IScene* CSceneDescription::NextScene()
{
	return new CSceneStageSelect();
}




#pragma endregion
// CSceneDescription methods
