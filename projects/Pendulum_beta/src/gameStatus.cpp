#include "gameStatus.h"
#include "gameManager.h"

#include "easing.h"

//==================================================================
#pragma region CGameStatus methods

CGameStatus::CGameStatus():
Base("GameStatus")
, slowInterval_(1)
, updateCnt_(0)
{
	std::ifstream f(gm()->fm()->GetPath("#StatusFile"));
	if (!f)
	{
		debug::BToMF("CGameStatus::CGameStatus StatusFile open error");
	}
	else
	{
		if (common::FindChunk(common::SeekSet(f), "#GravityAcc"))
		{
			f >> environment_.gravityAcc;
		}
		if (common::FindChunk(common::SeekSet(f), "#HitStopTime"))
		{
			float stopTime;
			f >> stopTime;
			hitStop_.init(stopTime);
		}
		if (common::FindChunk(common::SeekSet(f), "#SlowInterval"))
		{
			f >> slowInterval_;
		}

	}
	start();

}

CGameStatus::~CGameStatus()
{

}

void CGameStatus::start()
{
	__super::start();

	updateCnt_ = 0;
}

void CGameStatus::step()
{
	updateCnt_ = (updateCnt_ + 1) % INT_MAX;

	hitStop_.step();
}

void CGameStatus::draw()
{
	if (hitStop_.isStopping())
		hitStop_.draw();
}

const CGameStatus::Environment& CGameStatus::getEnv() const
{
	return environment_;
}

void CGameStatus::BeginHitStop(float extendTime)
{
	hitStop_.Begin(extendTime);
}

void CGameStatus::EndHitStop()
{
	hitStop_.End();
}

bool CGameStatus::isHitStopping() const
{
	return hitStop_.isStopping();
}

bool CGameStatus::isUpdatable() const
{
	// ヒットストップ中のみ更新フレームを気にする
	if (isHitStopping())
	{
		return (updateCnt_ % slowInterval_) == 0;
	}
	return true;
}

#pragma endregion	// CGameStatus methods

//==================================================================
//==================================================================
//==================================================================
#pragma region CGameStatus::HitStop methods
CGameStatus::HitStop::HitStop():
hitStopTime_(0.f)
, maxHitStopTime_(0.f)
, backAlpha_(0.f)
, state_(common::DispState::HIDE)
{
}

void CGameStatus::HitStop::init(float stopTime)
{
	hitStopTime_ = 0.f;
	maxHitStopTime_ = stopTime;
	backAlpha_ = 0.f;
	state_ = common::DispState::HIDE;
}

void CGameStatus::HitStop::step()
{
	if (hitStopTime_ < maxHitStopTime_)
		hitStopTime_ += system::FrameTime;
	switch (state_)
	{
	case common::DispState::HIDE:
		break;
	case common::DispState::APPEARING:
		// 濃さ調整
		backAlpha_ = Easing::QuartOut(backAnimTime_, 0.f, 200.f, 0.5f);
		backAnimTime_ += system::FrameTime;
		if (backAlpha_ >= 0.5f)
			state_ = common::DispState::SHOW;
		break;
	case common::DispState::SHOW:
		
		break;
	case common::DispState::DISAPPEARING:
		// 濃さ調整
		backAlpha_ = Easing::QuartOut(backAnimTime_, 200.f, -200.f, 0.5f);
		backAnimTime_ += system::FrameTime;
		if (backAlpha_ >= 0.5f)
			state_ = common::DispState::HIDE;
		break;
	default:
		break;
	}

}
void CGameStatus::HitStop::draw() const
{
	graph::Draw_BoxNC(0, 0, system::WINW, system::WINH, 0.9f,
		ARGB(static_cast<int>(backAlpha_), 0, 0, 0),
		ARGB(static_cast<int>(backAlpha_), 0, 0, 0),
		0, true);
}


void CGameStatus::HitStop::Begin(float extendTime)
{
	hitStopTime_ = -extendTime;
	//backAlpha_ = 0.f;
	state_ = common::DispState::APPEARING;
	switch (state_)
	{
	case common::DispState::HIDE:
	case common::DispState::APPEARING:
		backAnimTime_ = 0.f;
		break;
	case common::DispState::SHOW:
	case common::DispState::DISAPPEARING:
		break;
	}
}


void CGameStatus::HitStop::End()
{
	hitStopTime_ = maxHitStopTime_;
	backAnimTime_ = 0.f;
	state_ = common::DispState::DISAPPEARING;
}
bool CGameStatus::HitStop::isStopping() const
{
	return hitStopTime_ < maxHitStopTime_;
}

#pragma endregion	// CGameStatus::HitStop methods
//==================================================================