
#ifdef _DEBUG
//#define D_MOVE_TEST		// �L�[�ňړ�
//#define D_HANG_TEST			// �����Ȃ�
//#define D_GRAVITY_TEST		// �d�͂Ȃ�
//#define D_COLOR_TEST	// HSV�ϊ��e�X�g
//#define D_LOG_TEST	// �ړ��O��
#endif
#define D_ACT_TEST			// �}�E�X�N���b�N���W�łԂ牺����
#define D_ATK_TEST	// �U���͈͊g��
#define USE_CIRCLE_EXT

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

const float CPlayer::GRAVITY_ACC = 25.f;	// �ő�d�͑��x
const float CPlayer::MAX_G = 100.f;			// �d�͉����x
const float CPlayer::TENSION = 1500.f;		// �t�b�N�̒���(����)
const float CPlayer::DOWN_TENSION = 0.85f;	// ���͌�����
const float CPlayer::DOWNSP = 0.92f;		// �ړ�������(1-DOWNSP)
const float CPlayer::MAX_VY = 1500.f;		// ���������̍ő呬�x(�Q�[���Ƃ��Đ��藧�o�����X�����p)
const float CPlayer::MAX_VX = 2500.f;		// ���������̍ő呬�x(�Q�[���Ƃ��Đ��藧�o�����X�����p)

const float CPlayer::CHAIN_TIME[2] = {1.0f,2.0f};	// Chain�P�\����[0]:1�̖ڂ���2�̖ڂ܂� [1]:����ȍ~
const int CPlayer::MAX_CHAIN = 999;		// �ō�Chain��


#pragma region CPlayer methods

CPlayer::CPlayer(const mymath::Vec3f& pos):
	ICharacter("Player")
//	,isHanging(isHanging_)
	,isAttacking(isAttacking_)
	,power(power_)
{
	obj_.pos = pos;
	init();
}
CPlayer::CPlayer(float x, float y, float z):
	ICharacter("Player")
//	,isHanging(isHanging_)
	,isAttacking(isAttacking_)
	,power(power_)
{
	obj_.pos.x = x;
	obj_.pos.y = y;
	obj_.pos.z = z;
	init();
}

CPlayer::~CPlayer()
{
}

void CPlayer::init()
{
	obj_.add = 0.f;
	obj_.resname = "img_player";
	obj_.size(128,128);
	prePos_ = obj_.pos;
	isHanging_ = false;
	tensionAcc_ = 0.f;
	gravity_ = 0.f;
	gravityF_ = false;
	hangAcc_ = 0.f;
	attackRange_ = charabase::CharPtr(new charabase::CharBase(
			obj_.pos,
			mymath::Vec3f(),
			"img_circle",
			96,96));
	attackRange_->pos.z -= 0.1f;
	attackRange_->g = 0.f;
	attackRange_->b = 0.f;
	isAttacking_ = false;
	
	//----------------------------------
	chainState_ = ChainState::HIDE;
	chainTime_ = 0.f;
	chainAnimTime_ = 0.f;
	chainStaPos_(system::WINW-300.f, 30.f, 0.1f);
	chainMsg_ = charabase::CharPtr(new charabase::CharBase(
			chainStaPos_,
			mymath::Vec3f(),
			"img_chain",
			151,55));
	chainMsg_->alpha = 0.f;
	numberAnimTime_ = 0.f;
	
	//----------------------------------
	health_ = 100;
	power_ = 5;
	chainCnt_ = 0;
	
	//----------------------------------
	collisions_.clear();
	mymath::Rectf rc;
	rc.left		= -5.f;
	rc.top		= -5.f;
	rc.right	= +5.f;
	rc.bottom	= +5.f;
	collisions_.push_back(mymath::ShapefPtr(new mymath::Rectf(rc)));
	
	
	//----------------------------------
	// �J�����ړ�
	{
		mymath::Vec3f& pos = obj_.pos;
		mymath::Vec3f cameraPos;
		//const auto& sm = std::dynamic_pointer_cast<CStageMng>(gm()->GetObj(typeid(CStageMng)));
		const auto& sm = CStageMng::GetPtr();
		cameraPos.x = clamp(pos.x, (sm->rect.left + system::WINW)/2.f, sm->rect.right);
		cameraPos.y = clamp(pos.y, (sm->rect.top  + system::WINH)/2.f, sm->rect.bottom);
		camera::SetLookAt(cameraPos.x, cameraPos.y);
	}
}



#ifdef D_GRAVITY_TEST
bool gravityF = false;
#endif

#ifdef _DEBUG
mymath::Vec3f vel_log;	// �ō����x
#endif

#ifdef D_ATK_TEST
mymath::Circlef atk_range(0.f,0.f,0.5f,300.f);
#endif


void CPlayer::key()
{
	using namespace input;
	POINT pt = camera::GetCursorPosition();
	mymath::Vec3f mouse(float(pt.x),float(pt.y),obj_.pos.z);
	if(CheckPush(KEY_MOUSE_RBTN))
	{
#ifndef D_MOVE_TEST
	#ifdef D_ACT_TEST
		hangPoint_ = mouse;
		gravityF_ = true;
		tensionAcc_ = TENSION;
		isHanging_ = true;
	#else
		const auto& sm = CStageMng::GetPtr();
		for(const auto& act : sm->actionPoints)
		{
			mymath::Vec3f tmp;
			if(act->Contains(tmp,obj_.pos, mouse))
			{
				hangPoint_ = tmp;
				gravityF_ = true;
				tensionAcc_ = TENSION;
				isHanging_ = true;
				
				break;
			}
		}
	#endif
#endif
	}
#ifdef D_HANG_TEST
	if(CheckPress(KEY_BTN2))
	{
		isHanging_ = true;
	}
	else
	{
		//isHanging_ = false;
	}
#else
	if(isHanging_ && CheckPull(KEY_MOUSE_RBTN))
	{
		// �͂�ł��Ԃ��痣������
		isHanging_ = false;
		gravity_ = 0.f;
		gravityF_ = true;
	}
#endif
#ifdef D_ATK_TEST
		atk_range.center = obj_.pos;
#endif
	if(CheckPress(KEY_MOUSE_LBTN))
	{
#ifdef D_ATK_TEST
		isAttacking_ = atk_range.Contains(mouse);
#else
		mymath::Circlef range(obj_.pos, attackRange_->halfWidth());
		isAttacking_ = range.Contains(mouse);
#endif
	}
	if(CheckPull(KEY_MOUSE_LBTN))
	{
		isAttacking_ = false;
	}

#ifdef _DEBUG
	if(CheckPush(KEY_SPACE))
	{
		obj_.pos.x = 0.f;
		obj_.pos.y = 0.f;
		obj_.add = 0.f;
	}
#endif
	
#ifdef D_GRAVITY_TEST
	if( (CheckPush(KEY_LEFT))	|| (CheckPush(KEY_RIGHT)) ||
		(CheckPush(KEY_UP))		|| (CheckPush(KEY_DOWN)) )
	{
		gravityF = false;	
		obj_.add = 0.f;
	}
#endif

#ifdef D_MOVE_TEST
	if(CheckPress(KEY_LEFT))
	{
		obj_.pos.x -= 10.f;
	}
	if(CheckPress(KEY_RIGHT))
	{
		obj_.pos.x += 10.f;
	}
	if(CheckPress(KEY_UP))
	{
		obj_.pos.y -= 10.f;
	}
	if(CheckPress(KEY_DOWN))
	{
		obj_.pos.y += 10.f;
	}
#endif

}

#ifdef D_LOG_TEST
#include <deque>
std::deque<POINT> logs;
#endif

void CPlayer::move()
{
	mymath::Vec3f& pos = obj_.pos;
	mymath::Vec3f& velocity = obj_.add;
	if(mymath::PYTHA(velocity.x,velocity.y) < mymath::POW2(0.5f))
	{
		velocity = 0.f;
	}
	else
	{
#ifndef D_MOVE_TEST
		obj_.Move();
#endif
		velocity *= DOWNSP;
	}
#ifdef D_LOG_TEST
	POINT p = { (LONG)obj_.pos.x,
				(LONG)obj_.pos.y};
	if(!logs.empty())
	{
		if(logs.back() != p)
			logs.push_back(p);
		if(logs.size() > 120)
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
	if(gravityF_)
	{
		obj_.src.y = MotionType::FALL;
		gravity_ += GRAVITY_ACC;
		if(gravity_ >= MAX_G)
		{
			gravity_ = MAX_G;
		}
		velocity.y += gravity_;
	}

	if(isHanging_)
	{
		// �Ԃ牺���蒆
		obj_.src.y = MotionType::HANG;
		mymath::Vec3f dist = hangPoint_ - pos;	// pos -> hang
			
		const float min_radius = 150.f;	// �ŏ�����(�U��q�̔��a)

		float angle = std::atan2f(dist.y,dist.x);
		mymath::Vec3f tensionForce = mymath::Vec3f::Rotate(angle) * tensionAcc_;
		velocity += tensionForce;
		tensionAcc_ *= DOWN_TENSION;
#ifdef _DEBUG
		if(mymath::PYTHA(velocity.x,velocity.y) > mymath::PYTHA(vel_log.x,vel_log.y))
			vel_log = velocity;
#endif
		{/*
			hangAcc_ *= 0.98f;
			if(hangAcc_ < 1.f)
			{
			hangAcc_ = 0.f;
			obj_.velocity = 0.f;
			}*/
			hangAcc_ += GRAVITY_ACC;
			if (hangAcc_ > MAX_G)
			{
				hangAcc_ = MAX_G;
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
					velocity = stopPos - pos;
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
	if(gravityF)
	{
#endif
		tensionAcc_ = 0.f;
#ifdef D_GRAVITY_TEST
	}
#endif
	}
	//----------------------------------------
	// �ړ����x����
	if(velocity.x >= MAX_VX)
	{
		velocity.x = MAX_VX;
	}
	if(velocity.y >= MAX_VY)
	{
		velocity.y = MAX_VY;
	}
	// �X�e�[�W���W����
	{
		const auto& sm = CStageMng::GetPtr();
		pos.x = clamp(pos.x, sm->rect.left, sm->rect.right);
		pos.y = clamp(pos.y, sm->rect.top,	sm->rect.bottom);
	}
	attackRange_->pos = attackRange_->pos.TmpReplace(mymath::Vec3f::X|mymath::Vec3f::Y, pos);
}



void CPlayer::step()
{
	mymath::Vec3f& pos = obj_.pos;
	mymath::Vec3f& velocity = obj_.add;
	ICharacter::step();
	key();

	// �ړ�����
	move();

	// Chain����
#pragma region Chain����
	{
		const float appearTime = 0.5f;		// �o���A�j���[�V�����b��
		const float appearMoveX = 30.f;		// X�ړ���
		const float disappearTime = 0.2f;	// �����A�j���[�V�����b��
		switch(chainState_)
		{
		case ChainState::HIDE:
			break;

		case ChainState::APPEARING:
			chainMsg_->alpha = Easing::Linear(chainAnimTime_, 0.f, 255.f, appearTime);
			chainMsg_->pos.x = Easing::QuadOut(chainAnimTime_, chainStaPos_.x, 30.f, appearTime);
			
			if((chainAnimTime_ += system::FrameTime) >= appearTime)
			{
				// �o������
				chainState_ = ChainState::SHOW;
			}
			// �c�莞�Ԃ͏o�������������Ă�������break�͏����Ȃ�
		case ChainState::SHOW:
			// Chain���Ԃ͏��������Ȃǂɉe������āA���̃Q�[�����b���ŏI���
			if((chainTime_ -= system::ONEFRAME_TIME) <= disappearTime)
			{
				// �����J�n
				chainState_ = ChainState::DISAPPEARING;
				chainAnimTime_ = 0.f;
			}
			numberPos_.x = chainMsg_->pos.x;
			numberPos_.y = Easing::BackOut(numberAnimTime_, chainStaPos_.y+20.f, -20.f, 0.2f);
			numberAnimTime_ += system::FrameTime;
			
			if(chainState_ == ChainState::APPEARING) break;
			// SHOW�݂̂̏���
			chainMsg_->alpha = 255.f;
			chainMsg_->pos.x = chainStaPos_.x + appearMoveX;
			break;

		case ChainState::DISAPPEARING:
			chainMsg_->alpha = Easing::Linear(chainAnimTime_, 255.f, -255.f, disappearTime);
			chainMsg_->pos.x = Easing::QuadOut(chainAnimTime_, chainStaPos_.x+appearMoveX, 10.f, disappearTime);
			if((chainAnimTime_ += system::FrameTime) >= disappearTime)
			{
				// ��������
				chainState_ = ChainState::HIDE;
				chainMsg_->pos = chainStaPos_;
				chainMsg_->alpha = 0.f;
				chainAnimTime_ = 0.f;
				chainCnt_ = 0;
			}
			break;
		}
	}
#pragma endregion // Chain����

#ifdef D_COLOR_TEST
	static float c_cnt = 0.f;
	auto color = Color_ARGBtoAHSV(	attackRange_->alpha,
									attackRange_->r,
									attackRange_->g,
									attackRange_->b);

	if( (c_cnt += FrameTime) > 0.5f )
	{
		c_cnt = 0.f;
		color = Color_AddHChannel(color,5);
	}
	color = Color_ARGBFromAHSV(color);
	Color_SeparateARGB(color,
					attackRange_->alpha,
					attackRange_->r,
					attackRange_->g,
					attackRange_->b);
#endif
	
	//----------------------------------------
	// �J�����ړ�
	{
		mymath::Vec3f cameraPos;
		const auto& sm = CStageMng::GetPtr();
		cameraPos.x = clamp(pos.x, (sm->rect.left + system::WINW)/2.f, sm->rect.right);
		cameraPos.y = clamp(pos.y, (sm->rect.top  + system::WINH)/2.f, sm->rect.bottom);
		camera::SetLookAt(cameraPos.x, cameraPos.y);
	}
}

void CPlayer::draw()
{
	const mymath::Vec3f& pos = obj_.pos;

	static const std::string emply_string;
#ifdef D_LOG_TEST
	for(const auto& log : logs)
	{
		Draw_BoxCenter(log.x,log.y,4,4,pos.z-0.1f,
						0xffff0000,0xffff0000,1,true);
	}
#endif

#ifdef D_ATK_TEST
	mymath::Circlef& circle_ = atk_range;
	D3DCOLOR color = (isAttacking_) ? 0xfff0f020 : 0xa0efefef;
	circle_.draw(color);
#endif
	const mymath::Rectf& colrect = *std::dynamic_pointer_cast<mymath::Rectf>(GetCollisionAreas()[0]);
	graph::Draw_Box(
		(int)colrect.left, (int)colrect.top,
		(int)colrect.right, (int)colrect.bottom,
		pos.z - 0.1f,
		0xff00ff00, ARGB(255, 255, 255, 255), 1, true);
	
#ifdef _DEBUG
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
	float angle = std::atan2f(dist.y,dist.x);
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
	
	
	mymath::Recti arc = attackRange_->GetRect();
	ss.str(emply_string);
	ss <<	"ARGB(" << std::setw(3) << (int)attackRange_->alpha
		<<	","		<< std::setw(3) << (int)attackRange_->r
		<<	","		<< std::setw(3) << (int)attackRange_->g
		<<	","		<< std::setw(3) << (int)attackRange_->b
		<<	")";
	font::Draw_FontText(
		arc.left, arc.bottom,0.f,
		ss.str(),-1,
		setting::GetFontID("font_MSG15"));

#endif


	//attackRange_->draw();
	if(isHanging_)
	{
		graph::Draw_Line(
			static_cast<int>(pos.x),
			static_cast<int>(pos.y),
			static_cast<int>(hangPoint_.x),
			static_cast<int>(hangPoint_.y),
			pos.z - 0.1f,
			0x7fffffff, 2);
	}

	if(chainState_ != ChainState::HIDE)
	{
		// Chain��
		chainMsg_->drawNC(charabase::CharBase::MODE::LeftTop);
		// Chain�����摜�p�^�[����
		int digit = (chainCnt_/100==0)? ((chainCnt_/10==0)? 1: 2 ) : 3;
		int work = chainCnt_;	// chain���������Ƃɋ�؂�
		const int width = 32;
		const int height = 64;
		for(int i=1; i <= digit; ++i)
		{
			// ���ʌ����珇�ɕ`��(Chain�����̍����猸�炵�Ă���)
			graph::Draw_GraphicsNC(
				numberPos_.x - i*width,
				numberPos_.y - 10.f,
				numberPos_.z,
				"img_number",
				(work % 10) * width, 0,
				width, height);
			work /= 10;
		}
	}

	obj_.draw();
}


void CPlayer::hit(const ObjPtr& rival)
{
	if(rival->FindName("ActionPolygon"))
	{
		// �߂荞�ݕ␳,�ʉߕ␳
		const auto& ap = std::dynamic_pointer_cast<CActionPolygon>(rival);
		mymath::Vec3f dist = obj_.pos - prePos;
		mymath::Vec3f intersection;
		intersection = ap->IntersectionPoint2Nearest(prePos_, obj_.pos);
		obj_.pos = intersection;
		obj_.pos -= dist.Normalize();
		// �h�����Ă�̂Ńt���O��܂�
		gravityF_ = false;
		//isHanging_ = false;
	}
	else if(rival->FindName("Atk_"))
	{
		// �G����̍U��
		health_ -= std::dynamic_pointer_cast<IAttack>(rival)->GetForce();
		if(health_ <= 0)
		{
			health_ = 0;
			// �Q�[���I�[�o�[
			//kill();
			
		}
	}
}


ObjPtr CPlayer::GetPtr()
{
	extern CGameManager *gm;
	auto& pl = gm->GetObjects("Player");
	if(pl.empty()) return nullptr;
	return pl[0];
}


void CPlayer::SetHangPoint(const mymath::Vec3f& pos)
{
	hangPoint_ = pos;
}


void CPlayer::ApplyDamage(int dam)
{
	health_ -= dam;
	if(health_ < 0)
		health_ = 0;
}

void CPlayer::ApplyAttack(const mymath::Vec3f& pos)
{
	// �G�̏ꏊ�Ɉړ�
	//obj_.velocity += (pos - obj_.pos) / FrameTime / 3.f;
	
	obj_.pos = pos;
	obj_.src.y = MotionType::ATTACK;
	//obj_.velocity *= 0.5f;
	
	//---------------------------------
	// �t���O
	gravityF_ = true;
	isHanging_ = false;

}

void CPlayer::KilledEnemy()
{

	//---------------------------------
	// Chain
	if(chainTime_ > 0.f)
	{
		// Chain��
		chainTime_ = CHAIN_TIME[1];
	}
	else
	{
		// ����Kill
		chainTime_ = CHAIN_TIME[0];
	}
	chainCnt_++;
	if(chainCnt_ > MAX_CHAIN)
		chainCnt_ = MAX_CHAIN;
	if(chainCnt_ >= 2)
	{
		numberAnimTime_ = 0.f;
		switch(chainState_)
		{
		case ChainState::HIDE:
		case ChainState::APPEARING:
			// �o���J�n
			chainState_ = ChainState::APPEARING;
			chainAnimTime_ = 0.f;
			break;
		case ChainState::SHOW:
		case ChainState::DISAPPEARING:
			// �\���p��
			chainState_ = ChainState::SHOW;
			break;
		}
	}
}
//
//Base::Collisions CPlayer::GetCollisionAreas()
//{
//	collisions_.clear();
//
//	mymath::Rectf rc;
//	rc.left		= obj_.pos.x - 5.f;
//	rc.top		= obj_.pos.y - 5.f;
//	rc.right	= obj_.pos.x + 5.f;
//	rc.bottom	= obj_.pos.y + 5.f;
//	collisions_.push_back(mymath::ShapefPtr(new mymath::Rectf(rc)));
//	
//	return collisions_;
//}


#pragma endregion // CPlayer methods