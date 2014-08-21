#ifdef _DEBUG

#endif

#include "define.h"
#include "raybit.h"
#include "nWayShot.h"
#include "setting.h"

#include "common.h"

#include "effectAfterImage.h"
#include "effectFlash.h"

void (CRaybit::*CRaybit::StateStep_[])() =
{
	&CRaybit::WaitStep,
	&CRaybit::ChaseStep,
	&CRaybit::LostStep,
	&CRaybit::AttackStep,
	&CRaybit::DestroyStep,
};

CRaybit::CRaybit() :
IEnemy("E_Raybit")
, eye_(charabase::BaseData("img_raybitEye", -1, -1))
{
	//std::vector<int> move = { 1, 2, 4, 3, 2 };
	std::vector<int> move = { 0 };
	motionTable_.push_back(move);
	//std::vector<int> attack = { 1, 0, 1, 3, 4 };
	//motionTable_.push_back(attack);

	common::color::Color_SeparateARGB(ARGB(255, 63, 63, 255), eye_.alpha, eye_.r, eye_.g, eye_.b);

	
	targetCursor_ = charabase::CharBase(charabase::BaseData("img_cursor0", 64, 64));
	targetCursor_.g = 0.f;
	targetCursor_.b = 0.f;
	targetCursor_.SetUse(false);
}

CRaybit::CRaybit(const mymath::Vec3f& pos) :
IEnemy("E_Raybit")
{
	init(pos);
}
CRaybit::CRaybit(float x, float y, float z) :
IEnemy("E_Raybit")
{
	init(mymath::Vec3f(x, y, z));
}

CRaybit::~CRaybit()
{

}


void CRaybit::init(const mymath::Vec3f& pos)
{
	using common::FindChunk;

	gm()->GetData(*this);


	prePos_ = obj_.pos = pos;

	startPos_ = obj_.pos;

	elapsedTime_ = 0.f;

	state_ = State::WAIT;
	motionType_ = MotionType::MOVE;
	motionAnim_.set(motionTable_[static_cast<int>(motionType_)].size() - 1, 0.3f);
	obj_.src.x = motionTable_[static_cast<int>(motionType_)][motionAnim_.no];

	// 最初向いている向き
	turnFlag_ = math::GetRandom<int>(1, 100) <= 50;

	// 向きに合わせて移動させる
	obj_.add.x = turnFlag_ ? -loadInfo_.moveSpeed : loadInfo_.moveSpeed;
	nextActTime_ = loadInfo_.moveTime;
	moveState_ = MoveState::MOVE;

	attackNum_ = loadInfo_.attackNum;

}

void CRaybit::step()
{
	ICharacter::step();

	DecideState();

	// ヒットストップ中はスキップ
	if (!isUpdatable()) return;

	elapsedTime_ += system::FrameTime;

	// アニメーション処理
	if (motionAnim_.step())
	{
		if (motionType_ != MotionType::MOVE)
		{
			obj_.src.x = 0;
			obj_.src.y = 0;
			motionType_ = MotionType::MOVE;
			motionAnim_.set(motionTable_[static_cast<int>(motionType_)].size() - 1, 0.3f);
		}
	}
	else
	{
		obj_.src.x = motionTable_[static_cast<int>(motionType_)][motionAnim_.no];
	}

	//------------------------------------------------
	// 攻撃
	if (attack_ != nullptr)
		attack_->step();

	//------------------------------------------------
	// 目
	// 座標,角度合わせ
	eye_.pos = obj_.pos;
	eye_.angle = obj_.angle;
	// 色決め
	D3DCOLOR argb = ARGB(255, 255, 255, 255);
	switch (state_)
	{
	case CRaybit::State::WAIT:
		argb = ARGB(255, 63, 63, 255);
		break;
	case CRaybit::State::CHASE:
		argb = ARGB(255, 255, 15, 15);
		break;
	case CRaybit::State::LOST:
		argb = ARGB(255, 63, 63, 255);
		break;
	case CRaybit::State::ATTACK:
		argb = ARGB(255, 255, 15, 15);
		break;
	case CRaybit::State::DESTROY:
		break;
	default:
		break;
	}
	common::color::Color_SeparateARGB(argb, eye_.alpha, eye_.r, eye_.g, eye_.b);

	//------------------------------------------------
	// 照準
	if (state_ == State::ATTACK && targetCursor_.CheckUse())
	{
		const mymath::Vec3f& plPos = gm()->GetPlayerPos();
		targetCursor_.pos = plPos;
		targetCursor_.scale -= 9.f * system::FrameTime / (loadInfo_.attackInterval * 0.9f);
		if (targetCursor_.scale.x < 1.f)
			targetCursor_.scale = 1.f;
	}



	(this->*StateStep_[static_cast<int>(state_)])();

}

void CRaybit::draw()
{
	if (attack_.get())
		attack_->draw();
	if (targetCursor_.CheckUse())
		targetCursor_.draw();
	mymath::Rectf rect = camera::GetScreenRect();
	if (rect.Contains(obj_.GetRect()))
	{
		obj_.draw(charabase::CharBase::MODE::Center, turnFlag_);
		eye_.draw(charabase::CharBase::MODE::Center, turnFlag_);

#ifdef _DEBUG

		auto rc = obj_.GetRect();
		rc.Offset(obj_.pos);
		std::string states[] = {
			"WAIT",
			"CHASE",
			"LOST",
			"ATTACK",
			"DESTROY",
		};
		font::Draw_FontText(
			static_cast<int>(rc.left),
			static_cast<int>(rc.bottom),
			obj_.pos.z,
			states[static_cast<int>(state_)], -1, 0);
		rc.draw(-1, 1, false);
#endif
	}
}


void CRaybit::WaitStep()
{

	if (elapsedTime_ > nextActTime_)
		nextActTime_ = elapsedTime_ + loadInfo_.moveInterval;
	else
		Move();
}

void CRaybit::ChaseStep()
{
	const mymath::Vec3f& plPos = gm()->GetPlayerPos();
	const mymath::Vec3f dist = plPos - obj_.pos;
	if (moveState_ == MoveState::STOP && elapsedTime_ > nextActTime_)
	{
		float angle;
		if (mymath::POW2(dist.x) > mymath::POW2(dist.y))
		{
			// 横移動
			if (dist.x > 0.f)	// →
				angle = 0.f;
			else				// ←
				angle = mymath::PI * 2.f;
		}
		else
		{
			// 縦移動
			if (dist.y > 0.f)	// ↓
				angle = mymath::PI * 3.f;
			else				// ↑
				angle = mymath::PI;
		}
		obj_.add = mymath::Vec3f::Rotate(angle) * loadInfo_.moveSpeed;
		nextActTime_ = elapsedTime_ + loadInfo_.moveTime;
		moveState_ = MoveState::MOVE;
	}
	if (moveState_ == MoveState::MOVE)
	{
		Move();
		if (elapsedTime_ > nextActTime_)
		{
			moveState_ = MoveState::STOP;
			nextActTime_ = elapsedTime_ + loadInfo_.moveInterval;
		}
	}
}

void CRaybit::LostStep()
{
	// 元いた地点へ
	mymath::Vec3f dist = startPos_ - obj_.pos;

	if (mymath::PYTHA(dist.x, dist.y) < mymath::POW2(loadInfo_.moveSpeed*system::ONEFRAME_TIME))
	{
		state_ = State::WAIT;
		startPos_ = obj_.pos;
	}
	if (moveState_ == MoveState::STOP && elapsedTime_ > nextActTime_)
	{
		float angle;
		if (mymath::POW2(dist.x) > mymath::POW2(dist.y))
		{
			// 横移動
			if (dist.x > 0.f)	// →
				angle = 0.f;
			else				// ←
				angle = mymath::PI * 2.f;
		}
		else
		{
			// 縦移動
			if (dist.y > 0.f)	// ↓
				angle = mymath::PI * 3.f;
			else				// ↑
				angle = mymath::PI;
		}
		obj_.add = mymath::Vec3f::Rotate(angle) * loadInfo_.moveSpeed;
		nextActTime_ = elapsedTime_ + loadInfo_.moveTime;
		moveState_ = MoveState::MOVE;
	}
	if (moveState_ == MoveState::MOVE)
	{
		Move();
		if (elapsedTime_ > nextActTime_)
		{
			moveState_ = MoveState::STOP;
			nextActTime_ = elapsedTime_ + loadInfo_.moveInterval;
		}
	}

}

void CRaybit::AttackStep()
{
	// 攻撃
	if (moveState_ == MoveState::MOVE)
	{
		Move();
		if (elapsedTime_ > nextActTime_)
		{
			moveState_ = MoveState::STOP;
			// 連続間隔
			nextActTime_ = elapsedTime_ + loadInfo_.attackInterval;
			// 連続攻撃数
			attackNum_ = loadInfo_.attackNum;
			//-----------------------------------------
			// 予備動作系
			attackPreliminary_ = true;
			// 照準
			targetCursor_.SetUse(false);
		}
	}
	else if (elapsedTime_ > nextActTime_)
	{
		int i = se::DSound_Play("se_shot1");
		// パン振り
		mymath::Vec3f cameraCenter = camera::GetLookAt();
		auto screenWidth = gm()->winRect()->width();
		mymath::Vec3f dist = obj_.pos - cameraCenter;
		se::DSound_SetPan("se_shot1", i, static_cast<long>(10000.f * dist.x / (screenWidth / 2.f)));
		if (CreateAttack())
		{
			// 次の攻撃に備える
			nextActTime_ = elapsedTime_ + loadInfo_.oneAttackInterval;
		}
		else
		{
			// 攻撃終了
			state_ = State::WAIT;
			// 連続間隔
			nextActTime_ = elapsedTime_ + loadInfo_.attackInterval;
			// 連続攻撃数
			attackNum_ = loadInfo_.attackNum;
			//-----------------------------------------
			// 予備動作系
			attackPreliminary_ = true;
			// 照準
			targetCursor_.SetUse(false);
		}
	}
	else
	{
		// 溜め中
		if (attackPreliminary_)
		{
			mymath::Vec3f plpos = gm()->GetPlayerPos();		// プレイヤー座標
			// プレイヤーを向かせる
			Look(plpos);
			// プレイヤーからの向き
			float fromPlayer = mymath::Vec3f::Angle2(plpos, obj_.pos);


			attackPreliminary_ = false;
			//-----------------------------------------
			// Flashエフェクト
			// 発射口から
			mymath::Vec3f pos = obj_.pos + mymath::Vec3f::Rotate(math::Calc_DegreeToRad(obj_.angle)) * loadInfo_.attackPos[0] * ((turnFlag_) ? -1.f : 1.f);
			pos.z = obj_.pos.z - 0.1f;
			// プレイヤーとは反対方向へエフェクト出力
			mymath::Vec3f dir = mymath::Vec3f::Rotate(math::Calc_DegreeToRad(fromPlayer));
			gm()->AddObject(ObjPtr(new CEffectFlash(pos, dir*4.f, loadInfo_.attackInterval*0.9f, 0.5f)));
			//-----------------------------------------
			// 照準
			targetCursor_.SetUse(true);
			targetCursor_.scale = 10.f;
			targetCursor_.src.x = 1;
			targetCursor_.alpha = 200.f;
		}
	}
}

void CRaybit::DestroyStep()
{
	if (elapsedTime_ < nextActTime_ + 0.5f)
	{
		// 震え
		obj_.pos.x += math::GetRandom(-5.f, 5.f);
		obj_.pos.y += math::GetRandom(-5.f, 5.f);
	}
	else
	{
		// 落下
		obj_.add.y += gm()->gameStatus()->getEnv().gravityAcc;
		obj_.Move();
		if (!InScreen(-obj_.GetSize().y))
		{
			obj_.alpha = 0.f;
			state_ = State::WAIT;
			kill();
		}
	}
}

void CRaybit::Move()
{
	turnFlag_ = obj_.add.x < 0.f;
	prePos_ = obj_.pos;
	obj_.Move();

	auto& rect = sm()->getStageRect();
	if (obj_.pos.x - obj_.HalfWidth() < rect.left)
	{
		obj_.pos.x = rect.left + obj_.HalfWidth();
		moveState_ = MoveState::STOP;
	}
	else if (obj_.pos.x + obj_.HalfWidth() > rect.right)
	{
		obj_.pos.x = rect.right - obj_.HalfWidth();
		moveState_ = MoveState::STOP;
	}

	if (obj_.pos.y - obj_.HalfHeight() < rect.top)
	{
		obj_.pos.y = rect.top + obj_.HalfHeight();
		moveState_ = MoveState::STOP;
	}
	else if (obj_.pos.y + obj_.HalfHeight() > rect.bottom)
	{
		obj_.pos.y = rect.bottom - obj_.HalfHeight();
		moveState_ = MoveState::STOP;
	}
	
	// 残像生成
	gm()->AddObject(ObjPtr(new CEffectAfterImage(eye_, turnFlag_, prePos_, obj_.pos, 3, gplib::RENDER_MODE::ADD, 220.f, 1.0f, 0.f)));
}


void CRaybit::DecideState()
{
	if (state_ == State::DESTROY)
	{
		// 死亡中 復活は許されない
		return;
	}
	// プレイヤー座標
	const mymath::Vec3f& plPos = gm()->GetPlayerPos();
	
	// 初期位置へのベクトル now -> start
	mymath::Vec3f vdist(startPos_ - obj_.pos);
	auto atkRect = loadInfo_.attackRect;
	atkRect.Offset(obj_.pos);
	auto searchRect = loadInfo_.searchRect;
	searchRect.Offset(obj_.pos);
	auto chaseRect = loadInfo_.chaseRect;
	chaseRect.Offset(startPos_);
	if (state_ == State::ATTACK || atkRect.Contains(plPos))
	{
		// 攻撃範囲内 or 攻撃中
		if (state_ != State::ATTACK && moveState_ == MoveState::STOP)	// 攻撃範囲内に敵(プレイヤー)が入った時
			nextActTime_ = elapsedTime_ + loadInfo_.attackInterval;
		state_ = State::ATTACK;
		//motionType_ = MotionType::ATTACK;
	}
	else if (searchRect.Contains(plPos))
	{
		// 攻撃範囲外 索敵範囲内
		//if (staDist < mymath::POW2(loadInfo_.chaseRange))
		// 追跡範囲を初期位置を中心にずらしてから計算
		if (chaseRect.Contains(obj_.pos))
		{
			// 追跡可能範囲内
			state_ = State::CHASE;
		}
		else
		{
			// 追跡可能範囲外
			state_ = State::ATTACK;
		}
	}
	else
	{
		// 索敵範囲外
		state_ = State::LOST;
		motionType_ = MotionType::MOVE;
	}
	
}

bool CRaybit::CreateAttack()
{
	const mymath::Vec3f& mypos = obj_.pos;
	const mymath::Vec3f& plpos = gm()->GetPlayerPos();
	const float SP = 100.f;			// 初速度
	const float ACC = 0.f;			// 加速度
	mymath::Vec3f shotPos;
	for (auto& pos : loadInfo_.attackPos)
	{
		shotPos = mypos + (mymath::Vec3f::Rotate(math::Calc_DegreeToRad(obj_.angle)) * pos);
		float angle = math::Calc_RadToDegree(mymath::Vec3f::Angle2(shotPos, plpos));
		std::dynamic_pointer_cast<CNWayShot>(attack_)->CreateAttack(
			shotPos,
			1,
			angle, 0,
			SP, ACC, false);
	}
	attackNum_ -= 1;
	return attackNum_ > 0;
}


void CRaybit::hit(const ObjPtr& rival)
{
	// Polygon
	__super::hit(rival);
}



bool CRaybit::ApplyDamage(int dam)
{
	// 死亡アニメーション中はスキップ
	if (state_ == State::DESTROY) return true;
	// ダメージが入ったら即死
	state_ = State::DESTROY;
	obj_.add = 0.f;
	nextActTime_ = elapsedTime_;

	// 爆散エフェクト
	for (int i = 0; i < 3; ++i)
		gm()->AddObject(ObjPtr(new CEffectExplosion(obj_.pos)));
	// SE
	se::DSound_Play("se_explosion");

	// ピックアップアイテム
	gm()->AddObject(ObjPtr(new CPickupJewely(obj_.pos)));

	return true;
}

Base::Collisions CRaybit::GetDamageAreas() const
{
	// 死亡アニメーション中はスキップ
	if (state_ != State::DESTROY)
	{
		return __super::GetDamageAreas();
	}
	return Base::Collisions();
}

void CRaybit::SetInfo(const LoadInfo& info)
{

	loadInfo_ = info;
}
