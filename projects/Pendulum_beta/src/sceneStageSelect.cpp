#define ALPHA_VER		// �����\��o�[�W����

#include "sceneStageSelect.h"
#include "define.h"

#include "Fade.h"

#include "common.h"

#include "stageMng.h"

#include <iomanip>
#include <sstream>


//======================================
// CSceneStageSelect methods
#pragma region CSceneStageSelect methods
// �R���X�g���N�^
CSceneStageSelect::CSceneStageSelect() :
IScene("SceneStageSelect", "img_stageSelect", "bgm_stageSelect")
{
	std::string settingFile = gm()->fm().GetFile("#StageSelectFile");
	std::ifstream f(settingFile);
	back_ = charabase::CharPtr(new charabase::CharBase());
	back_->resname = BACK_RESNAME;
	back_->pos.z = 1.f;
	if (f.fail())
	{
		debug::BToM("CSceneStageSelect::CSceneStageSelect path:%s", settingFile.c_str());
		fieldRect_.top = fieldRect_.left = 0;
		fieldRect_.right = system::WINW;
		fieldRect_.bottom = system::WINH;
	}
	else
	{
		LoadInfo(f);
	}
}

void CSceneStageSelect::start()
{
	__super::start();
	descHeight_ = 0.f;

	camera::SetLookAt(startCameraPos_.x, startCameraPos_.y);
	camera::SetScale(1.f);

}
CSceneStageSelect::~CSceneStageSelect()
{
}

// �`��
void CSceneStageSelect::draw()
{
#ifdef _DEBUG
	const auto& mouse = gm()->GetCursorPos();
	std::stringstream ss;
	ss << "(" << std::setw(4) << (int)mouse.x
		<< "," << std::setw(4) << (int)mouse.y
		<< ")";
	font::Draw_FontText(
		(int)mouse.x,
		(int)mouse.y,
		0.f,
		ss.str(),
		-1,0);

	const mymath::Vec3i m = gm()->GetCursorPosNC();
	ss.str("");
	ss << "(" << std::setw(4) << m.x
		<< "," << std::setw(4) << m.y
		<< ")";
	font::Draw_FontTextNC(100, 100, 0.f, ss.str(), 0xff000000, 1);
#endif

	// �w�i
	back_->draw(charabase::CharBase::MODE::LeftTop);

	// �X�e�[�W
	for (auto& stage : stages_)
	{
		// �X�e�[�W�摜
		stage.first->draw();
		// �X�e�[�W������
		if (stage.second->CheckUse())
		{
			const auto& obj = stage.second;
			charabase::CharBase tmp(*obj);
			tmp.pos -= obj->HalfSize();
			tmp.size.y = static_cast<int>(obj->size.y * descHeight_);
			tmp.draw(charabase::CharBase::MODE::LeftTop);
		}
	}

}


// ����
bool CSceneStageSelect::update()
{
	// �^�C�g����ʂֈڍs���邩�̃`�F�b�N
	if (input::CheckPush(input::KEY_MOUSE_LBTN) && input::CheckPush(input::KEY_MOUSE_RBTN))
	{
		toTitleTime_ = 0.f;
	}
	if (input::CheckPress(input::KEY_MOUSE_LBTN) && input::CheckPress(input::KEY_MOUSE_RBTN))
	{
		toTitleTime_ += system::FrameTime;
		if (toTitleTime_ >= TO_TITLE_TIME / 10.f)
		{
			// �^�C�g����ʂ�
			nextScene_ = NextSceneType::TITLE;
			CFade::ChangeColor(0xff000000);
			return true;
		}
	}

	const mymath::Vec3f& mouse = gm()->GetCursorPos();
	POINT mouseNC = camera::GetCursorPositionNC();

	mymath::Vec3f lookAt = camera::GetLookAt();
	
	int halfWINW = system::WINW / 2;
	int halfWINH = system::WINH / 2;
	
	// �J�����ړ����x
	const float cameraMoveSp = static_cast<float>(fieldRect_.right - fieldRect_.left) / 1.5f;

	// �J�����ړ��J�n���C��(�E�B���h�E����̋���)
	const POINT border = { 100, 100 };
	if (mouseNC.x < border.x)
	{
		// ��
		float next = lookAt.x - (cameraMoveSp * system::FrameTime);
		lookAt.x = max(fieldRect_.left + halfWINW, next);

	}
	else if (mouseNC.x > system::WINW - border.x)
	{
		// ��
		float next = lookAt.x + (cameraMoveSp * system::FrameTime);
		lookAt.x = min(fieldRect_.right - halfWINW, next);
	}
	if (mouseNC.y < border.y)
	{
		// ��
		float next = lookAt.y - (cameraMoveSp * system::FrameTime);
		lookAt.y = max(fieldRect_.top + halfWINH, next);
	}
	else if (mouseNC.y > system::WINH - border.y)
	{
		// ��
		float next = lookAt.y + (cameraMoveSp * system::FrameTime);
		lookAt.y = min(fieldRect_.bottom - halfWINH, next);
	}
	camera::SetLookAt(lookAt.x, lookAt.y);

	// �X�e�[�W�����A�X�e�[�W�J�n
	const float descSp = 1.f / 0.5f;		// �\���A�j���[�V�������x
	int i = 0;
	bool noHover = true;	// �X�e�[�W�摜��ɃJ�[�\�����Ȃ�
	for (auto& stage : stages_)
	{
		auto& rt = stage.first->GetRect();
		if (rt.Contains(mouse))
		{
			noHover = false;
			// �X�e�[�W�����\��
			stage.second->SetUse(true);
			//=========================================================
			// �X�e�[�W�����A�j���[�V����
			if (descHeight_ < 1.f)
			{
				descHeight_ += descSp*system::FrameTime;
			}
			else
			{
				descHeight_ = 1.f;
			}
			//=========================================================
			// �J�����Ƃ̈ʒu�֌W�ɂ��\���ʒu�̕ύX
			stage.second->pos = stage.first->pos;
			// �X�e�[�W��NC���W�ɕϊ�
			mymath::Vec3f NCPos = stage.first->pos - camera::GetCameraPos();
			//-----------------------------------------
			// ������X�ʒu
			if (NCPos.x < halfWINW - stage.first->HalfWidth())
				// ��ʒ�����荶�ɂ���ꍇ�A�������͉E�֕\��
				stage.second->pos.x += (stage.first->HalfWidth()+ stage.second->HalfWidth() + 10.f);
			else
				// ��ʒ������E�ɂ���ꍇ�A�������͍��֕\��
				stage.second->pos.x -= (stage.first->HalfWidth() + stage.second->HalfWidth() + 10.f);
			//-----------------------------------------
			// ������Y�ʒu
			if (NCPos.y < halfWINH - stage.first->HalfHeight())
				// ��ʒ�������ɂ���ꍇ�A�������͉��֕\��
				stage.second->pos.y += (stage.first->HalfHeight() + stage.second->HalfHeight() + 10.f);
			else if (NCPos.y > halfWINH + stage.first->HalfHeight())
				// ��ʒ�����艺�ɂ���ꍇ�A�������͏�֕\��
				stage.second->pos.y -= (stage.first->HalfHeight() + stage.second->HalfHeight() + 10.f);
			else
				// �␳�����̏ꍇ�͂��炳�Ȃ�
				stage.second->pos.y;
			//=========================================================
			// �X�e�[�W�I��
			if (input::CheckPush(input::KEY_MOUSE_LBTN))
			{
#ifdef ALPHA_VER
				// ���łł̓X�e�[�W1�̂ݑI���\�Ƃ���
				if (i == 0)
#endif
				{
					CFade::ChangeColor(255, 255, 255);
					//stageName_ = common::StrReplace(stage.first->resname, "img_ss", "stage");

					std::stringstream ss;
					ss << "Stage" << std::setw(2) << std::setfill('0') << (i + 1);
					stageName_ = ss.str();
					nextScene_ = NextSceneType::MAIN;
					return true;
				}
			}
		}
		else
		{
			stage.second->SetUse(false);
		}
		++i;
	}
	if (noHover)
		descHeight_ = 0.f;

	return false;
}

int CSceneStageSelect::NextScene() const
{
	switch (nextScene_)
	{
	case CSceneStageSelect::NextSceneType::TITLE:
		return CSceneMng::Scene::TITLE;
	case CSceneStageSelect::NextSceneType::MAIN:
		{
			const auto& sm = gm()->GetObjects("StageMng");
			if (!sm.empty())
			{
				std::dynamic_pointer_cast<CStageMng>(sm[0])->LoadStage(stageName_);
			}
			else
			{
				ObjPtr stage(new CStageMng());
				//InsertObject(stage);
				gm()->AddObject(stage);
				std::dynamic_pointer_cast<CStageMng>(stage)->LoadStage(stageName_);
			}
			return CSceneMng::Scene::MAIN;
		}
		break;
	}
	return CSceneMng::Scene::STAGESELECT;
}



void CSceneStageSelect::LoadInfo(std::ifstream& f)
{
	//====================================================-
	// �w�i�摜
	//-------------------------------------
	// �摜��
	if (common::FindChunk(f, "#BGWidth"))
	{
		f >> back_->size.x;
	}
	//-------------------------------------
	// �摜����
	if (common::FindChunk(common::SeekSet(f), "#BGHeight"))
	{
		f >> back_->size.y;
	}
	//-------------------------------------
	// �`��n�_X
	if (common::FindChunk(common::SeekSet(f), "#BGLeft"))
	{
		f >> back_->pos.x;
	}
	//-------------------------------------
	// �`��n�_Y
	if (common::FindChunk(common::SeekSet(f), "#BGTop"))
	{
		f >> back_->pos.y;
	}
	back_->pos.z = 1.f;
	//====================================================-
	// �t�B�[���h
	//-------------------------------------
	// �t�B�[���h���[
	if (common::FindChunk(common::SeekSet(f), "#Left"))
	{
		f >> fieldRect_.left;
	}
	//-------------------------------------
	// �t�B�[���h��[
	if (common::FindChunk(common::SeekSet(f), "#Top"))
	{
		f >> fieldRect_.top;
	}
	//-------------------------------------
	// �t�B�[���h�E�[
	if (common::FindChunk(common::SeekSet(f), "#Right"))
	{
		f >> fieldRect_.right;
	}
	//-------------------------------------
	// �t�B�[���h���[
	if (common::FindChunk(common::SeekSet(f), "#Bottom"))
	{
		f >> fieldRect_.bottom;
	}
	//-------------------------------------
	// �J�������������_
	if (common::FindChunk(common::SeekSet(f), "#StartCameraPos"))
	{
		f >> startCameraPos_.x >> startCameraPos_.y;
	}
	//-------------------------------------
	// �X�e�[�W�ꗗ
	LoadStages(f);


	//====================================================-
	// �w�i�摜2(�n�_�ƕ����������������Ă������Ƃ�����)
	//-------------------------------------
	// �w�i�摜�I�_X
	if (common::FindChunk(common::SeekSet(f), "#BGRight"))
	{
		int right;
		f >> right;
		back_->scale.x = (static_cast<float>(right)-back_->pos.x) / static_cast<float>(back_->size.x);
	}
	//-------------------------------------
	// �w�i�摜�I�_Y
	if (common::FindChunk(common::SeekSet(f), "#BGBottom"))
	{
		int bottom;
		f >> bottom;
		back_->scale.y = (static_cast<float>(bottom)-back_->pos.y) / static_cast<float>(back_->size.y);
	}
}

void CSceneStageSelect::LoadStages(std::ifstream& f)
{
	for (int i = 1; i <= 4; ++i)
	{
		std::stringstream ss;
		ss << "#Stage" << std::setw(2) << std::setfill('0') << i;
		// �X�e�[�W���x��
		if (common::FindChunk(common::SeekSet(f), ss.str()))
		{
			//---------------------------------
			// �X�e�[�W
			charabase::CharPtr stage = charabase::CharPtr(new charabase::CharBase());
			// ���WXY
			f >> stage->pos.x >> stage->pos.y;
			// �X�e�[�W�摜�Ǘ���
			f >> stage->resname;
			// �摜�T�C�YWH
			f >> stage->size.x >> stage->size.y;
			//---------------------------------
			// ����
			charabase::CharPtr desc = charabase::CharPtr(new charabase::CharBase());
			// �X�e�[�W�����摜�Ǘ���
			f >> desc->resname;
			// �摜�T�C�YWH
			f >> desc->size.x >> desc->size.y;

			stage->pos.z = 0.5f;
			desc->pos.z = 0.4f;

			stages_.push_back(std::make_pair(stage, desc));
		}
	}
}



#pragma endregion CSceneStageSelect methods
// CSceneStageSelect methods

