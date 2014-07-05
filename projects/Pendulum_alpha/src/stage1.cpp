#include "stage1.h"

#include "easing.h"

CStage1::CStage1(ifstream& f) :
IStage("Stage1")
{
	load(f);
	LoadClear(f, goalArea_);
}

void CStage1::step()
{
	__super::step();

}

void CStage1::draw()
{
	__super::draw();

	graph::Draw_GraphicsLeftTop(
		cameraRect_.left, cameraRect_.top, 1.f,
		"img_stage01", 0, 0, 1280, 800, 0, 0,
		(cameraRect_.right - cameraRect_.left) / 1280.f,
		(cameraRect_.bottom - cameraRect_.top) / 800.f);

}

void CStage1::init(std::ifstream& f)
{
	__super::init(f);
	
	caPhase_ = ClearAnnouncePhase::TO_GOAL;
	announceTime_ = 0.f;
	//camera::SetLookAt(goalObj_.pos.x, goalObj_.pos.y);

	// タイトルアニメーション中はプレイヤーと敵の動きを止めておく必要がある
	auto& objs = gm()->GetObjects("Player EnemyMng", ' ');
	for (auto& obj : objs)
		obj->SetStatusDisp();
	//playerPos_ = gm()->GetPlayerPos();
}



void CStage1::LoadClear(std::ifstream& f, mymath::ShapefPtr& area)
{
	if (!common::FindChunk(common::SeekSet(f), "#StageClear"))
	{
		debug::BToM("CStage1::LoadGoal #StageClear not found");
		return;
	}
	std::string label;
	f >> label;
	if (label == "Goal")
	{
		// ゴールポイント
		std::vector<float> info;
		std::getline(f, label);
		std::stringstream ss(label);
		while (!ss.eof())
		{
			float val;
			ss >> val;
			info.push_back(val);
		}
		if (info.size() == 3)
		{
			// X Y Radius
			area = mymath::ShapefPtr(new mymath::Circlef(info[0], info[1], 0.5f, info[2]));
			auto& circle = std::dynamic_pointer_cast<mymath::Circlef>(area);
			goalObj_.pos = circle->center;
			goalObj_.size = static_cast<int>(circle->radius);
		}
		else if (info.size() == 4)
		{
			// X Y W H
			area = mymath::ShapefPtr(new mymath::Rectf(info[0], info[1], info[2], info[3]));
			auto& rect = std::dynamic_pointer_cast<mymath::Rectf>(area);
			goalObj_.size.x = static_cast<int>(rect->right - rect->left);
			goalObj_.size.y = static_cast<int>(rect->bottom - rect->top);
			goalObj_.pos.x = rect->left + goalObj_.HalfWidth();
			goalObj_.pos.y = rect->top + goalObj_.HalfHeight();
		}
		//goalObj_
	}
	else if (label == "Annihilation")
	{
		// 殲滅型
	}
}




bool CStage1::UpdateClearAnnounce()
{
	const mymath::Vec3f& playerPos_ = gm()->GetPlayerPos();
	mymath::Vec3f cameraPos = camera::GetLookAt();

	announceTime_ += system::FrameTime;

	// 自分→ゴール→自分
	switch (caPhase_)
	{
	case CStage1::ClearAnnouncePhase::TO_GOAL:
		{
			const float moveTime = 2.f;		// カメラ移動時間
			const mymath::Vec3f vec = goalObj_.pos - playerPos_;
			cameraPos.x = Easing::ExpoIn(announceTime_, playerPos_.x, vec.x, moveTime);
			cameraPos.y = Easing::ExpoIn(announceTime_, playerPos_.y, vec.y, moveTime);
			if (announceTime_ >= moveTime)
			{
				caPhase_ = ClearAnnouncePhase::WAIT;
				announceTime_ = 0.f;
			}
		}

		break;
	case CStage1::ClearAnnouncePhase::WAIT:
		{
			const float waitTime = 1.f;		// wait時間
			if (announceTime_ >= waitTime)
			{
				caPhase_ = ClearAnnouncePhase::TO_PLAYER;
				announceTime_ = 0.f;
			}
		}
		break;
	case CStage1::ClearAnnouncePhase::TO_PLAYER:
		{
			const float moveTime = 0.5f;		// カメラ移動時間
			const mymath::Vec3f vec = playerPos_ - goalObj_.pos;
			cameraPos.x = Easing::Linear(announceTime_, goalObj_.pos.x, vec.x, moveTime);
			cameraPos.y = Easing::Linear(announceTime_, goalObj_.pos.y, vec.y, moveTime);

			if (announceTime_ >= moveTime)
			{
				// 終了
				camera::SetLookAt(cameraPos.x, cameraPos.y);
				caPhase_ = ClearAnnouncePhase::WAIT;
				// プレイヤーと敵を始動
				auto& objs = gm()->GetObjects("Player EnemyMng", ' ');
				for (auto& obj : objs)
					obj->start();
				return true;
			}
		}
		break;
	}

	camera::SetLookAt(cameraPos.x, cameraPos.y);

	return false;
}


bool CStage1::UpdateNormal()
{
	return false;
}

bool CStage1::UpdateBoss()
{
	return false;
}






