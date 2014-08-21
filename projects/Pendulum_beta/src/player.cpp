
#ifdef _DEBUG
//#define D_MOVE_TEST		// キーで移動
//#define D_HANG_TEST			// 減速なし
//#define D_GRAVITY_TEST		// 重力なし
//#define D_COLOR_TEST	// HSV変換テスト
//#define D_LOG_TEST	// 移動軌跡
#define D_SCALE_TEST		// マウススケーリング
#define D_LINE_TEST		// 鎖Line補完
#endif

#define D_ACT_TEST			// マウスクリック座標でぶら下がる
#define D_ATK_TEST		// クリック時攻撃判定
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


#include "effectAfterImage.h"		// 残像
#include "effectSlash.h"

#include "griffon.h"

#ifdef D_GRAVITY_TEST
bool gravityF = false;
#endif

#ifdef _DEBUG
mymath::Vec3f vel_log;	// 最高速度
#endif

#ifdef D_LOG_TEST
#include <deque>
std::deque<POINT> logs;
#endif

#define JUMP_X		(3000.f)		// ジャンプ横距離
#define JUMP_Y		(3000.f)		// ジャンプ縦距離
#define JUMP_TIME	(1.f)		// ジャンプ時間
#define JUMP_G		(100.f)		// ジャンプ中重力


//Max = (n - 0) + (n - g) + (n - 2g) + ・・・ + (n - αg)
//⇒n = { Max / (α + 1) }+αg / 2
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
	// カメラ移動
	//sm()->MoveCamera(obj_.pos.x, obj_.pos.y);
	

#ifdef D_RANGE_TEST
	atk_range.radius = loadInfo_.attackRadius;
#endif
}

void CPlayer::step()
{
	__super::step();			// 更新フレーム数加算

	mymath::Vec3f& pos = obj_.pos;
	mymath::Vec3f& velocity = obj_.add;

	if (sm()->isEnterAnimating())
	{
		// 登場アニメーション
		EnterAnimation();
	}
	else if (sm()->isExitAnimating())
	{
		// 退場アニメーション
		ExitAnimation();
	}
	else
	{
		// 通常時

		// キー入力のみ随時受け付け
		key();

		// Chain文字
#pragma region Chain文字
		{
			const float appearTime = 0.5f;		// 出現アニメーション秒数
			const float appearMoveX = 30.f;		// X移動量
			const float disappearTime = 0.2f;	// 消失アニメーション秒数
			switch (chainState_)
			{
			case common::DispState::HIDE:
				/*
				if ((chainTime_ -= system::ONEFRAME_TIME) <= 0.f)
				{
					// 消失
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
					// 出現完了
					chainState_ = common::DispState::SHOW;
				}
				// 残り時間は出現中も減少していくためbreakは書かない
			case common::DispState::SHOW:
				if (chainTime_ <= disappearTime)
				{
					// 消失開始
					chainState_ = common::DispState::DISAPPEARING;
					chainAnimTime_ = 0.f;
				}
				numberPos_.x = chainMsg_->pos.x;
				numberPos_.y = Easing::BackOut(numberAnimTime_, chainStaPos_.y + 10.f, -20.f, 0.2f);
				numberAnimTime_ += system::FrameTime;

				if (chainState_ == common::DispState::APPEARING) break;
				// SHOWのみの処理
				chainMsg_->alpha = 255.f;
				chainMsg_->pos.x = chainStaPos_.x + appearMoveX;
				break;

			case common::DispState::DISAPPEARING:
				chainMsg_->alpha = Easing::Linear(chainAnimTime_, 255.f, -255.f, disappearTime);
				chainMsg_->pos.x = Easing::QuadOut(chainAnimTime_, chainStaPos_.x + appearMoveX, 10.f, disappearTime);
				if ((chainAnimTime_ += system::FrameTime) >= disappearTime)
				{
					// 消失完了
					chainState_ = common::DispState::HIDE;
					chainMsg_->pos = chainStaPos_;
					chainMsg_->alpha = 0.f;
					chainAnimTime_ = 0.f;
					chainCnt_ = 0;
				}
				break;
			}
		}
#pragma endregion // Chain文字


		// 更新フレームでない場合はスキップ
		if (!isUpdatable())
		{
			if (gm()->gameStatus()->isHitStopping())
				attackRadius_ = -50.f;
			return;
		}
		
		//----------------------------------------
		// Chain
		// Chain時間は処理落ちなどに影響されて、一定のゲーム内秒数で終わる
		chainTime_ -= system::ONEFRAME_TIME;

		//----------------------------------------
		// 移動処理
		move();

		//----------------------------------------
		// 攻撃範囲
		if (attackRadius_ < loadInfo_.maxAttackRadius)
			attackRadius_ += loadInfo_.addRadius * system::FrameTime;
		else
			attackRadius_ = loadInfo_.maxAttackRadius;

		//----------------------------------------
		// カメラ
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
	// 攻撃範囲
	UpdateAttackObject();

	//----------------------------------------
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
		if (invincibleAnim_ >= loadInfo_.invincibleTime / 20.f)
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
		// Chain中
		chainMsg_->drawNC(charabase::CharBase::LeftTop);
		// Chain数を画像パターンに
		const int width = 32;
		const int height = 64;
		charabase::CharBase numBase(numberPos_, mymath::Vec3f(), "img_UInumber", width, height);
		auto numObjs = common::GetNumberImageObjects(chainCnt_, numBase, common::RIGHT);
		for (auto& nobj : numObjs)
			nobj.drawNC(charabase::CharBase::LeftTop);
		/*
		int digit = (chainCnt_ / 100 == 0) ? ((chainCnt_ / 10 == 0) ? 1 : 2) : 3;
		int work = chainCnt_;	// chain数を桁ごとに区切る
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
		//*/
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
			draw_y + height, 0.2f,
			ARGB(200, 10, 10, 10), ARGB(255, 20, 100, 100),
			5, true);
		//------------------------
		// 中
		// 体力に応じて幅を計算
		float w = static_cast<float>(health_) / static_cast<float>(loadInfo_.health) * width;
		// 下半分
		graph::Draw_BoxNC(
			draw_x, draw_y,
			draw_x + static_cast<int>(w),
			draw_y + height, 0.1f,
			ARGB(255, 100, 200, 200)
			, ARGB(255, 50, 150, 150),
			0, true);
		// 上半分
		graph::Draw_BoxNC(
			draw_x, draw_y,
			draw_x + static_cast<int>(w),
			draw_y + height / 2, 0.1f,
			ARGB(255, 45, 150, 170), ARGB(255, 50, 150, 150),
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
	// 横方向
	obj_.add.x = (JUMP_X / JUMP_TIME);
	// 縦方向
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
	// 移動
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
		// 掴んでる状態から離した時
		UnHang();
	}
#endif

#ifdef D_RANGE_TEST
	atk_range.center = obj_.pos;
#endif

	//-------------------------------------------
	// 攻撃
	isAttacking_ = false;
	// ヒットストップ中かで条件を変える
	if (gm()->gameStatus()->isHitStopping())
	{
		// ヒットストップ中は移動or攻撃ボタン押し
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
	// ヒットストップ中以外は攻撃ボタン押下
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
	// 重力
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
		// ぶら下がり中
		gravityF_ = true;
		mymath::Vec3f dist = hangPoint_ - pos;	// pos -> hang

		const float min_radius = 150.f;	// 最小距離(振り子の半径)

		float angle = std::atan2f(dist.y, dist.x);
		mymath::Vec3f tensionForce = mymath::Vec3f::Rotate(angle) * tensionAcc_;
		velocity += tensionForce;
		tensionAcc_ *= loadInfo_.DOWN_TENSION;

		const float CAMERA_SCALE_MIN = 0.75f;
		if (mymath::POW2(tensionAcc_) < mymath::POW2(0.5f))
		{
			// 引っ張られなくなっている間
			tensionAcc_ = 0.f;
			if (!isCameraZoom_)
				zoomTime_ = ZOOM_TIME / 10.f;
			isCameraZoom_ = true;
		}
		else
		{
			// 引っ張られてる間
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
					//velocity = stopPos - pos;
					pos = stopPos;
					velocity = 0.f;
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

}


void CPlayer::UpdateAttackObject()
{
	// 座標
	attackObj_->pos.x = obj_.pos.x;
	attackObj_->pos.y = obj_.pos.y;

	// 大きさ
	attackObj_->scale = attackRadius_ * 2.f / 400.f;
	
	// 角度
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
	// 角度分
	for (size_t i = 0; i < attackObjAngle_.size(); ++i)
	{
		// 編集用オブジェクト
		auto work = *attackObj_;
		// 画像番号
		work.src.x = static_cast<int>(i);
		// 角度
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
		// 補正後の座標にカメラを移動させる
		sm()->MoveCamera(obj_.pos, 10.f);
	}
	if (rival->FindName("Atk_") || rival->FindName("Griffon"))
	{
		if (rival->FindName("Atk_"))
		{
			// 敵からの攻撃
			health_ -= std::dynamic_pointer_cast<IAttack>(rival)->GetForce();
		}
		else
		{
			// グリフォンタックル
			auto& gri = std::dynamic_pointer_cast<CGriffon>(rival);
			// グリフォンタックル中でない場合はダメージ処理系をスキップする
			if (!gri->isAttacking()) return;
			health_ -= gri->getPower();
		}
		// ぶら下がり状態を打ち消す
		UnHang();
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
			invincibleTime_ = loadInfo_.invincibleTime;
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


Base::Collisions CPlayer::GetDamageAreas() const
{
	if (isInvincible() || gm()->gameStatus()->isHitStopping())
	{
		// 無敵処理中は当たり判定を消す
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
	// ヒットストップ開始
	gm()->gameStatus()->BeginHitStop();

	//---------------------------------
	// 体当たり範囲縮小
	attackRadius_ = -50.f;

	// 相手方向のベクトル
	mymath::Vec3f vec = pos - obj_.pos;

	//---------------------------------
	// 相手の方を向く
	turnFlag_ = vec.x < 0.f;

	//---------------------------------
	// アニメーション
	obj_.src.y = static_cast<int>(MotionType::ATTACK);
	obj_.src.x = 0;
	motionAnim_.set(2, 0.25f);

	//---------------------------------
	// 斬り攻撃時残像処理
	if (type == AttackType::SLASH)
	{
		// 残っている残像の削除
		auto& afterImages = gm()->GetObjects("EffectAfterImage");
		for (auto& img : afterImages)
			img->kill();
		// 新しい残像の追加
		gm()->AddObject(ObjPtr(new CEffectAfterImage(obj_, turnFlag_, obj_.pos, pos, 10)));
	}

	//---------------------------------
	// 斬撃エフェクト
	gm()->AddObject(ObjPtr(new CEffectSlash(pos.TmpReplace(mymath::Vec3f::Z,pos+0.1f),
		math::DegreeOfPoints2(
		obj_.pos.x, obj_.pos.y,
		pos.x, pos.y))));

	//---------------------------------
	// 移動
	obj_.pos.x = pos.x;
	obj_.pos.y = pos.y;
	sm()->MoveCamera(obj_.pos);	// 移動後座標にカメラを向かせる


	//---------------------------------
	// 敵方向へ進むフォロースルー
	obj_.add = vec / system::FrameTime;

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
		case common::DispState::HIDE:
		case common::DispState::APPEARING:
			// 出現開始
			chainState_ = common::DispState::APPEARING;
			chainAnimTime_ = 0.f;
			break;
		case common::DispState::SHOW:
		case common::DispState::DISAPPEARING:
			// 表示継続
			chainState_ = common::DispState::SHOW;
			break;
		}
	}
	// 音階
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