#ifdef _DEBUG
//#define D_CIRCLE_TEST
//#define D_POLY_TEST
#define D_COLLISION_DRAW	// �����蔻��`��
#define D_STAGECOL_DRAW		// �X�e�[�W�����蔻��`��
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
	// �v���C���[
	if (player != nullptr)
	{
		auto& pl = std::dynamic_pointer_cast<CPlayer>(player);
		const mymath::Vec3f& plpos = pl->obj().pos;
		auto& plstageCollisions = pl->GetStageCollisions();

		//-----------------------------------------
		// �X�e�[�W�����蔻��
		for (auto& stacol : plstageCollisions)
		{
			//-----------------------------------------------------
			// �v���C���[ vs pickup
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
			// �v���C���[ vs ActionPoint
			for (auto& actp : actionPoints)
			{
#ifndef DEF_PREPOS
				const mymath::Vec3f& nextPos = pl->nextPos();
				if (actp->Contains(plpos, nextPos))
#else
				if (actp->Contains(pl->prePos(), plpos))
#endif
				{
					// ����
					pl->hit(actp);
				}
				//else if (actp->Contains(nextPos))
				{
					// ����
					//pl->hit(actp);
				}
			}
		}

		//-----------------------------------------
		// �U��
		if (pl->isAttacking())
		{
			POINT m = camera::GetCursorPosition();
			mymath::Vec3f mouse(static_cast<float>(m.x),
				static_cast<float>(m.y));
			//-----------------------------------------
			// �U�� vs �G
			if (em != nullptr)
			{
				for (auto& enemy : enemies)
				{
					const mymath::Vec3f& enpos = enemy->obj().pos;
					mouse.z = enpos.z;
					const auto& enemyCollisions = enemy->GetDamageAreas();
					// �����蔻��
					for (const auto& col : enemyCollisions)
					{
						// �����蔻����̂�
						if (!(col->Contains(mouse) && col->Contains(plpos, mouse))) continue;
						mymath::Linef line(plpos, col->IntersectionPoint2Nearest(plpos, mouse));
						// �G�Ƃ̒���������ActionPolygon���Ȃ��ꍇ�̂ݍU���L��
						bool atkFlag = true;
						for (const auto& actp : actionPoints)
						{
							// ActioinPolygon�ȊO�r��
							if (!actp->FindName("ActionPolygon"))continue;
							// ActionPolygon�ƌ���
							if (actp->Contains(line))
							{
								atkFlag = false;
								break;
							}
						}
						//-----------------------------------------
						// �U������
						if (atkFlag)
						{
							auto& epos = enemy->obj().pos;
							// �_���[�W
							pl->ApplyAttack(epos);
							gm()->AddObject(ObjPtr(new CEffectSlash(epos)));
							if (enemy->ApplyDamage(pl->getPower()))
							{
								//-----------------------------------------
								// �Gkill
								pl->KilledEnemy();
							}
							break;
						}
					}
				}
			}
		}


		//-----------------------------------------
		// ��炢����
		auto& plCols = player->GetDamageAreas();
		for (auto& pc : plCols)
		{
			//auto& pc = *std::dynamic_pointer_cast<mymath::Rectf>(plcol);

			//-----------------------------------------
			// �v���C���[ vs �G�U��
			if (em != nullptr)
			{
				for (auto& enemy : enemies)
				{
					const mymath::Vec3f& enpos = enemy->obj().pos;
					//-----------------------------------------
					// �G�U�� 
					auto& enemyAtks = enemy->GetAttackAreas();
					int i = 0;
					for (auto& col : enemyAtks)
					{
						// �G�U�� vs �v���C���[
						if (col->Contains(pc))
						{
							// ��_��
							pl->hit(enemy);
							break;
							// �U����ޕʏ���
							//if (enemy->attack->findName("Atk_NWayShot"))
							//{
							//	
							//}
						}
					}
				}
			}
			//-----------------------------------------------------
			// �v���C���[ vs �������U��
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
			// �v���C���[ vs pickup
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
	// �G
	if (em != nullptr)
	{
		for (auto& enemy : enemies)
		{
			auto& en = std::dynamic_pointer_cast<IEnemy>(enemy);
			const mymath::Vec3f& enpos = en->obj().pos;
			//-----------------------------------------
			// �G vs ActionPoint
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
						// ����
						en->hit(actp);
						//actp->hit(en);
					}
					//else if (actp->Contains(nextPos))
					{
						// ����
						//	en->hit(actp);
					}
				}
			}
		}
	}

	//-----------------------------------------------------
	// �������U��
	for (auto& shot : atk_shots)
	{
		auto& atkCols = shot->GetDamageAreas();
		for (auto& atkcol : atkCols)
		{
			//-----------------------------------------
			// �G�U�� vs Polygon
			for (auto& actp : actionPoints)
			{
				// ActionoPolygon�̂�
				if (!actp->FindName("ActionPolygon")) continue;
				// �����	
				if (actp->Contains(atkcol))
				{
					// �U����ޕʏ���
					//shot->kill();
					shot->hit(actp);
				}
				// �ʉߔ���
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
				// �H�炢�̈�
#ifdef D_COLLISION_DRAW
				const auto& cols = enemy->GetDamageAreas();
				for (const auto& col : cols)
				{
					col->Offset(mymath::Vec3f(0.f, 0.f, -0.1f));
					col->draw(0xffff0000, 1, false);
				}
#endif
				// �X�e�[�W�̈�
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
		// �H�炢�̈�
#ifdef D_COLLISION_DRAW
		const auto& cols = obj->GetDamageAreas();
		for (const auto& col : cols)
		{
			col->Offset(mymath::Vec3f(0.f, 0.f, -0.1f));
			col->draw(0xffff0000, 1, true);
		}
#endif
		// �X�e�[�W�̈�
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
		// �o�^���̐���
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
		// �I�_�ƃ}�E�X������
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
		// �}�E�X�Ő������������Ƃ̓����蔻��
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