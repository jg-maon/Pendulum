
#include "GameManager.h"
#include "gameover.h"

#include "stageMng.h"
#include "scoreMng.h"
#include "Collision.h"


#include	<algorithm>
#include	<functional>
#include	<sstream>
#include	<fstream>

mymath::Recti* CGameManager::winRect_ = nullptr;

CGameManager::CGameManager() :
Base("GameManager")
, showCursor_(false)
, fileMng_("res/dat/path.ini")
//, cursorAnim_(360.f,10.f)
{
	ShowCursor(showCursor_ == true);
	init();

	cursor_ = charabase::CharPtr(new charabase::CharBase(
		mymath::Vec3f(), mymath::Vec3f(),
		"img_cursor",
		CursorSize::width,
		CursorSize::height));
	cursor_->alpha = 200.f;
}

CGameManager::~CGameManager()
{
	objs_.clear();
	addObjs_.clear();
}

void CGameManager::start()
{
	__super::start();

	fileMng_.Load();

	if (!GetObj(typeid(CStageMng)).get())
		AddObject2(ObjPtr(new CStageMng()));

	if (!GetObj(typeid(CScoreMng)).get())
		AddObject2(ObjPtr(new CScoreMng()));

	if (!GetObj(typeid(CCollision)).get())
		AddObject2(ObjPtr(new CCollision()));

	objs_ = objs_;
}


void CGameManager::MargeObjects()
{
	objs_.erase(
		std::remove_if(objs_.begin(), objs_.end(),
		std::mem_fn(&Base::isDestroy)),
		objs_.end());

	if (!addObjs_.empty())
	{
		objs_.insert(objs_.end(), addObjs_.begin(), addObjs_.end());
		addObjs_.clear();
	}

}

void CGameManager::init()
{
	objs_.clear();
	addObjs_.clear();
	//clear_ = false;
}

void CGameManager::step()
{
	//=======================================================
	// �J�[�\������
	if (input::CheckPush(input::KEY_F1))
	{
		showCursor_ ^= 1;
		ShowCursor(showCursor_ == true);
	}
	//-------------------------------------
	// �J�[�\���A�j���[�V����
	//cursorAnim_.step();
	if ((cursor_->angle += 4.f) >= 360.f)
		cursor_->angle -= 360.f;
	//-------------------------------------
	// �E�B���h�E�O�֏o�Ȃ��悤��
	mymath::Vec3f& cursorPos = cursor_->pos;
	const float& halfWidth = cursor_->HalfWidth();
	const float& halfHeight = cursor_->HalfHeight();
	cursorPos = camera::GetCursorPosition();
	RECT rt = camera::GetScreenRect();
	if (cursorPos.x - halfWidth < rt.left)
		cursorPos.x = rt.left + halfWidth;
	else if (cursorPos.x + halfWidth > rt.right)
		cursorPos.x = rt.right - halfWidth;
	if (cursorPos.y - halfHeight < rt.top)
		cursorPos.y = rt.top + halfHeight;
	else if (cursorPos.y + halfHeight > rt.bottom)
		cursorPos.y = rt.bottom - halfHeight;
	//=======================================================

	//�Q�[���I�[�o�[���A�N���A���ɔ��菈���͍s��Ȃ��B
	if (gameover::isGameOver()) return;

	//ClearToChangeScreen(CLEARSCREEN);
	//if (getClear()) return;

	//�e��X�V
	for (const auto& obj : objs_)
	{
		const auto& status = obj->getStatus();
		if (status == Status::run || status == Status::update)
			obj->step();
	}

	MargeObjects();


}

void CGameManager::draw()
{
	graph::Draw_SetRenderMode(ADD);
	for (int i = 0; i < 5; ++i)
		cursor_->draw();
	graph::Draw_EndRenderMode();
	for (const auto& obj : objs_)
	{
		const auto& status = obj->getStatus();
		if (status == Status::run || status == Status::disp)
			obj->draw();
	}
}

void CGameManager::AllStop()
{
	// ���ݓo�^��
	for (auto& obj : objs_)
		obj->stop();

	//*
	// �ǉ��\��
	for (auto& obj : addObjs_)
		obj->stop();
	//*/
}

void CGameManager::AddObject(const ObjPtr& obj)
{
	addObjs_.push_back(obj);
}

void CGameManager::AddObject2(const ObjPtr& obj)
{
	addObjs_.push_back(obj);
	//�ǉ��v�f�z�񂩂�Ǘ��z��֒ǉ�����B
	objs_.insert(objs_.end(), addObjs_.begin(), addObjs_.end());
	//�ǉ��z����N���A
	addObjs_.clear();
}



//�w��̃N���X�������o���ĕԂ�
ObjPtr CGameManager::GetObj(const type_info& objinfo)
{
	for (auto& obj : objs_)
	{
		const type_info& info = typeid(*obj);
		if (info == objinfo)
		{
			return obj;
		}
	}
	//�ǉ��O�̂��̂��`�F�b�N
	for (auto& obj : addObjs_)
	{
		const type_info& info = typeid(*obj);
		if (info == objinfo)
		{
			return obj;
		}
	}
	return nullptr;
}

std::vector<ObjPtr>& CGameManager::GetObjects()
{
	return objs_;
}

std::vector<ObjPtr> CGameManager::GetObjects(const std::string& taskName)
{
	std::vector<ObjPtr> ret;
	for (const auto& obj : objs_)
	{
		if (!obj->FindName(taskName)) continue;
		ret.push_back(obj);
	}
	// �ǉ��\��̃I�u�W�F�N�g���T��
	for (const auto& obj : addObjs_)
	{
		if (!obj->FindName(taskName)) continue;
		ret.push_back(obj);
	}
	return ret;
}

std::vector<ObjPtr> CGameManager::GetObjects(const std::string& taskName, const char delim)
{
	std::vector<ObjPtr> ret;
	std::vector<std::string> taskNames;
	// taskNames�̍쐬
	{
		std::stringstream ss(taskName);
		std::string temp;
		while (std::getline(ss, temp, delim))
		{
			taskNames.push_back(temp);
		}
	}
	for (const auto& obj : objs_)
	{
		for (const auto& taskName : taskNames)
		{
			if (!obj->FindName(taskName)) continue;
			ret.push_back(obj);
		}
	}
	// �ǉ��\��̃I�u�W�F�N�g���T��
	for (const auto& obj : addObjs_)
	{
		for (const auto& taskName : taskNames)
		{
			if (!obj->FindName(taskName)) continue;
			ret.push_back(obj);
		}
	}
	return ret;
}

void CGameManager::ClearObjects()
{
	// ���ݓo�^��
	for (auto& obj : objs_)
		obj->kill();
	/*
	// �ǉ��\��
	for (auto& obj : addObjs_)
	obj->kill();
	//*/
}



/*
void CGameManager::setClear(bool clear)
{
	clear_ = clear;
}
bool CGameManager::getClear() const
{
	return clear_;
}


void CGameManager::StartClear()
{
	setClear(true);
	count_ = 3.f;
}

void CGameManager::ClearToChangeScreen(int next)
{
	if (getClear())
	{
		count_ -= system::ONEFRAME_TIME;
		debug::SToMNC(0, 100, "GameClearCount:%0.2f", count_);
		if (count_ <= 0)
		{
			stage::ChangeStage(next);
		}
	}
}
//*/

const mymath::Recti* CGameManager::winRect() const
{
	return winRect_;
}

void CGameManager::winRect(const mymath::Recti& newRect)
{
	*winRect_ = newRect;
}

void CGameManager::winRect(mymath::Recti* newRect)
{
	winRect_ = newRect;
}

const mymath::Vec3f& CGameManager::GetCursorPos() const
{
	return cursor_->pos;
}


mymath::Vec3f CGameManager::GetPlayerPos() const
{
	mymath::Vec3f v = { -1, -1, -1 };
	for (const auto& s : objs_){
		if (s->FindName("Player")){
			v = std::dynamic_pointer_cast<IObject>(s)->obj().pos;
			return v;
		}
	}
	return v;
}

CFileMng& CGameManager::fm()
{
	return fileMng_;
}


/*
void InsertObject(const ObjPtr& obj)
{
extern CGameManager* gm;
gm->AddObject(obj);
}
//*/