#include "stage2.h"

#include "gameManager.h"
#include "common.h"
#include "setting.h"
#include "easing.h"

CStage2::CStage2(std::ifstream& f) :
IStage("Stage2")
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
	//------------------------------------
}

void CStage2::step()
{
	bool f = (phase_ == Phase::FADE_OUT);
	__super::step();
	// �t�F�[�h�C������t�F�[�h�A�E�g�ɕς�����u��
	if (f && phase_ == Phase::FADE_IN)
	{
		//---------------------------------
		// �{�X�X�e�[�W�����[�h����O�ɎG���X�e�[�W�ł̃{�[�i�X�X�R�A�v�Z
		auto& sm = gm()->scoreMng();
		auto& em = gm()->enemyMng();
		sm->CalcBonusScore(em);

		//---------------------------------
		// �X�e�[�W�Ɏc�����Ă���ז��I�u�W�F�N�g�폜
		{
			em->ClearEnemies();
			auto& objs = gm()->GetObjects("Atk_ Effect Pickup", ' ');
			for (auto& obj : objs)
				obj->kill();
		}
		//---------------------------------
		// �{�X�X�e�[�W���[�h
		{
			std::ifstream bossF(bossFile_);
			__super::init(bossF);
			auto& objs = gm()->GetObjects("Player EnemyMng", ' ');
			for (auto& obj : objs)
				obj->start();
		}
	}

}

void CStage2::draw()
{
	// ActionPoint
	__super::draw();

	// �N���A����
	if (caObj_->CheckUse())
		caObj_->drawNC();


	// �c��G��
	if (!isBossStage())
	{
		auto& em = gm()->enemyMng();
		size_t sta = em->GetStartEnemyNum();
		size_t now = em->getEnemies().size();
		size_t killNum = sta - now;
		std::string fontName("font_GYOSHO_01_40");
		std::stringstream msg;
		int num = clearNorma_ - killNum;
		msg << "�c�� " << (num <= 0 ? 0 : num);
		font::Draw_FontTextNC(
			(system::WINW - font::Draw_GetStringWidth(msg.str(), setting::GetFontID(fontName))) / 2,
			40,
			0.1f,
			msg.str(),
			ARGB(255, 220, 220, 220),
			setting::GetFontID(fontName)
			);
	}

	// �w�i
	for (int i = 0; i < 8; ++i)
	{
		std::stringstream resname;
		resname << "img_stage02_" << std::setw(2) << std::setfill('0') << i;
		
		SIZE s = graph::Draw_GetImageSize2(resname.str());
		int x = s.cx * (i % 4);
		int y = s.cy * (i / 4);

		graph::Draw_GraphicsLeftTop(
			x, y, 1.f,
			resname.str(),
			0, 0, s.cx, s.cy);
	}
}

void CStage2::init(std::ifstream& f)
{
	__super::init(f);

	phase_ = IStage::Phase::CLEAR_ANNOUNCE;

	caPhase_ = ClearAnnouncePhase::WAIT;
	announceTime_ = 0.f;

	// �v���C���[�o��A�j���[�V����
	auto& objs = gm()->GetObjects("Player");
	for (auto& obj : objs)
		obj->start();
	auto playerPos_ = gm()->GetPlayerPos();

	sm_.lock()->MoveCamera(playerPos_);

	gm()->enemyMng()->SetStatusDisp();

	// �����͍��������痈��
	caObj_->pos.x = -caObj_->HalfWidth();
	caObj_->pos.y = static_cast<float>(system::WINH) / 4.f;
	caObj_->pos.z = 0.f;
	caStartPos_ = caObj_->pos;
	caObj_->SetUse(false);
}



void CStage2::LoadClear(std::ifstream& f, mymath::ShapefPtr& area)
{
	if (!common::FindChunk(common::SeekSet(f), "#StageClear"))
	{
		debug::BToMF("CStage2::LoadGoal #StageClear not found");
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
		f >> clearNorma_;
	}
}




bool CStage2::UpdateClearAnnounce()
{
	const mymath::Vec3f& playerPos_ = gm()->GetPlayerPos();
	mymath::Vec3f cameraPos = camera::GetLookAt();

	mymath::Rectf cameraRect = getCameraRect();

	announceTime_ += system::FrameTime;

	// ���������ォ�玞�v��聨����
	switch (caPhase_)
	{
	case CStage2::ClearAnnouncePhase::WAIT:
	{
		// �^�C�g���A�j���[�V�������̓v���C���[�ƓG�̓������~�߂Ă����K�v������
		auto& objs = gm()->GetObjects("Player EnemyMng", ' ');
		for (auto& obj : objs)
			obj->SetStatusDisp();

		announceTime_ = 0.f;
		caPhase_ = ClearAnnouncePhase::LEFTTOP;
	}
		break;
	case CStage2::ClearAnnouncePhase::LEFTTOP:
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
	case CStage2::ClearAnnouncePhase::RIGHTTOP:
	{
		const float moveTime = 1.5f;		// �J�����ړ�����
		const float vecx = cameraRect.width();
		cameraPos.x = Easing::ExpoInOut(announceTime_, cameraRect.left, vecx, moveTime);
		if (announceTime_ >= moveTime + 0.5f)
		{
			caPhase_ = ClearAnnouncePhase::RIGHTBOTTOM;
			announceTime_ = 0.f;
		}
	}
		break;
	case CStage2::ClearAnnouncePhase::RIGHTBOTTOM:
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
	case CStage2::ClearAnnouncePhase::LEFTBOTTOM:
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
	case CStage2::ClearAnnouncePhase::TO_PLAYER:
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
	case CStage2::ClearAnnouncePhase::MSGIN:
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
	case CStage2::ClearAnnouncePhase::MSGOUT:
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


bool CStage2::UpdateNormal()
{
	auto& em = gm()->enemyMng();
	size_t sta = em->GetStartEnemyNum();
	size_t now = em->getEnemies().size();
	size_t killNum = sta - now;
	if (killNum >= clearNorma_)
	{
		sm_.lock()->setStageState(CStageMng::StageState::PLAYER_EXIT);
		return true;
	}
	return false;
}

bool CStage2::UpdateBoss()
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






