#include "sceneDemoPlay.h"
#include "gameManager.h"

//========================================================
#pragma region CSceneDemoPlay methods
CSceneDemoPlay::CSceneDemoPlay():
Base("SceneDemoPlay")
{
}
CSceneDemoPlay::~CSceneDemoPlay()
{
}

void CSceneDemoPlay::start()
{
	__super::start();
	camera::SetLookAt(system::WINW / 2.f, system::WINH / 2.f);
	camera::SetScale(1.f);

	// スタッフクレジット初期化
	creditObjs_.clear();
	std::vector<charabase::BaseData> bds = {
		charabase::BaseData("img_creditPG00", -1, -1),
		charabase::BaseData("img_creditPG01", -1, -1),
		charabase::BaseData("img_creditGFX00", -1, -1),
		charabase::BaseData("img_creditGFX01", -1, -1),
		charabase::BaseData("img_creditGFX02", -1, -1),
		charabase::BaseData("img_creditGFX03", -1, -1),
		charabase::BaseData("img_creditGFX04", -1, -1),
		charabase::BaseData("img_creditSND00", -1, -1),
		charabase::BaseData("img_creditSND01", -1, -1),
	};
	
	std::vector<CreditObject> creObjs = {
		{ -15.f, mymath::Linef(system::WINW / 2.f, 100.f, 0.1f, system::WINW - graph::Draw_GetImageWidth("img_creditPG00") / 2.f - 30.f, 100.f, 0.1f), charabase::CharBase(), common::DispState::APPEARING },
		{ -17.f, mymath::Linef(system::WINW / 2.f, system::WINH - 100.f, 0.1f, graph::Draw_GetImageWidth("img_creditPG01") / 2.f + 30.f, system::WINH - 100.f, 0.1f), charabase::CharBase(), common::DispState::APPEARING },
		{ -19.f, mymath::Linef(system::WINW / 2.f, system::WINH - 100.f, 0.1f, system::WINW - graph::Draw_GetImageWidth("img_creditGFX00") / 2.f - 30.f, system::WINH - 100.f, 0.1f), charabase::CharBase(), common::DispState::APPEARING },
		{ -21.f, mymath::Linef(system::WINW / 2.f, 100.f, 0.1f, graph::Draw_GetImageWidth("img_creditGFX01") / 2.f + 30.f, 100.f, 0.1f), charabase::CharBase(), common::DispState::APPEARING },
		{ -23.f, mymath::Linef(system::WINW / 2.f, system::WINH - 100.f, 0.1f, graph::Draw_GetImageWidth("img_creditGFX02") / 2.f + 30.f, system::WINH - 100.f, 0.1f), charabase::CharBase(), common::DispState::APPEARING },
		{ -25.f, mymath::Linef(system::WINW / 2.f, 100.f, 0.1f, graph::Draw_GetImageWidth("img_creditGFX03") / 2.f + 30.f, 100.f, 0.1f), charabase::CharBase(), common::DispState::APPEARING },
		{ -25.f, mymath::Linef(system::WINW / 2.f, 100.f, 0.1f, system::WINW - graph::Draw_GetImageWidth("img_creditGFX04") / 2.f - 30.f, 100.f, 0.1f), charabase::CharBase(), common::DispState::APPEARING },
		{ -27.f, mymath::Linef(system::WINW / 2.f, system::WINH - 100.f, 0.1f, graph::Draw_GetImageWidth("img_creditSND00") / 2.f + 30.f, system::WINH - 100.f, 0.1f), charabase::CharBase(), common::DispState::APPEARING },
		{ -27.f, mymath::Linef(system::WINW / 2.f, system::WINH - 100.f, 0.1f, system::WINW - graph::Draw_GetImageWidth("img_creditSND01") / 2.f - 30.f, system::WINH - 100.f, 0.1f), charabase::CharBase(), common::DispState::APPEARING },
	};
	creditObjs_ = creObjs;
	{
		int i = 0;
		for (auto& obj : creditObjs_)
			obj.obj.SetImgData(bds[i++]);
	}

	// デモステージの初期化
	gm()->stageMng()->start();
	gm()->stageMng()->LoadStage("DemoStage");
	auto& objs = gm()->GetObjects("Collision ScoreMng", ' ');
	for (auto& obj : objs)
		obj->start();
	player_ = std::dynamic_pointer_cast<CDemoPlayer>(gm()->GetObj(typeid(CDemoPlayer)));
}

void CSceneDemoPlay::stop()
{
	gm()->stageMng()->stop();
	//-----------------------------------
	// 使用デモプレイヤーオブジェクト, 敵オブジェクトを消す
	{
		auto& objs = gm()->GetObjects("Player Pickup Atk_ Effect", ' ');
		for (auto& obj : objs)
			obj->kill();
	}
	gm()->enemyMng()->ClearEnemies();
	{
		auto& objs = gm()->GetObjects("Collision ScoreMng StageMng", ' ');
		for (auto& obj : objs)
			obj->stop();
	}
}

bool CSceneDemoPlay::update()
{

#pragma region スタッフクレジット
	for (auto& obj : creditObjs_)
	{
		const float appearTime = 0.3f;		// 出現アニメーション秒数
		const mymath::Vec3f appearMove = obj.pos.end - obj.pos.sta;		// 移動量
		const float waitTime = 1.f;			// 表示秒数
		const float disappearTime = 0.4f;	// 消失アニメーション秒数
		switch (obj.state)
		{
		case common::DispState::HIDE:
			break;

		case common::DispState::APPEARING:
			obj.obj.alpha = Easing::Linear(obj.time, 0.f, 220.f, appearTime);
			obj.obj.pos.x = Easing::Linear(obj.time, obj.pos.sta.x, appearMove.x, appearTime);
			obj.obj.pos.y = Easing::Linear(obj.time, obj.pos.sta.y, appearMove.y, appearTime);

			if ((obj.time += system::ONEFRAME_TIME) >= appearTime)
			{
				// 出現完了
				obj.state = common::DispState::SHOW;
				obj.time = 0.f;
			}
			break;
		case common::DispState::SHOW:
			if ((obj.time+= system::ONEFRAME_TIME) >= waitTime)
			{
				// 消失開始
				obj.state = common::DispState::DISAPPEARING;
				obj.time = 0.f;
			}
			break;
		case common::DispState::DISAPPEARING:
			obj.obj.alpha = Easing::Linear(obj.time, 220.f, -220.f, disappearTime);
			obj.obj.pos.x = Easing::BackIn(obj.time, obj.pos.sta.x + appearMove.x, appearMove.x, disappearTime);
			obj.obj.pos.y = Easing::Linear(obj.time, obj.pos.sta.y, appearMove.y, appearTime);
			if ((obj.time += system::ONEFRAME_TIME) >= disappearTime)
			{
				// 消失完了
				obj.state = common::DispState::HIDE;
				obj.obj.pos = obj.pos.sta;
				obj.obj.alpha = 0.f;
				obj.time = 0.f;
			}
			break;
		}
	}
#pragma endregion // スタッフクレジット

	return player_.lock()->isEndDemoPlay();
}

void CSceneDemoPlay::draw()
{
	// スタッフクレジット
	graph::Draw_SetRenderMode(RENDER_MODE::ADD);
	for (auto& obj : creditObjs_)
		if (obj.state != common::DispState::HIDE)
			obj.obj.drawNC();
	graph::Draw_EndRenderMode();
}

#pragma endregion	// CSceneDemoPlay methods
//========================================================



//========================================================
#pragma region CSceneDemoPlay::CDemoPlayer methods

CSceneDemoPlay::CDemoPlayer::CDemoPlayer(float x, float y, float z) :
CPlayer(x, y, z)
{
	name_ = "DemoPlayer";
	timeIndex_ = 0;
	preActTime_ = 0.f;
	elapsedTime_ = 0.f;

	//-------------------------------------
	// カーソル
	auto cursor = gm()->cursor();
	cursor.obj.pos = camera::GetLookAt();
	cursor.obj.pos.z = 0.1f;
	cursor.obj.SetUse(true);
	cursor.obj.r = 255.f;
	cursor.obj.g = 255.f;
	cursor.obj.b = 0.f;
	gm()->cursor(cursor);
	gm()->SetCursorSynchronicity(false);


	std::vector<TableContent> tc = {
		{ 0.5f, mymath::Vec3f(2000.f, 1900.f), CDemoPlayer::TimeActType::HANG },
		{ 0.7f, mymath::Vec3f(2000.f, 1900.f), CDemoPlayer::TimeActType::HANG },
		{ 1.0f, mymath::Vec3f(2000.f, 1500.f), CDemoPlayer::TimeActType::HANG },
		{ 2.7f, mymath::Vec3f(1600.f, 1200.f), CDemoPlayer::TimeActType::HANG },
		{ 3.0f, mymath::Vec3f(1600.f, 900.f), CDemoPlayer::TimeActType::HANG },
		{ 3.2f, mymath::Vec3f(1300.f, 800.f), CDemoPlayer::TimeActType::HANG },
		{ 3.5f, mymath::Vec3f(1200.f, 700.f), CDemoPlayer::TimeActType::HANG },
		{ 4.0f, mymath::Vec3f(1100.f, 600.f), CDemoPlayer::TimeActType::HANG },
		{ 4.2f, mymath::Vec3f(1100.f, 500.f), CDemoPlayer::TimeActType::HANG },
		{ 4.4f, mymath::Vec3f(1000.f, 400.f), CDemoPlayer::TimeActType::ATTACK },
		{ 4.5f, mymath::Vec3f(1000.f, 500.f), CDemoPlayer::TimeActType::ATTACK },
		{ 4.6f, mymath::Vec3f(1000.f, 600.f), CDemoPlayer::TimeActType::ATTACK },
		{ 4.7f, mymath::Vec3f(1000.f, 700.f), CDemoPlayer::TimeActType::ATTACK },
		{ 4.8f, mymath::Vec3f(1000.f, 800.f), CDemoPlayer::TimeActType::ATTACK },
		{ 4.9f, mymath::Vec3f(1000.f, 900.f), CDemoPlayer::TimeActType::ATTACK },
		{ 5.0f, mymath::Vec3f(1000.f, 1000.f), CDemoPlayer::TimeActType::ATTACK },
		{ 5.1f, mymath::Vec3f(1000.f, 1100.f), CDemoPlayer::TimeActType::ATTACK },

		{ 100.0f, mymath::Vec3f(1000.f, 400.f), CDemoPlayer::TimeActType::UNHANG },
	};
	timeTable_ = tc;
}

void CSceneDemoPlay::CDemoPlayer::step()
{
	__super::step();

	elapsedTime_ += system::ONEFRAME_TIME;

	//-------------------------------------
	// カーソル
	mymath::Vec3f nextPos;
	const auto& cursor = gm()->GetCursorPos();
	const auto tpos = timeTable_[timeIndex_].pos;
	nextPos.x = Easing::Linear(elapsedTime_ - preActTime_, cursor.x, tpos.x - cursor.x, timeTable_[timeIndex_].time);
	nextPos.y = Easing::Linear(elapsedTime_ - preActTime_, cursor.y, tpos.y - cursor.y, timeTable_[timeIndex_].time);
	nextPos.z = cursor.z;
	gm()->SetCursorPos(nextPos);
}


void CSceneDemoPlay::CDemoPlayer::key()
{
	// キー入力を受け付けない
	// 時間により特定箇所をクリックしたものとする
	if (elapsedTime_ >= timeTable_[timeIndex_].time)
	{
		preActTime_ = elapsedTime_;
		switch (timeTable_[timeIndex_].act)
		{
		case CDemoPlayer::TimeActType::HANG:
			SetHangPoint(gm()->GetCursorPos());
			break;
		case CDemoPlayer::TimeActType::UNHANG:
			UnHang();
			break;
		case CDemoPlayer::TimeActType::ATTACK:
			isAttacking_ = true;
			break;

		default:
			break;
		}
		++timeIndex_;
		if (timeIndex_ >= static_cast<int>(timeTable_.size()))
			timeIndex_--;
	}
}



void CSceneDemoPlay::CDemoPlayer::MoveObject()
{
	// 移動時間を1 / 60固定
	mymath::Vec3f v(obj_.add * system::ONEFRAME_TIME);
	obj_.pos.x += v.x;
	obj_.pos.y += v.y;
}

bool CSceneDemoPlay::CDemoPlayer::isEndDemoPlay() const
{
	return elapsedTime_ > 35.f;
}
#pragma endregion	// CSceneDemoPlay::CDemoPlayer methods
//========================================================
