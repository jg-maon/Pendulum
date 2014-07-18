
#include "characterBase.h"

std::weak_ptr<CStageMng> ICharacter::sm_;


ICharacter::ICharacter(const std::string& name) :
	IColObject(name)
	, health_(1)
	, power_(0)
	, prePos_(obj_.pos)
{}

ICharacter::~ICharacter()
{}

void ICharacter::step()
{
	prePos_ = obj_.pos; 
}

void ICharacter::draw()
{}


std::shared_ptr<CStageMng> ICharacter::sm()
{
	return sm_.lock();
}

void ICharacter::sm(const std::shared_ptr<CStageMng>& sm)
{
	sm_ = sm;
}





void ICharacter::hit(const ObjPtr& rival)
{
	__super::hit(rival);
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
}

Base::Collisions ICharacter::GetDamageAreas() const
{
	return __super::GetDamageAreas(); 
}

int ICharacter::getPower() const
{
	return power_; 
}

#ifdef DEF_PREPOS

const mymath::Vec3f& ICharacter::prePos() const
{
	return prePos_;
}

#endif

mymath::Vec3f ICharacter::nextPos() const
{
	return obj_.pos + (obj_.add * system::FrameTime);
}

