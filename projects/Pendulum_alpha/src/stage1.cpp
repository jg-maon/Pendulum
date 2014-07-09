#include "stage1.h"

#include "easing.h"

CStage1::CStage1(ifstream& f) :
IStage("Stage1")
{
	goalObj_ = charabase::CharPtr(new charabase::CharBase());

	load(f);
	LoadClear(f, goalArea_);
}

void CStage1::step()
{
	__super::step();

}

void CStage1::draw()
{
	// ActionPoint
	__super::draw();

	if (caObj_->CheckUse())
		caObj_->drawNC();

	auto s = graph::Draw_GetImageSize2("img_stage01");
	graph::Draw_GraphicsLeftTop(
		cameraRect_.left, cameraRect_.top, 1.f,
		"img_stage01", 0, 0, s.cx, s.cy, 0, 0,
		(cameraRect_.right - cameraRect_.left) / static_cast<float>(system::WINW),
		(cameraRect_.bottom - cameraRect_.top) / static_cast<float>(system::WINH));

}

void CStage1::init(std::ifstream& f)
{
	__super::init(f);
	
	caPhase_ = ClearAnnouncePhase::TO_GOAL;
	announceTime_ = 0.f;

	// タイトルアニメーション中はプレイヤーと敵の動きを止めておく必要がある
	auto& objs = gm()->GetObjects("Player EnemyMng", ' ');
	for (auto& obj : objs)
		obj->SetStatusDisp();
	auto playerPos_ = gm()->GetPlayerPos();
	camera::SetLookAt(playerPos_.x, playerPos_.y);

	// 条件は左中央から来る
	RECT rt = camera::GetScreenRect();
	caObj_->pos.x = -caObj_->HalfWidth();
	caObj_->pos.y = static_cast<float>(system::WINH) / 4.f;
	caObj_->pos.z = 0.f;
	caStartPos_ = caObj_->pos;
	caObj_->SetUse(false);
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
			goalObj_->pos = circle->center;
			goalObj_->size = static_cast<int>(circle->radius);
		}
		else if (info.size() == 4)
		{
			// X Y W H
			area = mymath::ShapefPtr(new mymath::Rectf(info[0], info[1], info[0] + info[2], info[1] + info[3]));
			goalObj_->size.x = static_cast<int>(info[2]);
			goalObj_->size.y = static_cast<int>(info[3]);
			goalObj_->pos.x = info[0] + goalObj_->HalfWidth();
			goalObj_->pos.y = info[1] + goalObj_->HalfHeight();
		}
		goalObj_->pos.z = 0.4f;
		//---------------------------------------------------
		// ゴールオブジェクト画像
		charabase::BaseData bd;
		f >> bd;
		goalObj_->SetImgData(bd);
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
			const mymath::Vec3f vec = goalObj_->pos - playerPos_;
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
			const mymath::Vec3f vec = playerPos_ - goalObj_->pos;
			cameraPos.x = Easing::Linear(announceTime_, goalObj_->pos.x, vec.x, moveTime);
			cameraPos.y = Easing::Linear(announceTime_, goalObj_->pos.y, vec.y, moveTime);

			if (announceTime_ >= moveTime)
			{
				camera::SetLookAt(cameraPos.x, cameraPos.y);
				announceTime_ = 0.f;
				caPhase_ = ClearAnnouncePhase::MSGIN;
				caObj_->SetUse(true);
			}
		}
		break;
	case CStage1::ClearAnnouncePhase::MSGIN:
		{
			const float moveTime = 0.3f;
			const float animTime = 2.0f;	// 半分色変化半分待機
			if (announceTime_ <= moveTime)
			{
				// 移動時間中
				// 画像が画面中心にくるように
				caObj_->pos.x = Easing::Linear(announceTime_,
					caStartPos_.x,
					caObj_->HalfWidth() + static_cast<float>(system::WINW) / 2.f,
					moveTime);
			}
			else if (announceTime_ <= moveTime + animTime)
			{
				caObj_->pos.x = static_cast<float>(system::WINW) / 2.f;
				// アニメーション開始してからの経過時間
				float t = announceTime_ - moveTime;
				// 終了値rgb( r, g, b)へ向かう
				float r = 100.f, g = 190.f, b = 250.f;
				caObj_->r = Easing::ElasticOut(t, 255.f, -255.f+r, animTime / 2.f);
				caObj_->g = Easing::ExpoOut(t, 255.f, -255.f+g, animTime / 2.f);
				caObj_->b = Easing::BackOut(t, 255.f, -255.f+b, animTime / 2.f);

			}
			else
			{
				// 終了
				announceTime_ = 0.f;
				caPhase_ = ClearAnnouncePhase::MSGOUT;
				// 次のイージング用に座標更新
				caStartPos_ = caObj_->pos;
			}
		}
		break;
	case CStage1::ClearAnnouncePhase::MSGOUT:
		{
			const float moveTime = 0.5f;
			// 移動
			// 画面外へ
			caObj_->pos.x = Easing::BackIn(
				announceTime_,
				caStartPos_.x,
				system::WINW / 2.f + caObj_->HalfWidth(),
				moveTime);
			if (announceTime_ >= moveTime)
			{
				// 終了
				announceTime_ = 0.f;
				caObj_->SetUse(false);
				// プレイヤーと敵を始動
				auto& objs = gm()->GetObjects("Player EnemyMng", ' ');
				for (auto& obj : objs)
					obj->start();
				return true;
			}
		}
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






