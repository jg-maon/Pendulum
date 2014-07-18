#include "sceneEnd.h"
#include "define.h"

#include "sceneNameInput.h"

#include "scoreMng.h"

//======================================
// CSceneEnd methods
#pragma region


const float CSceneEnd::BONUSPOINT_X = 1000.f;
const float CSceneEnd::BONUSINFO_X = 800.f;
const float CSceneEnd::BONUSPOINT_WIDTH = 50.f;


// �R���X�g���N�^
CSceneEnd::CSceneEnd():
IScene("img_end", "bgm_end")
//, sceneEndBg_(0.f, 0.f, 1.f, 0.f, 0.f, "img_endbg", system::WINW, system::WINH, 1.f, 1.f, 0.f, 0, 0)
{
	// �K�v�ȃI�u�W�F�N�g�̎n��
	auto& objs = gm()->GetObjects("ScoreMng");
	for (auto& obj : objs)
		obj->start();

	//sceneEndBg_.SetUse(true);

	state_ = CSceneEnd::State::RESULT;

	PointDataInit();

	selectChar_ = -1;
	registerAnimTime_ = 0.f;

}
CSceneEnd::~CSceneEnd()
{
}


// ���|�C���g����
// �O������|�C���g��n���悤�ɂ��邱��
void CSceneEnd::PointDataInit(){
	
	//���|�C���g
	int point = 100;

	int sumpoint = 0;
	float distance = 0.f;

	for (auto& bonusPoint : bonusPoints)
	{
		//�\�����W
		bonusPoint.cb.pos.y = 100.f + distance;
		distance += BONUSPOINT_WIDTH;
		
		//�|�C���g�i���j
		bonusPoint.point = point;
		sumpoint += bonusPoint.point;
		point *= 2;
	}

	// ���̃{�[�i�X�|�C���g�ڍ�
	// �O���t�@�C���Q�ƂȂǂ�\��
	bonusPoints[0].bonusInfo = "bonus1";
	bonusPoints[1].bonusInfo = "bonus2";
	bonusPoints[2].bonusInfo = "bonus3";
	bonusPoints[3].bonusInfo = "bonus4";
	bonusPoints[4].bonusInfo = "bonus5";


	//���v�̃f�[�^
	resultPoint_.cb.pos(BONUSPOINT_X, 600, 0.f);
	resultPoint_.point = sumpoint;

}

void CSceneEnd::PointDataDraw(){

	for (auto & bonusPoint : bonusPoints)
	{
		font::Draw_FontTextNC(
			(int)BONUSINFO_X, (int)bonusPoint.cb.pos.y,
			0.f, bonusPoint.bonusInfo,
			ARGB(255,200,200,100), 0);

		font::Draw_FontTextNC(
			(int)BONUSPOINT_X, (int)bonusPoint.cb.pos.y,
			0.f, std::to_string(bonusPoint.point),
			ARGB(255, 200, 200, 100), 0);

	}

	font::Draw_FontTextNC(
		(int)resultPoint_.cb.pos.x, (int)resultPoint_.cb.pos.y,
		0.f, std::to_string(resultPoint_.point),
		ARGB(255, 200, 200, 100), 0);

}

void CSceneEnd::ResultDraw()
{
	font::Draw_FontTextNC(100, 100, 0.5f, "�E�{�^���ŏI��", -1, 0);

	PointDataDraw();
	
}

void CSceneEnd::NameInputDraw()
{

	font::Draw_FontTextNC(100, 700, 0.5f, "�E�{�^���Ŋ���", -1, 0);


	// �w�i
	//__super::draw();
	static float s = 0.f;
	for (int i = 0; i < 26; ++i)
	{
		graph::Draw_GraphicsNC(
			100 + i * 25,
			300,
			0.1f,
			"img_alphabet",
			i * 20, 0, 20, 40);
		graph::Draw_GraphicsNC(
			100 + i * 25,
			300,
			0.f,
			"img_alphabet",
			i * 20, 0, 20, 40,
			0, 0,
			1.f + std::sinf(s),
			1.f + std::sinf(s),
			static_cast<u_char>(255.f / 2.f + (255.f / 2.f * std::cosf(s) / 2.f)));
	}

	if ((s += math::Calc_DegreeToRad(2.f)) >= math::PI)
		s -= math::PI;
}

void CSceneEnd::HiscoreDispDraw()
{
	font::Draw_FontTextNC(100, 200, 0.5f, "�n�C�X�R�A�\��", -1, 0);
}

bool CSceneEnd::InputNextKey(){

	return input::CheckPush(input::KEY_MOUSE_RBTN);

}


// �`��
void CSceneEnd::draw()
{

	__super::draw();
	//sceneEndBg_.drawNC(charabase::CharBase::MODE::LeftTop);

	switch (state_)
	{
	case CSceneEnd::State::RESULT:
		ResultDraw();
		break;

	}

}

// ����
bool CSceneEnd::update()
{
	switch (state_)
	{
	case CSceneEnd::State::RESULT:

		if (InputNextKey())				//���A�X�R�A���n�C�X�R�A�̎�
		{
			state_ = State::END_EXIT;
		}

		break;

	case CSceneEnd::State::END_EXIT:

		return true;

		break;
	}


	//auto& sm = std::dynamic_pointer_cast<CScoreMng>(gm()->GetObj(typeid(CScoreMng)));
	//sm->RegisterRanking("MAON");


	/*switch (state_)
	{
	case CSceneEnd::State::NAME:
		DecideName();
		break;
	case CSceneEnd::State::REGISTER_ANIM:
		RegisterAnimation();
		break;
	case CSceneEnd::State::ANIM_END:
		auto& sm = std::dynamic_pointer_cast<CScoreMng>(gm()->GetObj(typeid(CScoreMng)));
		sm->RegisterRanking("MAON");
		return true;
	}*/

	
	return false;
}

IScene* CSceneEnd::NextScene()
{
	return new CSceneNameInput();
}


void CSceneEnd::DecideName()
{
	/*
	if (input::CheckPush(input::KEY_MOUSE_LBTN))
	{

	}
	if (input::CheckPush(input::KEY_MOUSE_RBTN))
	{
		state_ = CSceneEnd::State::REGISTER_ANIM;
	}*/
}

void CSceneEnd::RegisterAnimation()
{

}



#pragma endregion
// CSceneEnd methods
