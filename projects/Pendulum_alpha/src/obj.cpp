#include "define.h"
#include "obj.h"

CGameManager* Base::gm_ = nullptr;


#pragma region Base methods

Base::Base(const std::string& name) :
name_(name)
, status_(Status::idle)
{
}
Base::~Base()
{
}
CGameManager* Base::gm() const
{
	return gm_;
}

void Base::gm(CGameManager* gm)
{
	gm_ = gm;
}

bool Base::isDestroy() const
{
	return status_ == Status::destroy;
}

bool Base::FindName(const std::string& name) const
{
	std::string::size_type pos = name_.find(name);
	return (pos != std::string::npos);
}

const std::string& Base::getName() const
{
	return name_;
}

void Base::kill()
{
	status_ = Status::destroy;
}
void Base::start()
{
	status_ = Status::run;
}
void Base::stop()
{
	status_ = Status::idle;
}
void Base::SetStatusDisp()
{
	status_ = Status::disp;
}
/*
void Base::setStatus(Base::Status status)
{
	status_ = status;
}
//*/

Base::Status Base::getStatus() const
{
	return status_;
}


void Base::hit(const std::shared_ptr<Base>& rival)
{
}

Base::Collisions Base::GetDamageAreas() const
{
	return Collisions();
}
Base::Collisions Base::GetStageCollisions() const
{
	return Collisions();
}


#pragma endregion Base methods

//=============================================================
//=============================================================
//=============================================================

#pragma region IObject methods

IObject::IObject(const std::string& name) :
Base(name)
, turnFlag_(false)
{
}

IObject::~IObject()
{
}

bool IObject::InScreen(int border) const
{
	RECT rt = camera::GetScreenRect();
	if (obj_.pos.x < rt.left - border || obj_.pos.y < rt.top - border
		|| obj_.pos.x > rt.right + border || obj_.pos.y > rt.bottom + border)
	{
		return false;
	}
	return true;
}


void IObject::kill()
{
	status_ = Status::destroy;
	obj_.show = false;
}

void IObject::hit(const ObjPtr& rival)
{
}

Base::Collisions IObject::GetDamageAreas() const
{
	return Collisions();
}
Base::Collisions IObject::GetStageCollisions() const
{
	return Collisions();
}

const charabase::CharBase& IObject::obj() const
{
	return obj_;
}

void IObject::obj(const charabase::CharBase& o)
{
	obj_ = o;
}

#pragma endregion // IObject methods

//=============================================================
//=============================================================
//=============================================================

#pragma region IColObject methods

IColObject::IColObject(const std::string& name) :
IObject(name)
{
}

IColObject::~IColObject()
{
}

void IColObject::ClearCollisions()
{
	collisions_.clear();
}

//bool IColObject::LoadCollisions(std::ifstream& f, Base::MotionCollisions& collisions)
bool IColObject::LoadCollisions(std::ifstream& f, Base::Collisions& collisions)
{
	std::string buf;
	f >> buf;
	if (buf != "{") return f.eof();
	int openNum = 0;	// {階層数
	Collisions temp;
	collisions.clear();
	while (!f.eof())
	{
		if (buf == "}")
		{
			if (!temp.empty())
			{
				// 当たり判定の追加
				collisions = temp;
				//collisions.push_back(temp);
				temp.clear();
			}
			// 一つ階層を上げる
			if (--openNum == 0)
			{
				// 全部閉じたので読み込みを終了させる
				break;
			}
		}
		else if (buf == "{")
		{
			openNum++;
		}

		else if (buf == "#Circle")
		{
			mymath::Circlef c;
			if (LoadValue(f, obj_, c.center.x)) return true;
			if (LoadValue(f, obj_, c.center.y)) return true;
			if (LoadValue(f, obj_, c.radius)) return true;
			temp.push_back(mymath::ShapefPtr(new mymath::Circlef(c)));
		}
		else if (buf == "#Polygon")
		{
			int vNum;	// 頂点数
			f >> vNum;
			mymath::Polyf p(vNum);
			for (int i = 0; i < vNum; ++i)
			{
				if (LoadValue(f, obj_, p.points[i].x)) return true;
				if (LoadValue(f, obj_, p.points[i].y)) return true;
			}
			temp.push_back(mymath::ShapefPtr(new mymath::Polyf(p)));
			//return f.eof();
		}
		else if (buf == "#Rect")
		{
			mymath::Rectf r;
			if (LoadValue(f, obj_, r.left))return true;
			if (LoadValue(f, obj_, r.top))return true;
			if (LoadValue(f, obj_, r.right))return true;
			if (LoadValue(f, obj_, r.bottom))return true;
			temp.push_back(mymath::ShapefPtr(new mymath::Rectf(r)));
		}
		f >> buf;
	}
	return f.eof();
}

Base::Collisions IColObject::GetWorldCollisions(const Base::Collisions& collisions) const
{
	using namespace mymath;
	Base::Collisions cols;
	for (const auto& col : collisions)
	{
		const auto& id = typeid(*(col.get()));
		if (id == typeid(Circlef))
		{
			cols.push_back(ShapefPtr(new Circlef(*std::dynamic_pointer_cast<Circlef>(col))));
		}
		else if (id == typeid(Rectf))
		{
			cols.push_back(ShapefPtr(new Rectf(*std::dynamic_pointer_cast<Rectf>(col))));
		}
		else if (id == typeid(Polyf))
		{
			cols.push_back(ShapefPtr(new Polyf(*std::dynamic_pointer_cast<Polyf>(col))));
		}
	}
	for (auto& col : cols)
	{
		// 画像に合わせて反転
		if (turnFlag_)
			col->ReverseX();
		// 画像の角度に合わせて回転
		col->Rotate(math::Calc_DegreeToRad(obj_.angle));
		// 画像のワールド座標系に移動
		col->Offset(obj_.pos);
	}
	return cols;
}


//void IColObject::kill()
//{
//	IObject::kill();
//}

void IColObject::hit(const ObjPtr& rival)
{
}

bool IColObject::LoadCollisions(std::ifstream& f)
{
	return this->LoadCollisions(f, collisions_);
}

bool IColObject::LoadStageCollisions(std::ifstream& f)
{
	return this->LoadCollisions(f, stageCollisions_);
}

Base::Collisions IColObject::GetDamageAreas() const
{
	if (status_ == Base::Status::run || status_ == Base::Status::update)
		return this->GetWorldCollisions(collisions_);
	else
		return Base::Collisions();
}

Base::Collisions IColObject::GetStageCollisions() const
{
	if (status_ == Base::Status::run || status_ == Base::Status::update)
		return this->GetWorldCollisions(stageCollisions_);
	else
		return Base::Collisions();
}

void IColObject::SetCollisionAreas(const Base::Collisions& collisions)
{
	collisions_ = collisions;
}

void IColObject::SetCollisionAreas(const IColObject& obj)
{
	collisions_ = obj.collisions_;
}

void IColObject::SetStageCollisions(const Base::Collisions& collisions)
{
	stageCollisions_ = collisions;
}

void IColObject::SetStageCollisions(const IColObject& obj)
{
	stageCollisions_ = obj.stageCollisions_;
}
#pragma endregion // IColObject methods



//=============================================================
//=============================================================
//=============================================================