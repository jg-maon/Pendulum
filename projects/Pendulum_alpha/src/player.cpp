
#ifdef _DEBUG
#define D_MOVE_TEST		// キーで移動
//#define D_HANG_TEST			// 減速なし
//#define D_GRAVITY_TEST		// 重力なし
//#define D_COLOR_TEST	// HSV変換テスト
//#define D_LOG_TEST	// 移動軌跡
#endif
#define D_ACT_TEST			// マウスクリック座標でぶら下がる
#define D_ATK_TEST	// 攻撃範囲拡大
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

const float CPlayer::GRAVITY_ACC = 25.f;	// 最大重力速度
const float CPlayer::MAX_G = 100.f;			// 重力加速度
const float CPlayer::TENSION = 1500.f;		// フックの張力(初速)
const float CPlayer::DOWN_TENSION = 0.85f;	// 張力減速率
const float CPlayer::DOWNSP = 0.92f;		// 移動減速率(1-DOWNSP)
const float CPlayer::MAX_VY = 1500.f;		// 水平方向の最大速度(ゲームとして成り立つバランス調整用)
const float CPlayer::MAX_VX = 2500.f;		// 鉛直方向の最大速度(ゲームとして成り立つバランス調整用)

const float CPlayer::CHAIN_TIME[2] = {1.0f,2.0f};	// Chain猶予時間[0]:1体目から2体目まで [1]:それ以降
const int CPlayer::MAX_CHAIN = 999;		// 最高Chain数


#pragma region CPlayer methods

CPlayer::CPlayer(const CStage& stage, const mymath::Vec3f& pos):
	ICharacter("Player")
	,stage_(stage)
//	,isHanging(isHanging_)
	,isAttacking(isAttacking_)
	,power(power_)
{
	obj_.pos = pos;
	init();
}
CPlayer::CPlayer(const CStage& stage, float x, float y, float z):
	ICharacter("Player")
	,stage_(stage)
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
	obj_.velocity = 0.f;
	obj_.img = playerIMG;
	obj_.src(128,128);
	prePos_ = obj_.pos;
	isHanging_ = false;
	tensionAcc_ = 0.f;
	gravity_ = 0.f;
	gravityF_ = false;
	hangAcc_ = 0.f;
	attackRange_ = CharPtr(new CharBase(
			obj_.pos,
			mymath::Vec3f(),
			96,96,
			circleIMG));
	attackRange_->pos.z -= 0.1f;
	attackRange_->g = 0.f;
	attackRange_->b = 0.f;
	isAttacking_ = false;
	
	//----------------------------------
	chainState_ = ChainState::HIDE;
	chainTime_ = 0.f;
	chainAnimTime_ = 0.f;
	chainStaPos_(WINW-300.f, 30.f, 0.1f);
	chainMsg_ = CharPtr(new CharBase(
			chainStaPos_,
			mymath::Vec3f(),
			151,55,
			chainIMG));
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
	// カメラ移動
	{
		mymath::Vec3f& pos = obj_.pos;
		mymath::Vec3f cameraPos;
		cameraPos.x = clamp(pos.x, (stage_.rect.left + WINW)/2.f, stage_.rect.right);
		cameraPos.y = clamp(pos.y, (stage_.rect.top  + WINH)/2.f, stage_.rect.bottom);
		SetLookAt(cameraPos.x, cameraPos.y);
	}
}



#ifdef D_GRAVITY_TEST
bool gravityF = false;
#endif

#ifdef _DEBUG
mymath::Vec3f vel_log;	// 最高速度
#endif

#ifdef D_ATK_TEST
mymath::Circlef atk_range(0.f,0.f,0.5f,300.f);
#endif


void CPlayer::key()
{
	POINT pt = GetCursorPosition();
	mymath::Vec3f mouse(float(pt.x),float(pt.y),obj_.pos.z);
	if(CheckPush(KEY_MOUSE_RBTN))
	{
#ifndef Da_MOVE_TEST
	#ifdef D_ACT_TEST
		hangPoint_ = mouse;
		gravityF_ = true;
		tensionAcc_ = TENSION;
		isHanging_ = true;
	#else
		for(const auto& act : stage_.actionPoints)
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
		// 掴んでる状態から離した時
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
		obj_.velocity = 0.f;
	}
#endif
	
#ifdef D_GRAVITY_TEST
	if( (CheckPush(KEY_LEFT))	|| (CheckPush(KEY_RIGHT)) ||
		(CheckPush(KEY_UP))		|| (CheckPush(KEY_DOWN)) )
	{
		gravityF = false;	
		obj_.velocity = 0.f;
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
	mymath::Vec3f& velocity = obj_.velocity;
	if(PYTHA(velocity.x,velocity.y) < POW2(0.5f))
	{
		obj_.velocity = 0.f;
	}
	else
	{
#ifndef D_MOVE_TEST
		obj_.Move();
#endif
		obj_.velocity *= DOWNSP;
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
	// 重力
	if(gravityF_)
	{
		obj_.dir = MotionType::FALL;
		gravity_ += GRAVITY_ACC;
		if(gravity_ >= MAX_G)
		{
			gravity_ = MAX_G;
		}
		velocity.y += gravity_;
	}

	if(isHanging_)
	{
		// ぶら下がり中
		obj_.dir = MotionType::HANG;
		mymath::Vec3f dist = hangPoint_ - obj_.pos;	// pos -> hang
			
		const float min_radius = 150.f;	// 最小距離(振り子の半径)

		float angle = std::atan2f(dist.y,dist.x);
		mymath::Vec3f tensionForce = mymath::Vec3f::Rotate(angle) * tensionAcc_;
		obj_.velocity += tensionForce;
		tensionAcc_ *= DOWN_TENSION;
#ifdef _DEBUG
		if(PYTHA(velocity.x,velocity.y) > PYTHA(vel_log.x,vel_log.y))
			vel_log = obj_.velocity;
#endif
		{/*
			hangAcc_ *= 0.98f;
			if(hangAcc_ < 1.f)
			{
				hangAcc_ = 0.f;
				obj_.velocity = 0.f;
			}*/
			hangAcc_ += GRAVITY_ACC;
			if(hangAcc_ > MAX_G)
			{
				hangAcc_ = MAX_G;
			}

			if(dist.x*dist.x + dist.y*dist.y > min_radius*min_radius)
			{
					
				// 一フレーム次の位置(重力分)
				mymath::Vec3f nextPos(pos.x,pos.y+hangAcc_,pos.z);
				mymath::Vec3f intersection = mymath::Circlef(hangPoint_,min_radius).IntersectionPoint2Nearest(nextPos,hangPoint_);
					
				// 鉛直方向に伸ばした静止点
				mymath::Vec3f stopPos(hangPoint_.x, hangPoint_.y+min_radius, hangPoint_.z);
				mymath::Vec3f isVec = stopPos - intersection;
				mymath::Vec3f psVec = stopPos - pos;
				if(PYTHA(isVec.x,isVec.y) < POW2(2.f) 
					&& PYTHA(psVec.x,psVec.y) < POW2(2.f) )
				{
					// 距離が極めて近ければ静止
					velocity = stopPos - pos;
				}
				else
				{
					// 振り子風運動
					velocity += hangPoint_ - nextPos;
				}
			}
		}
			
			
	}
	else
	{
		// 空中
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
	// 移動速度制限
	if(velocity.x >= MAX_VX)
	{
		velocity.x = MAX_VX;
	}
	if(velocity.y >= MAX_VY)
	{
		velocity.y = MAX_VY;
	}
	// ステージ座標制限
	pos.x = clamp(pos.x,	stage_.rect.left,	stage_.rect.right);
	pos.y = clamp(pos.y,	stage_.rect.top,	stage_.rect.bottom);

	attackRange_->pos = attackRange_->pos.TmpReplace(mymath::Vec3f::X|mymath::Vec3f::Y, obj_.pos);
}



void CPlayer::step()
{
	mymath::Vec3f& pos = obj_.pos;
	mymath::Vec3f& velocity = obj_.velocity;
	ICharacter::step();
	key();

	// 移動処理
	move();

	// Chain文字
#pragma region Chain文字
	{
		const float appearTime = 0.5f;		// 出現アニメーション秒数
		const float appearMoveX = 30.f;		// X移動量
		const float disappearTime = 0.2f;	// 消失アニメーション秒数
		switch(chainState_)
		{
		case ChainState::HIDE:
			break;

		case ChainState::APPEARING:
			chainMsg_->alpha = Easing::Linear(chainAnimTime_, 0.f, 255.f, appearTime);
			chainMsg_->pos.x = Easing::QuadOut(chainAnimTime_, chainStaPos_.x, 30.f, appearTime);
			
			if((chainAnimTime_ += FrameTime) >= appearTime)
			{
				// 出現完了
				chainState_ = ChainState::SHOW;
			}
			// 残り時間は出現中も減少していくためbreakは書かない
		case ChainState::SHOW:
			// Chain時間は処理落ちなどに影響されて、一定のゲーム内秒数で終わる
			if((chainTime_ -= ONEFRAME_TIME) <= disappearTime)
			{
				// 消失開始
				chainState_ = ChainState::DISAPPEARING;
				chainAnimTime_ = 0.f;
			}
			numberPos_.x = chainMsg_->pos.x;
			numberPos_.y = Easing::BackOut(numberAnimTime_, chainStaPos_.y+20.f, -20.f, 0.2f);
			numberAnimTime_ += FrameTime;
			
			if(chainState_ == ChainState::APPEARING) break;
			// SHOWのみの処理
			chainMsg_->alpha = 255.f;
			chainMsg_->pos.x = chainStaPos_.x + appearMoveX;
			break;

		case ChainState::DISAPPEARING:
			chainMsg_->alpha = Easing::Linear(chainAnimTime_, 255.f, -255.f, disappearTime);
			chainMsg_->pos.x = Easing::QuadOut(chainAnimTime_, chainStaPos_.x+appearMoveX, 10.f, disappearTime);
			if((chainAnimTime_ += FrameTime) >= disappearTime)
			{
				// 消失完了
				chainState_ = ChainState::HIDE;
				chainMsg_->pos = chainStaPos_;
				chainMsg_->alpha = 0.f;
				chainAnimTime_ = 0.f;
				chainCnt_ = 0;
			}
			break;
		}
	}
#pragma endregion // Chain文字

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
	// カメラ移動
	{
		mymath::Vec3f cameraPos;
		cameraPos.x = clamp(pos.x, (stage_.rect.left + WINW)/2.f, stage_.rect.right);
		cameraPos.y = clamp(pos.y, (stage_.rect.top  + WINH)/2.f, stage_.rect.bottom);
		SetLookAt(cameraPos.x, cameraPos.y);
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
	Draw_Box(	(int)colrect.left,	(int)colrect.top,
				(int)colrect.right,	(int)colrect.bottom,
				pos.z-0.1f,
				0xff00ff00,ARGB(255,255,255,255),1,true);
	
#ifdef _DEBUG
	mymath::Recti rc = obj_.getRect();

	//rc.offset((int)obj_.pos.x, (int)obj_.pos.y);
	std::stringstream ss;
	ss << "pos("
		<< std::setw(4) << (int)pos.x
		<< "," 
		<< std::setw(4) << (int)pos.y
		<< ")";
	Draw_FontText(rc.right,rc.top,0.f,ss.str(),0xffff0000,MSG15);

	ss.str(emply_string);
	const mymath::Vec3f& velocity = obj_.velocity;
	ss << "velocity(" 
		<< std::setw(4) << (int)velocity.x
		<< "," 
		<< std::setw(4) << (int)velocity.y
		<< ")";
	Draw_FontText(rc.right,rc.top+Draw_GetCharHeight(MSG15)*1,0.f,ss.str(),0xffff0000,MSG15);
	
	ss.str(emply_string);
	mymath::Vec3f dist = hangPoint_ - pos;
	float angle = std::atan2f(dist.y,dist.x);
	mymath::Vec3f tensionForce = mymath::Vec3f::Rotate(angle) * tensionAcc_;
	ss << "tensionForce(" 
		<< std::setw(4) << (int)tensionForce.x
		<< "," 
		<< std::setw(4) << (int)tensionForce.y
		<< ")";
	Draw_FontText(rc.right,rc.top+Draw_GetCharHeight(MSG15)*2,0.f,ss.str(),0xffff0000,MSG15);
	
	ss.str(emply_string);
	ss << "tensionAcc_:" 
		<< std::setw(4) << (int)tensionAcc_;
		
	Draw_FontText(rc.right,rc.top+Draw_GetCharHeight(MSG15)*3,0.f,ss.str(),0xffff0000,MSG15);
	
	ss.str(emply_string);
	ss << "vel_log(" 
		<< std::setw(4) << (int)vel_log.x
		<< "," 
		<< std::setw(4) << (int)vel_log.y
		<< ")";
	Draw_FontText(rc.right,rc.top+Draw_GetCharHeight(MSG15)*4,0.f,ss.str(),0xffff0000,MSG15);
	
	
	mymath::Recti arc = attackRange_->getRect();
	ss.str(emply_string);
	ss <<	"ARGB(" << std::setw(3) << (int)attackRange_->alpha
		<<	","		<< std::setw(3) << (int)attackRange_->r
		<<	","		<< std::setw(3) << (int)attackRange_->g
		<<	","		<< std::setw(3) << (int)attackRange_->b
		<<	")";
	Draw_FontText(arc.left, arc.bottom,0.f,ss.str(),-1,MSG15);

#endif


	//attackRange_->draw();
	if(isHanging_)
	{
		Draw_Line(	static_cast<int>(pos.x),
					static_cast<int>(pos.y),
					static_cast<int>(hangPoint_.x),
					static_cast<int>(hangPoint_.y),
					pos.z-0.1f,
					0x7fffffff,2);
	}

	if(chainState_ != ChainState::HIDE)
	{
		// Chain中
		chainMsg_->drawNC(CharBase::MODE::LeftTop);
		// Chain数を画像パターンに
		int digit = (chainCnt_/100==0)? ((chainCnt_/10==0)? 1: 2 ) : 3;
		int work = chainCnt_;	// chain数を桁ごとに区切る
		const int width = 32;
		const int height = 64;
		for(int i=1; i <= digit; ++i)
		{
			// 下位桁から順に描画(Chain文字の左から減らしていく)
			Draw_GraphicsNC(numberPos_.x - i*width,
							numberPos_.y-10.f,
							numberPos_.z,
							numberIMG,
							(work%10) * width, 0,
							width, height);
			work /= 10;
		}
	}

	obj_.draw();
}


void CPlayer::hit(const ObjPtr& rival)
{
	if(rival->findName("ActionPolygon"))
	{
		// めり込み補正,通過補正
		const auto& ap = std::dynamic_pointer_cast<CActionPolygon>(rival);
		mymath::Vec3f dist = obj_.pos - prePos;
		mymath::Vec3f intersection;
		intersection = ap->IntersectionPoint2Nearest(prePos_, obj_.pos);
		obj_.pos = intersection;
		obj_.pos -= dist.Normalize();
		// 刺さってるのでフラグを折る
		gravityF_ = false;
		//isHanging_ = false;
	}
	else if(rival->findName("Atk_"))
	{
		// 敵からの攻撃
		health_ -= static_cast<IAttack*>(rival.get())->GetForce();
		if(health_ <= 0)
		{
			health_ = 0;
			// ゲームオーバー
			//kill();
			
		}
	}
}


ObjPtr CPlayer::GetPtr()
{
	auto& pl = GetObjects("Player");
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
	// 敵の場所に移動
	//obj_.velocity += (pos - obj_.pos) / FrameTime / 3.f;
	
	obj_.pos = pos;
	obj_.dir = MotionType::ATTACK;
	//obj_.velocity *= 0.5f;
	
	//---------------------------------
	// フラグ
	gravityF_ = true;
	isHanging_ = false;

}

void CPlayer::KilledEnemy()
{

	//---------------------------------
	// Chain
	if(chainTime_ > 0.f)
	{
		// Chain中
		chainTime_ = CHAIN_TIME[1];
	}
	else
	{
		// 初期Kill
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
			// 出現開始
			chainState_ = ChainState::APPEARING;
			chainAnimTime_ = 0.f;
			break;
		case ChainState::SHOW:
		case ChainState::DISAPPEARING:
			// 表示継続
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