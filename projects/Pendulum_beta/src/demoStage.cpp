#include "demoStage.h"

#include "gameManager.h"
#include "common.h"

#include "easing.h"

#include "sceneDemoPlay.h"

CDemoStage::CDemoStage(std::ifstream& f) :
IStage("DemoStage")
{
	goalObj_ = charabase::CharPtr(new charabase::CharBase());
	sm_ = gm()->stageMng();
	LoadEnv(f);		// �X�e�[�W�V�X�e���ǂݍ���
	load(f, 0);		// �G���X�e�[�W�ǂݍ���
	LoadClear(f, goalArea_);	// �N���A�����ǂݍ���
	//------------------------------------
	// �{�X�X�e�[�W�ǂݍ���
	if (common::FindChunk(common::SeekSet(f), "#BossStage"))
	{
		//std::string bossFile;
		f >> bossFile_;
		std::ifstream bossF(bossFile_);
		if (bossF)
			load(bossF, 1);
		else
			debug::BToMF("bossFile_ not found path:%s", bossFile_.c_str());
	}
	sm_.lock()->setStageState(CStageMng::StageState::BATTLE);
	//------------------------------------
}

void CDemoStage::step()
{
	switch (phase_)
	{
	case IStage::Phase::CLEAR_ANNOUNCE:
		if (!gm()->stageMng()->isClearAnnounceAnimating())
			break;
		if (UpdateClearAnnounce())
		{
			phase_ = IStage::Phase::NORMAL;
			gm()->stageMng()->setStageState(CStageMng::StageState::BATTLE);
		}
		break;
	case IStage::Phase::NORMAL:
		for (auto& ap : stage_[0].actionPoints)
			ap->step();
		if (UpdateNormal())
		{
			nextPhase_ = IStage::Phase::BOSS;
			phase_ = IStage::Phase::FADE_OUT;
			gm()->stageMng()->setStageState(CStageMng::StageState::PLAYER_ENTER);
			CFade::ChangeColor(0xff000000);
			CFade::StartFadeOut();
		}
		break;
	case IStage::Phase::FADE_OUT:
		if (CFade::FadeOut(255.f / 0.5f*system::ONEFRAME_TIME))
		{
			if (nextPhase_ == IStage::Phase::RESULT)
			{
				phase_ = nextPhase_;
			}
			else
			{
				phase_ = IStage::Phase::FADE_IN;
				CFade::StartFadeIn();
				
				std::ifstream bossF(bossFile_);
				__super::init(bossF);
				auto& objs = gm()->GetObjects("Player EnemyMng", ' ');
				for (auto& obj : objs)
					obj->start();
			}
		}
		break;
	case IStage::Phase::FADE_IN:
		if (CFade::FadeIn(255.f / 1.f*system::ONEFRAME_TIME))
		{
			phase_ = nextPhase_;
		}
		break;
	case IStage::Phase::BOSS:
		for (auto& ap : stage_[1].actionPoints)
			ap->step();
		if (isBossKilled_ = UpdateBoss())
		{
			phase_ = IStage::Phase::RESULT;
			CFade::ChangeColor(0xffffffff);
		}
		break;
	}
}

void CDemoStage::draw()
{
	// ActionPoint
	__super::draw();

	if (caObj_->CheckUse())
		caObj_->drawNC();


	auto s = graph::Draw_GetImageSize2("img_stage01");
	mymath::Recti& cameraRect = (phase_ == Phase::BOSS) ? stage_[1].cameraRect : stage_[0].cameraRect;
	mymath::Recti& stageRect = (phase_ == Phase::BOSS) ? stage_[1].stageRect : stage_[0].stageRect;
	mymath::Rectf screenRect = camera::GetScreenRect();

	// �X�e�[�W�̕��ɑ΂���J�������̔�
	float ratioCameraX = (s.cx - screenRect.width()) / static_cast<float>(cameraRect.right - screenRect.width());
	float ratioCameraY = (s.cy - screenRect.height()) / static_cast<float>(cameraRect.bottom - screenRect.height());
	// �摜�ɑ΂���X�e�[�W�ƃJ������̍��W
	int src_x = static_cast<int>(screenRect.left * ratioCameraX);
	int src_y = static_cast<int>(screenRect.top * ratioCameraY);
	graph::Draw_GraphicsLeftTopNC(
		0,
		0,
		1.f,
		"img_stage01",
		src_x, src_y,
		system::WINW, system::WINH
		);
}

void CDemoStage::init(std::ifstream& f)
{
	__super::init(f);

	phase_ = IStage::Phase::CLEAR_ANNOUNCE;

	caPhase_ = ClearAnnouncePhase::WAIT;
	announceTime_ = 0.f;

	// �^�C�g���A�j���[�V�������̓v���C���[�v���C���[�o��A�j���[�V����
	auto& objs = gm()->GetObjects("Player");
	for (auto& obj : objs)
		obj->start();
	auto playerPos_ = gm()->GetPlayerPos();

	sm_.lock()->MoveCamera(playerPos_);

	// �����͍��������痈��
	caObj_->pos.x = -caObj_->HalfWidth();
	caObj_->pos.y = static_cast<float>(system::WINH) / 4.f;
	caObj_->pos.z = 0.f;
	caStartPos_ = caObj_->pos;
	caObj_->SetUse(false);
	
}



void CDemoStage::LoadClear(std::ifstream& f, mymath::ShapefPtr& area)
{
	if (!common::FindChunk(common::SeekSet(f), "#StageClear"))
	{
		debug::BToMF("CDemoStage::LoadGoal #StageClear not found");
		return;
	}
	std::string label;
	f >> label;
	if (label == "Goal")
	{
		// �S�[���|�C���g
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
		// �S�[���I�u�W�F�N�g�摜
		charabase::BaseData bd;
		f >> bd;
		goalObj_->SetImgData(bd);
	}
	else if (label == "Annihilation")
	{
		// �r�Ō^
	}
}


bool CDemoStage::LoadPlayer(std::ifstream& f)
{
	if (common::FindChunk(common::SeekSet(f), "#Player"))
	{
		// �v���C���[���W���擾
		float pos[2];	// [0]:x [1]:y
		for (auto& p : pos)
		{
			std::string label;
			f >> label;
			p = static_cast<float>(std::atof(label.c_str()));
		}
		// �v���C���[�I�u�W�F�N�g��ǉ�
		auto& pl = gm()->GetObj(typeid(CSceneDemoPlay::CDemoPlayer));
		std::shared_ptr<CSceneDemoPlay::CDemoPlayer> player;
		if (!pl)
		{
			player = std::shared_ptr<CSceneDemoPlay::CDemoPlayer>(new CSceneDemoPlay::CDemoPlayer(pos[0], pos[1]));
			gm()->AddObject(player);
		}
		else
		{
			// ���d�o�^�h�~
			player = std::dynamic_pointer_cast<CSceneDemoPlay::CDemoPlayer>(pl);
			player->init(pos[0], pos[1], player->obj().pos.z);
		}

	}

	return f.eof();
}



bool CDemoStage::UpdateClearAnnounce()
{
	const mymath::Vec3f& playerPos_ = gm()->GetPlayerPos();
	mymath::Vec3f cameraPos = camera::GetLookAt();

	mymath::Rectf cameraRect = getCameraRect();

	announceTime_ += system::ONEFRAME_TIME;

	// ���������ォ�玞�v��聨����
	switch (caPhase_)
	{
	case CDemoStage::ClearAnnouncePhase::WAIT:
	{
		// �^�C�g���A�j���[�V�������̓v���C���[�ƓG�̓������~�߂Ă����K�v������
		auto& objs = gm()->GetObjects("Player EnemyMng", ' ');
		for (auto& obj : objs)
			obj->SetStatusDisp();

		announceTime_ = 0.f;
		caPhase_ = ClearAnnouncePhase::LEFTTOP;
	}
		break;
	case CDemoStage::ClearAnnouncePhase::LEFTTOP:
	{
		const float moveTime = 1.5f;		// �J�����ړ�����
		const float vecx = cameraRect.left - playerPos_.x;
		const float vecy = cameraRect.top - playerPos_.y;
		cameraPos.x = Easing::QuartOut(announceTime_, playerPos_.x, vecx, moveTime);
		cameraPos.y = Easing::QuartIn(announceTime_, playerPos_.y, vecy, moveTime);
		if (announceTime_ >= moveTime + 0.5f)
		{
			caPhase_ = ClearAnnouncePhase::RIGHTTOP;
			announceTime_ = 0.f;
		}
	}

		break;
	case CDemoStage::ClearAnnouncePhase::RIGHTTOP:
	{
		const float moveTime = 1.5f;		// �J�����ړ�����
		const float vecx = cameraRect.width();
		cameraPos.x = Easing::ExpoInOut(announceTime_, playerPos_.x, vecx, moveTime);
		if (announceTime_ >= moveTime + 0.5f)
		{
			caPhase_ = ClearAnnouncePhase::RIGHTBOTTOM;
			announceTime_ = 0.f;
		}
	}

		break;
	case CDemoStage::ClearAnnouncePhase::RIGHTBOTTOM:
	{
		const float moveTime = 1.5f;		// �J�����ړ�����
		const float vecy = cameraRect.height();
		cameraPos.y = Easing::ExpoInOut(announceTime_, cameraRect.top, vecy, moveTime);
		if (announceTime_ >= moveTime + 0.5f)
		{
			caPhase_ = ClearAnnouncePhase::LEFTBOTTOM;
			announceTime_ = 0.f;
		}
	}

		break;
	case CDemoStage::ClearAnnouncePhase::LEFTBOTTOM:
	{
		const float moveTime = 1.5f;		// �J�����ړ�����
		const float vecx = -cameraRect.width();
		cameraPos.x = Easing::ExpoInOut(announceTime_, cameraRect.right, vecx, moveTime);
		if (announceTime_ >= moveTime + 0.5f)
		{
			caPhase_ = ClearAnnouncePhase::TO_PLAYER;
			announceTime_ = 0.f;
		}
	}

		break;
	case CDemoStage::ClearAnnouncePhase::TO_PLAYER:
	{
		float animTime = announceTime_;
		const float moveTime = 0.5f;		// �J�����ړ�����
		mymath::Vec3f vec = playerPos_;
		vec.x -= cameraRect.left;
		vec.y -= cameraRect.bottom;
		cameraPos.x = Easing::QuartIn(animTime, cameraRect.left, vec.x, moveTime);
		cameraPos.y = Easing::QuartOut(animTime, cameraRect.bottom, vec.y, moveTime);

		if (animTime >= moveTime)
		{
			camera::SetLookAt(cameraPos.x, cameraPos.y);
			announceTime_ = 0.f;
			caPhase_ = ClearAnnouncePhase::MSGIN;
			caObj_->SetUse(true);
		}
	}
		break;
	case CDemoStage::ClearAnnouncePhase::MSGIN:
	{
		const float moveTime = 0.3f;
		const float animTime = 2.0f;	// �����F�ω������ҋ@
		if (announceTime_ <= moveTime)
		{
			// �ړ����Ԓ�
			// �摜����ʒ��S�ɂ���悤��
			caObj_->pos.x = Easing::Linear(announceTime_,
				caStartPos_.x,
				caObj_->HalfWidth() + static_cast<float>(system::WINW) / 2.f,
				moveTime);
		}
		else if (announceTime_ <= moveTime + animTime)
		{
			caObj_->pos.x = static_cast<float>(system::WINW) / 2.f;
			// �A�j���[�V�����J�n���Ă���̌o�ߎ���
			float t = announceTime_ - moveTime;
			// �I���lrgb( r, g, b)�֌�����
			float r = 240.f, g = 10.f, b = 10.f;
			caObj_->r = Easing::ElasticOut(t, 255.f, -255.f + r, animTime / 2.f);
			caObj_->g = Easing::ExpoInOut(t, 255.f, -255.f + g, animTime / 2.f);
			caObj_->b = Easing::SineIn(t, 255.f, -255.f + b, animTime / 2.f);
		}
		else
		{
			// �I��
			announceTime_ = 0.f;
			caPhase_ = ClearAnnouncePhase::MSGOUT;
			// ���̃C�[�W���O�p�ɍ��W�X�V
			caStartPos_ = caObj_->pos;
		}
	}
		break;
	case CDemoStage::ClearAnnouncePhase::MSGOUT:
	{
		const float moveTime = 0.5f;
		// �ړ�
		// ��ʊO��
		caObj_->pos.x = Easing::BackIn(
			announceTime_,
			caStartPos_.x,
			system::WINW / 2.f + caObj_->HalfWidth(),
			moveTime);
		if (announceTime_ >= moveTime)
		{
			// �I��
			announceTime_ = 0.f;
			caObj_->SetUse(false);
			// �v���C���[�ƓG���n��
			auto& objs = gm()->GetObjects("Player EnemyMng", ' ');
			for (auto& obj : objs)
				obj->start();
			return true;
		}
	}
		break;
	}

	sm_.lock()->MoveCamera(cameraPos);

	return false;
}


bool CDemoStage::UpdateNormal()
{
	return gm()->enemyMng()->getEnemies().empty();
}

bool CDemoStage::UpdateBoss()
{
	// �{�X
	auto& em = gm()->enemyMng();
	auto& enemies = em->getEnemies();

	for (auto& enemy : enemies)
	{
		// �{�X�I�u�W�F�N�g���������Ă���ꍇ�p��
		if (enemy->FindName("E_Boss"))
			return false;
	}

	// pickup(�{�X����ʂɃh���b�v����\��������)
	auto& pickups = gm()->GetObjects("Pickup");
	if (!pickups.empty())
		return false;


	return true;
}






