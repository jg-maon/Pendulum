#include "stage1.h"

#include "gameManager.h"
#include "common.h"

#include "easing.h"

CStage1::CStage1(std::ifstream& f) :
IStage("Stage1")
{
	goalObj_ = charabase::CharPtr(new charabase::CharBase());
	sm_ = std::dynamic_pointer_cast<CStageMng>(gm()->GetObj(typeid(CStageMng)));
	LoadEnv(f);		// ステージシステム読み込み
	load(f, 0);		// 雑魚ステージ読み込み
	LoadClear(f, goalArea_);	// クリア条件読み込み
	//------------------------------------
	// ボスステージ読み込み
	if (common::FindChunk(common::SeekSet(f), "#BossStage"))
	{
		//std::string bossFile;
		f >> bossFile_;
		std::ifstream bossF(bossFile_);
		if (f)
			load(bossF, 1);
		else
			debug::BToMF("bossFile_ not found path:%s", bossFile_.c_str());
	}
	//------------------------------------
}

void CStage1::step()
{
	bool f = (phase_ == Phase::FADE_OUT);
	__super::step();
	// フェードインからフェードアウトに変わった瞬間
	if (f && phase_ == Phase::FADE_IN)
	{
		// ボスステージをロードする前に雑魚ステージでのボーナススコア計算
		auto& sm = gm()->scoreMng();
		auto& em = gm()->enemyMng();
		sm->CalcBonusScore(em);
		std::ifstream bossF(bossFile_);
		__super::init(bossF);
		auto& objs = gm()->GetObjects("Player EnemyMng", ' ');
		for (auto& obj : objs)
			obj->start();
	}

}

void CStage1::draw()
{
	// ActionPoint
	__super::draw();

	if (caObj_->CheckUse())
		caObj_->drawNC();


	auto s = graph::Draw_GetImageSize2("img_stage01");
	mymath::Recti rc = getCameraRect();
	
#if 0
	rc.draw(-1, 1, false);
#endif
	float sx = rc.width() / static_cast<float>(s.cx);
	float sy = rc.height() / static_cast<float>(s.cy);
	/*
	for (int y = 0; y <= s.cy / 1024; ++y)
	{
		for (int x = 0; x <= s.cx / 1024; ++x)
		{
			graph::Draw_GraphicsLeftTop(
				rc.left + x * 1024,
				rc.top + y * 1024,
				1.f,
				"img_stage01",
				x * 1024*sx, y * 1024*sy,
				1024*sx, 1024*sy,
				0.f, nullptr,
				//sx, sy
				1.f
				);
		}
	}
	//*/
	//*
	graph::Draw_GraphicsLeftTop(
		rc.left, rc.top, 1.f,
		"img_stage01", 0, 0, s.cx, s.cy, 0, 0,
		sx,
		sy);
	//*/

}

void CStage1::init(std::ifstream& f)
{
	__super::init(f);

	phase_ = IStage::Phase::CLEAR_ANNOUNCE;
	
	caPhase_ = ClearAnnouncePhase::WAIT;
	announceTime_ = 0.f;

	// タイトルアニメーション中はプレイヤープレイヤー登場アニメーション
	auto& objs = gm()->GetObjects("Player");
	for (auto& obj : objs)
		obj->start();
	auto playerPos_ = gm()->GetPlayerPos();
	
	sm_.lock()->MoveCamera(playerPos_);

	// 条件は左中央から来る
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
		debug::BToMF("CStage1::LoadGoal #StageClear not found");
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
	case CStage1::ClearAnnouncePhase::WAIT:
		if (!(sm_.lock()->isEnterAnimating() || sm_.lock()->isExitAnimating()))
		{
			// タイトルアニメーション中はプレイヤーと敵の動きを止めておく必要がある
			auto& objs = gm()->GetObjects("Player EnemyMng", ' ');
			for (auto& obj : objs)
				obj->SetStatusDisp();

			announceTime_ = 0.f;
			caPhase_ = ClearAnnouncePhase::TO_GOAL;
		}
		break;
	case CStage1::ClearAnnouncePhase::TO_GOAL:
		{
			const float moveTime = 2.f;		// カメラ移動時間
			const mymath::Vec3f vec = goalObj_->pos - playerPos_;
			cameraPos.x = Easing::ExpoIn(announceTime_, playerPos_.x, vec.x, moveTime);
			cameraPos.y = Easing::ExpoIn(announceTime_, playerPos_.y, vec.y, moveTime);
			if (announceTime_ >= moveTime)
			{
				caPhase_ = ClearAnnouncePhase::TO_PLAYER;
				announceTime_ = 0.f;
			}
		}

		break;
	case CStage1::ClearAnnouncePhase::TO_PLAYER:
		{
			const float waitTime = 1.f;		// wait時間
			if (announceTime_ < waitTime) break;
			float animTime = announceTime_ - waitTime;
			const float moveTime = 0.5f;		// カメラ移動時間
			const mymath::Vec3f vec = playerPos_ - goalObj_->pos;
			cameraPos.x = Easing::Linear(animTime, goalObj_->pos.x, vec.x, moveTime);
			cameraPos.y = Easing::Linear(animTime, goalObj_->pos.y, vec.y, moveTime);

			if (animTime >= moveTime)
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
				caObj_->g = Easing::ExpoInOut(t, 255.f, -255.f+g, animTime / 2.f);
				caObj_->b = Easing::SineIn(t, 255.f, -255.f+b, animTime / 2.f);
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

	sm_.lock()->MoveCamera(cameraPos);

	return false;
}


bool CStage1::UpdateNormal()
{
	const auto& player = gm()->GetPlayer();
	auto& stacols = player->GetStageCollisions();
	for (auto& col : stacols)
	{
		if (goalArea_->Contains(col))
		{
			sm_.lock()->setStageState(CStageMng::StageState::EXIT);
			//sm_.lock()
			return true;
		}
	}

	return false;
}

bool CStage1::UpdateBoss()
{
	// ボス
	auto& em = gm()->enemyMng();
	auto& enemies = em->getEnemies();
	
	for (auto& enemy : enemies)
	{
		// ボスオブジェクトが生存している場合継続
		if (enemy->FindName("E_Boss"))
			return false;
	}

	// pickup(ボスが大量にドロップする可能性も兼ね)
	auto& pickups = gm()->GetObjects("Pickup");
	if (!pickups.empty())
		return false;


	return true;
}






