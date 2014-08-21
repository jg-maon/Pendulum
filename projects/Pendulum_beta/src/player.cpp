
#ifdef _DEBUG
//#define D_MOVE_TEST		// �L�[�ňړ�
//#define D_HANG_TEST			// �����Ȃ�
//#define D_GRAVITY_TEST		// �d�͂Ȃ�
//#define D_COLOR_TEST	// HSV�ϊ��e�X�g
//#define D_LOG_TEST	// �ړ��O��
#define D_SCALE_TEST		// �}�E�X�X�P�[�����O
#define D_LINE_TEST		// ��Line�⊮
#endif

#define D_ACT_TEST			// �}�E�X�N���b�N���W�łԂ牺����
#define D_ATK_TEST		// �N���b�N���U������
#define DEF_SHAPE_DRAW

#include "player.h"

#ifndef DEF_DEFINE_H
#include "define.h"
#endif

#ifndef DEF_ATTACKBASE_HPP
#include "attackBase.hpp"
#endif

#include "easing.h"

#ifndef DEF_SETTING_H
#include "setting.h"
#endif

#ifndef DEF_STAGEMNG_H
#include "stageMng.h"
#endif

#include "common.h"

#include "gameManager.h"


#include "effectAfterImage.h"		// �c��
#include "effectSlash.h"

#include "griffon.h"

#ifdef D_GRAVITY_TEST
bool gravityF = false;
#endif

#ifdef _DEBUG
mymath::Vec3f vel_log;	// �ō����x
#endif

#ifdef D_LOG_TEST
#include <deque>
std::deque<POINT> logs;
#endif

#define JUMP_X		(3000.f)		// �W�����v������
#define JUMP_Y		(3000.f)		// �W�����v�c����
#define JUMP_TIME	(1.f)		// �W�����v����
#define JUMP_G		(100.f)		// �W�����v���d��


//Max = (n - 0) + (n - g) + (n - 2g) + �E�E�E + (n - ��g)
//��n = { Max / (�� + 1) }+��g / 2
#define JUMP_VY		(JUMP_Y / (JUMP_TIME/system::ONEFRAME_TIME + 1) + JUMP_TIME/system::ONEFRAME_TIME*JUMP_G/2.f)


#pragma region CPlayer methods

CPlayer::CPlayer():
ICharacter("Player")
{
	status_ = Status::idle;
	attackObjAngle_.resize(3);
}

CPlayer::CPlayer(float x, float y, float z) :
ICharacter("Player")
//	,isHanging(isHanging_)
{
	init(x, y, z);
}

CPlayer::~CPlayer()
{
}

void CPlayer::init(float x, float y, float z)
{
	gm()->GetData(*this);

	startPos_(x, y, z);

	obj_.pos = startPos_;
	obj_.src.x = 0;
	obj_.src.y = static_cast<int>(MotionType::HANG);
	obj_.pos.x -= JUMP_X;
	obj_.add.y = -JUMP_VY + JUMP_G / 2.f;

	prePos_ = obj_.pos;
	isHanging_ = false;
	tensionAcc_ = 0.f;
	gravity_ = 0.f;
	gravityF_ = false;
	hangAcc_ = 0.f;
	attackObj_ = charabase::CharPtr(new charabase::CharBase(
		obj_.pos,
		mymath::Vec3f(),
		"img_attackCircle",
		512, 512));
	attackObj_->pos.z -= 0.1f;
	attackObj_->alpha = 220.f;
	attackObj_->g = 10.f;
	attackObj_->b = 10.f;
	for (auto& angle : attackObjAngle_)
		angle = 0.f;
	attackRadius_ = loadInfo_.maxAttackRadius;
	isAttacking_ = false;

	//invincible_ = false;
	invincibleTime_ = 0.f;
	invincibleAnim_ = 0.f;

	//----------------------------------
	chainState_ = common::DispState::HIDE;
	chainTime_ = 0.f;
	chainAnimTime_ = 0.f;
	chainStaPos_(system::WINW - 300.f, 30.f, 0.1f);
	chainMsg_ = charabase::CharPtr(new charabase::CharBase(
		chainStaPos_,
		mymath::Vec3f(),
		"img_UIchain",
		151, 55));
	chainMsg_->alpha = 0.f;
	numberAnimTime_ = 0.f;

	//----------------------------------
	health_ = loadInfo_.health;
	power_ = loadInfo_.power;
	chainCnt_ = 0;

	//----------------------------------

	//----------------------------------
	// �J�����ړ�
	//sm()->MoveCamera(obj_.pos.x, obj_.pos.y);
	

#ifdef D_RANGE_TEST
	atk_range.radius = loadInfo_.attackRadius;
#endif
}

void CPlayer::step()
{
	__super::step();			// �X�V�t���[�������Z

	mymath::Vec3f& pos = obj_.pos;
	mymath::Vec3f& velocity = obj_.add;

	if (sm()->isEnterAnimating())
	{
		// �o��A�j���[�V����
		EnterAnimation();
	}
	else if (sm()->isExitAnimating())
	{
		// �ޏ�A�j���[�V����
		ExitAnimation();
	}
	else
	{
		// �ʏ펞

		// �L�[���͂̂ݐ����󂯕t��
		key();

		// Chain����
#pragma region Chain����
		{
			const float appearTime = 0.5f;		// �o���A�j���[�V�����b��
			const float appearMoveX = 30.f;		// X�ړ���
			const float disappearTime = 0.2f;	// �����A�j���[�V�����b��
			switch (chainState_)
			{
			case common::DispState::HIDE:
				/*
				if ((chainTime_ -= system::ONEFRAME_TIME) <= 0.f)
				{
					// ����
					chainState_ = common::DispState::HIDE;
					chainAnimTime_ = 0.f;
					chainCnt_ = 0;
				}
				//*/
				break;

			case common::DispState::APPEARING:
				chainMsg_->alpha = Easing::Linear(chainAnimTime_, 0.f, 255.f, appearTime);
				chainMsg_->pos.x = Easing::QuadOut(chainAnimTime_, chainStaPos_.x, appearMoveX, appearTime);

				if ((chainAnimTime_ += system::FrameTime) >= appearTime)
				{
					// �o������
					chainState_ = common::DispState::SHOW;
				}
				// �c�莞�Ԃ͏o�������������Ă�������break�͏����Ȃ�
			case common::DispState::SHOW:
				if (chainTime_ <= disappearTime)
				{
					// �����J�n
					chainState_ = common::DispState::DISAPPEARING;
					chainAnimTime_ = 0.f;
				}
				numberPos_.x = chainMsg_->pos.x;
				numberPos_.y = Easing::BackOut(numberAnimTime_, chainStaPos_.y + 10.f, -20.f, 0.2f);
				numberAnimTime_ += system::FrameTime;

				if (chainState_ == common::DispState::APPEARING) break;
				// SHOW�݂̂̏���
				chainMsg_->alpha = 255.f;
				chainMsg_->pos.x = chainStaPos_.x + appearMoveX;
				break;

			case common::DispState::DISAPPEARING:
				chainMsg_->alpha = Easing::Linear(chainAnimTime_, 255.f, -255.f, disappearTime);
				chainMsg_->pos.x = Easing::QuadOut(chainAnimTime_, chainStaPos_.x + appearMoveX, 10.f, disappearTime);
				if ((chainAnimTime_ += system::FrameTime) >= disappearTime)
				{
					// ��������
					chainState_ = common::DispState::HIDE;
					chainMsg_->pos = chainStaPos_;
					chainMsg_->alpha = 0.f;
					chainAnimTime_ = 0.f;
					chainCnt_ = 0;
				}
				break;
			}
		}
#pragma endregion // Chain����


		// �X�V�t���[���łȂ��ꍇ�̓X�L�b�v
		if (!isUpdatable())
		{
			if (gm()->gameStatus()->isHitStopping())
				attackRadius_ = -50.f;
			return;
		}
		
		//----------------------------------------
		// Chain
		// Chain���Ԃ͏��������Ȃǂɉe������āA���̃Q�[�����b���ŏI���
		chainTime_ -= system::ONEFRAME_TIME;

		//----------------------------------------
		// �ړ�����
		move();

		//----------------------------------------
		// �U���͈�
		if (attackRadius_ < loadInfo_.maxAttackRadius)
			attackRadius_ += loadInfo_.addRadius * system::FrameTime;
		else
			attackRadius_ = loadInfo_.maxAttackRadius;

		//----------------------------------------
		// �J����
		if (isCameraZoom_)
		{
			if ((zoomTime_ -= system::ONEFRAME_TIME) <= 0.f)
			{
				float s = camera::GetScale();
				s = min(1.f, (s + 0.02f));
				camera::SetScale(s);
				zoomTime_ = 0.f;
			}
		}
		sm()->MoveCamera(obj_.pos);
	}

	//----------------------------------------
	// �U���͈�
	UpdateAttackObject();

	//----------------------------------------
	// �A�j���[�V��������
	if (motionAnim_.step())
	{
		obj_.src.y = static_cast<int>(MotionType::FALL);
		obj_.src.x = 0;
	}
	else
	{
		obj_.src.x = motionAnim_.no;
	}

	//----------------------------------------
	// ���G
	if (invincibleTime_ > 0.f)
	{
		// �_�ŃA�j��
		invincibleAnim_ += system::ONEFRAME_TIME;
		if (invincibleAnim_ >= loadInfo_.invincibleTime / 20.f)
		{
			invincibleAnim_ = 0.f;
			if (obj_.alpha > 200.f)
				obj_.alpha = 150.f;		// ����
			else
				obj_.alpha = 220.f;		// �o��
		}
		// ���Ԍ���
		invincibleTime_ -= system::ONEFRAME_TIME;
		if (invincibleTime_ < 0.f)
		{
			// ���G�I��
			obj_.alpha = 255.f;
		}
	}

	//----------------------------------------
	// �U�����
	//obj_.scale.x = (velocity.x > 0.f) ? 1.f : -1.f;
	if (!turnFlag_ && velocity.x < 0.f)
		turnFlag_ ^= 1;
	else if (turnFlag_ && velocity.x > 0.f)
		turnFlag_ ^= 1;

#ifdef D_COLOR_TEST
	static float c_cnt = 0.f;
	auto color = Color_ARGBtoAHSV(attackRange_->alpha,
		attackRange_->r,
		attackRange_->g,
		attackRange_->b);

	if ((c_cnt += FrameTime) > 0.5f)
	{
		c_cnt = 0.f;
		color = Color_AddHChannel(color, 5);
	}
	color = Color_ARGBFromAHSV(color);
	Color_SeparateARGB(color,
		attackRange_->alpha,
		attackRange_->r,
		attackRange_->g,
		attackRange_->b);
#endif

}



void CPlayer::draw()
{
	const mymath::Vec3f& pos = obj_.pos;

#ifdef D_LOG_TEST
	//charabase::CharBase b = obj_;
	for (const auto& log : logs)
	{
		//b.pos.x = log.x;
		//b.pos.y = log.y;
		//b.pos.z = obj_.pos.z - 0.1f;
		//b.alpha = 120.f;
		//b.draw();
		graph::Draw_BoxCenter(log.x, log.y, 4, 4, pos.z - 0.1f,
			0xffff0000, 0xffff0000, 1, true);
	}
#endif

#if defined(D_RANGE_TEST) && !defined(D_ATK_TEST)
	mymath::Circlef& circle_ = atk_range;
	D3DCOLOR color = (isAttacking_) ? 0xfff0f020 : 0xa0efefef;
	circle_.draw(color);
#endif


#ifdef _DEBUG
	static const std::string emply_string;
	mymath::Recti rc = obj_.GetRect();

	//rc.offset((int)obj_.pos.x, (int)obj_.pos.y);
	std::stringstream ss;
	ss << "pos("
		<< std::setw(4) << (int)pos.x
		<< ","
		<< std::setw(4) << (int)pos.y
		<< ")";
	font::Draw_FontText(
		rc.right, rc.top, 0.f,
		ss.str(), 0xffff0000,
		setting::GetFontID("font_MSG15"));

	mymath::Vec3i pt = gm()->GetCursorPos();
	font::Draw_FontText(
		pt.x, pt.y, 0.f,
		ss.str(), 0xffff0000,
		setting::GetFontID("font_MSG15"));

	ss.str(emply_string);
	ss << "mouse("
		<< std::setw(4) << (int)pt.x
		<< ","
		<< std::setw(4) << (int)pt.y
		<< ")";
	font::Draw_FontText(
		pt.x, pt.y + font::Draw_GetCharHeight(setting::GetFontID("font_MSG15")) * 1, 0.f,
		ss.str(), 0xffff0000,
		setting::GetFontID("font_MSG15"));

	ss.str(emply_string);
	const mymath::Vec3f& velocity = obj_.add;
	ss << "add("
		<< std::setw(4) << (int)velocity.x
		<< ","
		<< std::setw(4) << (int)velocity.y
		<< ")";
	font::Draw_FontText(
		rc.right, rc.top + font::Draw_GetCharHeight(setting::GetFontID("font_MSG15")) * 1, 0.f,
		ss.str(), 0xffff0000,
		setting::GetFontID("font_MSG15"));

	ss.str(emply_string);
	mymath::Vec3f dist = hangPoint_ - pos;
	float angle = std::atan2f(dist.y, dist.x);
	mymath::Vec3f tensionForce = mymath::Vec3f::Rotate(angle) * tensionAcc_;
	ss << "tensionForce("
		<< std::setw(4) << (int)tensionForce.x
		<< ","
		<< std::setw(4) << (int)tensionForce.y
		<< ")";
	font::Draw_FontText(
		rc.right, rc.top + font::Draw_GetCharHeight(setting::GetFontID("font_MSG15")) * 2, 0.f,
		ss.str(), 0xffff0000,
		setting::GetFontID("font_MSG15"));

	ss.str(emply_string);
	ss << "tensionAcc_:"
		<< std::setw(4) << (int)tensionAcc_;
	font::Draw_FontText(
		rc.right, rc.top + font::Draw_GetCharHeight(setting::GetFontID("font_MSG15")) * 3, 0.f,
		ss.str(), 0xffff0000,
		setting::GetFontID("font_MSG15"));

	ss.str(emply_string);
	ss << "vel_log("
		<< std::setw(4) << (int)vel_log.x
		<< ","
		<< std::setw(4) << (int)vel_log.y
		<< ")";
	font::Draw_FontText(
		rc.right, rc.top + font::Draw_GetCharHeight(setting::GetFontID("font_MSG15")) * 4, 0.f,
		ss.str(), 0xffff0000,
		setting::GetFontID("font_MSG15"));

	/*
	mymath::Recti arc = attackRange_->GetRect();
	ss.str(emply_string);
	ss << "ARGB(" << std::setw(3) << (int)attackRange_->alpha
	<< "," << std::setw(3) << (int)attackRange_->r
	<< "," << std::setw(3) << (int)attackRange_->g
	<< "," << std::setw(3) << (int)attackRange_->b
	<< ")";
	font::Draw_FontText(
	arc.left, arc.bottom, 0.f,
	ss.str(), -1,
	setting::GetFontID("font_MSG15"));
	//*/

#endif


	//-----------------------------------------------------------
	// AttackRange
	if (attackObj_ && attackRadius_ >= 0.f)
		DrawAttackObject();

	/*
	mymath::Circlef circle_(obj_.pos, attackRadius_);
	circle_.draw(ARGB(
		static_cast<int>(attackObj_->alpha),
		static_cast<int>(attackObj_->r),
		static_cast<int>(attackObj_->g),
		static_cast<int>(attackObj_->b)));
	//*/
	//-----------------------------------------------------------
	// Chain
	if (chainState_ != common::DispState::HIDE)
	{
		// Chain��
		chainMsg_->drawNC(charabase::CharBase::LeftTop);
		// Chain�����摜�p�^�[����
		const int width = 32;
		const int height = 64;
		charabase::CharBase numBase(numberPos_, mymath::Vec3f(), "img_UInumber", width, height);
		auto numObjs = common::GetNumberImageObjects(chainCnt_, numBase, common::RIGHT);
		for (auto& nobj : numObjs)
			nobj.drawNC(charabase::CharBase::LeftTop);
		/*
		int digit = (chainCnt_ / 100 == 0) ? ((chainCnt_ / 10 == 0) ? 1 : 2) : 3;
		int work = chainCnt_;	// chain���������Ƃɋ�؂�
		for (int i = 1; i <= digit; ++i)
		{
			// ���ʌ����珇�ɕ`��(Chain�����̍����猸�炵�Ă���)
			graph::Draw_GraphicsLeftTopNC(
				numberPos_.x - i*width,
				numberPos_.y - 10.f,
				numberPos_.z,
				"img_UInumber",
				(work % 10) * width, 0,
				width, height);
			work /= 10;
		}
		//*/
	}

	//-----------------------------------------------------------
	// �̗̓Q�[�W
	{
		enum
		{
			draw_x = 30,
			draw_y = 30,
			width = 400,
			height = 45,
		};
		//------------------------
		// �g
		graph::Draw_BoxNC(
			draw_x, draw_y,
			draw_x + width,
			draw_y + height, 0.2f,
			ARGB(200, 10, 10, 10), ARGB(255, 20, 100, 100),
			5, true);
		//------------------------
		// ��
		// �̗͂ɉ����ĕ����v�Z
		float w = static_cast<float>(health_) / static_cast<float>(loadInfo_.health) * width;
		// ������
		graph::Draw_BoxNC(
			draw_x, draw_y,
			draw_x + static_cast<int>(w),
			draw_y + height, 0.1f,
			ARGB(255, 100, 200, 200)
			, ARGB(255, 50, 150, 150),
			0, true);
		// �㔼��
		graph::Draw_BoxNC(
			draw_x, draw_y,
			draw_x + static_cast<int>(w),
			draw_y + height / 2, 0.1f,
			ARGB(255, 45, 150, 170), ARGB(255, 50, 150, 150),
			0, true);

	}
	//-----------------------------------------------------------
	// �v���C���[�摜
	obj_.draw(charabase::CharBase::MODE::Center, turnFlag_);
	//-----------------------------------------
	// ���r+��
	{
		// �r
		float z = pos.z;
		SIZE armsz = graph::Draw_GetImageSize2(loadInfo_.armImg);

		float x;	// ���]����
		if (turnFlag_)
		{
			// �������̎��͑̂���O�ɕ`��
			z -= 0.1f;
			// ���]
			x = -1.f;
		}
		else
		{
			// �E�����̎��͑̂�艜�ɕ`��
			z += 0.1f;
			x = 1.f;
		}

		// �����ɂ͌�
		mymath::Vec3f armPos(
			pos.x + (loadInfo_.armX) * x,
			pos.y + (loadInfo_.armY),
			z);
		POINT c = { 
			static_cast<LONG>(loadInfo_.armRotateX),
			static_cast<LONG>(loadInfo_.armRotateY) };
		float angle = (isHanging_) ? math::DegreeOfPoints2(armPos.x, armPos.y, hangPoint_.x, hangPoint_.y) : 270.f;

		graph::Draw_GraphicsLeftTop(
			armPos.x,
			armPos.y,
			armPos.z,
			loadInfo_.armImg,
			0, 0, armsz.cx, armsz.cy,
			angle, &c);

		//----------------------------------------------
		// ��
		if (isHanging_)
		{
			mymath::Vec3f vec = armPos - hangPoint_;
			angle = math::DegreeOfPoints2(0.f, 0.f, vec.x, vec.y);
			SIZE chainsz = graph::Draw_GetImageSize2(loadInfo_.chainImg);
			POINT c = { 0, chainsz.cy / 2 };
			float sx = 1.f;	// �g�k��
			if (mymath::PYTHA(vec.x, vec.y) < mymath::PYTHA(chainsz.cx, chainsz.cy))
			{
				// �摜�̕���؂�
				chainsz.cx = static_cast<LONG>(vec.Length2());
			}
			else
			{
				// �摜�̊g�嗦��ς���
				sx = vec.Length2() / static_cast<float>(chainsz.cx);
			}
			graph::Draw_GraphicsLeftTop(
				hangPoint_.x,
				hangPoint_.y,
				z + 0.1f, "img_chain",
				0, 0, chainsz.cx, chainsz.cy,
				angle, &c,
				sx);
#ifdef D_LINE_TEST
			graph::Draw_Line(
				static_cast<int>(hangPoint_.x),
				static_cast<int>(hangPoint_.y),
				static_cast<int>(armPos.x),
				static_cast<int>(armPos.y),
				pos.z - 0.2f,
				0x7fffffff, 2);
#endif
		}
	}
}

void CPlayer::EnterAnimation()
{
	// ������
	obj_.add.x = (JUMP_X / JUMP_TIME);
	// �c����
	obj_.add.y += JUMP_G;

	obj_.pos += obj_.add * system::ONEFRAME_TIME;
	//obj_.Move();

#ifdef _DEBUG
	static int i = 0;
	++i;
#endif
	if (obj_.add.y > 0.f && obj_.pos.y > startPos_.y)
	{
#ifdef _DEBUG
		i = i;
#endif
		obj_.pos = startPos_;
		obj_.add = 0.f;
		obj_.src.y = static_cast<int>(MotionType::FALL);
		sm()->setStageState(CStageMng::StageState::BATTLE);
	}
	else
	{
		sm()->MoveCamera(startPos_);
	}
}

void CPlayer::ExitAnimation()
{
	sm()->setStageState(CStageMng::StageState::ENTER);
}

void CPlayer::key()
{
	using namespace input;
	POINT pt = camera::GetCursorPosition();
	mymath::Vec3f mouse(float(pt.x), float(pt.y), obj_.pos.z);


	//-------------------------------------------
	// �ړ�
	if (CheckPush(MOVE_BTN))
	{
#ifndef D_MOVE_TEST
#ifdef D_ACT_TEST
		SetHangPoint(mouse);
#else
		const auto& aps = sm()->getActionPoints();
		for(const auto& act : aps)
		{
			if (act->Contains(mouse) && act->Contains(obj_.pos, mouse))
			{
				SetHangPoint(act->IntersectionPoint2Nearest(obj_.pos, mouse));
				break;
			}
		}
#endif
#endif
	}
#ifdef D_SCALE_TEST
	float s = camera::GetScale();
	WHEEL w = input::GetWheelRoll();
	switch (w)
	{
	case gplib::MWHEEL_UP:
		s -= 0.1f;
		break;
	case gplib::MWHEEL_NEUTRAL:
		break;
	case gplib::MWHEEL_DOWN:
		s += 0.1f;
		break;
	}
	camera::SetScale(s);
#endif
#ifdef D_HANG_TEST
	if (CheckPress(KEY_BTN2))
	{
		isHanging_ = true;
	}
	else
	{
		//isHanging_ = false;
	}
#else

	if (isHanging_ && CheckPull(MOVE_BTN))
	{
		// �͂�ł��Ԃ��痣������
		UnHang();
	}
#endif

#ifdef D_RANGE_TEST
	atk_range.center = obj_.pos;
#endif

	//-------------------------------------------
	// �U��
	isAttacking_ = false;
	// �q�b�g�X�g�b�v�����ŏ�����ς���
	if (gm()->gameStatus()->isHitStopping())
	{
		// �q�b�g�X�g�b�v���͈ړ�or�U���{�^������
		if (CheckPush(MOVE_BTN) || CheckPush(ATK_BTN))
		{
			/*
#ifdef D_ATK_TEST
			isAttacking_ = true;
#elif defined(D_RANGE_TEST)
			isAttacking_ = atk_range.Contains(mouse);
#else
			mymath::Circlef range(obj_.pos, attackRange_->halfWidth());
			isAttacking_ = range.Contains(mouse);
#endif
			//*/
			isAttacking_ = true;
		}
	}
	// �q�b�g�X�g�b�v���ȊO�͍U���{�^������
	else if (CheckPress(ATK_BTN))
	{
		isAttacking_ = true;
	}

#ifdef _DEBUG
	if (CheckPush(KEY_SPACE))
	{
		obj_.pos.x = 0.f;
		obj_.pos.y = 0.f;
		obj_.add = 0.f;
		prePos_ = obj_.pos;
	}
#endif

#ifdef D_GRAVITY_TEST
	if ( (CheckPush(KEY_LEFT))	|| (CheckPush(KEY_RIGHT)) ||
		(CheckPush(KEY_UP))		|| (CheckPush(KEY_DOWN)) )
	{
		gravityF = false;	
		obj_.add = 0.f;
	}
#endif

#ifdef D_MOVE_TEST
	if (CheckPress(KEY_LEFT))
	{
		obj_.pos.x -= 10.f;
		turnFlag_ = true;
	}
	if (CheckPress(KEY_RIGHT))
	{
		obj_.pos.x += 10.f;
		turnFlag_ = false;
	}
	if (CheckPress(KEY_UP))
	{
		obj_.pos.y -= 10.f;
	}
	if (CheckPress(KEY_DOWN))
	{
		obj_.pos.y += 10.f;
	}
#endif

}


void CPlayer::move()
{
	mymath::Vec3f& pos = obj_.pos;
	mymath::Vec3f& velocity = obj_.add;
	
#ifdef D_LOG_TEST
	POINT p = { (LONG)obj_.pos.x,
		(LONG)obj_.pos.y};
	if (!logs.empty())
	{
		if (logs.back() != p)
			logs.push_back(p);
		if (logs.size() > 120)
		{
			logs.pop_front();
		}
	}
	else
	{
		logs.push_back(p);
	}
#endif
	// �d��
	if (gravityF_)
	{
		gravity_ += gm()->gameStatus()->getEnv().gravityAcc;
		if (gravity_ >= loadInfo_.MAX_G)
		{
			gravity_ = loadInfo_.MAX_G;
		}
		velocity.y += gravity_;
	}

	if (isHanging_)
	{
		// �Ԃ牺���蒆
		gravityF_ = true;
		mymath::Vec3f dist = hangPoint_ - pos;	// pos -> hang

		const float min_radius = 150.f;	// �ŏ�����(�U��q�̔��a)

		float angle = std::atan2f(dist.y, dist.x);
		mymath::Vec3f tensionForce = mymath::Vec3f::Rotate(angle) * tensionAcc_;
		velocity += tensionForce;
		tensionAcc_ *= loadInfo_.DOWN_TENSION;

		const float CAMERA_SCALE_MIN = 0.75f;
		if (mymath::POW2(tensionAcc_) < mymath::POW2(0.5f))
		{
			// ���������Ȃ��Ȃ��Ă����
			tensionAcc_ = 0.f;
			if (!isCameraZoom_)
				zoomTime_ = ZOOM_TIME / 10.f;
			isCameraZoom_ = true;
		}
		else
		{
			// ���������Ă��
			isCameraZoom_ = false;
			float s = camera::GetScale();
			s = max(CAMERA_SCALE_MIN, (s - 0.04f));
			camera::SetScale(s);
		}

#ifdef _DEBUG
		if (mymath::PYTHA(velocity.x, velocity.y) > mymath::PYTHA(vel_log.x, vel_log.y))
			vel_log = velocity;
#endif

		{/*
			hangAcc_ *= 0.98f;
			if (hangAcc_ < 1.f)
			{
			hangAcc_ = 0.f;
			obj_.velocity = 0.f;
			}*/
			hangAcc_ += gm()->gameStatus()->getEnv().gravityAcc;
			if (hangAcc_ > loadInfo_.MAX_G)
			{
				hangAcc_ = loadInfo_.MAX_G;
			}

			if (mymath::PYTHA(dist.x, dist.y) > mymath::POW2(min_radius))
			{
				// ��t���[�����̈ʒu(�d�͕�)
				mymath::Vec3f nextPos(pos.x, pos.y + hangAcc_, pos.z);
				mymath::Vec3f intersection = mymath::Circlef(hangPoint_, min_radius).IntersectionPoint2Nearest(nextPos, hangPoint_);

				// ���������ɐL�΂����Î~�_
				mymath::Vec3f stopPos(hangPoint_.x, hangPoint_.y + min_radius, hangPoint_.z);
				mymath::Vec3f isVec = stopPos - intersection;
				mymath::Vec3f psVec = stopPos - pos;
				if (mymath::PYTHA(isVec.x, isVec.y) < mymath::POW2(2.f)
					&& mymath::PYTHA(psVec.x, psVec.y) < mymath::POW2(2.f))
				{
					// �������ɂ߂ċ߂���ΐÎ~
					//velocity = stopPos - pos;
					pos = stopPos;
					velocity = 0.f;
				}
				else
				{
					// �U��q���^��
					velocity += hangPoint_ - nextPos;
				}
			}
		}


	}
	else
	{
		// ��
#ifdef D_GRAVITY_TEST
		if (gravityF)
#endif
		{
			tensionAcc_ = 0.f;

		}
	}
	//----------------------------------------
	// �ړ����x����
	if (velocity.x > loadInfo_.MAX_VX)
	{
		velocity.x = loadInfo_.MAX_VX;
	}
	else if (velocity.x < -loadInfo_.MAX_VX)
	{
		velocity.x = -loadInfo_.MAX_VX;
	}
	if (velocity.y > loadInfo_.MAX_VY)
	{
		velocity.y = loadInfo_.MAX_VY;
	}
	else if (velocity.y < -loadInfo_.MAX_VY)
	{
		velocity.y = -loadInfo_.MAX_VY;
	}
	if (mymath::PYTHA(velocity.x, velocity.y) < mymath::POW2(1.f / system::FrameTime))
	{
		velocity = 0.f;
	}
	else
	{
#ifndef D_MOVE_TEST
		MoveObject();
#endif
		velocity *= loadInfo_.DOWN_SPEED;
	}
	// �X�e�[�W���W����
	{
		auto& col = std::dynamic_pointer_cast<mymath::Rectf>(stageCollisions_[0]);
		mymath::Vec3f size;
		size.x = col->right - col->left;
		size.y = col->bottom - col->top;
		//const mymath::Vec3f size = obj_.size / 2;
		const auto& stageRect = sm()->getStageRect();
		pos.x = clamp(pos.x, stageRect.left + size.x, stageRect.right - size.x);
		pos.y = clamp(pos.y, stageRect.top + size.y, stageRect.bottom - size.y);
	}

}


void CPlayer::UpdateAttackObject()
{
	// ���W
	attackObj_->pos.x = obj_.pos.x;
	attackObj_->pos.y = obj_.pos.y;

	// �傫��
	attackObj_->scale = attackRadius_ * 2.f / 400.f;
	
	// �p�x
	float dt = system::FrameTime;
	if((attackObjAngle_[0] += dt * 30.f) >= 360.f)
		attackObjAngle_[0] -= 360.f;
	if ((attackObjAngle_[1] += dt * -90.f) < 0.f)
		attackObjAngle_[1] += 360.f;
	if ((attackObjAngle_[2] += dt * 90.f) >= 360.f)
		attackObjAngle_[2] -= 360.f;
}

void CPlayer::DrawAttackObject() const
{
	graph::Draw_SetRenderMode(RENDER_MODE::HALFADD);
	// �p�x��
	for (size_t i = 0; i < attackObjAngle_.size(); ++i)
	{
		// �ҏW�p�I�u�W�F�N�g
		auto work = *attackObj_;
		// �摜�ԍ�
		work.src.x = static_cast<int>(i);
		// �p�x
		work.angle = attackObjAngle_[i];
		//work.draw(work.Center, turnFlag_);
		work.draw();
	}
	graph::Draw_EndRenderMode();
}



void CPlayer::hit(const ObjPtr& rival)
{
	// Polygon
	__super::hit(rival);
	if (rival->FindName("Polygon"))
	{
		// �␳��̍��W�ɃJ�������ړ�������
		sm()->MoveCamera(obj_.pos, 10.f);
	}
	if (rival->FindName("Atk_") || rival->FindName("Griffon"))
	{
		if (rival->FindName("Atk_"))
		{
			// �G����̍U��
			health_ -= std::dynamic_pointer_cast<IAttack>(rival)->GetForce();
		}
		else
		{
			// �O���t�H���^�b�N��
			auto& gri = std::dynamic_pointer_cast<CGriffon>(rival);
			// �O���t�H���^�b�N�����łȂ��ꍇ�̓_���[�W�����n���X�L�b�v����
			if (!gri->isAttacking()) return;
			health_ -= gri->getPower();
		}
		// �Ԃ牺�����Ԃ�ł�����
		UnHang();
		if (health_ <= 0)
		{
			health_ = 0;
			// �Q�[���I�[�o�[
			//kill();	
		}
		else
		{
			obj_.src.y = static_cast<int>(MotionType::DAMAGE);
			obj_.src.x = 0;
			motionAnim_.set(2, 0.1f);
			invincibleTime_ = loadInfo_.invincibleTime;
		}
	}
	else if (rival->FindName("PickupJewely"))
	{
		// �s�b�N�A�b�v���
		const auto& jewely = std::dynamic_pointer_cast<CPickupJewely>(rival);
		// �X�R�A���Z
		auto& sm = std::dynamic_pointer_cast<CScoreMng>(gm()->GetObj(typeid(CScoreMng)));
		sm->AddScore(jewely->getScore());
	}
}


Base::Collisions CPlayer::GetDamageAreas() const
{
	if (isInvincible() || gm()->gameStatus()->isHitStopping())
	{
		// ���G�������͓����蔻�������
		return Base::Collisions();
	}
	return __super::GetDamageAreas();
}

Base::Collisions CPlayer::GetAttackAreas() const
{
	Base::Collisions cols(1);
	cols[0] = mymath::ShapefPtr(new mymath::Circlef(obj_.pos, attackRadius_));
	return cols;
}

ObjPtr CPlayer::GetPtr()
{
	extern CGameManager *gm;
	return gm->GetPlayer();
	/*
	auto& pl = gm->GetObjects("Player");
	if (pl.empty()) return nullptr;
	return pl[0];
	//*/
}

bool CPlayer::isAttacking() const
{
	return isAttacking_;
}

bool CPlayer::isInvincible() const
{
	return invincibleTime_ > 0.f;
}

void CPlayer::SetInfo(const LoadInfo& info)
{
	loadInfo_ = info;
}


void CPlayer::SetHangPoint(const mymath::Vec3f& pos)
{
	gm()->gameStatus()->EndHitStop();
	hangPoint_ = pos;
	gravityF_ = true;
	tensionAcc_ = loadInfo_.TENSION;
	isHanging_ = true;
	motionAnim_.set(0, 0.f);
	obj_.src.y = static_cast<int>(MotionType::HANG);
}

void CPlayer::UnHang()
{
	isHanging_ = false;
	if (!isCameraZoom_)
		zoomTime_ = ZOOM_TIME / 10.f;
	isCameraZoom_ = true;
	gravity_ = 0.f;
	gravityF_ = true;
	motionAnim_.set(2, 0.2f);
	obj_.src.y = static_cast<int>(MotionType::FALL);
}

/*
bool CPlayer::ApplyDamage(int dam)
{
	isHanging_ = false;
	health_ -= dam;
	if (health_ <= 0)
	{
		health_ = 0;
		return true;
	}
	return false;
}
//*/

void CPlayer::ApplyAttack(CPlayer::AttackType type, const mymath::Vec3f& pos)
{
	//---------------------------------
	// �q�b�g�X�g�b�v�J�n
	gm()->gameStatus()->BeginHitStop();

	//---------------------------------
	// �̓�����͈͏k��
	attackRadius_ = -50.f;

	// ��������̃x�N�g��
	mymath::Vec3f vec = pos - obj_.pos;

	//---------------------------------
	// ����̕�������
	turnFlag_ = vec.x < 0.f;

	//---------------------------------
	// �A�j���[�V����
	obj_.src.y = static_cast<int>(MotionType::ATTACK);
	obj_.src.x = 0;
	motionAnim_.set(2, 0.25f);

	//---------------------------------
	// �a��U�����c������
	if (type == AttackType::SLASH)
	{
		// �c���Ă���c���̍폜
		auto& afterImages = gm()->GetObjects("EffectAfterImage");
		for (auto& img : afterImages)
			img->kill();
		// �V�����c���̒ǉ�
		gm()->AddObject(ObjPtr(new CEffectAfterImage(obj_, turnFlag_, obj_.pos, pos, 10)));
	}

	//---------------------------------
	// �a���G�t�F�N�g
	gm()->AddObject(ObjPtr(new CEffectSlash(pos.TmpReplace(mymath::Vec3f::Z,pos+0.1f),
		math::DegreeOfPoints2(
		obj_.pos.x, obj_.pos.y,
		pos.x, pos.y))));

	//---------------------------------
	// �ړ�
	obj_.pos.x = pos.x;
	obj_.pos.y = pos.y;
	sm()->MoveCamera(obj_.pos);	// �ړ�����W�ɃJ��������������


	//---------------------------------
	// �G�����֐i�ރt�H���[�X���[
	obj_.add = vec / system::FrameTime;

	//---------------------------------
	// �t���O
	gravityF_ = true;
	isHanging_ = false;


}

void CPlayer::KilledEnemy()
{

	//---------------------------------
	// Chain
	if (chainTime_ > 0.f)
	{
		// Chain��
		chainTime_ = loadInfo_.CHAIN_TIME[1];
	}
	else
	{
		// ����Kill
		chainTime_ = loadInfo_.CHAIN_TIME[0];
	}
	chainCnt_++;
	if (chainCnt_ > MAX_CHAIN)
		chainCnt_ = MAX_CHAIN;
	if (chainCnt_ >= 2)
	{
		numberAnimTime_ = 0.f;
		switch (chainState_)
		{
		case common::DispState::HIDE:
		case common::DispState::APPEARING:
			// �o���J�n
			chainState_ = common::DispState::APPEARING;
			chainAnimTime_ = 0.f;
			break;
		case common::DispState::SHOW:
		case common::DispState::DISAPPEARING:
			// �\���p��
			chainState_ = common::DispState::SHOW;
			break;
		}
	}
	// ���K
	std::stringstream slash;
	slash << "se_slash" << std::setw(2) << std::setfill('0') <<
		((chainCnt_ <= MAX_SLASH) ? chainCnt_ : MAX_SLASH);
	se::DSound_Play(slash.str());
}


int CPlayer::getChain() const
{
	return chainCnt_;
}

bool CPlayer::isNoDamage() const
{
	return health_ == loadInfo_.health;
}

#pragma endregion // CPlayer methods