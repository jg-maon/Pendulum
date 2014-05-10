#include "define.h"
#include "obj.h"

CGameManager* Base::gm_ = nullptr;


#pragma region Base methods

Base::Base(const std::string& name)	:
	name_(name)
	,status_(Status::run)
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


void Base::kill()
{
	status_ = Status::destroy;
}

void Base::hit(const std::shared_ptr<Base>& rival)
{
}

Base::Collisions Base::GetCollisionAreas()
{
	return Collisions();
}

#pragma endregion Base methods

//=============================================================
//=============================================================
//=============================================================

#pragma region IObject methods

IObject::IObject(const std::string& name):
	Base(name)
{
}

IObject::~IObject()
{
}

bool IObject::InScreen(int border) const
{
	RECT rt = camera::GetScreenRect();
	if (	obj_.pos.x < rt.left - border  || obj_.pos.y < rt.top - border
		||	obj_.pos.x > rt.right + border || obj_.pos.y > rt.bottom + border)
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

const charabase::CharBase& IObject::obj() const
{
	return obj_;
}

#pragma endregion // IObject methods

//=============================================================
//=============================================================
//=============================================================

#pragma region IColObject methods

IColObject::IColObject(const std::string& name):
	IObject(name)
	,collisions(collisions_)
{
}

IColObject::~IColObject()
{
}

void IColObject::ClearCollisions()
{
	collisions_.clear();
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
	using namespace mymath;
	std::string buf;
	f >> buf;
	if(buf != "{") return f.eof();
	while(!f.eof() && buf != "}")
	{
		if(buf == "Circle")
		{
			mymath::Circlef c;
			if(LoadValue(f, obj_, c.center.x))return true;
			if(LoadValue(f, obj_, c.center.y))return true;
			if(LoadValue(f, obj_, c.radius))return true;
			collisions_.push_back(mymath::ShapefPtr(new mymath::Circlef(c)));
		}
		else if(buf == "Polygon")
		{
			int vNum;	// ’¸“_”
			f >> vNum;
			//Polygonf p(vNum);
			for(int i=0; i<vNum; ++i)
			{
				//if(LoadValue(f, obj_, p.points[i].x))return true;
				//if(LoadValue(f, obj_, p.points[i].y))return true;
			}
			//collisions_.push_back(mymath::ShapefPtr(new Polygonf(p))));
		}
		else if(buf == "Rect")
		{
			mymath::Rectf r;
			if(LoadValue(f, obj_, r.left))return true;
			if(LoadValue(f, obj_, r.top))return true;
			if(LoadValue(f, obj_, r.right))return true;
			if(LoadValue(f, obj_, r.bottom))return true;
			collisions_.push_back(mymath::ShapefPtr(new mymath::Rectf(r)));
		}
		f >> buf;
	}
	return f.eof();
}

Base::Collisions IColObject::GetCollisionAreas() const
{
	using namespace mymath;
	Base::Collisions cols;
	for (const auto& col : collisions_)
	{
		const auto& id = *(col.get());
		if (typeid(id) == typeid(Circlef))
		{
			cols.push_back(ShapefPtr(new Circlef(*std::dynamic_pointer_cast<Circlef>(col))));
		}
		else if (typeid(id) == typeid(Rectf))
		{
			cols.push_back(ShapefPtr(new Rectf(*std::dynamic_pointer_cast<Rectf>(col))));
		}
		else if (typeid(id) == typeid(Polyf))
		{
			cols.push_back(ShapefPtr(new Polyf(*std::dynamic_pointer_cast<Polyf>(col))));
		}
	}
	for(auto& col : cols)
	{
		col->Offset(obj_.pos);
	}
	return cols;
}

void IColObject::SetCollisionAreas(const Base::Collisions& collisions)
{
	collisions_ = collisions;
}

#pragma endregion // IColObject methods



//=============================================================
//=============================================================
//=============================================================