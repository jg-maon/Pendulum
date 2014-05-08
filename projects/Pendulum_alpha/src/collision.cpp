#ifdef _DEBUG
//#define D_CIRCLE_TEST
//#define D_POLY_TEST
#endif

#if defined(D_POLY_TEST) | defined(D_CIRCLE_TEST)
	std::vector<mymath::Vec3f> vertexes;
	#ifdef D_POLY_TEST
	mymath::Linef line(0.f,0.f,0.5f, 500.f,1000.f,0.5f);
	#endif
#endif



#include "collision.h"
#include "define.h"

#include "stageMng.h"
#include "player.h"
#include "enemyMng.h"
#include "actionPoint.h"


#include "MyMath.hpp"
#include "setting.h"

#include "nWayShot.h"

#include "effectExplosion.h"

#include <memory>


CCollision::CCollision() :
	IObject("Collision")
{
#ifdef D_CIRCLE_TEST
	std::tuple<int,int,float> acs[] =
	{
		std::tuple<int,int,float>(100,100,50.f),
		std::tuple<int,int,float>(200,200,50.f),
		std::tuple<int,int,float>(300,300,10.f),
		std::tuple<int,int,float>(400,500,100.f),
		std::tuple<int,int,float>(200,100,50.f),
		std::tuple<int,int,float>(100,200,20.f),
		std::tuple<int,int,float>(1000,1000,150.f),
	};
	for(auto& ac : acs)
		InsertObject(ObjPtr(new CActionCircle(
							static_cast<float>(std::get<0>(ac)),
							static_cast<float>(std::get<1>(ac)),
							std::get<2>(ac))));
									
#endif
}
void CCollision::step()
{
	auto& player = CPlayer::GetPtr();
	auto& enemies = (std::static_pointer_cast<CEnemyMng>(
						CEnemyMng::GetPtr()))->GetEnemies();
	auto& atk_shots = gm()->GetObjects("Atk_Shot");
	const auto& sm = *std::dynamic_pointer_cast<CStageMng>(gm()->GetObj(typeid(CStageMng)));
	//-----------------------------------------------------
	// ÉvÉåÉCÉÑÅ[
	if(player != nullptr)
	{
		auto& pl = std::dynamic_pointer_cast<CPlayer>(player);
		const mymath::Vec3f& plpos = pl->obj().pos;
		//-----------------------------------------
		// ÉvÉåÉCÉÑÅ[ vs ActionPoint
		for(auto& actp : sm.actionPoints)
		{
			if(actp->Contains(mymath::Vec3f(),pl->prePos,plpos))
			{
				// åç∑
				pl->hit(actp);
			}
			else if(actp->Contains(mymath::Vec3f(),plpos))
			{
				// ì‡ïÔ
				pl->hit(actp);
			}
		}
		
		
		//-----------------------------------------
		// çUåÇ
		if(pl->isAttacking)
		{
			POINT m = camera::GetCursorPosition();
			mymath::Vec3f mouse(static_cast<float>(m.x),
								static_cast<float>(m.y));
			//-----------------------------------------
			// çUåÇ vs ìG
			for(auto& enemy : enemies)
			{
				const mymath::Vec3f& enpos = enemy->obj().pos;
				mouse.z = enpos.z;
				auto& enemyCollisions = enemy->GetCollisionAreas();
				// ìñÇΩÇËîªíË
				for(auto& col : enemyCollisions)
				{
					// ìñÇΩÇËîªíËì‡ÇÃÇ›
					if(!(col->Contains(mouse))) continue;
					mymath::Linef line(plpos, col->IntersectionPoint2Nearest(plpos,mouse));
					// ìGÇ∆ÇÃíºê¸ãóó£Ç…ActionPolygonÇ™Ç»Ç¢èÍçáÇÃÇ›çUåÇóLå¯
					bool atkFlag = true;
					for(auto& actp : sm.actionPoints)
					{
						// ActioinPolygonà»äOîrèú
						if(!actp->FindName("ActionPolygon"))continue;
						// ActionPolygonÇ∆åç∑
						if(actp->Contains(mymath::Vec3f(),line))
						{
							atkFlag = false;
							break;
						}
					}
					//-----------------------------------------
					// çUåÇèàóù
					if(atkFlag)
					{
						// É_ÉÅÅ[ÉW
						pl->ApplyAttack(enemy->obj().pos);
						if(enemy->ApplyDamage(pl->power))
						{
							//-----------------------------------------
							// ìGkill
							pl->KilledEnemy();
						
							// îöéUÉGÉtÉFÉNÉg
							for(int i=0; i<3; ++i)
								InsertObject(ObjPtr(new CEffectExplosion(enemy->obj().pos)));
							// SE
							//DSound_SetFrequency(SE::EXPLODE, 1000);
							se::DSound_Play("img_explosion");
						}
					}
				}
			}
		}

		
		auto& plCols = player->GetCollisionAreas();
		for(auto& plcol : plCols)
		{
			auto& pc = *std::dynamic_pointer_cast<mymath::Rectf>(plcol);
			
			//-----------------------------------------
			// ÉvÉåÉCÉÑÅ[ vs ìGçUåÇ
			for(auto& enemy : enemies)
			{
				const mymath::Vec3f& enpos = enemy->obj().pos;
				//-----------------------------------------
				// ìGçUåÇ 
				auto& enemyAtks = enemy->GetAttackCollisions();
				int i=0;
				for(auto& col : enemyAtks)
				{
					// ìGçUåÇ vs ÉvÉåÉCÉÑÅ[
					if(col->Contains(pc))
					{
						// îÒÉ_ÉÅ
						pl->ApplyDamage(enemy->attack->GetForce());
						break;
						// çUåÇéÌóﬁï èàóù
						//if(enemy->attack->findName("Atk_NWayShot"))
						//{
						//	
						//}
					}
				}
			}
			//-----------------------------------------------------
			// ÉvÉåÉCÉÑÅ[ vs âìãóó£çUåÇ
			for(auto& shot : atk_shots)
			{
				auto& atkCols = shot->GetCollisionAreas();
				for(auto& atkcol : atkCols)
				{
					if(atkcol->Contains(pc))
					{
						auto& s = std::dynamic_pointer_cast<CShot>(shot);
						pl->ApplyDamage(s->GetForce());
						s->kill();
						break;
					}
				}
			}
		}
	}
	
	//-----------------------------------------------------
	// ìG
	for(auto& enemy : enemies)
	{
		auto& en = std::dynamic_pointer_cast<IEnemy>(enemy);
		const mymath::Vec3f& enpos = en->obj().pos;
		//-----------------------------------------
		// ÉvÉåÉCÉÑÅ[ vs ActionPoint
		for(auto& actp : sm.actionPoints)
		{
			if(actp->Contains(mymath::Vec3f(),en->prePos,enpos))
			{
				// åç∑
				en->hit(actp);
			}
			else if(actp->Contains(mymath::Vec3f(),enpos))
			{
				// ì‡ïÔ
				en->hit(actp);
			}
		}
	}
	
	//-----------------------------------------------------
	// âìãóó£çUåÇ
	for(auto& shot : atk_shots)
	{
		auto& atkCols = shot->GetCollisionAreas();
		for(auto& atkcol : atkCols)
		{
			//-----------------------------------------
			// ìGçUåÇ vs Polygon
			for(auto& act : sm.actionPoints)
			{
				// ActionoPolygonÇÃÇ›
				if(!act->FindName("ActionPolygon")) continue;
				// ì‡ïÔîªíË	
				if(act->Contains(atkcol))
				{
					// çUåÇéÌóﬁï èàóù
					shot->kill();
				}
				// í âﬂîªíË
				//else if(
			}
		}
	}

#if defined(D_POLY_TEST) | defined(D_CIRCLE_TEST)
	if(CheckPush(KEY_MOUSE_LBTN))
	{
		POINT mouse = GetCursorPosition();
		mymath::Vec3f p((float)mouse.x,(float)mouse.y,0.f);
		vertexes.push_back(p);
	}
	if(CheckPush(KEY_MOUSE_RBTN))
	{
		InsertObject(ObjPtr(new CActionPolygon(vertexes)));
		vertexes.clear();
	}
	if(CheckPush(KEY_SPACE))
	{
		vertexes.clear();
		for(auto& obj : objs)
		{
			if(obj->findName("ActionPolygon"))
			{
				obj->kill();
			}
		}
	}
#endif
}

void CCollision::draw()
{
#ifdef D_POLY_TEST
	Draw_Line(	(int)line.sta.x,
				(int)line.sta.y,
				(int)line.end.x,
				(int)line.end.y,
				line.sta.z,
				-1,
				1);
	auto& actPolys = GetObjects("ActionPolygon");
	if(!actPolys.empty())
	{
		for(auto& act : actPolys)
		{
			CActionPolygon* ap = (CActionPolygon*)act.get();
			std::vector<mymath::Vec3f> points;
			if(ap->Contains(points, line))
			{
				for(auto& point : points)
				{
					Draw_BoxCenter(int(point.x), int(point.y),
									5,5,0.f,
									ARGB(255,0,0,0),
									ARGB(255,0,0,0),
									0,true);
				}
			}
		}
	}
#endif
#if defined(D_POLY_TEST) | defined(D_CIRCLE_TEST)
	if(!vertexes.empty())
	{
		// ìoò^íÜÇÃê¸ï™
		if(vertexes.size() >= 2)
		{
			//Draw_
			for(size_t sta = 0; sta < vertexes.size()-1; ++sta)
			{
				size_t end = (sta+1) % vertexes.size();
				Draw_Line(	int(vertexes[sta].x),
							int(vertexes[sta].y),
							int(vertexes[end].x),
							int(vertexes[end].y),
							vertexes[sta].z,
							ARGB(255,255,255,0),
							1);
			}
		}
		// èIì_Ç∆É}ÉEÉXÇåãÇ‘
		mymath::Vec3f sta = vertexes.back();
		POINT mouse = GetCursorPosition();
		Draw_Line(	int(sta.x),
					int(sta.y),
					mouse.x,
					mouse.y,
					sta.z,
					ARGB(255,0,255,255),
					1);
	}
#endif
#ifdef D_CIRCLE_TEST
	
	auto& actCircles = GetObjects("ActionCircle");
	if(!actCircles.empty())
	{
		POINT mouse = GetCursorPosition();
		mymath::Vec3f p((float)mouse.x,(float)mouse.y);
		for(auto& act : actCircles)
		{
			CActionCircle* ap = (CActionCircle*)act.get();
			mymath::Vec3f point;
			if(ap->Contains(point, p))
			{
				Draw_BoxCenter(int(point.x), int(point.y),
								5,5,0.f,
								-1,//ARGB(255,0,0,0),
								-1,//ARGB(255,0,0,0),
								0,true);
			}
		}
		// É}ÉEÉXÇ≈ê∂ê¨ÇµÇΩê¸ï™Ç∆ÇÃìñÇΩÇËîªíË
		auto& mousePolys = GetObjects("ActionPolygon");
		if(!mousePolys.empty())
		{
			for(auto& mousePoly : mousePolys)
			{
				std::vector<mymath::Vec3f> vers = ((CActionPolygon*)mousePoly.get())->vertexes;
				if(vers.size() >= 2)
				for(size_t sta = 0; sta < vers.size(); ++sta)
				{
					size_t end = (sta+1) % vers.size();
					for(auto& act : actCircles)
					{
						CActionCircle* ap = (CActionCircle*)act.get();
						std::vector<mymath::Vec3f> points;
						//if(ap->Contains(points, vers[sta], vers[end]))
						mymath::Vec3f point;
						if(ap->Contains(point, vers[sta], vers[end]))
						{
							//for(auto& point : points)
							{
								Draw_BoxCenter(int(point.x), int(point.y),
												5,5,0.f,
												-1,//ARGB(255,0,0,0),
												-1,//ARGB(255,0,0,0),
												0,true);
							}
						}
					}
				}
			}
		}
	}
#endif
}
