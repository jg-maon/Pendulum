
#ifdef _DEBUG
//#define D_MOVE_TEST		// キーで移動
//#define D_HANG_TEST			// 減速なし
//#define D_GRAVITY_TEST		// 重力なし
//#define D_COLOR_TEST	// HSV変換テスト
//#define D_LOG_TEST	// 移動軌跡
#define D_SCALE_TEST		// マウススケーリング
#define D_LINE_TEST		// 鎖Line補完
#endif
//#define D_ACT_TEST			// マウスクリック座標でぶら下がる
#define D_ATK_TEST	// 攻撃範囲拡大
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


#include "effectSlash.h"


#ifdef D_GRAVITY_TEST
bool gravityF = false;
#endif

#ifdef _DEBUG
mymath::Vec3f vel_log;	// 最高速度
#endif

#ifdef D_ATK_TEST
mymath::Circlef atk_range(0.f, 0.f, 0.5f, 300.f);
#endif


/*
const float CPlayer::GRAVITY_ACC = 25.f;	// 最大重力速度
const float CPlayer::MAX_G = 100.f;			// 重力加速度
const float CPlayer::TENSION = 1500.f;		// フックの張力(初速)
const float CPlayer::DOWN_TENSION = 0.85f;	// 張力減速率
const float CPlayer::DOWN_SPEED = 0.92f;		// 移動減速率(1-DOWN_SPEED)
const float CPlayer::MAX_VX = 2500.f;		// 鉛直方向の最大速度(ゲームとして成り立つバランス調整用)
const float CPlayer::MAX_VY = 1500.f;		// 水平方向の最大速度(ゲームとして成り立つバランス調整用)

const float CPlayer::CHAIN_TIME[2] = {1.0f,2.0f};	// Chain猶予時間[0]:1体目から2体目まで [1]:それ以降
//*/

#pragma region CPlayer methods

CPlayer::CPlayer():
ICharacter("Player")
{
	status_ = Status::idle;
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

	obj_.pos(x, y, z);

	obj_.add = 0.f;

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
		96, 96));
	attackRange_->pos.z -= 0.1f;
	attackRange_->g = 0.f;
	attackRange_->b = 0.f;
	isAttacking_ = false;

	//invincible_ = false;
	invincibleTime_ = 0.f;
	invincibleAnim_ = 0.f;

	//----------------------------------
	chainState_ = ChainState::HIDE;
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
	// カメラ移動
	//sm()->MoveCamera(obj_.pos.x, obj_.pos.y);
	

#ifdef D_ATK_TEST
	atk_range.radius = loadInfo_.attackRadius;
#endif
}


void CPlayer::key()
{
	using namespace input;
	POINT pt = camera::GetCursorPosition();
	mymath::Vec3f mouse(float(pt.x), float(pt.y), obj_.pos.z);
	if (CheckPush(KEY_MOUSE_RBTN))
	{
#ifndef D_MOVE_TEST
#ifdef D_ACT_TEST
		hangPoint_ = mouse;
		gravityF_ = true;
		tensionAcc_ = loadInfo_.TENSION;
		isHanging_ = true;
		motionAnim_.set(0, 0.f);
#else
		const auto& aps = sm()->getActionPoints();
		for(const auto& act : aps)
		{
			if (act->Contains(mouse) && act->Contains(obj_.pos, mouse))
			{
				hangPoint_ = act->IntersectionPoint2Nearest(obj_.pos, mouse);
				gravityF_ = true;
				tensionAcc_ = loadInfo_.TENSION;
				isHanging_ = true;
				motionAnim_.set(0, 0.f);
				obj_.src.y = static_cast<int>(MotionType::HANG);

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
	if (isHanging_ && CheckPull(KEY_MOUSE_RBTN))
	{
		// 掴んでる状態から離した時
		UnHang();
	}
#endif
#ifdef D_ATK_TEST
	atk_range.center = obj_.pos;
#endif
	if (CheckPress(KEY_MOUSE_LBTN))
	{
#ifdef D_ATK_TEST
		isAttacking_ = atk_range.Contains(mouse);
#else
		mymath::Circlef range(obj_.pos, attackRange_->halfWidth());
		isAttacking_ = range.Contains(mouse);
#endif
	}
	if (CheckPull(KEY_MOUSE_LBTN))
	{
		isAttacking_ = false;
	}

#ifdef _DEBUG
	if (CheckPush(KEY_SPACE))
	{
		obj_.pos.x = 0.f;
		obj_.pos.y = 0.f;
		obj_.add = 0.f;
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

#ifdef D_LOG_TEST
#include <deque>
std::deque<POINT> logs;
#endif

void CPlayer::move()
{
	mymath::Vec3f& pos = obj_.pos;
	mymath::Vec3f& velocity = obj_.add;
	if (mymath::PYTHA(velocity.x, velocity.y) < mymath::POW2(1.f/system::FrameTime))
	{
		velocity = 0.f;
	}
	else
	{
#ifndef D_MOVE_TEST
		obj_.Move();
#endif
		velocity *= loadInfo_.DOWN_SPEED;
	}
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
	// 重力
	if (gravityF_)
	{
		gravity_ += loadInfo_.GRAVITY_ACC;
		if (gravity_ >= loadInfo_.MAX_G)
		{
			gravity_ = loadInfo_.MAX_G;
		}
		velocity.y += gravity_;
	}

	if (isHanging_)
	{
		// ぶら下がり中
		gravityF_ = true;
		mymath::Vec3f dist = hangPoint_ - pos;	// pos -> hang

		const float min_radius = 150.f;	// 最小距離(振り子の半径)

		float angle = std::atan2f(dist.y, dist.x);
		mymath::Vec3f tensionForce = mymath::Vec3f::Rotate(angle) * tensionAcc_;
		velocity += tensionForce;
		tensionAcc_ *= loadInfo_.DOWN_TENSION;
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
			hangAcc_ += loadInfo_.GRAVITY_ACC;
			if (hangAcc_ > loadInfo_.MAX_G)
			{
				hangAcc_ = loadInfo_.MAX_G;
			}

			if (mymath::PYTHA(dist.x, dist.y) > mymath::POW2(min_radius))
			{
				// 一フレーム次の位置(重力分)
				mymath::Vec3f nextPos(pos.x, pos.y + hangAcc_, pos.z);
				mymath::Vec3f intersection = mymath::Circlef(hangPoint_, min_radius).IntersectionPoint2Nearest(nextPos, hangPoint_);

				// 鉛直方向に伸ばした静止点
				mymath::Vec3f stopPos(hangPoint_.x, hangPoint_.y + min_radius, hangPoint_.z);
				mymath::Vec3f isVec = stopPos - intersection;
				mymath::Vec3f psVec = stopPos - pos;
				if (mymath::PYTHA(isVec.x, isVec.y) < mymath::POW2(2.f)
					&& mymath::PYTHA(psVec.x, psVec.y) < mymath::POW2(2.f))
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
		if (gravityF)
#endif
		{
			tensionAcc_ = 0.f;

		}
	}
	//----------------------------------------
	// 移動速度制限
	if (velocity.x >= loadInfo_.MAX_VX)
	{
		velocity.x = loadInfo_.MAX_VX;
	}
	if (velocity.y >= loadInfo_.MAX_VY)
	{
		velocity.y = loadInfo_.MAX_VY;
	}
	// ステージ座標制限
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
	attackRange_->pos = attackRange_->pos.TmpReplace(mymath::Vec3f::X | mymath::Vec3f::Y, pos);

}

void CPlayer::UnHang()
{
	isHanging_ = false;
	gravity_ = 0.f;
	gravityF_ = true;
	motionAnim_.set(2, 0.2f);
	obj_.src.y = static_cast<int>(MotionType::FALL);
}


void CPlayer::step()
{
	mymath::Vec3f& pos = obj_.pos;
	mymath::Vec3f& velocity = obj_.add;
	ICharacter::step();
	key();

	// 移動処理
	move();

	// アニメーション処理
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
	// 無敵
	if (invincibleTime_ > 0.f)
	{
		// 点滅アニメ
		invincibleAnim_ += system::ONEFRAME_TIME;
		if (invincibleAnim_ >= loadInfo_.INV_TIME / 20.f)
		{
			invincibleAnim_ = 0.f;
			if (obj_.alpha > 200.f)
				obj_.alpha = 150.f;		// 消す
			else
				obj_.alpha = 220.f;		// 出す
		}
		// 時間減少
		invincibleTime_ -= system::ONEFRAME_TIME;
		if (invincibleTime_ < 0.f)
		{
			// 無敵終了
			obj_.alpha = 255.f;
		}
	}

	//----------------------------------------
	// 振り向き
	//obj_.scale.x = (velocity.x > 0.f) ? 1.f : -1.f;
	if (!turnFlag_ && velocity.x < 0.f)
		turnFlag_ ^= 1;
	else if (turnFlag_ && velocity.x > 0.f)
		turnFlag_ ^= 1;

	// Chain文字
#pragma region Chain文字
	{
		const float appearTime = 0.5f;		// 出現アニメーション秒数
		const float appearMoveX = 30.f;		// X移動量
		const float disappearTime = 0.2f;	// 消失アニメーション秒数
		switch (chainState_)
		{
		case ChainState::HIDE:
			break;

		case ChainState::APPEARING:
			chainMsg_->alpha = Easing::Linear(chainAnimTime_, 0.f, 255.f, appearTime);
			chainMsg_->pos.x = Easing::QuadOut(chainAnimTime_, chainStaPos_.x, appearMoveX, appearTime);

			if ((chainAnimTime_ += system::FrameTime) >= appearTime)
			{
				// 出現完了
				chainState_ = ChainState::SHOW;
			}
			// 残り時間は出現中も減少していくためbreakは書かない
		case ChainState::SHOW:
			// Chain時間は処理落ちなどに影響されて、一定のゲーム内秒数で終わる
			if ((chainTime_ -= system::ONEFRAME_TIME) <= disappearTime)
			{
				// 消失開始
				chainState_ = ChainState::DISAPPEARING;
				chainAnimTime_ = 0.f;
			}
			numberPos_.x = chainMsg_->pos.x;
			numberPos_.y = Easing::BackOut(numberAnimTime_, chainStaPos_.y + 20.f, -20.f, 0.2f);
			numberAnimTime_ += system::FrameTime;

			if (chainState_ == ChainState::APPEARING) break;
			// SHOWのみの処理
			chainMsg_->alpha = 255.f;
			chainMsg_->pos.x = chainStaPos_.x + appearMoveX;
			break;

		case ChainState::DISAPPEARING:
			chainMsg_->alpha = Easing::Linear(chainAnimTime_, 255.f, -255.f, disappearTime);
			chainMsg_->pos.x = Easing::QuadOut(chainAnimTime_, chainStaPos_.x + appearMoveX, 10.f, disappearTime);
			if ((chainAnimTime_ += system::FrameTime) >= disappearTime)
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

	if ( (c_cnt += FrameTime) > 0.5f )
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
	sm()->MoveCamera(obj_.pos);
}



void CPlayer::draw()
{
	const mymath::Vec3f& pos = obj_.pos;

#ifdef D_LOG_TEST
	//charabase::CharBase b = obj_;
	for(const auto& log : logs)
	{
		//b.pos.x = log.x;
		//b.pos.y = log.y;
		//b.pos.z = obj_.pos.z - 0.1f;
		//b.alpha = 120.f;
		//b.draw();
		graph::Draw_BoxCenter(log.x,log.y,4,4,pos.z-0.1f,
			0xffff0000,0xffff0000,1,true);
	}
#endif

#ifdef D_ATK_TEST
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


	//attackRange_->draw();

	//-----------------------------------------------------------
	// Chain
	if (chainState_ != ChainState::HIDE)
	{
		// Chain中
		chainMsg_->drawNC(charabase::CharBase::MODE::LeftTop);
		// Chain数を画像パターンに
		int digit = (chainCnt_ / 100 == 0) ? ((chainCnt_ / 10 == 0) ? 1 : 2) : 3;
		int work = chainCnt_;	// chain数を桁ごとに区切る
		const int width = 32;
		const int height = 64;
		for (int i = 1; i <= digit; ++i)
		{
			// 下位桁から順に描画(Chain文字の左から減らしていく)
			graph::Draw_GraphicsLeftTopNC(
				numberPos_.x - i*width,
				numberPos_.y - 10.f,
				numberPos_.z,
				"img_UInumber",
				(work % 10) * width, 0,
				width, height);
			work /= 10;
		}
	}

	//-----------------------------------------------------------
	// 体力ゲージ
	{
		enum
		{
			draw_x = 30,
			draw_y = 30,
			width = 400,
			height = 45,
		};
		//------------------------
		// 枠
		graph::Draw_BoxNC(
			draw_x, draw_y,
			draw_x + width,
			draw_y + height, 0.8f,
			ARGB(200, 10, 10, 10), ARGB(255, 0, 0, 0),
			5, true);
		//------------------------
		// 中
		// 体力に応じて幅を計算
		float w = static_cast<float>(health_) / static_cast<float>(loadInfo_.health) * width;
		graph::Draw_BoxNC(
			draw_x, draw_y,
			draw_x + static_cast<int>(w),
			draw_y + height, 0.8f,
			ARGB(255, 100, 250, 250), ARGB(255, 0, 0, 0),
			0, true);

	}
	//-----------------------------------------------------------
	// プレイヤー画像
	obj_.draw(charabase::CharBase::MODE::Center, turnFlag_);
	//-----------------------------------------
	// 左腕+鎖
	{
		// 腕
		float z = pos.z;
		SIZE armsz = graph::Draw_GetImageSize2(loadInfo_.armImg);

		float x;	// 反転成分
		if (turnFlag_)
		{
			// 左向きの時は体より手前に描画
			z -= 0.1f;
			// 反転
			x = -1.f;
		}
		else
		{
			// 右向きの時は体より奥に描画
			z += 0.1f;
			x = 1.f;
		}

		// 厳密には肩
		mymath::Vec3f armPos(
			pos.x + (loadInfo_.armX - loadInfo_.armRotateX) * x,
			pos.y + (loadInfo_.armY - loadInfo_.armRotateY),
			z);
		POINT c = { static_cast<LONG>(loadInfo_.armRotateX),
					static_cast<LONG>(loadInfo_.armRotateY) };
		float angle = (isHanging_) ? math::DegreeOfPoints2(armPos.x, armPos.y, hangPoint_.x, hangPoint_.y) : 270.f;

		graph::Draw_Graphics(
			armPos.x,
			armPos.y,
			armPos.z,
			loadInfo_.armImg,
			0, 0, armsz.cx, armsz.cy,
			static_cast<int>(angle), &c);

		graph::Draw_Graphics(
			armPos.x,
			armPos.y,
			armPos.z,
			loadInfo_.armImg,
			0, 0, armsz.cx, armsz.cy,
			static_cast<int>(angle), &c);

		//----------------------------------------------
		// 鎖
		if (isHanging_)
		{
			mymath::Vec3f vec = armPos - hangPoint_;
			angle = math::DegreeOfPoints2(0.f, 0.f, vec.x, vec.y);
			SIZE chainsz = graph::Draw_GetImageSize2(loadInfo_.chainImg);
			POINT c = { 0, chainsz.cy / 2 };
			float sx = 1.f;	// 拡縮率
			if (mymath::PYTHA(vec.x, vec.y) < mymath::PYTHA(chainsz.cx, chainsz.cy))
			{
				// 画像の幅を切る
				chainsz.cx = static_cast<LONG>(vec.Length2());
			}
			else
			{
				// 画像の拡大率を変える
				sx = vec.Length2() / static_cast<float>(chainsz.cx);
			}
			graph::Draw_GraphicsLeftTop(
				hangPoint_.x,
				hangPoint_.y,
				z + 0.1f, "img_chain",
				0, 0, chainsz.cx, chainsz.cy,
				static_cast<int>(angle), &c,
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

void CPlayer::hit(const ObjPtr& rival)
{
	if (rival->FindName("Polygon"))
	{
		// 壁
		// めり込み補正,通過補正
		const auto& ap = std::dynamic_pointer_cast<CActionPolygon>(rival);
		/*
#ifdef DEF_PREPOS
		mymath::Vec3f dist = obj_.pos - prePos_;
		mymath::Vec3f intersection = ap->IntersectionPoint2Nearest(prePos_, obj_.pos);
		#else
		mymath::Vec3f dist = nextPos() - obj_.pos;
		mymath::Vec3f intersection = ap->IntersectionPoint2Nearest(obj_.pos, nextPos());
		#endif
		obj_.pos = intersection;
		obj_.pos -= dist.Normalize();
		//*/





		// 交点を元に座標補正
		auto& stacols = GetStageCollisions();
		auto& vertexes = ap->vertexes;
		auto it = stageCollisions_.begin();
		for (auto& col : stacols)
		{
			auto& colRc = std::dynamic_pointer_cast <mymath::Rectf>(col);
			auto& rect = std::dynamic_pointer_cast<mymath::Rectf>(*it);

			std::vector<mymath::Vec3f> intersections;
			//if (col->Contains(line, false, true))
			if (ap->Contains(col, false, true))
			{
				// 全交点から補正をかける
				// 交差
				intersections = ap->IntersectionPoint2(col);
			}
			else if (ap->Contains(obj_.pos))
			{
				// 内包
				intersections.push_back(ap->IntersectionPoint2(obj_.pos));
			}
			else
			{
				//col->Contains(, false, true);
				ap->Contains(obj_.pos);
				//intersections.push_back(mymath::);
			}
			mymath::Vec3f adjustPos = intersections[0];
			for (auto& intersection : intersections)
			{
				//======================================
				// 横方向
				if (obj_.add.x < 0.f)
				{
					// ←
					adjustPos.x = min(adjustPos.x, intersection.x);
				}
				else if (obj_.add.x > 0.f)
				{
					// →
					adjustPos.x = max(adjustPos.x, intersection.x);
				}
				//======================================
				// 縦方向
				//---------------------------
				if (obj_.add.y < 0.f)
				{
					// ↑
					adjustPos.y = min(adjustPos.y, intersection.y);
				}
				else if (obj_.add.y > 0.f)
				{
					// ↓
					adjustPos.y = max(adjustPos.y, intersection.y);
				}
			}
			//======================================
			// 横方向
			if (obj_.add.x < 0.f)
			{
				// ←
				obj_.pos.x = adjustPos.x - rect->left + 1.f;
			}
			else if (obj_.add.x > 0.f)
			{
				// →
				obj_.pos.x = adjustPos.x - rect->right - 1.f;
			}
			//======================================
			// 縦方向
			//---------------------------
			if (obj_.add.y < 0.f)
			{
				// ↑
				obj_.pos.y = adjustPos.y - rect->top + 1.f;
			}
			else if (obj_.add.y > 0.f)
			{
				// ↓
				obj_.pos.y = adjustPos.y - rect->bottom - 1.f;
				gravityF_ = false;
			}
			/*
			mymath::Vec3f intersection = colRc->IntersectionPoint2Nearest(line);
			//----------------------------------------------
			// 横方向
			if (obj_.add.x < 0.f && colRc->left < intersection.x )
			{
			// ←へ進んでいた →へ補正
			obj_.pos.x = intersection.x - rect->left + 1;
			}
			if (obj_.add.x > 0.f && intersection.x < colRc->right)
			{
			// →へ進んでいた ←へ補正
			obj_.pos.x = intersection.x - rect->right - 1;
			}
			//---------------------------
			// 縦方向
			if (obj_.add.y < 0.f &&  colRc->top < intersection.y )
			{
			// ↑へ進んでいた ↓へ補正
			obj_.pos.y = intersection.y - rect->top + 1;
			}
			if (obj_.add.y > 0.f && intersection.y < colRc->bottom)
			{
			// ↓へ進んでいた ↑へ補正
			obj_.pos.y = intersection.y - rect->bottom - 1;
			}
			//*/
			/*
			//if ((intersection.y == colRc->top || intersection.y == colRc->bottom)
			//|| (intersection.x == colRc->right || interserc)
			{

			if (obj_.add.y > 0.f)
			obj_.pos.y = intersection.y - rect->bottom;
			else if (obj_.add.y < 0.f)
			obj_.pos.y = intersection.y - rect->top;
			if (intersection.y == colRc->top || intersection.y == colRc->bottom)
			{
			if (obj_.add.x > 0.f)
			obj_.pos.x = intersection.x - rect->right;
			else if (obj_.add.x < 0.f)
			obj_.pos.x = intersection.x - rect->left;
			}


			}
			//*/
#ifdef _DEBUG
			auto lines = ap->MakeLines();
			for (auto& line : lines)
			{
				auto intersections = colRc->IntersectionPoint2(line);
				for (auto& intersection : intersections)
				{
					graph::Draw_Line(
						(int)intersection.x - 3, (int)intersection.y - 3,
						(int)intersection.x + 3, (int)intersection.y + 3,
						0.f,
						0xffffff00, 1);
					graph::Draw_Line(
						(int)intersection.x + 3, (int)intersection.y - 3,
						(int)intersection.x - 3, (int)intersection.y + 3,
						0.f,
						0xffffff00, 1);
				}
			}
#endif
			//break;
			//------------------------------------------------
			// 移動後が別のPolygonにぶつかっていないか判断
			/*
			auto& acpts = sm()->getActionPoints();
			for (auto& acpt : acpts)
			{
			if (acpt->FindName("Polygon"))
			{
			if (acpt->Contains(col))
			{
			hit(acpt);
			break;
			}
			}
			}
			//*/
		
			++it;
		}
		// 補正後の座標にカメラを移動させる
		sm()->MoveCamera(obj_.pos, 10.f);
		obj_.add = 0.f;
	}
	else if (rival->FindName("Atk_"))
	{
		// 敵からの攻撃
		// ぶら下がり状態を打ち消す
		UnHang();
		health_ -= std::dynamic_pointer_cast<IAttack>(rival)->GetForce();
		if (health_ <= 0)
		{
			health_ = 0;
			// ゲームオーバー
			//kill();	
		}
		else
		{
			obj_.src.y = static_cast<int>(MotionType::DAMAGE);
			obj_.src.x = 0;
			motionAnim_.set(2, 0.1f);
			invincibleTime_ = loadInfo_.INV_TIME;
		}
	}
	else if (rival->FindName("PickupJewely"))
	{
		// ピックアップ宝石
		const auto& jewely = std::dynamic_pointer_cast<CPickupJewely>(rival);
		// スコア加算
		auto& sm = std::dynamic_pointer_cast<CScoreMng>(gm()->GetObj(typeid(CScoreMng)));
		sm->AddScore(jewely->getScore());
	}
}


ObjPtr CPlayer::GetPtr()
{
	extern CGameManager *gm;
	auto& pl = gm->GetObjects("Player");
	if (pl.empty()) return nullptr;
	return pl[0];
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
	/*
	MAX_G = info.MAX_G;
	GRAVITY_ACC = info.GRAVITY_ACC;
	TENSION = info.TENSION;
	DOWN_TENSION = info.DOWN_TENSION;
	DOWN_SPEED = info.DOWN_SPEED;
	MAX_VX = info.MAX_VX;
	MAX_VY = info.MAX_VY;
	for (int i = 0; i < arrayof(info.CHAIN_TIME); ++i)
		CHAIN_TIME[i] = info.CHAIN_TIME[i];
	MAX_CHAIN = info.MAX_CHAIN;
	//*/

	loadInfo_ = info;
}


void CPlayer::SetHangPoint(const mymath::Vec3f& pos)
{
	hangPoint_ = pos;
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

void CPlayer::ApplyAttack(const mymath::Vec3f& pos)
{
	// 敵の場所に移動
	//obj_.velocity += (pos - obj_.pos) / FrameTime / 3.f;

	// 相手方向のベクトル
	//mymath::Vec3f vec = pos - obj_.pos;

	gm()->AddObject(ObjPtr(new CEffectSlash(pos,
							math::Calc_RadToDegree(
								mymath::Vec3f::Angle(obj_.pos, pos)))));
	obj_.pos = pos;
	obj_.src.y = static_cast<int>(MotionType::ATTACK);
	obj_.src.x = 0;
	motionAnim_.set(2, 0.15f);
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
	if (chainTime_ > 0.f)
	{
		// Chain中
		chainTime_ = loadInfo_.CHAIN_TIME[1];
	}
	else
	{
		// 初期Kill
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
	// 音階
	std::stringstream slash;
	slash << "se_slash" << std::setw(2) << std::setfill('0') <<
		(chainCnt_ <= MAX_SLASH) ? chainCnt_ : MAX_SLASH;
	se::DSound_Play(slash.str());
}

Base::Collisions CPlayer::GetDamageAreas() const
{
	if (isInvincible())
	{
		// 無敵処理中は当たり判定を消す
		return Base::Collisions();
	}
	return __super::GetDamageAreas();
}

#pragma endregion // CPlayer methods