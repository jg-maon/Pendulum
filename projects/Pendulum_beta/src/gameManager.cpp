
#include "GameManager.h"
#include "gameover.h"

#include "Collision.h"


#include	<algorithm>
#include	<functional>
#include	<sstream>
#include	<fstream>

mymath::Recti* CGameManager::winRect_ = nullptr;

CGameManager::CGameManager() :
Base("GameManager")
, showCursor_(false)
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

	if (winRect_)
		winRect_ = new mymath::Recti(0, 0, system::WINW, system::WINH);

	fileMng_ = std::unique_ptr<CFileMng>(new CFileMng("res/dat/path.ini"));

}

CGameManager::~CGameManager()
{
	objs_.clear();
	addObjs_.clear();
	if (winRect_ != nullptr)
		delete winRect_;
}

void CGameManager::start()
{
	__super::start();

	fileMng_->Load();
	
	sceneMng_ = std::unique_ptr<CSceneMng>(new CSceneMng());


	if (!GetObj(typeid(CStageMng)))
	{
		auto sm = std::shared_ptr<CStageMng>(new CStageMng());
		AddObject2(sm);
		TempCharacter t(sm);
		sm->load();
#ifdef DEF_GM_PTR
		SetStageMngPtr(sm);
#endif
	}

	if (!GetObj(typeid(CScoreMng)))
	{
		auto sm = std::shared_ptr<CScoreMng>(new CScoreMng());
		AddObject2(sm);
#ifdef DEF_GM_PTR
		SetScoreMngPtr(sm);
#endif
	}

	if (!GetObj(typeid(CCollision)))
		AddObject2(ObjPtr(new CCollision()));

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
	// カーソル処理
	if (input::CheckPush(input::KEY_F1))
	{
		showCursor_ ^= 1;
		ShowCursor(showCursor_ == true);
	}
	//-------------------------------------
	// カーソルアニメーション
	//cursorAnim_.step();
	if ((cursor_->angle += 4.f) >= 360.f)
		cursor_->angle -= 360.f;
	//-------------------------------------
	// ウィンドウ外へ出ないように
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
	//-------------------------------------
	// シーン
	sceneMng_->step();
	//=======================================================

	//ゲームオーバー時、クリア時に判定処理は行わない。
	if (gameover::isGameOver()) return;

	//ClearToChangeScreen(CLEARSCREEN);
	//if (getClear()) return;

	//各種更新
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
	//-------------------------------------
	// シーン
	sceneMng_->draw();
	//-------------------------------------
	// オブジェクト
	for (const auto& obj : objs_)
	{
		const auto& status = obj->getStatus();
		if (status == Status::run || status == Status::disp)
			obj->draw();
	}
	//-------------------------------------
	// カーソル
	graph::Draw_SetRenderMode(ADD);
	for (int i = 0; i < 5; ++i)
		cursor_->draw();
	graph::Draw_EndRenderMode();
}

void CGameManager::AllStop()
{
	// 現在登録中
	for (auto& obj : objs_)
		obj->stop();

	//*
	// 追加予定
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
	//追加要素配列から管理配列へ追加する。
	objs_.insert(objs_.end(), addObjs_.begin(), addObjs_.end());
	//追加配列をクリア
	addObjs_.clear();
}



//指定のクラスを見つけ出して返す
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
	//追加前のものもチェック
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
	// 追加予定のオブジェクトも探査
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
	// taskNamesの作成
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
	// 追加予定のオブジェクトも探査
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
	// 現在登録中
	for (auto& obj : objs_)
		obj->kill();
	/*
	// 追加予定
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

mymath::Vec3f CGameManager::GetCursorPosNC() const
{
	RECT rt = camera::GetScreenRect();
	mymath::Vec3f pos = cursor_->pos;
	pos.x -= rt.left;
	pos.y -= rt.top;
	return pos;
}


mymath::Vec3f CGameManager::GetPlayerPos() const
{
#ifdef DEF_GM_PTR
	if (pPlayer_.lock())
	{
		// プレイヤーポインタが設定されているならばポインタから座標を取得
		return pPlayer_.lock()->obj().pos;
	}
#endif
	// オブジェクト配列内から探し出す
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
	return *fileMng_;
}

#ifdef DEF_GM_PTR
//*

void CGameManager::SetPlayerPtr(const std::weak_ptr<CPlayer>& player)
{
	pPlayer_ = player;
	//std::swap(pPlayer_, player);
	//pPlayer_.swap(player);
}

void CGameManager::SetEnemyMngPtr(const std::weak_ptr<CEnemyMng>& enemymng)
{
	pEnemyMng_ = enemymng;
}

void CGameManager::SetStageMngPtr(const std::weak_ptr<CStageMng>& stagemng)
{
	pStageMng_ = stagemng;
}

void CGameManager::SetScoreMngPtr(const std::weak_ptr<CScoreMng>& scoremng)
{
	pScoreMng_ = scoremng;
}


std::shared_ptr<CStageMng> CGameManager::stageMng()
{
	return pStageMng_.lock();
}

std::shared_ptr<CScoreMng> CGameManager::scoreMng()
{
	return pScoreMng_.lock();
}

std::shared_ptr<CPlayer> CGameManager::GetPlayer()
{
	return pPlayer_.lock();
}

std::shared_ptr<CEnemyMng> CGameManager::enemyMng()
{
	return pEnemyMng_.lock();
}

//*/
#endif

/*
void InsertObject(const ObjPtr& obj)
{
extern CGameManager* gm;
gm->AddObject(obj);
}
//*/