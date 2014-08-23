#ifdef _DEBUG
#define DEF_SHAPE_DRAW
#endif

#include "define.h"
#include "griffon.h"
#include "tackle.h"
#include "setting.h"

//#include "player.h"
#include "common.h"

#include "actionPoint.h"

void (CGriffon::*CGriffon::StateStep_[])() =
{
	&CGriffon::EntryStep,
	&CGriffon::WaitStep,
	&CGriffon::ChaseStep,
	&CGriffon::DamageStep,
	&CGriffon::AttackStep,
	&CGriffon::DestroyStep,
};

CGriffon::CGriffon() :
IEnemy("E_Boss_Griffon")
{
	std::vector<int> move = { 0, 1, 2, 3, 2, 1 };
	motionTable_.push_back(move);
	std::vector<int> entry = { 0, 1, 2, 3, 2, 1, 0 };
	motionTable_.push_back(entry);
	std::vector<int> attack = { 0, 1, 2, 3 };
	motionTable_.push_back(attack);
	std::vector<int> fall = { 0, 1, 2, 3};
	motionTable_.push_back(fall);
	attack_ = AttackPtr(new CTackle());
}

CGriffon::CGriffon(const mymath::Vec3f& pos) :
IEnemy("E_Boss_Griffon")
{
	init(pos);
}
CGriffon::CGriffon(float x, float y, float z) :
IEnemy("E_Boss_Griffon")
{
	init(mymath::Vec3f(x, y, z));
}

CGriffon::~CGriffon()
{

}


void CGriffon::init(const mymath::Vec3f& pos)
{

	gm()->GetData(*this);

	obj_.pos = pos;

	startPos_ = obj_.pos;
	endPos_   = obj_.pos;
	backPos_  = obj_.pos;
	actPos_   = obj_.pos;

	obj_.pos.x += loadInfo_.entryWidth;
	obj_.pos.y += loadInfo_.entryHeight;

	elapsedTime_ = 0.f;
	nextActTime_ = 0.f;


	roarFlag_ = false;

	battleState_ = BattleState::ENTRY;
	motionType_ = MotionType::MOVE;
	motionAnim_.set(motionTable_[static_cast<int>(motionType_)].size() - 1, loadInfo_.moveAnimSpeed);
	obj_.src.x = motionTable_[static_cast<int>(motionType_)][motionAnim_.no];


	invincibleTime_ = 0.f;
	invincibleAnim_ = 0.f;

	fallTurnCount_ = 0.f;

	health_ = loadInfo_.health;
	power_ = loadInfo_.power;

	sway_ = 0.f;
}

void CGriffon::step()
{
	ICharacter::step();
	elapsedTime_ += system::FrameTime;

	// 無敵
	if (isInvincible())
	{
		// 点滅アニメ
		invincibleAnim_ += system::ONEFRAME_TIME;
		if (invincibleAnim_ >= loadInfo_.INV_TIME / 20.f)
		{
			invincibleAnim_ = 0.f;
			if (obj_.alpha > 200.f)
			{
				obj_.alpha = 150.f;		// 消す
				obj_.g = 20.f;
				obj_.b = 20.f;

			}
			else
			{
				obj_.alpha = 220.f;		// 出す
				obj_.g = 220.f;
				obj_.b = 220.f;
			}
		}
		// 時間減少
		invincibleTime_ -= system::ONEFRAME_TIME;
		if (invincibleTime_ < 0.f)
		{
			// 無敵終了
			obj_.alpha = 255.f;
			obj_.g = 255.f;
			obj_.b = 255.f;
		}
	}

	DecideState();

	// アニメーション処理
	if (motionAnim_.step())
	{
		if (motionType_ != MotionType::MOVE)
		{
			obj_.src.x = 0;
			obj_.src.y = 0;
			motionType_ = MotionType::MOVE;
			motionAnim_.set(motionTable_[static_cast<int>(motionType_)].size() - 1, loadInfo_.moveAnimSpeed);
		}
	}
	else
	{
		obj_.src.x = motionTable_[static_cast<int>(motionType_)][motionAnim_.no];
		obj_.src.y = static_cast<int>(motionType_);
	}

	if (isAttack())
	{
		if (attack_ != nullptr)
			attack_->step();
	}

	// プレイヤーを向く
	if (!isBack() && !isAttack() &&
		battleState_ != BattleState::DESTROY)
	{
		const mymath::Vec3f& plPos = gm()->GetPlayerPos();

		if (obj_.pos.x < plPos.x && turnFlag_)
			turnFlag_ ^= 1;
		else if (obj_.pos.x > plPos.x && !turnFlag_)
			turnFlag_ ^= 1;
	}

	(this->*StateStep_[static_cast<int>(battleState_)])();

}

void CGriffon::draw()
{
	if (attack_ != nullptr)
		attack_->draw();
	mymath::Rectf rect = camera::GetScreenRect();
	if (rect.Contains(obj_.GetRect()))
	{
		obj_.draw(charabase::CharBase::MODE::Center, turnFlag_);
	}
	
#ifdef _DEBUG
	static const std::string emply_string;
	mymath::Recti rc = obj_.GetRect();

	//rc.offset((int)obj_.pos.x, (int)obj_.pos.y);
	std::stringstream ss;
	ss << "pos("
		<< std::setw(4) << (int)obj_.pos.x
		<< ","
		<< std::setw(4) << (int)obj_.pos.y
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
	ss << "backPos("
		<< std::setw(4) << (int)backPos_.x
		<< ","
		<< std::setw(4) << (int)backPos_.y
		<< ")";
	font::Draw_FontText(
		rc.right, rc.top + font::Draw_GetCharHeight(setting::GetFontID("font_MSG15")) * 2, 0.f,
		ss.str(), 0xffff0000,
		setting::GetFontID("font_MSG15"));

	ss.str(emply_string);
	ss << "endPos("
		<< std::setw(4) << (int)endPos_.x
		<< ","
		<< std::setw(4) << (int)endPos_.y
		<< ")";
	font::Draw_FontText(
		rc.right, rc.top + font::Draw_GetCharHeight(setting::GetFontID("font_MSG15")) * 3, 0.f,
		ss.str(), 0xffff0000,
		setting::GetFontID("font_MSG15"));


#endif
}

void CGriffon::EntryStep()
{
	const mymath::Vec3f dist = actPos_ - obj_.pos;
	const float allow = 3.f;	// 座標ずれの許容範囲
	// 移動中
	if (!roarFlag_)
	{
		if (abs(dist.x) <= allow)
		{
			obj_.pos.x = actPos_.x;
			obj_.add = 0.f;
			obj_.add.y = loadInfo_.MOVE_SPEED;
		}
		else
		{
			obj_.add.x = loadInfo_.MOVE_SPEED;
			if (obj_.pos.x > actPos_.x)
			{
				obj_.add.x = -loadInfo_.MOVE_SPEED;
			}
		}
		if (abs(dist.y) <= allow)
		{
			//咆哮始動
			obj_.pos = actPos_;
			obj_.add = 0.f;
			roarFlag_ = true;
			motionType_ = MotionType::ROAR;
			motionAnim_.set(motionTable_[static_cast<int>(motionType_)].size() - 1, loadInfo_.roarAnimSpeed);

		}

		obj_.Move();
	}
	// 咆哮中は何もしない
	else
	{
		int max = motionTable_[static_cast<int>(motionType_)].size() - 1;
		// 咆哮終了
		if (motionAnim_.no > max -1)
		{
			battleState_ = BattleState::WAIT;
		}
	}
}

void CGriffon::WaitStep()
{

}

void CGriffon::ChaseStep()
{
	const mymath::Vec3f& plPos = gm()->GetPlayerPos();
	const float radian = mymath::Vec3f::Angle(obj_.pos, plPos);
	obj_.add = mymath::Vec3f::Rotate(radian) * loadInfo_.MOVE_SPEED;
	obj_.Move();
}

void CGriffon::DamageStep()
{
	if (elapsedTime_ > nextActTime_ + loadInfo_.damageTime)
	{
		backFlag_ = false;
		sway_ = 0;

		battleState_ = BattleState::WAIT;
		motionType_ = MotionType::MOVE;
		motionAnim_.set(motionTable_[static_cast<int>(motionType_)].size() - 1, loadInfo_.moveAnimSpeed);
	}

	sway_ = static_cast<float>((static_cast<int>(sway_ + 45) % 360));
	// 横揺れ
	obj_.add.x = std::cosf(math::Calc_DegreeToRad(sway_)) * loadInfo_.swayRange;
	obj_.add.y = 0.f;
	obj_.Move();
}

void CGriffon::AttackStep()
{
	// 行動を起こす時間になったら後退開始
	if (elapsedTime_ > nextActTime_)
	{
		actPos_ = obj_.pos;
		const mymath::Vec3f& plpos = gm()->GetPlayerPos();
		// グリフォンとプレーヤーとの角度
		float radian = mymath::Vec3f::Angle(obj_.pos, plpos);
		//obj_.angle = angle;

		// バック距離
		obj_.add = mymath::Vec3f::Rotate(radian) * -loadInfo_.MOVE_SPEED;

		nextActTime_ = elapsedTime_ + loadInfo_.attackInterval;		// 連続間隔
		backFlag_ = true;

		motionType_ = MotionType::ATTACK;
		motionAnim_.set(motionTable_[static_cast<int>(motionType_)].size() - 1, loadInfo_.backAnimSpeed);
	}
	// 後退中なら
	if (isBack())
	{
		mymath::Vec3f dist = obj_.pos - actPos_;
		float length = dist.Length2();

		if (motionTable_[static_cast<int>(motionType_)][motionAnim_.no] == 2)
		{
			motionAnim_.stop();
		}

		// 後退したら突進開始
		if (length > loadInfo_.backDist)
		{
			// アニメーションの修正
			if (motionAnim_.isStoped())
			{
				motionAnim_.no++;
			}
			else
			{
				motionAnim_.no = motionTable_[static_cast<int>(motionType_)].size() - 1;
				motionAnim_.stop();
			}

			backFlag_ = false;
			attackFlag_ = true;
			const mymath::Vec3f& plpos = gm()->GetPlayerPos();
			endPos_ = plpos;
			dist = obj_.pos - endPos_;
			float radian = mymath::Vec3f::Angle(obj_.pos, endPos_);
			obj_.add = mymath::Vec3f::Rotate(radian) * loadInfo_.ATTACK_SPEED;
		}
		else
		{
			obj_.Move();

			if (length > loadInfo_.attackDist)
			{
				obj_.add.x = 0.f;
				obj_.add.y = 0.f;
				battleState_ = BattleState::WAIT;
			}
		}
	}
	// 突進中なら
	if (isAttack())
	{
		mymath::Vec3f dist = obj_.pos - actPos_;
		float length = dist.Length2();
		
		if (length > loadInfo_.attackDist)
		{
			attackFlag_ = false;
			battleState_ = BattleState::WAIT;
			motionAnim_.start();
			nextActTime_ = elapsedTime_ + loadInfo_.attackInterval;
		}
		else
		{
			obj_.Move();
		}
	}
}

void CGriffon::DestroyStep()
{
	if (elapsedTime_ > nextActTime_)
	{
		fallTurnCount_++;
		if (fallTurnCount_ > loadInfo_.fallTurnSpeed)
		{
			fallTurnCount_ = 0.f;
			turnFlag_ ^= 1;
		}
		obj_.add.x = 0.f;
		obj_.add.y = loadInfo_.fallSpeed;
		obj_.Move();
		motionAnim_.stop();
		
	}
	// カメラの外に出たら
	if (obj_.pos.y + obj_.HalfHeight() > sm()->getCameraRect().bottom)
	{
		kill();
	}
}

void CGriffon::DecideState()
{
	if (battleState_ == BattleState::DESTROY)
	{
		// 死亡中 復活は許されない
		return;
	}

	if (battleState_ == BattleState::DAMAGE)
	{
		return;
	}

	if (battleState_ == BattleState::ENTRY)
	{
		return;
	}

	const mymath::Vec3f& plPos = gm()->GetPlayerPos();
	// プレイヤーとの距離ベクトル e -> p
	mymath::Vec3f Vdist = plPos - obj_.pos;
	// プレイヤーとの直線距離
	const float plyDist = Vdist.Length2();

	// 攻撃範囲内か？
	if (plyDist < loadInfo_.ATTACK_RANGE
		|| battleState_ == BattleState::ATTACK)
	{
		// 攻撃中
		battleState_ = BattleState::ATTACK;
	}
	else if (plyDist < loadInfo_.SEARCH_RANGE)
	{
		// 攻撃範囲外 索敵範囲内
		if (plyDist < loadInfo_.CHASE_RANGE)
		{
			// 追跡可能範囲内
			battleState_ = BattleState::CHASE;
		}
		else
		{
			//battleState_ = BattleState::ATTACK;
			battleState_ = BattleState::CHASE;
		}
	}
	else
	{
		// 保険(各行動の最後にはWAITに戻してるはず)
		battleState_ = BattleState::WAIT;
	}
}

void CGriffon::CreateAttack()
{
	//std::dynamic_pointer_cast<CTackle>(attack_)->CreateAttack(backPos_, endPos_,
	//	loadInfo_.ATTACK_SPEED, loadInfo_.power);
}

bool CGriffon::isInvincible() const
{
	return invincibleTime_ > 0.f;
}

void CGriffon::hit(const ObjPtr& rival)
{
	// Polygon
	//__super::hit(rival);
}

bool CGriffon::ApplyDamage(int dam)
{
	// 死亡アニメーション中はスキップ
	if (battleState_ == BattleState::DESTROY) return true;

	// 無敵中
	if (isInvincible()) return false;

	health_ -= dam;

	// ひるみ処理
	// 攻撃中は揺れない
	if (!isAttack())
	{
		battleState_ = BattleState::DAMAGE;
		nextActTime_ = elapsedTime_ + loadInfo_.damageTime;
		motionType_ = MotionType::FALL;
		motionAnim_.set(motionTable_[static_cast<int>(motionType_)].size() - 1, 0.f);
		motionAnim_.stop();

	}

	invincibleTime_ = loadInfo_.INV_TIME;

	if (health_ <= 0)
	{
		battleState_ = BattleState::DESTROY;
		motionType_ = MotionType::FALL;
		motionAnim_.set(motionTable_[static_cast<int>(motionType_)].size() - 1, loadInfo_.fallTime);
		nextActTime_ = elapsedTime_ + loadInfo_.fallTime;
		return true;
	}

	return false;
}


bool CGriffon::isAttacking() const
{
	return attackFlag_;
}
bool CGriffon::isAttack() const
{
	return attackFlag_;
}
bool CGriffon::isBack() const
{
	return backFlag_;
}

Base::Collisions CGriffon::GetDamageAreas() const
{
	// 被ダメ後無敵中、死亡アニメーション中はスキップ
	if (isInvincible() || battleState_ == BattleState::DESTROY)
	{
		return Base::Collisions();
	}
	return __super::GetDamageAreas();
}

void CGriffon::SetInfo(const LoadInfo& info)
{

	loadInfo_ = info;
}
