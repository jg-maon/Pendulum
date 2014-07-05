#ifdef _DEBUG
//#define D_CIRCLE_TEST
//#define D_POLY_TEST
#define D_COLLISION_DRAW	// 当たり判定描画
#define D_STAGECOL_DRAW		// ステージ当たり判定描画
#endif

#if defined(D_COLLISION_DRAW) || defined(D_STAGECOL_DRAW)
#define DEF_SHAPE_DRAW
#endif


#if defined(D_POLY_TEST) || defined(D_CIRCLE_TEST)
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

#include "effectSlash.h"

#include <memory>


CCollision::CCollision() :
IObject("Collision")
{
	status_ = Status::idle;
#ifdef D_CIRCLE_TEST
	std::tuple<int, int, float> acs[] =
	{
		std::tuple<int, int, float>(100, 100, 50.f),
		std::tuple<int, int, float>(200, 200, 50.f),
		std::tuple<int, int, float>(300, 300, 10.f),
		std::tuple<int, int, float>(400, 500, 100.f),
		std::tuple<int, int, float>(200, 100, 50.f),
		std::tuple<int, int, float>(100, 200, 20.f),
		std::tuple<int, int, float>(1000, 1000, 150.f),
	};
	for (auto& ac : acs)
		gm()->AddObject(ObjPtr(new CActionCircle(
		static_cast<float>(std::get<0>(ac)),
		static_cast<float>(std::get<1>(ac)),
		std::get<2>(ac))));

#endif
}

void CCollision::step()
{
	auto& player = CPlayer::GetPtr();

	const auto& em = CEnemyMng::GetPtr();
	auto& enemies = std::dynamic_pointer_cast<CEnemyMng>(em)->getEnemies();

	auto& atk_shots = gm()->GetObjects("Atk_Shot,Atk_Sing", ',');
	const auto& sm = std::dynamic_pointer_cast<CStageMng>(gm()->GetObj(typeid(CStageMng)));

	const auto& actionPoints = sm->getActionPoints();

	const auto& pickups = gm()->GetObjects("Pickup");

	//-----------------------------------------------------
	// プレイヤー
	if (player != nullptr)
	{
		auto& pl = std::dynamic_pointer_cast<CPlayer>(player);
		const mymath::Vec3f& plpos = pl->obj().pos;
		auto& plstageCollisions = pl->GetStageCollisions();

		//-----------------------------------------
		// ステージ当たり判定
		for (auto& stacol : plstageCollisions)
		{
			//-----------------------------------------------------
			// プレイヤー vs pickup
			for (auto& pickup : pickups)
			{
				auto& cols = pickup->GetDamageAreas();
				for (auto& col : cols)
				{
					if (stacol->Contains(col))
					{
						pl->hit(pickup);
						pickup->hit(pl);
					}
				}
			}
		}
		{
			//-----------------------------------------
			// プレイヤー vs ActionPoint
			for (auto& actp : actionPoints)
			{
#ifndef DEF_PREPOS
				const mymath::Vec3f& nextPos = pl->nextPos();
				if (actp->Contains(plpos, nextPos))
#else
				if (actp->Contains(pl->prePos(), plpos))
#endif
				{
					// 交差
					pl->hit(actp);
				}
				//else if (actp->Contains(nextPos))
				{
					// 内包
					//pl->hit(actp);
				}
			}
		}

		//-----------------------------------------
		// 攻撃
		if (pl->isAttacking())
		{
			POINT m = camera::GetCursorPosition();
			mymath::Vec3f mouse(static_cast<float>(m.x),
				static_cast<float>(m.y));
			//-----------------------------------------
			// 攻撃 vs 敵
			if (em != nullptr)
			{
				for (auto& enemy : enemies)
				{
					const mymath::Vec3f& enpos = enemy->obj().pos;
					mouse.z = enpos.z;
					const auto& enemyCollisions = enemy->GetDamageAreas();
					// 当たり判定
					for (const auto& col : enemyCollisions)
					{
						// 当たり判定内のみ
						if (!(col->Contains(mouse) && col->Contains(plpos, mouse))) continue;
						mymath::Linef line(plpos, col->IntersectionPoint2Nearest(plpos, mouse));
						// 敵との直線距離にActionPolygonがない場合のみ攻撃有効
						bool atkFlag = true;
						for (const auto& actp : actionPoints)
						{
							// ActioinPolygon以外排除
							if (!actp->FindName("ActionPolygon"))continue;
							// ActionPolygonと交差
							if (actp->Contains(line))
							{
								atkFlag = false;
								break;
							}
						}
						//-----------------------------------------
						// 攻撃処理
						if (atkFlag)
						{
							auto& epos = enemy->obj().pos;
							// ダメージ
							pl->ApplyAttack(epos);
							gm()->AddObject(ObjPtr(new CEffectSlash(epos)));
							if (enemy->ApplyDamage(pl->getPower()))
							{
								//-----------------------------------------
								// 敵kill
								pl->KilledEnemy();
							}
							break;
						}
					}
				}
			}
		}


		//-----------------------------------------
		// 喰らい判定
		auto& plCols = player->GetDamageAreas();
		for (auto& pc : plCols)
		{
			//auto& pc = *std::dynamic_pointer_cast<mymath::Rectf>(plcol);

			//-----------------------------------------
			// プレイヤー vs 敵攻撃
			if (em != nullptr)
			{
				for (auto& enemy : enemies)
				{
					const mymath::Vec3f& enpos = enemy->obj().pos;
					//-----------------------------------------
					// 敵攻撃 
					auto& enemyAtks = enemy->GetAttackAreas();
					int i = 0;
					for (auto& col : enemyAtks)
					{
						// 敵攻撃 vs プレイヤー
						if (col->Contains(pc))
						{
							// 非ダメ
							pl->hit(enemy);
							break;
							// 攻撃種類別処理
							//if (enemy->attack->findName("Atk_NWayShot"))
							//{
							//	
							//}
						}
					}
				}
			}
			//-----------------------------------------------------
			// プレイヤー vs 遠距離攻撃
			for (auto& shot : atk_shots)
			{
				auto& atkCols = shot->GetDamageAreas();
				for (auto& atkcol : atkCols)
				{
					if (pc->Contains(atkcol))
					{
						//auto& s = std::dynamic_pointer_cast<CShot>(shot);
						//s->hit(pl);
						pl->hit(shot);
						shot->hit(pl);
						break;
					}
				}
			}
			
			/*
			//-----------------------------------------------------
			// プレイヤー vs pickup
			for (auto& pickup : pickups)
			{
				auto& cols = pickup->GetDamageAreas();
				for (auto& col : cols)
				{
					if (pc->Contains(col))
					{
						pl->hit(pickup);
						pickup->hit(pl);
					}
				}
			}
			//*/
		}
	}

	//-----------------------------------------------------
	// 敵
	if (em != nullptr)
	{
		for (auto& enemy : enemies)
		{
			auto& en = std::dynamic_pointer_cast<IEnemy>(enemy);
			const mymath::Vec3f& enpos = en->obj().pos;
			//-----------------------------------------
			// 敵 vs ActionPoint
			//auto& enstageCollisions = en->GetStageCollisions();
			//for (auto& stacol : enstageCollisions)
			{
				for (auto& actp : actionPoints)
				{
#ifndef DEF_PREPOS
					const mymath::Vec3f& nextPos = en->nextPos();
					if (actp->Contains(enpos, nextPos))
#else
					if (actp->Contains(en->prePos(), enpos))
#endif
					{
						// 交差
						en->hit(actp);
						//actp->hit(en);
					}
					//else if (actp->Contains(nextPos))
					{
						// 内包
						//	en->hit(actp);
					}
				}
			}
		}
	}

	//-----------------------------------------------------
	// 遠距離攻撃
	for (auto& shot : atk_shots)
	{
		auto& atkCols = shot->GetDamageAreas();
		for (auto& atkcol : atkCols)
		{
			//-----------------------------------------
			// 敵攻撃 vs Polygon
			for (auto& actp : actionPoints)
			{
				// ActionoPolygonのみ
				if (!actp->FindName("ActionPolygon")) continue;
				// 内包判定	
				if (actp->Contains(atkcol))
				{
					// 攻撃種類別処理
					//shot->kill();
					shot->hit(actp);
				}
				// 通過判定
				//else if (
			}
		}
	}

#if defined(D_POLY_TEST) | defined(D_CIRCLE_TEST)
	if (CheckPush(KEY_MOUSE_LBTN))
	{
		POINT mouse = GetCursorPosition();
		mymath::Vec3f p((float)mouse.x,(float)mouse.y,0.f);
		vertexes.push_back(p);
	}
	if (CheckPush(KEY_MOUSE_RBTN))
	{
		InsertObject(ObjPtr(new CActionPolygon(vertexes)));
		vertexes.clear();
	}
	if (CheckPush(KEY_SPACE))
	{
		vertexes.clear();
		for(auto& obj : objs)
		{
			if (obj->findName("ActionPolygon"))
			{
				obj->kill();
			}
		}
	}
#endif
}


void CCollision::draw()
{
#if defined(D_COLLISION_DRAW) || defined(D_STAGECOL_DRAW)
	const auto& objs = gm()->GetObjects();
	for (const auto& obj : objs)
	{
		if (obj->FindName("EnemyMng"))
		{
			const auto& em = std::dynamic_pointer_cast<CEnemyMng>(obj);
			const auto& enemies = em->getEnemies();
			for (const auto& enemy : enemies)
			{
				// 食らい領域
#ifdef D_COLLISION_DRAW
				const auto& cols = enemy->GetDamageAreas();
				for (const auto& col : cols)
				{
					col->Offset(mymath::Vec3f(0.f, 0.f, -0.1f));
					col->draw(0xffff0000, 1, false);
				}
#endif
				// ステージ領域
#ifdef D_STAGECOL_DRAW
				const auto& stac = enemy->GetStageCollisions();
				for (const auto& col : stac)
				{
					col->Offset(mymath::Vec3f(0.f, 0.f, +0.1f));
					col->draw(0xffff00ff, 1, false);
				}
#endif
			}
		}
		// 食らい領域
#ifdef D_COLLISION_DRAW
		const auto& cols = obj->GetDamageAreas();
		for (const auto& col : cols)
		{
			col->Offset(mymath::Vec3f(0.f, 0.f, -0.1f));
			col->draw(0xffff0000, 1, true);
		}
#endif
		// ステージ領域
#ifdef D_STAGECOL_DRAW
		const auto& stac = obj->GetStageCollisions();
		for(const auto& col : stac)
		{
			col->Offset(mymath::Vec3f(0.f, 0.f, +0.1f));
			col->draw(0xffff00ff, 1, false);
		}
#endif
	}
#endif

#ifdef D_POLY_TEST
	Draw_Line((int)line.sta.x,
		(int)line.sta.y,
		(int)line.end.x,
		(int)line.end.y,
		line.sta.z,
		-1,
		1);
	auto& actPolys = GetObjects("ActionPolygon");
	if (!actPolys.empty())
	{
		for (auto& act : actPolys)
		{
			CActionPolygon* ap = (CActionPolygon*)act.get();
			std::vector<mymath::Vec3f> points;
			if (ap->Contains(points, line))
			{
				for (auto& point : points)
				{
					Draw_BoxCenter(int(point.x), int(point.y),
						5, 5, 0.f,
						ARGB(255, 0, 0, 0),
						ARGB(255, 0, 0, 0),
						0, true);
				}
			}
		}
	}
#endif
#if defined(D_POLY_TEST) | defined(D_CIRCLE_TEST)
	if (!vertexes.empty())
	{
		// 登録中の線分
		if (vertexes.size() >= 2)
		{
			//Draw_
			for (size_t sta = 0; sta < vertexes.size() - 1; ++sta)
			{
				size_t end = (sta + 1) % vertexes.size();
				Draw_Line(int(vertexes[sta].x),
					int(vertexes[sta].y),
					int(vertexes[end].x),
					int(vertexes[end].y),
					vertexes[sta].z,
					ARGB(255, 255, 255, 0),
					1);
			}
		}
		// 終点とマウスを結ぶ
		mymath::Vec3f sta = vertexes.back();
		POINT mouse = GetCursorPosition();
		Draw_Line(int(sta.x),
			int(sta.y),
			mouse.x,
			mouse.y,
			sta.z,
			ARGB(255, 0, 255, 255),
			1);
	}
#endif
#ifdef D_CIRCLE_TEST

	auto& actCircles = GetObjects("ActionCircle");
	if (!actCircles.empty())
	{
		POINT mouse = GetCursorPosition();
		mymath::Vec3f p((float)mouse.x, (float)mouse.y);
		for (auto& act : actCircles)
		{
			CActionCircle* ap = (CActionCircle*)act.get();
			mymath::Vec3f point;
			if (ap->Contains(point, p))
			{
				Draw_BoxCenter(int(point.x), int(point.y),
					5, 5, 0.f,
					-1,//ARGB(255,0,0,0),
					-1,//ARGB(255,0,0,0),
					0, true);
			}
		}
		// マウスで生成した線分との当たり判定
		auto& mousePolys = GetObjects("ActionPolygon");
		if (!mousePolys.empty())
		{
			for (auto& mousePoly : mousePolys)
			{
				std::vector<mymath::Vec3f> vers = ((CActionPolygon*)mousePoly.get())->vertexes;
				if (vers.size() >= 2)
				for (size_t sta = 0; sta < vers.size(); ++sta)
				{
					size_t end = (sta + 1) % vers.size();
					for (auto& act : actCircles)
					{
						CActionCircle* ap = (CActionCircle*)act.get();
						std::vector<mymath::Vec3f> points;
						//if (ap->Contains(points, vers[sta], vers[end]))
						mymath::Vec3f point;
						if (ap->Contains(point, vers[sta], vers[end]))
						{
							//for(auto& point : points)
							{
								Draw_BoxCenter(int(point.x), int(point.y),
								5, 5, 0.f,
								-1,//ARGB(255,0,0,0),
								-1,//ARGB(255,0,0,0),
								0, true);
							}
						}
					}
				}
			}
		}
	}
#endif

}