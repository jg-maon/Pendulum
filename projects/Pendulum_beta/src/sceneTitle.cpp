#ifdef _DEBUG
//#define D_SCALE_TEST	// �g��A�J�����ړ��e�X�g
#define D_EFFECT_TEST	// �G�t�F�N�g�����e�X�g
//#define D_PICKUP_TEST	//  �s�b�N�A�b�v�A�C�e���e�X�g
//#define D_SHAPE_TEST	// �}�`�e�X�g
#define D_TURN_TEST	// ���]�`��e�X�g
#define D_SOUND_TEST	// ���e�X�g �p���U��
#endif

#ifdef D_SHAPE_TEST
#define DEF_SHAPE_DRAW
#endif

//=================================================================================
// include �L�q
//=================================================================================
#include "sceneTitle.h"
#include "define.h"

#include "setting.h"


#include "easing.h"

#include "fade.h"



#ifdef D_EFFECT_TEST
#include "effectExplosion.h"
#include "effectSlash.h"
#include "effectFlash.h"
#endif

#ifdef D_PICKUP_TEST
#include "pickupJewely.h"
#endif

#ifdef _DEBUG

namespace titleDebug
{
	mymath::Vec3f pt;
#ifdef D_SHAPE_TEST
	float scale = 1.f + 0.01f;
	mymath::Rectf rc(400.f, 200.f, 700.f, 400.f);
	mymath::Circlef ci(600.f, 100.f, 0.f, 50.f);
	mymath::Polyf pl(0);
#endif
#ifdef D_TURN_TEST
	struct TurnObject : public IObject
	{
		static int i;
		int my;
		TurnObject(mymath::Vec3f& pos):
		IObject("TurnObject")
		{
			my = i++;
			obj_ = charabase::CharBase(charabase::BaseData("img_bird", 192, 192));
			obj_.pos = pos;
			obj_.pos.z = 0.5f;
		}
		void step()
		{
			mymath::Vec3f mouse = gm()->GetCursorPos();
			/*
			if (input::CheckPress(input::KEY_UP) || input::CheckPress(input::KEY_LEFT))
			{
				obj_.angle -= 10.f;
			}
			if (input::CheckPress(input::KEY_DOWN) || input::CheckPress(input::KEY_RIGHT))
			{
				obj_.angle += 10.f;
			}
			//*/
			obj_.angle = math::Calc_RadToDegree(mymath::Vec3f::Angle2(obj_.pos, mouse));
			if (obj_.angle >= 360.f)
				obj_.angle -= 360.f;
			else if (obj_.angle < 0.f)
				obj_.angle += 360.f;
			if (my == 0)
			{
				
				if (obj_.angle < 90.f || 270.f < obj_.angle)
				{
					turnFlag_ = false;
				}
				if (90.f <= obj_.angle && 270.f <= obj_.angle)
				{
					if (!turnFlag_)
					{
						turnFlag_ = true;
						obj_.angle = 180.f - obj_.angle;
						//if (obj_.angle <= 0.f)
					//		obj_.angle += 360.f;
					}
				}
			}
			else if (my == 1)
			{
				Look(mouse);
			}
			else if (my == 2)
			{
				if (90.f < obj_.angle && obj_.angle <= 270.f)
				{
					turnFlag_ = true;
					obj_.angle = obj_.angle - 180.f;
					if (obj_.angle <= 0.f)
						obj_.angle += 360.f;
				}
				else
				{
					turnFlag_ = false;
				}
				
			}
			
		}

		void draw()
		{
			__super::draw();
			std::stringstream ss;
			ss << std::boolalpha << turnFlag_ << " " << obj_.angle;
			font::Draw_FontText(
				(int)(obj_.pos.x + obj_.HalfSize().x),
				(int)(obj_.pos.y),
				obj_.pos.z - 0.1f,
				ss.str(), -1, 1);
		}
	};
	int TurnObject::i = 0;
	std::vector<TurnObject> turnObjs_;
#endif
	
}
using namespace titleDebug;

#endif


//=================================================================================
// �ϐ��萔��`
//=================================================================================
#pragma region �ϐ��萔��`
//���S�֘A
const float CSceneTitle::TITLELOGO_X = (system::WINW / 2.f);
const float CSceneTitle::TITLELOGO_Y = (system::WINH / 3.f);

//���S�A�j���֘A
const float CSceneTitle::LOGO_A_X = 0.f;
const float CSceneTitle::LOGO_A_Y = (system::WINH * 2.f / 3.f);
const float CSceneTitle::LOGO_A_SIZE = 3.5f;
const float CSceneTitle::LOGO_A_STARTTIME = 0.5f;

const float CSceneTitle::LOGO_B_X = static_cast<float>(system::WINW);
const float CSceneTitle::LOGO_B_Y = (system::WINH * 2.f / 3.f);
const float CSceneTitle::LOGO_B_SIZE = 4.f;
const float CSceneTitle::LOGO_B_STARTTIME = 1.4f;

const float CSceneTitle::LOGO_C_X = (system::WINW / 2.f);			//WINW/2
const float CSceneTitle::LOGO_C_Y = static_cast<float>(system::WINH);			//WINH
const float CSceneTitle::LOGO_C_SIZE = 5.f;
const float CSceneTitle::LOGO_C_STARTTIME = 2.3f;

const float CSceneTitle::LOGO_TIME = 0.8f;

//�a���֘A
const float CSceneTitle::SLASH_A_X = 600.f;
const float CSceneTitle::SLASH_A_Y = (system::WINH / 2.f);
const float CSceneTitle::SLASH_A_DEG = 30.f;
const float CSceneTitle::SLASH_B_X = 700.f;
const float CSceneTitle::SLASH_B_Y = (system::WINH / 2.f);
const float CSceneTitle::SLASH_B_DEG = 120.f;
const float CSceneTitle::SLASH_NEXT_TIME = 0.3f;


//�N���b�N�֘A
const float CSceneTitle::CLICK_ON = 1.f;
//const int CSceneTitle::CLICK_DEG_ALPHA = 5;
//const int CSceneTitle::CLICK_MIN_ALPHA = 50;

//�\������
const float CSceneTitle::TITLETIME = 30.f;
const float CSceneTitle::DEMOTIME = 0.f;

#pragma endregion // �ϐ��萔��`
//=================================================================================



//=================================================================================
// CSceneTitle methods
#pragma region CSceneTitle methods

// �R���X�g���N�^
//BGM�Ǘ����A�t�F�[�h�C�����ԁA�t�F�[�h�A�E�g���Ԃ̏�����

CSceneTitle::CSceneTitle() :
IScene("SceneTitle", "img_title", "bgm_title")
, titleLogo_(TITLELOGO_X, TITLELOGO_Y, 0.5f, 0.f, 0.5f, "img_logo", 400, 200, 1.f, 1.f, 0.f, 0, 0)
, titleAnim_(LOGO_A_X, LOGO_A_Y, 0.4f, 0.f, 0.f, "img_logo", 400, 200, 1.f, 1.f, 0.f, 0, 1)
, titleSlash_(SLASH_A_X, SLASH_A_Y, 0.f, 0.f, 0.f, "img_titleSlash", 240, 360, 2.f, 3.f, 0.f, 0, 0)
{
#ifdef D_SHAPE_TEST
	{
		std::vector<mymath::Vec3f> points =
		{
			mymath::Vec3f(10.f, 20.f),
			mymath::Vec3f(90.f, 20.f),
			mymath::Vec3f(50.f, 90.f),
			mymath::Vec3f(30.f, 50.f),
			mymath::Vec3f(10.f, 10.f),
		};

		titleDebug::pl = mymath::Polyf(points);
		titleDebug::pl.Offset(mymath::Vec3f(100.f, 200.f));
	}
#endif

#ifdef D_TURN_TEST
	{
		std::vector<mymath::Vec3f> points =
		{
			mymath::Vec3f(100.f, 200.f),
			mymath::Vec3f(400.f, 200.f),
			mymath::Vec3f(700.f, 200.f),
		};
		for (auto& point : points)
			titleDebug::turnObjs_.push_back(TurnObject(point));
	}
#endif
}

CSceneTitle::~CSceneTitle()
{

}

void CSceneTitle::start()
{
	__super::start();


	phase_ = (Phase::TITLE);
	fadeState_ = (State::MAIN);
	slashAnim_.set(9, 0.9f);

	camera::SetLookAt(system::WINW / 2.f, system::WINH / 2.f);
	camera::SetScale(1.f);

	CFade::ChangeColor(0xffffffff);
	TitleInit();

}



//======================================
// �`��A�����A������
//======================================


// ����
bool CSceneTitle::update()
{
#ifdef _DEBUG
	titleDebug::pt = gm()->GetCursorPos();
#endif

#ifdef D_SHAPE_TEST
	titleDebug::ci.Scale(titleDebug::scale);
	titleDebug::rc.Scale(titleDebug::scale);
	titleDebug::pl.Scale(titleDebug::scale);
#endif


	// �^�C�g���f���Ԃ̃t�F�[�h�A�E�g�A�C��
	switch (fadeState_)
	{

	case IScene::State::INNING:
		// �t�F�[�h�C������
		if (CFade::FadeIn(255.f / 0.3f * system::FrameTime))
		{
			fadeState_ = State::MAIN;

		}
		break;

	case IScene::State::MAIN:
		switch (phase_)
		{
		case CSceneTitle::Phase::TITLE:

			// �^�C�g����ʕ\����
			TitleStep();
			// �V�[���ω�
			if (input::CheckPush(input::KEY_MOUSE_LBTN)
				|| input::CheckPush(input::KEY_MOUSE_RBTN))
			{
				bgm::DShow_Stop("bgm_title");
				se::DSound_Play("se_enter0");
				return true;
			}
			break;
		case CSceneTitle::Phase::DEMO:
			// �f���v���C�\����
			DemoStep();
			break;
		}
		break;

	case IScene::State::OUTING:

		if (CFade::FadeOut(255.f / 0.3f * system::FrameTime))
		{
			fadeState_ = IScene::State::INNING;
			CFade::StartFadeIn();
			if (phase_ == Phase::DEMO)
			{
				phase_ = Phase::TITLE;
				TitleInit();
			}
			else if (phase_ == Phase::TITLE)
			{
				phase_ = Phase::DEMO;
				DemoInit();
			}
		}
		break;
	}

#ifdef D_TURN_TEST
	for (auto& obj : turnObjs_)
		obj.step();
#endif


#ifdef D_EFFECT_TEST
	if (input::CheckPress(input::KEY_LSHIFT))
	{
		if (input::CheckPush(input::KEY_BTN0))
			gm()->AddObject(ObjPtr(new CEffectSlash(pt, math::GetRandom(0.f, 360.f))));
		if (input::CheckPush(input::KEY_BTN1))
			gm()->AddObject(ObjPtr(new CEffectFlash(pt, mymath::Vec3f(0.f, 0.f, 0.f), 1.f, 1.f)));
		if (input::CheckPush(input::KEY_BTN2))
			gm()->AddObject(ObjPtr(new CEffectFlash(pt, mymath::Vec3f(200.f,-200.f,0.f), 1.f, 1.f)));

	}
	else
	{
		if (input::CheckPush(input::KEY_BTN0))
			gm()->AddObject(ObjPtr(new CEffectSlash(pt)));
		if (input::CheckPush(input::KEY_BTN1))
			gm()->AddObject(ObjPtr(new CEffectExplosion(pt)));
	}
#endif
#ifdef D_SOUND_TEST
	if (input::CheckPush(input::KEY_SPACE))
	{
		int i = se::DSound_Play("se_shot1");

		// �p���U��
		mymath::Vec3f cameraCenter = camera::GetLookAt();
		auto stageWidth = gm()->winRect()->width();
		mymath::Vec3f dist = pt - cameraCenter;
		long pan = static_cast<long>(10000.f * dist.x / (stageWidth / 2.f));
		se::DSound_SetPan("se_shot1", i, pan);
	}

#endif


	return false;
}




//-------------------------
//Draw
//-------------------------

// �`��
void CSceneTitle::draw()
{
#ifdef D_SHAPE_TEST
	titleDebug::rc.draw();
	titleDebug::ci.draw();
	titleDebug::pl.draw();
#endif
#ifdef D_TURN_TEST
	for (auto& obj : turnObjs_)
		obj.draw();
#endif


	//-------------------------
	//Title
	//-------------------------

	if (phase_ == Phase::TITLE)
	{

		// �w�i
		__super::draw();

#ifdef D_SCALE_TEST
		auto& point = camera::GetLookAt();
		graph::Draw_Line(point.x - 3, point.y, point.x + 3, point.y, 0.f, -1, 1);
		graph::Draw_Line(point.x, point.y - 3, point.x, point.y + 3, 0.f, -1, 1);
		std::stringstream ss;
		ss << "(" << point.x << ", " << point.y << ")";
		font::Draw_TextXY(point.x, point.y, ss.str(), -1);


		graph::Draw_GraphicsLeftTop(0, 0, 0.8f,
			BACK_RESNAME,
			0, 0, 1200, 800);
		graph::Draw_GraphicsLeftTop(0, 0, 9.f,
			BACK_RESNAME,
			0, 0, 1200, 800, 0, 0, 2.f, 2.f, 128);
		graph::Draw_GraphicsLeftTop(0, 0, 0.9f,
			BACK_RESNAME,
			0, 0, 1200, 800, 0, 0, 5.f, 5.f, 128);
		graph::Draw_GraphicsLeftTop(0, 0, 1.f,
			BACK_RESNAME,
			0, 0, 1200, 800, 0, 0, 10.f, 6.2f, 128);
#endif

		//titleBack_.draw(charabase::CharBase::LeftTop);

		// ���S
		if (titleLogo_.CheckUse())
			titleLogo_.draw(charabase::CharBase::Center);

		// Pleas Click
		if (clickPhase_ != ClickPhase::HIDDEN)
			font::Draw_FontTextNC(system::WINW / 2 - 80, system::WINH / 3 * 2, 0.5f,
			"Please Click!", ARGB(clickAlpha_, 255, 255, 255), setting::GetFontID("font_MSGOSICK30"));

		// �ʃA�j���[�V����
		if (titleAnim_.CheckUse())
			titleAnim_.draw(charabase::CharBase::Center);


		// ���A�j���[�V����
		if (titleSlash_.CheckUse())
			titleSlash_.draw(charabase::CharBase::Center);

		//debug
		//debug::Dbg_DisplayToMessage(100, 100, "%f", titleAnim_.scale.x);
		debug::SToM(100, 100, "%f", titleAnim_.scale.x);
	}

	//-------------------------
	//Demo
	//-------------------------
	else if (phase_ == Phase::DEMO)
	{

	}

}


int CSceneTitle::NextScene() const
{
	return CSceneMng::Scene::STAGESELECT;
}


//-------------------------
//Title
//-------------------------
// �^�C�g��������
void CSceneTitle::TitleInit()
{
	// �A�j���[�V����������
	titleLogo_.src(0, 0, 0);
	titleAnim_.pos(LOGO_A_X, LOGO_A_Y);
	titleAnim_.add(0.f);
	titleAnim_.src(0, 1, 0);
	titleAnim_.alpha = 255;
	clickAlpha_ = 255;

	titleSlash_.pos(SLASH_A_X, SLASH_A_Y, 0.f);
	titleSlash_.angle = SLASH_A_DEG;

	// �t�F�[�Y������
	logoPhase_ = LogoPhase::RED;
	animPhase_ = AnimPhase::LOGO;
	clickPhase_ = ClickPhase::HIDDEN;
	slashPhase_ = SlashPhase::HIDDEN1;
	flashPhase_ = FlashPhase::FADEOUT;

	// �^�C�}������
	phaseTime_ = 0.f;
	offTime_ = 0.f;
	nextTime_ = 0.f;

	// �������t���O

	sePlaying_ = true;
	flashFlag_ = true;

	// ���s��
	//titleBack_.SetUse(true);
	titleLogo_.SetUse(true);
	titleAnim_.SetUse(false);
	titleSlash_.SetUse(false);



}

//�^�C�g������
void CSceneTitle::TitleStep()
{
	phaseTime_ += system::FrameTime;

	switch (animPhase_)
	{
	case CSceneTitle::AnimPhase::LOGO:
		LogoAnime();
		break;

	case CSceneTitle::AnimPhase::SLASH:
		SlashAnime();
		break;

	case CSceneTitle::AnimPhase::FLASH:
		TitleFlash();
		break;

	case CSceneTitle::AnimPhase::WAIT:
		PleaseClick();
		break;

	default:

		break;
	}


	//const float demoTime = 20.f;
	if (phaseTime_ >= TITLETIME)
	{
		ChangePhase();
	}


#ifdef D_PICKUP_TEST

	if (input::CheckPush(input::KEY_BTN0))
	{
		gm()->AddObject(ObjPtr(new CPickupJewely(gm()->GetCursorPos())));
	}

#endif



#ifdef D_SCALE_TEST
	using namespace input;
	struct
	{
		input::CODE_KEY code;
		POINT move;
	} a[4] =
	{
		{ KEY_UP, 0, -3 },
		{ KEY_DOWN, 0, +3 },
		{ KEY_LEFT, -3, 0 },
		{ KEY_RIGHT, +3, 0 },
	};
	for (auto& aa : a)
	{
		if (input::CheckPress(aa.code))
		{
			pt.x += aa.move.x;
			pt.y += aa.move.y;
		}
	}
	camera::SetLookAt(pt.x, pt.y);
#endif


}


//-------------------------
//Demo
//-------------------------

//�f��������
void CSceneTitle::DemoInit()
{
	phaseTime_ = 0.f;

}

//�f������
void CSceneTitle::DemoStep()
{
	phaseTime_ += system::FrameTime;



	// �f���v���C�I��
	if (input::CheckPush(input::KEY_MOUSE_LBTN))
	{
		phaseTime_ = DEMOTIME;
	}


	if (phaseTime_ >= DEMOTIME)
	{
		ChangePhase();
	}
}





//======================================
//�֐�
//======================================

//���S�A�j���[�V����
void CSceneTitle::LogoAnime()
{
	switch (logoPhase_)
	{
	case CSceneTitle::LogoPhase::RED:
		if (phaseTime_ <= LOGO_A_STARTTIME)
			return;

		if (sePlaying_)
		{
			se::DSound_Play("se_drain");
			sePlaying_ = false;
		}

		titleAnim_.SetUse(true);			//��

		titleAnim_.pos.x = Easing::QuadInOut(phaseTime_ - LOGO_A_STARTTIME, LOGO_A_X, (TITLELOGO_X - LOGO_A_X), LOGO_TIME);
		titleAnim_.pos.y = Easing::QuadInOut(phaseTime_ - LOGO_A_STARTTIME, LOGO_A_Y, (TITLELOGO_Y - LOGO_A_Y), LOGO_TIME);
		titleAnim_.alpha -= (200.f / LOGO_TIME)*system::FrameTime;		//������

		titleAnim_.scale.x = titleAnim_.scale.y = LogoSize(LOGO_A_SIZE, LOGO_A_Y);

		//�t�F�[�Y�ړ�
		if (titleAnim_.pos.y <= TITLELOGO_Y)
		{

			titleAnim_.SetUse(false);
			titleAnim_.pos(LOGO_B_X, LOGO_B_Y);
			titleAnim_.src.y = 2;
			titleAnim_.alpha = 255;
			titleAnim_.scale(1.f, 1.f, 1.f);

			sePlaying_ = true;
			logoPhase_ = LogoPhase::GREEN;

		}
		break;

	case CSceneTitle::LogoPhase::GREEN:

		if (phaseTime_ <= LOGO_B_STARTTIME)
			return;

		if (sePlaying_)
		{
			se::DSound_Play("se_drain");
			sePlaying_ = false;
		}

		titleAnim_.SetUse(true);
		titleAnim_.pos.x = Easing::QuadInOut(phaseTime_ - LOGO_B_STARTTIME, LOGO_B_X, (TITLELOGO_X - LOGO_B_X), LOGO_TIME);
		titleAnim_.pos.y = Easing::QuadInOut(phaseTime_ - LOGO_B_STARTTIME, LOGO_B_Y, (TITLELOGO_Y - LOGO_B_Y), LOGO_TIME);
		titleAnim_.alpha -= (200.f / LOGO_TIME)*system::FrameTime;

		titleAnim_.scale.x = titleAnim_.scale.y = LogoSize(LOGO_B_SIZE, LOGO_B_Y);

		if (titleAnim_.pos.y <= TITLELOGO_Y)
		{
			titleAnim_.SetUse(false);
			titleAnim_.pos(LOGO_C_X, LOGO_C_Y);
			titleAnim_.alpha = 255;
			titleAnim_.src.y = 3;
			titleAnim_.scale(1.f, 1.f, 1.f);

			sePlaying_ = true;
			logoPhase_ = LogoPhase::BLUE;

		}
		break;

	case CSceneTitle::LogoPhase::BLUE:

		if (phaseTime_ <= LOGO_C_STARTTIME)
			return;

		if (sePlaying_)
		{
			se::DSound_Play("se_drain");
			sePlaying_ = false;
		}

		titleAnim_.SetUse(true);
		titleAnim_.pos.y = Easing::SineInOut(phaseTime_ - LOGO_C_STARTTIME, LOGO_C_Y, (TITLELOGO_Y - LOGO_C_Y), LOGO_TIME);
		titleAnim_.alpha -= (200 / LOGO_TIME)*system::FrameTime;

		titleAnim_.scale.x = titleAnim_.scale.y = LogoSize(LOGO_C_SIZE, LOGO_C_Y);

		if (titleAnim_.pos.y <= TITLELOGO_Y)
		{
			titleAnim_.SetUse(false);
			animPhase_ = AnimPhase::SLASH;

		}
		break;

	}

	//titleAnim_.Move();

}

//���S�T�C�Y�ύX
float CSceneTitle::LogoSize(float firstsize, float firsty)
{

	//------------------------------------------------------------
	// ����
	//
	// ���݃T�C�Y = �ŏI�T�C�Y�l + �T�C�Y�ω���* | �ړ���/���� |
	// NowSize = 1.0f + (��Size) * |(X0- X)/ (X0-Xf)|
	//
	//------------------------------------------------------------

	return firstsize - (firstsize - 1.0f) * fabsf((firsty - titleAnim_.pos.y) / (firsty - TITLELOGO_Y));

}

//�v���[�Y�N���b�N�A�j��
void CSceneTitle::PleaseClick()
{
	//�N���b�N�\���A�j��
	switch (clickPhase_)
	{
	case CSceneTitle::ClickPhase::HIDDEN:

		clickPhase_ = ClickPhase::SHOW;
		break;

	case CSceneTitle::ClickPhase::SHOW:
		//�I�t�ɂȂ鎞��
		offTime_ += system::FrameTime;

		//�t�F�[�h����
		if (offTime_ > CLICK_ON)
		{
			offTime_ = 0.f;
			clickPhase_ = ClickPhase::FADEOUT;
		}

		break;

	case CSceneTitle::ClickPhase::FADEOUT:

		clickAlpha_ -= CLICK_DEG_ALPHA;

		if (clickAlpha_ <= CLICK_MIN_ALPHA)
		{
			clickAlpha_ = CLICK_MIN_ALPHA;
			clickPhase_ = ClickPhase::FADEIN;
		}

		break;


	case CSceneTitle::ClickPhase::FADEIN:

		clickAlpha_ += CLICK_DEG_ALPHA;
		if (clickAlpha_ >= 255)
		{
			clickAlpha_ = 255;
			clickPhase_ = ClickPhase::SHOW;
		}

		break;


	default:
		break;
	}


}

//�^�C�g���A�f���ړ����̃t�F�[�h�A�E�g
void CSceneTitle::ChangePhase()
{
	fadeState_ = State::OUTING;
	CFade::StartFadeOut();
}

//�a���A�j���[�V����
void CSceneTitle::SlashAnime()
{
	//�a���A�j��
	switch (slashPhase_)
	{
		//�����ҋ@
	case CSceneTitle::SlashPhase::HIDDEN1:

		slashPhase_ = SlashPhase::SLASH1;
		sePlaying_ = true;
		break;

		//���a��
	case CSceneTitle::SlashPhase::SLASH1:

		//����
		titleSlash_.SetUse(true);
		if (sePlaying_)
		{
			se::DSound_Play("se_shot0");
			sePlaying_ = false;
		}
		if (slashAnim_.step())
		{
			titleSlash_.SetUse(false);
			slashPhase_ = SlashPhase::HIDDEN2;
		}
		titleSlash_.src.x = slashAnim_.no;
		break;

		//���Ԓ�~
	case CSceneTitle::SlashPhase::HIDDEN2:

		nextTime_ += system::FrameTime;

		//���ԂŎ��̃t�F�[�Y��
		if (nextTime_ > SLASH_NEXT_TIME)
		{
			//���a���̏�����
			sePlaying_ = true;
			titleSlash_.pos(SLASH_B_X, SLASH_B_Y, 0.f);
			titleSlash_.angle = SLASH_B_DEG;
			slashPhase_ = SlashPhase::SLASH2;
		}

		break;

		//���a��
	case CSceneTitle::SlashPhase::SLASH2:

		//����
		titleSlash_.SetUse(true);

		if (sePlaying_)
		{
			se::DSound_Play("se_shot0");
			sePlaying_ = false;
		}

		if (slashAnim_.step())
		{
			titleSlash_.SetUse(false);
			animPhase_ = AnimPhase::FLASH;
		}
		titleSlash_.src.x = slashAnim_.no;

		break;

	}
}

//�t���b�V���A�j���[�V����
void CSceneTitle::TitleFlash()
{
	switch (flashPhase_)
	{
	case CSceneTitle::FlashPhase::FADEOUT:
		if (flashFlag_)
		{
			CFade::StartFadeOut();
			flashFlag_ = false;
		}
		if (CFade::FadeOut(255.f / 0.7f * system::FrameTime))
		{
			flashFlag_ = true;
			flashPhase_ = FlashPhase::FADEIN;
		}

		break;
	case CSceneTitle::FlashPhase::FADEIN:

		if (flashFlag_)
		{
			CFade::StartFadeIn();
			flashFlag_ = false;
		}
		if (CFade::FadeIn(255.f / 0.5f * system::FrameTime))
		{
			animPhase_ = AnimPhase::WAIT;

		}

		break;

	default:
		break;
	}

}



#pragma endregion	// CSceneTitle methods
//=================================================================================


/*
//======================================
// CSceneTitle methods
#pragma region CSceneTitle methods
// �R���X�g���N�^
CSceneTitle::CSceneTitle() :
IScene("img_title", "bgm_title")
, Phase_(Phase::TITLE)
{
	CFade::ChangeColor(-1);
#ifdef D_SCALE_TEST
	pt = camera::GetLookAt();
#endif
}
CSceneTitle::~CSceneTitle()
{
	
}


// �`��
void CSceneTitle::draw()
{
	font::Draw_FontTextNC(100, 200, 0.5f, "�^�C�g�����", -1, 0);
	font::Draw_FontTextNC(100, 250, 0.5f, "�{�^��1�ŃV�[���؂�ւ�", -1, 0);
	// �w�i
	__super::draw();
#ifdef D_SCALE_TEST
	auto& point = camera::GetLookAt();
	graph::Draw_Line(point.x - 3, point.y, point.x + 3, point.y, 0.f, -1, 1);
	graph::Draw_Line(point.x, point.y - 3, point.x, point.y + 3, 0.f, -1, 1);
	std::stringstream ss;
	ss << "(" << point.x << ", " << point.y << ")";
	font::Draw_TextXY(point.x, point.y, ss.str(), -1);

	
	graph::Draw_GraphicsLeftTop(0, 0, 0.8f,
		BACK_RESNAME,
		0, 0, 1200, 800);
	graph::Draw_GraphicsLeftTop(0, 0, 9.f,
		BACK_RESNAME,
		0, 0, 1200, 800, 0, 0, 2.f, 2.f, 128);
	graph::Draw_GraphicsLeftTop(0, 0, 0.9f,
		BACK_RESNAME,
		0, 0, 1200, 800, 0, 0, 5.f, 5.f, 128);
	graph::Draw_GraphicsLeftTop(0, 0, 1.f,
		BACK_RESNAME,
		0, 0, 1200, 800,0,0,10.f,6.2f,128);
#endif
}



void CSceneTitle::TitleInit()
{
	phaseTime_ = 0.f;
}


void CSceneTitle::TitleStep()
{
	phaseTime_ += system::FrameTime;
	const float demoTime = 2.f;	// �f���v���C�Ɉړ�����܂ł̎���
	if (phaseTime_ >= demoTime)
	{
		DemoInit();
		Phase_ = Phase::DEMO;	// �f���v���C��Ԃ�
	}
}

void CSceneTitle::DemoInit()
{
	demo_.pos.x = 100.f;
}
void CSceneTitle::DemoStep()
{
	demo_.pos.x += 100.f * system::FrameTime;
	// �f���v���C�I��
	if (demo_.pos.x >= 200.f)
	{
		TitleInit();
		Phase_ = Phase::TITLE;	// �^�C�g����ʂ�
	}
}

// ����
bool CSceneTitle::update()
{
	switch (Phase_)
	{
	case Phase::TITLE:
		// �^�C�g����ʕ\����
		TitleStep();
		break;
	case Phase::DEMO:
		// �f���v���C�\����
		DemoStep();
		break;
	}

#ifdef D_EFFECT_TEST
	if (input::CheckPush(input::KEY_MOUSE_LBTN))
		gm()->AddObject(ObjPtr(new CEffectSlash(gm()->GetCursorPos())));
	if (input::CheckPush(input::KEY_MOUSE_RBTN))
		gm()->AddObject(ObjPtr(new CEffectExplosion(gm()->GetCursorPos())));
#endif

	// �����A�N�V�������N�����ăV�[�����؂�ւ��Ƃ�
	if (input::CheckPush(input::KEY_BTN0))
	{
		return true;
	}

#ifdef D_SCALE_TEST
	using namespace input;
	struct
	{
		input::CODE_KEY code;
		POINT move;
	} a[4] = 
	{
		{ KEY_UP, 0, -3 },
		{ KEY_DOWN, 0, +3 },
		{ KEY_LEFT, -3, 0 },
		{ KEY_RIGHT, +3, 0 },
	};
	for (auto& aa : a)
	{
		if (input::CheckPress(aa.code))
		{
			pt.x += aa.move.x;
			pt.y += aa.move.y;
		}
	}
	camera::SetLookAt(pt.x, pt.y);
#endif

	return false;
}

IScene* CSceneTitle::NextScene()
{
	return new CSceneStageSelect();
}


#pragma endregion	// CSceneTitle methods

//*/