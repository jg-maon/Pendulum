#ifdef _DEBUG

#endif

#include "define.h"
#include "roboticArm.h"
#include "armAttack.h"
#include "setting.h"

//#include "player.h"
#include "common.h"

#include "actionPoint.h"

//-------------------------------------------------------------------

void (CRoboticArm::*CRoboticArm::StateStep_[])() =
{
	&CRoboticArm::WaitStep,
	&CRoboticArm::AttackStep,
	&CRoboticArm::DestroyStep,
};

CRoboticArm::CRoboticArm() :
IEnemy("E_RoboticArm")
{
	std::vector<int> move = { 0, 0 };
	motionTable_.push_back(move);
	//std::vector<int> attack = { 1, 0, 1, 3, 4 };
	std::vector<int> attack = { 0 };
	motionTable_.push_back(attack);
	std::vector<int> destroy = { 0 };
	motionTable_.push_back(destroy);
}

CRoboticArm::CRoboticArm(const mymath::Vec3f& pos, int dir) :
IEnemy("E_RoboticArm")
{
	init(pos, dir);
}
CRoboticArm::CRoboticArm(float x, float y, int dir, float z) :
IEnemy("E_RoboticArm")
{
	init(mymath::Vec3f(x, y, z), dir);
}

CRoboticArm::~CRoboticArm()
{

}


void CRoboticArm::init(const mymath::Vec3f& pos, int dir)
{
	using common::FindChunk;

	gm()->GetData(*this);

	charabase::CharBase childObj;
	childObj.resname = loadInfo_.childResname;
	childObj.src = loadInfo_.childSrcPos;
	childObj.size = loadInfo_.childSrcSize;

	childAngle_ = 0.f;
	rotateDir_ = 1.f;

	mymath::Vec3f center;  // 支点の格納(回転の中心座標)
	mymath::Vec3f length;  // 支点から中心座標までの長さ
	
	// 向きの修正
	SetArmDirection(dir);
	SwichArmDir();
	// 固定アームの修正
	obj_.pos = pos;
	length = loadInfo_.supportParent - obj_.HalfSize();
	center = obj_.pos + length;
	float len = length.Length2();
	// 可動アーム
	length = childObj.HalfSize() - loadInfo_.supportChild;

	if (attack_ != nullptr)
	{
		attack_->obj(childObj);
		dynamic_pointer_cast<CArmAttack>(attack_)->SetArmInfo(childAngle_, len, center);
	}
	// 修正ここまで

	elapsedTime_ = 0.f;
	nextActTime_ = 0.f;
	state_ = State::WAIT;
	motionType_ = MotionType::MOVE;
	motionAnim_.set(motionTable_[static_cast<int>(motionType_)].size() - 1, 0.3f);
	obj_.src.x = motionTable_[static_cast<int>(motionType_)][motionAnim_.no];

}

void CRoboticArm::SwichArmDir()
{
	float angle = 0.f;
	mymath::Vec3f support = loadInfo_.supportParent;
	switch (armDir_)
	{
	case ArmDirection::RIGHT_UP:
		break;
	case ArmDirection::RIGHT_DOWN:
		support.y = obj_.size.y - loadInfo_.supportParent.y;
		break;
	case ArmDirection::LEFT_UP:
		angle += 180.f;
		support.x = obj_.size.x - loadInfo_.supportParent.x;
		turnParentFlag_ = true;
		break;
	case ArmDirection::LEFT_DOWN:
		angle += 180.f;
		support.x = obj_.size.x - loadInfo_.supportParent.x;
		support.y = obj_.size.y - loadInfo_.supportParent.y;
		turnParentFlag_ = true;
		break;
	default:
		break;
	}
	loadInfo_.supportParent = support;
	loadInfo_.maxAngle += angle;
	loadInfo_.minAngle += angle;
	childAngle_ = angle;
}

void CRoboticArm::step()
{
	ICharacter::step();

	DecideState();

	// ヒットストップ時はスキップ
	if (!isUpdatable())
		return;

	(this->*StateStep_[static_cast<int>(state_)])();

	if (attack_ != nullptr)
	{
		// 可動アームの回転
		childAngle_ += loadInfo_.rotateSpeed * rotateDir_;

		if (childAngle_ > loadInfo_.maxAngle)
		{
			childAngle_ = loadInfo_.maxAngle;
			elapsedTime_ += system::FrameTime;
		}
		else if (childAngle_ < loadInfo_.minAngle)
		{
			childAngle_ = loadInfo_.minAngle;
			elapsedTime_ += system::FrameTime;
		}

		dynamic_pointer_cast<CArmAttack>(attack_)->Rotate(childAngle_);


		// 可動アームの修正
		mymath::Vec3f center;  // 支点の格納(回転の中心座標)
		mymath::Vec3f length;  // 支点から中心座標までの長さ
		// 固定アーム
		length = loadInfo_.supportParent - obj_.HalfSize();
		center = obj_.pos + length;
		// 可動アーム
		//length = loadInfo_.supportChild - obj_.HalfSize();
		length = obj_.HalfSize() - loadInfo_.supportChild;
		//obj_.pos.x = math::ROUND_X(childAngle_, length.x, center.x);
		//obj_.pos.y = math::ROUND_Y(childAngle_, length.x, center.y);
		//画像の角度
		//obj_.angle = childAngle_;
		// 修正ここまで
	}
}

void CRoboticArm::draw()
{

	mymath::Rectf rect = camera::GetScreenRect();
	if (rect.Contains(obj_.GetRect()))
	{
		// 固定アームの上下反転
		if (armDir_ == ArmDirection::LEFT_DOWN || armDir_ == ArmDirection::RIGHT_DOWN)
		{
			obj_.scale.y = -obj_.scale.y;
		}
		//固定アーム
		obj_.draw(charabase::CharBase::MODE::Center, turnParentFlag_);

		// 固定アームの上下反転の修正
		if (armDir_ == ArmDirection::LEFT_DOWN || armDir_ == ArmDirection::RIGHT_DOWN)
		{
			obj_.scale.y = -obj_.scale.y;
		}

	}
	if (attack_)
		if (rect.Contains(attack_->obj().GetRect()))
			attack_->draw();
}


void CRoboticArm::WaitStep()
{
}

void CRoboticArm::AttackStep()
{
	// 攻撃
	if (elapsedTime_ > nextActTime_)
	{
		state_ = State::WAIT;
		elapsedTime_ = 0;
		rotateDir_ = -rotateDir_;
		CreateAttack();
	}
}

void CRoboticArm::DestroyStep()
{
	// 0.5秒
	obj_.alpha -= 255.f / 0.5f * system::FrameTime;
	if (obj_.alpha < 0.f)
	{
		obj_.alpha = 0.f;
		state_ = State::WAIT;
		kill();
	}
}

void CRoboticArm::DecideState()
{
	if (state_ == State::DESTROY)
	{
		// 死亡中 復活は許されない
		return;
	}
	const mymath::Vec3f& plPos = gm()->GetPlayerPos();
	// プレイヤーとの距離ベクトル e -> p
	mymath::Vec3f Vdist = plPos - obj_.pos;
	const float plyDist = mymath::PYTHA(Vdist.x, Vdist.y);
	if (plyDist < mymath::POW2(loadInfo_.attackRange) || state_ == State::ATTACK)
	{
		// 攻撃範囲内 or 攻撃中
		if (state_ != State::ATTACK)
			nextActTime_ = elapsedTime_ + loadInfo_.attackInterval;
		state_ = State::ATTACK;
		motionType_ = MotionType::ATTACK;
	}
	else
	{
		// 保険(各行動の最後にはWAITに戻してるはず)
		state_ = State::WAIT;
		motionType_ = MotionType::MOVE;
	}
}

void CRoboticArm::CreateAttack()
{
	mymath::Vec3f center;
	mymath::Vec3f length;
	length = loadInfo_.supportParent - obj_.HalfSize();
	center = obj_.pos + length;
	length = obj_.HalfSize() - loadInfo_.supportChild;

	/*
	CArmAttack armAtk(attack_->obj());// アーム攻撃の情報
	armAtk.obj(obj_);
	armAtk.SetCollisionAreas(collisions_);
	// 回転できる最大値
	float max = 0;
	// 回転速度
	float speed = loadInfo_.rotateSpeed * rotateDir_;
	if (rotateDir_ > 0)
	{
		max = loadInfo_.maxAngle;
	}
	else if (rotateDir_ < 0)
	{
		max = loadInfo_.minAngle;
	}
	//*/
}


void CRoboticArm::hit(const ObjPtr& rival)
{
	// Polygon
	__super::hit(rival);
}

bool CRoboticArm::ApplyDamage(int dam)
{
	// 死亡アニメーション中はスキップ
	if (state_ == State::DESTROY) return true;
	// ダメージが入ったら即死
	state_ = State::DESTROY;

	// 爆散エフェクト
	for (int i = 0; i < 3; ++i)
		gm()->AddObject(ObjPtr(new CEffectExplosion(obj_.pos)));
	// SE
	se::DSound_Play("se_explosion");

	// ピックアップアイテム
	gm()->AddObject(ObjPtr(new CPickupJewely(obj_.pos)));

	return true;
}

Base::Collisions CRoboticArm::GetDamageAreas() const
{
	// 死亡アニメーション中はスキップ
	if (state_ != State::DESTROY)
	{
		return __super::GetDamageAreas();
	}
	return Base::Collisions();
}

void CRoboticArm::SetInfo(const CRoboticArm::LoadInfo& info)
{
	loadInfo_ = info;
}

void CRoboticArm::SetArmDirection(CRoboticArm::ArmDirection dir)
{
	armDir_ = dir;
}

void CRoboticArm::SetArmDirection(const int dir)
{
	armDir_ = static_cast<ArmDirection>(dir);
}
